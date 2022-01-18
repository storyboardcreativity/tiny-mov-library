#pragma once

#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include <byteswap.h>

#define MKTAG(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))

class TinyMovTrack
{
public:

};

class TinyMovFile
{
public:
    std::vector<TinyMovTrack>& Tracks()
    {
        return _tracks;
    }

protected:
    std::vector<TinyMovTrack> _tracks;
};

class fstream_wrapper
{
public:
    fstream_wrapper(std::fstream &stream, int64_t initial_size) : _stream(stream)
    {
        _atom_offsets_stack.push(0);
        _atom_sizes_stack.push(initial_size);
        _atom_ids_stack.push(MKTAG('R', 'O', 'O', 'T'));
    }

    uint8_t get_byte()
    {
        uint8_t value;
        if (atom_left() < sizeof(value))
            throw std::exception();

        _stream.read((char *)&value, sizeof(value));
        _atom_offsets_stack.top() += sizeof(value);
        return value;
    }
    uint16_t get_le16()
    {
        uint16_t value;
        if (atom_left() < sizeof(value))
            throw std::exception();

        _stream.read((char *)&value, sizeof(value));
        _atom_offsets_stack.top() += sizeof(value);
        return value;
    }
    uint16_t get_be16()
    {
        return bswap_16(get_le16());
    }
    uint32_t get_le32()
    {
        uint32_t value;
        if (atom_left() < sizeof(value))
            throw std::exception();

        _stream.read((char *)&value, sizeof(value));
        _atom_offsets_stack.top() += sizeof(value);
        return value;
    }
    uint32_t get_be32()
    {
        return bswap_32(get_le32());
    }
    uint64_t get_le64()
    {
        uint64_t value;
        if (atom_left() < sizeof(value))
            throw std::exception();

        _stream.read((char *)&value, sizeof(value));
        _atom_offsets_stack.top() += sizeof(value);
        return value;
    }
    uint64_t get_be64()
    {
        return bswap_64(get_le64());
    }
    void seekg(int64_t offset)
    {
        if (atom_left() < offset)
            throw std::exception();
        _atom_offsets_stack.top() += offset;

        _stream.seekg(offset, _stream.cur);
    }
    void seekp(int64_t offset)
    {
        _stream.seekp(offset, _stream.cur);
    }
    int64_t tellg()
    {
        return _stream.tellg();
    }
    int64_t tellp()
    {
        return _stream.tellp();
    }
    void push_atom(int64_t size, uint32_t parent_id)
    {
        if (atom_left() < size)
            throw std::exception();

        _atom_offsets_stack.push(0);
        _atom_sizes_stack.push(size);
        _atom_ids_stack.push(parent_id);
    }
    void pop_atom()
    {
        if (atom_level() == 0)
            throw std::exception();

        auto top_size = _atom_sizes_stack.top();
        _atom_offsets_stack.pop();
        _atom_sizes_stack.pop();
        _atom_ids_stack.pop();

        _atom_offsets_stack.top() += top_size;
    }
    int64_t atom_left()
    {
        return _atom_sizes_stack.top() - _atom_offsets_stack.top();
    }
    int64_t atom_level()
    {
        return (int64_t)_atom_offsets_stack.size() - 1;
    }
    uint32_t atom_id()
    {
        return _atom_ids_stack.top();
    }
    uint32_t atom_parent_id()
    {
        auto id = atom_id();

        _atom_ids_stack.pop();
        auto parent_id = _atom_ids_stack.top();
        _atom_ids_stack.push(id);

        return parent_id;
    }

protected:
    std::fstream &_stream;
    std::stack<uint32_t> _atom_ids_stack;
    std::stack<int64_t> _atom_offsets_stack;
    std::stack<int64_t> _atom_sizes_stack;
};

class ITinyMovFileAtomProcessor
{
public:
    virtual ~ITinyMovFileAtomProcessor() {}

    virtual uint32_t Type() = 0;
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov) = 0;
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) = 0;
};

std::vector<ITinyMovFileAtomProcessor*> TinyMovFileAtomProcessors;

class TinyMovFileAtomProcessorBase : public ITinyMovFileAtomProcessor
{
public:
    TinyMovFileAtomProcessorBase()
    {
        TinyMovFileAtomProcessors.push_back(this);
    }

    virtual uint32_t Type() = 0;
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov) = 0;
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) = 0;
};

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return 0x00000000;
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        while (stream.atom_left() > 0)
        {
            int64_t size = stream.get_be32();
            uint32_t type = stream.get_le32();

            if (size == 1)
            {
                // "length may error!"
                size = stream.get_be64() - 8;
            }

            stream.push_atom(size - 8, type);
            
            int i = 0;
            for (; i < TinyMovFileAtomProcessors.size(); ++i)
                if (TinyMovFileAtomProcessors[i]->Type() == type)
                    break;

            char name[5];
            *(uint32_t*)name = type;
            name[4] = '\0';
            std::cout << "Atom: ";
            for (int i = 0; i < stream.atom_level() - 1; ++i)
                std::cout << "---";

            if (i < TinyMovFileAtomProcessors.size())
            {
                std::cout << "<" << name << ">" << std::endl;

                auto res = TinyMovFileAtomProcessors[i]->ProcessRead(stream, mov);
                if (!res)
                    return false;

                if (stream.atom_left() < 0)
                {
                    std::cout << "Atom left: " << stream.atom_left() << std::endl;
                    throw std::exception();
                }

                if (stream.atom_left() != 0)
                {
                    std::cout << "Warning: " << name << " atom left " << stream.atom_left() << std::endl;
                }
            }
            else
                std::cout << name << std::endl;

            stream.seekg(stream.atom_left());   // Skip atom garbage

            stream.pop_atom();
        }

        return true;
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_default;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('f', 't', 'y', 'p');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('R', 'O', 'O', 'T'))
        {
            std::cout << "Warning! Parent is not ROOT!" << std::endl;
            return false;
        }

        uint32_t major = stream.get_le32();
        uint32_t minor = stream.get_be32();

        char name[5];
        *(uint32_t*)name = major;
        name[4] = '\0';

        //std::cout << "major " << name << std::endl;
        //std::cout << "minor " << minor << std::endl;

        // Skip other
        stream.seekg(stream.atom_left());

        return true;
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_ftyp;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'o', 'o', 'v');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('R', 'O', 'O', 'T'))
        {
            std::cout << "Warning! Parent is not ROOT!" << std::endl;
            return false;
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_moov;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('t', 'r', 'a', 'k');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'o', 'o', 'v'))
        {
            std::cout << "Warning! Parent is not moov!" << std::endl;
            return false;
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_trak;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'v', 'h', 'd');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'o', 'o', 'v'))
        {
            std::cout << "Warning! Parent is not moov!" << std::endl;
            return false;
        }

        std::cout << "Atom left: " << stream.atom_left() << std::endl;

        uint8_t version = stream.get_byte();
        uint8_t flags0 = stream.get_byte();
        uint8_t flags1 = stream.get_byte();
        uint8_t flags2 = stream.get_byte();

        if (version == 1)
        {
            uint64_t creation_time = stream.get_be64();
            uint64_t modification_time = stream.get_be64();
        }
        else
        {
            uint32_t creation_time = stream.get_be32();
            uint32_t modification_time = stream.get_be32();
        }

        uint32_t time_scale = stream.get_be32();
        int64_t duration = (int32_t)((version == 1) ? stream.get_be64() : stream.get_be32());


        uint32_t preferred_rate = stream.get_be32();
        uint16_t preferred_volume = stream.get_be16();

        stream.seekg(10);               // reserved

        for (int i = 0; i < 3; ++i)     // display matrix
        {
            stream.get_be32();
            stream.get_be32();
            stream.get_be32();
        }

        uint32_t preview_duration = stream.get_be32();
        uint32_t poster_time = stream.get_be32();
        uint32_t selection_time = stream.get_be32();
        uint32_t selection_duration = stream.get_be32();
        uint32_t current_time = stream.get_be32();
        uint32_t next_track_id = stream.get_be32();

        return true;
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_mvhd;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'd', 'i', 'a');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('t', 'r', 'a', 'k'))
        {
            std::cout << "Warning! Parent is not trak!" << std::endl;
            return false;
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_mdia;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('t', 'k', 'h', 'd');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('t', 'r', 'a', 'k'))
        {
            std::cout << "Warning! Parent is not trak!" << std::endl;
            return false;
        }

        uint8_t version = stream.get_byte();
        uint8_t flags0 = stream.get_byte();
        uint8_t flags1 = stream.get_byte();
        uint8_t flags2 = stream.get_byte();

        if (version == 1)
        {
            uint64_t creation_time = stream.get_be64();
            uint64_t modification_time = stream.get_be64();
        }
        else
        {
            uint32_t creation_time = stream.get_be32();
            uint32_t modification_time = stream.get_be32();
        }

        uint32_t track_id = stream.get_be32();

        stream.get_be32();      // reserved

        uint64_t duration = (version == 1) ? stream.get_be64() : stream.get_be32();

        stream.get_be64();      // reserved

        uint16_t layer = stream.get_be16();
        uint16_t alternative_group = stream.get_be16();
        uint16_t volume = stream.get_be16();

        stream.get_be16();      // reserved

        for (int i = 0; i < 3; ++i)     // display matrix
        {
            stream.get_be32();
            stream.get_be32();
            stream.get_be32();
        }

        uint32_t track_width = stream.get_be32();
        uint32_t track_height = stream.get_be32();

        return true;
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_tkhd;

class TinyMovFileReader
{
public:
    TinyMovFile OpenMovFile(std::string path)
    {
        TinyMovFile mov;

        std::fstream f_in(path, std::ios::in | std::ios::binary);
        
        auto fsize = f_in.tellg();
        f_in.seekg(0, std::ios::end);
        fsize = f_in.tellg() - fsize;
        f_in.seekg(0, std::ios::beg);

        fstream_wrapper f_in_wrapper(f_in, fsize);
        TinyMovFileAtomProcessor_default.ProcessRead(f_in_wrapper, mov);

        return mov;
    }
};

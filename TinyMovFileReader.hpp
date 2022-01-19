#pragma once

#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include <memory.h>
#include <byteswap.h>

#define MKTAG(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))

class TinyMovMetadata
{
public:
    std::vector<std::string>& Keys()
    {
        return _keys;
    }
    std::vector<std::string>& Values()
    {
        return _values;
    }

    bool IsOk()
    {
        if (Keys().size() != Values().size())
            return false;
        
        return true;
    }

protected:
    std::vector<std::string> _keys;
    std::vector<std::string> _values;
};

class TinyMovTrackMediaInfo
{
public:
    std::vector<uint64_t>& SampleSizes()
    {
        return _sample_sizes;
    }
    std::vector<uint64_t>& SampleOffsets()
    {
        return _sample_offsets;
    }

    bool IsOk()
    {
        if (SampleSizes().size() != SampleOffsets().size())
            return false;

        return true;
    }

protected:
    std::vector<uint64_t> _sample_sizes;
    std::vector<uint64_t> _sample_offsets;
};

class TinyMovTrackMedia
{
public:
    void Flags(uint8_t flags[3])
    {
        memcpy(_flags, flags, sizeof(_flags));
    }
    void CreationTime(uint64_t creation_time)
    {
        _creation_time = creation_time;
    }
    void ModificationTime(uint64_t modification_time)
    {
        _modification_time = modification_time;
    }
    void TimeScale(uint32_t time_scale)
    {
        _time_scale = time_scale;
    }
    void Duration(uint64_t duration)
    {
        _duration = duration;
    }
    void Language(uint16_t language)
    {
        _language = language;
    }
    void Quality(uint16_t quality)
    {
        _quality = quality;
    }

    TinyMovTrackMediaInfo& Info()
    {
        return _info;
    }

protected:
    // Media Header
    uint8_t _flags[3];
    uint64_t _creation_time;
    uint64_t _modification_time;
    uint32_t _time_scale;
    uint64_t _duration;
    uint16_t _language;
    uint16_t _quality;

    TinyMovTrackMediaInfo _info;
};

class TinyMovTrack
{
public:
    void Flags(uint8_t flags[3])
    {
        memcpy(_flags, flags, sizeof(_flags));
    }
    void CreationTime(uint64_t creation_time)
    {
        _creation_time = creation_time;
    }
    void ModificationTime(uint64_t modification_time)
    {
        _modification_time = modification_time;
    }
    void TrackId(uint32_t track_id)
    {
        _track_id = track_id;
    }
    void Duration(uint64_t duration)
    {
        _duration = duration;
    }
    void Layer(uint16_t layer)
    {
        _layer = layer;
    }
    void AlternativeGroup(uint16_t alternative_group)
    {
        _alternative_group = alternative_group;
    }
    void Volume(uint16_t volume)
    {
        _volume = volume;
    }
    void DisplayMatrix(uint32_t display_matrix[3][3])
    {
        memcpy(_display_matrix, display_matrix, sizeof(_display_matrix));
    }
    void TrackWidth(uint32_t track_width)
    {
        _track_width = track_width;
    }
    void TrackHeight(uint32_t track_height)
    {
        _track_height = track_height;
    }

    TinyMovTrackMedia& Media()
    {
        return _media;
    }
protected:
    // Track Header
    uint8_t _flags[3];
    uint64_t _creation_time;
    uint64_t _modification_time;
    uint32_t _track_id;
    uint64_t _duration;
    uint16_t _layer;
    uint16_t _alternative_group;
    uint16_t _volume;
    uint32_t _display_matrix[3][3];
    uint32_t _track_width;
    uint32_t _track_height;

    // Track Media
    TinyMovTrackMedia _media;
};

class TinyMovFile
{
public:
    std::vector<TinyMovTrack>& Tracks()
    {
        return _tracks;
    }
    TinyMovMetadata& Metadata()
    {
        return _metadata;
    }

    void Flags(uint8_t flags[3])
    {
        memcpy(_flags, flags, sizeof(_flags));
    }
    void CreationTime(uint64_t creation_time)
    {
        _creation_time = creation_time;
    }
    void ModificationTime(uint64_t modification_time)
    {
        _modification_time = modification_time;
    }
    void TimeScale(uint32_t time_scale)
    {
        _time_scale = time_scale;
    }
    void Duration(uint64_t duration)
    {
        _duration = duration;
    }
    void PreferredRate(uint16_t preferred_rate)
    {
        _preferred_rate = preferred_rate;
    }
    void PreferredVolume(uint16_t preferred_volume)
    {
        _preferred_volume = preferred_volume;
    }
    void DisplayMatrix(uint32_t display_matrix[3][3])
    {
        memcpy(_display_matrix, display_matrix, sizeof(_display_matrix));
    }
    void PreviewDuration(uint32_t preview_duration)
    {
        _preview_duration = preview_duration;
    }
    void PosterTime(uint32_t poster_time)
    {
        _poster_time = poster_time;
    }
    void SelectionTime(uint32_t selection_time)
    {
        _selection_time = selection_time;
    }
    void SelectionDuration(uint32_t selection_duration)
    {
        _selection_duration = selection_duration;
    }
    void CurrentTime(uint32_t current_time)
    {
        _current_time = current_time;
    }
    void NextTrackId(uint32_t next_track_id)
    {
        _next_track_id = next_track_id;
    }

protected:
    std::vector<TinyMovTrack> _tracks;
    TinyMovMetadata _metadata;

    // Movie Header
    uint8_t _flags[3];
    uint64_t _creation_time;
    uint64_t _modification_time;
    uint32_t _time_scale;
    uint64_t _duration;
    uint32_t _preferred_rate;
    uint16_t _preferred_volume;
    uint32_t _display_matrix[3][3];
    uint32_t _preview_duration;
    uint32_t _poster_time;
    uint32_t _selection_time;
    uint32_t _selection_duration;
    uint32_t _current_time;
    uint32_t _next_track_id;
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
    void read(void* dst, uint32_t size)
    {
        uint8_t* dest8 = (uint8_t*)dst;
        for (uint32_t i = 0; i < size; ++i)
            dest8[i] = get_byte();
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

    void set_current_track(TinyMovTrack* track)
    {
        _current_track = track;
    }
    TinyMovTrack& get_current_track()
    {
        if (_current_track == nullptr)
            throw std::exception();
        return *_current_track;
    }

protected:
    std::fstream &_stream;
    std::stack<uint32_t> _atom_ids_stack;
    std::stack<int64_t> _atom_offsets_stack;
    std::stack<int64_t> _atom_sizes_stack;

    TinyMovTrack* _current_track;
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

        TinyMovTrack track;

        stream.set_current_track(&track);
        auto result = TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
        stream.set_current_track(nullptr);

        mov.Tracks().push_back(track);

        return result;
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

        uint8_t flags[3];
        flags[0] = stream.get_byte();
        flags[1] = stream.get_byte();
        flags[2] = stream.get_byte();
        mov.Flags(flags);

        mov.CreationTime((version == 1) ? stream.get_be64() : stream.get_be32());
        mov.ModificationTime((version == 1) ? stream.get_be64() : stream.get_be32());
        mov.TimeScale(stream.get_be32());
        mov.Duration((version == 1) ? stream.get_be64() : stream.get_be32());
        mov.PreferredRate(stream.get_be32());
        mov.PreferredVolume(stream.get_be16());

        stream.seekg(10);               // reserved

        uint32_t display_matrix[3][3];
        for (int i = 0; i < 3; ++i)     // display matrix
        {
            display_matrix[i][0] = stream.get_be32();
            display_matrix[i][1] = stream.get_be32();
            display_matrix[i][2] = stream.get_be32();
        }
        mov.DisplayMatrix(display_matrix);

        mov.PreviewDuration(stream.get_be32());
        mov.PosterTime(stream.get_be32());
        mov.SelectionTime(stream.get_be32());
        mov.SelectionDuration(stream.get_be32());
        mov.CurrentTime(stream.get_be32());
        mov.NextTrackId(stream.get_be32());

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

        uint8_t flags[3];
        flags[0] = stream.get_byte();
        flags[1] = stream.get_byte();
        flags[2] = stream.get_byte();
        stream.get_current_track().Flags(flags);

        stream.get_current_track().CreationTime(
            (version == 1) ?
            stream.get_be64() :
            stream.get_be32());
        
        stream.get_current_track().ModificationTime(
            (version == 1) ?
            stream.get_be64() :
            stream.get_be32());

        stream.get_current_track().TrackId(stream.get_be32());

        stream.get_be32();      // reserved

        stream.get_current_track().Duration(
            (version == 1) ?
            stream.get_be64() :
            stream.get_be32());

        stream.get_be64();      // reserved

        stream.get_current_track().Layer(stream.get_be16());
        stream.get_current_track().AlternativeGroup(stream.get_be16());
        stream.get_current_track().Volume(stream.get_be16());

        stream.get_be16();      // reserved

        uint32_t display_matrix[3][3];
        for (int i = 0; i < 3; ++i)     // display matrix
        {
            display_matrix[i][0] = stream.get_be32();
            display_matrix[i][1] = stream.get_be32();
            display_matrix[i][2] = stream.get_be32();
        }
        stream.get_current_track().DisplayMatrix(display_matrix);

        stream.get_current_track().TrackWidth(stream.get_be32());
        stream.get_current_track().TrackHeight(stream.get_be32());

        return true;
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_tkhd;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'd', 'h', 'd');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'd', 'i', 'a'))
        {
            std::cout << "Warning! Parent is not mdia!" << std::endl;
            return false;
        }

        uint8_t version = stream.get_byte();

        uint8_t flags[3];
        flags[0] = stream.get_byte();
        flags[1] = stream.get_byte();
        flags[2] = stream.get_byte();
        stream.get_current_track().Media().Flags(flags);

        stream.get_current_track().Media().CreationTime(
            (version == 1) ?
            stream.get_be64() :
            stream.get_be32());
        
        stream.get_current_track().Media().ModificationTime(
            (version == 1) ?
            stream.get_be64() :
            stream.get_be32());
        
        stream.get_current_track().Media().TimeScale(stream.get_be32());

        stream.get_current_track().Duration(
            (version == 1) ?
            stream.get_be64() :
            stream.get_be32());
        
        stream.get_current_track().Media().Language(stream.get_be16());
        stream.get_current_track().Media().Quality(stream.get_be16());

        return true;
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_mdhd;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'i', 'n', 'f');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'd', 'i', 'a'))
        {
            std::cout << "Warning! Parent is not mdia!" << std::endl;
            return false;
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_minf;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('s', 't', 'b', 'l');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'i', 'n', 'f'))
        {
            std::cout << "Warning! Parent is not minf!" << std::endl;
            return false;
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_stbl;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('s', 't', 's', 'z');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('s', 't', 'b', 'l'))
        {
            std::cout << "Warning! Parent is not stbl!" << std::endl;
            return false;
        }

        uint8_t version = stream.get_byte();

        uint8_t flags[3];
        flags[0] = stream.get_byte();
        flags[1] = stream.get_byte();
        flags[2] = stream.get_byte();

        uint32_t sample_size = stream.get_be32();

        uint32_t entries = stream.get_be32();
        if (!entries)
            return true;

        if (sample_size)
            return true;

        for (int i = 0; i < entries; ++i)
            stream.get_current_track().Media().Info().SampleSizes().push_back(stream.get_be32());

        return true;
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_stsz;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('s', 't', 'c', 'o');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('s', 't', 'b', 'l'))
        {
            std::cout << "Warning! Parent is not stbl!" << std::endl;
            return false;
        }

        uint8_t version = stream.get_byte();

        uint8_t flags[3];
        flags[0] = stream.get_byte();
        flags[1] = stream.get_byte();
        flags[2] = stream.get_byte();

        uint32_t entries = stream.get_be32();
        if (!entries)
            return true;

        for (int i = 0; i < entries; ++i)
            stream.get_current_track().Media().Info().SampleOffsets().push_back(stream.get_be32());

        return true;
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_stco;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'e', 't', 'a');
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
} TinyMovFileAtomProcessor_meta;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('k', 'e', 'y', 's');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'e', 't', 'a'))
        {
            std::cout << "Warning! Parent is not meta!" << std::endl;
            return false;
        }

        stream.seekg(4);    // Skip

        uint32_t count = stream.get_be32();

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_keys;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'd', 't', 'a');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('k', 'e', 'y', 's'))
        {
            std::cout << "Warning! Parent is not keys!" << std::endl;
            return false;
        }

        uint32_t size = stream.atom_left();
        char* text = new char[size + 1];
        stream.read(text, size);
        text[size] = '\0';
        mov.Metadata().Keys().push_back(text);
        delete[] text;

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_mdta;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('i', 'l', 's', 't');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'e', 't', 'a'))
        {
            std::cout << "Warning! Parent is not meta!" << std::endl;
            return false;
        }

        while (stream.atom_left() > 0)
        {
            uint32_t len = stream.get_be32();
            uint32_t idx = stream.get_be32();

            stream.push_atom(len - 8, Type());

            if(!TinyMovFileAtomProcessor_default.ProcessRead(stream, mov))
                return false;

            stream.pop_atom();
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_ilst;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('d', 'a', 't', 'a');
    }
    virtual bool ProcessRead(fstream_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('i', 'l', 's', 't'))
        {
            std::cout << "Warning! Parent is not ilst!" << std::endl;
            return false;
        }
        
        uint32_t type_indicator = stream.get_be32();
        if (type_indicator != 1)
            throw std::exception();     // FIXME: make support for each data type!

        uint32_t locale_indicator = stream.get_be32();
        uint32_t size = stream.atom_left();
        char* text = new char[size + 1];
        stream.read(text, size);
        text[size] = '\0';
        mov.Metadata().Values().push_back(text);
        delete[] text;

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    virtual bool ProcessWrite(fstream_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_data;

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

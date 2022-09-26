#pragma once

#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <memory.h>
#include <byteswap.h>

#ifndef __TINY_MOV_FILE_BASE_IMPL__
#define __TINY_MOV_FILE_BASE_IMPL__

#define MKTAG(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))
#define __TINY_MOV_FILE_OUT_INITIAL_MDAT_ATOM_OFFSET 0x8000

class TinyMovMetadataKey
{
public:
    void Data(std::vector<uint8_t>& data)
    {
        _data = data;
    }
    std::vector<uint8_t>& Data()
    {
        return _data;
    }

    void Namespace(uint32_t value)
    {
        _namespace = value;
    }
    uint32_t Namespace()
    {
        return _namespace;
    }

protected:
    uint32_t _namespace;
    std::vector<uint8_t> _data;
};

class TinyMovMetadataValue
{
public:
    void Data(std::vector<uint8_t>& data)
    {
        _data = data;
    }
    std::vector<uint8_t>& Data()
    {
        return _data;
    }

    void Type(uint32_t type)
    {
        _type = type;
    }
    uint32_t Type()
    {
        return _type;
    }

    void Locale(uint32_t locale)
    {
        _locale = locale;
    }
    uint32_t Locale()
    {
        return _locale;
    }

protected:
    uint32_t _type;
    uint32_t _locale;
    std::vector<uint8_t> _data;
};

class TinyMovMetadata
{
public:
    std::vector<TinyMovMetadataKey>& Keys()
    {
        return _keys;
    }
    std::vector<TinyMovMetadataValue>& Values()
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
    std::vector<TinyMovMetadataKey> _keys;
    std::vector<TinyMovMetadataValue> _values;
};

class TinyMovTrackMedia;
class TinyMovTrackMediaInfo
{
public:
    TinyMovTrackMediaInfo(TinyMovTrackMedia& parent) : _parent(parent) {}

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

    TinyMovTrackMedia& Parent()
    {
        return _parent;
    }

protected:
    std::vector<uint64_t> _sample_sizes;
    std::vector<uint64_t> _sample_offsets;

    TinyMovTrackMedia& _parent;
};

class TinyMovTrack;
class TinyMovTrackMedia
{
public:
    TinyMovTrackMedia(TinyMovTrack& parent) : _parent(parent), _info(*this) {}

    struct Flags_t
    {
        uint8_t values[3];
    };

    void Flags(uint8_t flags[3])
    {
        memcpy(_flags.values, flags, sizeof(_flags.values));
    }
    Flags_t Flags()
    {
        return _flags;
    }
    void CreationTime(uint64_t creation_time)
    {
        _creation_time = creation_time;
    }
    uint64_t CreationTime()
    {
        return _creation_time;
    }
    void ModificationTime(uint64_t modification_time)
    {
        _modification_time = modification_time;
    }
    uint64_t ModificationTime()
    {
        return _modification_time;
    }
    void TimeScale(uint32_t time_scale)
    {
        _time_scale = time_scale;
    }
    uint32_t TimeScale()
    {
        return _time_scale;
    }
    void Duration(uint64_t duration)
    {
        _duration = duration;
    }
    uint64_t Duration()
    {
        return _duration;
    }
    void Language(uint16_t language)
    {
        _language = language;
    }
    uint16_t Language()
    {
        return _language;
    }
    void Quality(uint16_t quality)
    {
        _quality = quality;
    }
    uint16_t Quality()
    {
        return _quality;
    }

    TinyMovTrackMediaInfo& Info()
    {
        return _info;
    }

    TinyMovTrack& Parent()
    {
        return _parent;
    }

protected:
    // Media Header
    Flags_t _flags;
    uint64_t _creation_time;
    uint64_t _modification_time;
    uint32_t _time_scale;
    uint64_t _duration;
    uint16_t _language;
    uint16_t _quality;

    TinyMovTrackMediaInfo _info;

    TinyMovTrack& _parent;
};

class TinyMovFile;
class TinyMovTrack
{
public:
    TinyMovTrack(TinyMovFile& parent) : _parent(parent), _media(*this) {}

    struct DisplayMatrix_t
    {
        uint32_t values[3][3];
    };
    struct Flags_t
    {
        uint8_t values[3];
    };

    void Flags(uint8_t flags[3])
    {
        memcpy(_flags.values, flags, sizeof(_flags.values));
    }
    Flags_t Flags()
    {
        return _flags;
    }
    void CreationTime(uint64_t creation_time)
    {
        _creation_time = creation_time;
    }
    uint64_t CreationTime()
    {
        return _creation_time;
    }
    void ModificationTime(uint64_t modification_time)
    {
        _modification_time = modification_time;
    }
    uint64_t ModificationTime()
    {
        return _modification_time;
    }
    void TrackId(uint32_t track_id)
    {
        _track_id = track_id;
    }
    uint32_t TrackId()
    {
        return _track_id;
    }
    void Duration(uint64_t duration)
    {
        _duration = duration;
    }
    uint64_t Duration()
    {
        return _duration;
    }
    void Layer(uint16_t layer)
    {
        _layer = layer;
    }
    uint16_t Layer()
    {
        return _layer;
    }
    void AlternativeGroup(uint16_t alternative_group)
    {
        _alternative_group = alternative_group;
    }
    uint16_t AlternativeGroup()
    {
        return _alternative_group;
    }
    void Volume(uint16_t volume)
    {
        _volume = volume;
    }
    uint16_t Volume()
    {
        return _volume;
    }
    void DisplayMatrix(uint32_t display_matrix[3][3])
    {
        memcpy(_display_matrix.values, display_matrix, sizeof(_display_matrix.values));
    }
    DisplayMatrix_t DisplayMatrix()
    {
        return _display_matrix;
    }
    void TrackWidth(uint32_t track_width)
    {
        _track_width = track_width;
    }
    uint32_t TrackWidth()
    {
        return _track_width;
    }
    void TrackHeight(uint32_t track_height)
    {
        _track_height = track_height;
    }
    uint32_t TrackHeight()
    {
        return _track_height;
    }

    TinyMovTrackMedia& Media()
    {
        return _media;
    }
    TinyMovFile& Parent()
    {
        return _parent;
    }
protected:
    // Track Header
    Flags_t _flags;
    uint64_t _creation_time;
    uint64_t _modification_time;
    uint32_t _track_id;
    uint64_t _duration;
    uint16_t _layer;
    uint16_t _alternative_group;
    uint16_t _volume;
    DisplayMatrix_t _display_matrix;
    uint32_t _track_width;
    uint32_t _track_height;

    // Track Media
    TinyMovTrackMedia _media;

    // Parent
    TinyMovFile& _parent;
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

    struct DisplayMatrix_t
    {
        uint32_t values[3][3];
    };
    struct Flags_t
    {
        uint8_t values[3];
    };

    void Flags(uint8_t flags[3])
    {
        memcpy(_flags.values, flags, sizeof(_flags.values));
    }
    Flags_t Flags()
    {
        return _flags;
    }
    void CreationTime(uint64_t creation_time)
    {
        _creation_time = creation_time;
    }
    uint64_t CreationTime()
    {
        return _creation_time;
    }
    void ModificationTime(uint64_t modification_time)
    {
        _modification_time = modification_time;
    }
    uint64_t ModificationTime()
    {
        return _modification_time;
    }
    void TimeScale(uint32_t time_scale)
    {
        _time_scale = time_scale;
    }
    uint32_t TimeScale()
    {
        return _time_scale;
    }
    void Duration(uint64_t duration)
    {
        _duration = duration;
    }
    uint64_t Duration()
    {
        return _duration;
    }
    void PreferredRate(uint32_t preferred_rate)
    {
        _preferred_rate = preferred_rate;
    }
    uint32_t PreferredRate()
    {
        return _preferred_rate;
    }
    void PreferredVolume(uint16_t preferred_volume)
    {
        _preferred_volume = preferred_volume;
    }
    uint16_t PreferredVolume()
    {
        return _preferred_volume;
    }
    void DisplayMatrix(uint32_t display_matrix[3][3])
    {
        memcpy(_display_matrix.values, display_matrix, sizeof(_display_matrix.values));
    }
    DisplayMatrix_t DisplayMatrix()
    {
        return _display_matrix;
    }
    void PreviewDuration(uint32_t preview_duration)
    {
        _preview_duration = preview_duration;
    }
    uint32_t PreviewDuration()
    {
        return _preview_duration;
    }
    void PosterTime(uint32_t poster_time)
    {
        _poster_time = poster_time;
    }
    uint32_t PosterTime()
    {
        return _poster_time;
    }
    void SelectionTime(uint32_t selection_time)
    {
        _selection_time = selection_time;
    }
    uint32_t SelectionTime()
    {
        return _selection_time;
    }
    void SelectionDuration(uint32_t selection_duration)
    {
        _selection_duration = selection_duration;
    }
    uint32_t SelectionDuration()
    {
        return _selection_duration;
    }
    void CurrentTime(uint32_t current_time)
    {
        _current_time = current_time;
    }
    uint32_t CurrentTime()
    {
        return _current_time;
    }
    void NextTrackId(uint32_t next_track_id)
    {
        _next_track_id = next_track_id;
    }
    uint32_t NextTrackId()
    {
        return _next_track_id;
    }

    void Path(std::string path)
    {
        _path = path;
    }
    std::string Path()
    {
        return _path;
    }
protected:
    std::vector<TinyMovTrack> _tracks;
    TinyMovMetadata _metadata;
    std::string _path;

    // Movie Header
    Flags_t _flags;
    uint64_t _creation_time;
    uint64_t _modification_time;
    uint32_t _time_scale;
    uint64_t _duration;
    uint32_t _preferred_rate;
    uint16_t _preferred_volume;
    DisplayMatrix_t _display_matrix;
    uint32_t _preview_duration;
    uint32_t _poster_time;
    uint32_t _selection_time;
    uint32_t _selection_duration;
    uint32_t _current_time;
    uint32_t _next_track_id;
};

class fstream_in_wrapper
{
public:
    fstream_in_wrapper(std::fstream &stream, int64_t initial_size) : _stream(stream)
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

#endif

class fstream_out_wrapper
{
public:
    fstream_out_wrapper(std::fstream &stream) :
        _file_stream(stream)
    {
        uint32_t valA = bswap_32(0x00000001);           // Use 64-bit size after 'mdat' tag
        _file_stream.write((char *)&valA, sizeof(valA));

        uint32_t valB = MKTAG('m', 'd', 'a', 't');
        _file_stream.write((char *)&valB, sizeof(valB));

        uint64_t valC = bswap_64(0x0000000000000000);   // This field will be filled in finalize_file()
        _file_stream.write((char *)&valC, sizeof(valC));
    }

    void put_byte(uint8_t value)
    {
        _post_atoms_stream.write((char *)&value, sizeof(value));
        _atom_offsets_stack.top() += sizeof(value);
    }
    void put_le16(uint16_t value)
    {
        _post_atoms_stream.write((char *)&value, sizeof(value));
        _atom_offsets_stack.top() += sizeof(value);
    }
    void put_be16(uint16_t value)
    {
        return put_le16(bswap_16(value));
    }
    void put_le32(uint32_t value)
    {
        _post_atoms_stream.write((char *)&value, sizeof(value));
        _atom_offsets_stack.top() += sizeof(value);
    }
    void put_be32(uint32_t value)
    {
        return put_le32(bswap_32(value));
    }
    void put_le64(uint64_t value)
    {
        _post_atoms_stream.write((char *)&value, sizeof(value));
        _atom_offsets_stack.top() += sizeof(value);
    }
    void put_be64(uint64_t value)
    {
        return put_le64(bswap_64(value));
    }
    void write(void* src, uint32_t size)
    {
        _post_atoms_stream.write((char *)src, size);
        _atom_offsets_stack.top() += size;
    }
    int64_t tellp()
    {
        return _post_atoms_stream.tellp();
    }

    void push_atom(uint32_t atom_id)
    {
        _atom_offsets_stack.push(0);
        put_be32(0x00000000);
        put_le32(atom_id);
    }
    void pop_atom()
    {
        auto atom_size = _atom_offsets_stack.top();
        uint32_t atom_size_be = bswap_32(atom_size);

        // Seek back to atom size and write calculated atom size
        _post_atoms_stream.seekp(-atom_size, _post_atoms_stream.cur);

        // Rewrite atom size field
        _post_atoms_stream.write((char *)&atom_size_be, sizeof(atom_size_be));

        // Seek back to end of the written atom
        _post_atoms_stream.seekp(atom_size - sizeof(atom_size_be), _post_atoms_stream.cur);

        // Update parent atom info
        _atom_offsets_stack.pop();
        _atom_offsets_stack.top() += atom_size;
    }

    void write_mdat_sample(void* src, uint32_t size)
    {
        _file_stream.write((char *)src, size);
    }
    int64_t tellp_mdat()
    {
        return _file_stream.tellp();
    }

    void finalize_file()
    {
        // Finalize 'mdat' atom
        auto mdat_atom_end = _file_stream.tellp();
        uint64_t mdat_atom_size_be = bswap_64(mdat_atom_end);

        // Seek back to atom size and write calculated atom size
        _file_stream.seekp(8, _file_stream.beg);
        _file_stream.write((char *)&mdat_atom_size_be, sizeof(mdat_atom_size_be));

        // Seek back to end of the written atom
        _file_stream.seekp(mdat_atom_end, _file_stream.beg);

        // Do other stuff
        _atom_offsets_stack = std::stack<int64_t>();
        _file_stream << _post_atoms_stream.rdbuf();
        _post_atoms_stream.clear();
    }

protected:
    std::stringstream _post_atoms_stream;
    std::fstream &_file_stream;
    std::stack<int64_t> _atom_offsets_stack;
};

class ITinyMovFileAtomProcessor
{
public:
    virtual ~ITinyMovFileAtomProcessor() {}

    virtual uint32_t Type() = 0;
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov) = 0;
};

std::vector<ITinyMovFileAtomProcessor*> TinyMovFileAtomProcessors;

class TinyMovFileAtomProcessorBase : public ITinyMovFileAtomProcessor
{
public:
    TinyMovFileAtomProcessorBase()
    {
        TinyMovFileAtomProcessors.push_back(this);
    }
};

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('f', 't', 'y', 'p');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
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
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovFile& mov)
    {
        stream.push_atom(Type());

        // major
        stream.put_le32(MKTAG('q', 't', ' ', ' '));

        // minor
        stream.put_le32(MKTAG(0x20, 0x04, 0x06, 0x00));

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_ftyp;

class TinyMovFileAtomProcessor_default_t : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return 0x00000000;
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
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
    
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovFile& mov) {return false;}
} TinyMovFileAtomProcessor_default;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'v', 'h', 'd');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
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
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovFile& mov)
    {
        stream.push_atom(Type());
        
        uint8_t version = 1;
        stream.put_byte(version);

        auto flags = mov.Flags();
        stream.put_byte(flags.values[0]);
        stream.put_byte(flags.values[1]);
        stream.put_byte(flags.values[2]);

        (version == 1) ? stream.put_be64(mov.CreationTime()) : stream.put_be32(mov.CreationTime());
        (version == 1) ? stream.put_be64(mov.ModificationTime()) : stream.put_be32(mov.ModificationTime());
        stream.put_be32(mov.TimeScale());
        (version == 1) ? stream.put_be64(mov.Duration()) : stream.put_be32(mov.Duration());
        stream.put_be32(mov.PreferredRate());
        stream.put_be16(mov.PreferredVolume());

        // reserved
        stream.put_le64(0);
        stream.put_le16(0);
        // ===

        auto display_matrix = mov.DisplayMatrix();
        for (int i = 0; i < 3; ++i)     // display matrix
        {
            stream.put_be32(display_matrix.values[i][0]);
            stream.put_be32(display_matrix.values[i][1]);
            stream.put_be32(display_matrix.values[i][2]);
        }
        
        stream.put_be32(mov.PreviewDuration());
        stream.put_be32(mov.PosterTime());
        stream.put_be32(mov.SelectionTime());
        stream.put_be32(mov.SelectionDuration());
        stream.put_be32(mov.CurrentTime());
        stream.put_be32(mov.NextTrackId());

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_mvhd;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('t', 'k', 'h', 'd');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
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
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovTrack& track)
    {
        stream.push_atom(Type());

        uint8_t version = 1;
        stream.put_byte(version);

        auto flags = track.Flags();
        stream.put_byte(flags.values[0]);
        stream.put_byte(flags.values[1]);
        stream.put_byte(flags.values[2]);

        (version == 1) ? stream.put_be64(track.CreationTime()) : stream.put_be32(track.CreationTime());
        (version == 1) ? stream.put_be64(track.ModificationTime()) : stream.put_be32(track.ModificationTime());
        stream.put_be32(track.TrackId());
        stream.put_be32(0);      // reserved
        (version == 1) ? stream.put_be64(track.Duration()) : stream.put_be32(track.Duration());
        stream.put_be64(0);      // reserved
        stream.put_be16(track.Layer());
        stream.put_be16(track.AlternativeGroup());
        stream.put_be16(track.Volume());
        stream.put_be16(0);      // reserved

        auto display_matrix = track.DisplayMatrix();
        for (int i = 0; i < 3; ++i)     // display matrix
        {
            stream.put_be32(display_matrix.values[i][0]);
            stream.put_be32(display_matrix.values[i][1]);
            stream.put_be32(display_matrix.values[i][2]);
        }

        stream.put_be32(track.TrackWidth());
        stream.put_be32(track.TrackWidth());

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_tkhd;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'd', 'h', 'd');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
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

    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovTrack& track)
    {
        stream.push_atom(Type());
        
        uint8_t version = 1;
        stream.put_byte(version);

        auto flags = track.Media().Flags();
        stream.put_byte(flags.values[0]);
        stream.put_byte(flags.values[1]);
        stream.put_byte(flags.values[2]);

        (version == 1) ?
            stream.put_be64(track.Media().CreationTime()) :
            stream.put_be32(track.Media().CreationTime());

        (version == 1) ?
            stream.put_be64(track.Media().ModificationTime()) :
            stream.put_be32(track.Media().ModificationTime());
        
        stream.put_be32(track.Media().TimeScale());

        (version == 1) ?
            stream.put_be64(track.Media().Duration()) :
            stream.put_be32(track.Media().Duration());

        stream.put_be16(track.Media().Language());
        stream.put_be16(track.Media().Quality());

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_mdhd;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('s', 't', 's', 'z');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
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

    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovTrack& track)
    {
        stream.push_atom(Type());

        // version
        stream.put_byte(0);

        // flags
        stream.put_byte(0);
        stream.put_byte(0);
        stream.put_byte(0);

        // sample size
        stream.put_be32(0);     // 0 because we always are ready for different sizes

        // entries (count)
        stream.put_be32(track.Media().Info().SampleSizes().size());

        for (auto it = track.Media().Info().SampleSizes().begin(); it != track.Media().Info().SampleSizes().end(); ++it)
            stream.put_be32(*it);

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_stsz;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('s', 't', 'c', 'o');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
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

    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovTrack& track)
    {
        stream.push_atom(Type());

        // version
        stream.put_byte(0);

        // flags
        stream.put_byte(0);
        stream.put_byte(0);
        stream.put_byte(0);

        // entries (count)
        stream.put_be32(track.Media().Info().SampleSizes().size());

        for (int i = 0; i < track.Media().Info().SampleSizes().size(); ++i)
        {
            auto& sample_offset = track.Media().Info().SampleOffsets()[i];
            auto& sample_size = track.Media().Info().SampleSizes()[i];

            // Get offset of new sample
            uint64_t offset = stream.tellp_mdat();

            std::fstream f_in(track.Parent().Path(), std::ios::in | std::ios::binary);
            if (!f_in.is_open())
                return false;

            std::vector<uint8_t> tmp_sample_data(sample_size);

            f_in.seekg(sample_offset);
            f_in.read((char*)tmp_sample_data.data(), sample_size);

            f_in.close();

            // Write sample
            stream.write_mdat_sample(tmp_sample_data.data(), tmp_sample_data.size());

            stream.put_be32(offset);
        }

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_stco;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('s', 't', 'b', 'l');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'i', 'n', 'f'))
        {
            std::cout << "Warning! Parent is not minf!" << std::endl;
            return false;
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovTrack& track)
    {
        stream.push_atom(Type());

        // stsd
        // stts
        // stsc
        
        // Add 'stsz' atom
        if (!TinyMovFileAtomProcessor_stsz.ProcessWrite(stream, track))
            return false;
        
        // Add 'stco' atom
        if (!TinyMovFileAtomProcessor_stco.ProcessWrite(stream, track))
            return false;

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_stbl;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'i', 'n', 'f');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'd', 'i', 'a'))
        {
            std::cout << "Warning! Parent is not mdia!" << std::endl;
            return false;
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovTrack& track)
    {
        stream.push_atom(Type());

        // ...

        // Add 'stbl' atom
        if (!TinyMovFileAtomProcessor_stbl.ProcessWrite(stream, track))
            return false;
        
        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_minf;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'd', 'i', 'a');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('t', 'r', 'a', 'k'))
        {
            std::cout << "Warning! Parent is not trak!" << std::endl;
            return false;
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }

    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovTrack& track)
    {
        stream.push_atom(Type());

        // Add 'mdhd' atom
        if (!TinyMovFileAtomProcessor_mdhd.ProcessWrite(stream, track))
            return false;

        // Add 'minf' atom
        if (!TinyMovFileAtomProcessor_minf.ProcessWrite(stream, track))
            return false;

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_mdia;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('t', 'r', 'a', 'k');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'o', 'o', 'v'))
        {
            std::cout << "Warning! Parent is not moov!" << std::endl;
            return false;
        }

        TinyMovTrack track(mov);

        stream.set_current_track(&track);
        auto result = TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
        stream.set_current_track(nullptr);

        mov.Tracks().push_back(track);

        return result;
    }

    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovTrack& track)
    {
        stream.push_atom(Type());

        // Add 'tkhd' atom
        if (!TinyMovFileAtomProcessor_tkhd.ProcessWrite(stream, track))
            return false;

        // Add 'mdia' atom
        if (!TinyMovFileAtomProcessor_mdia.ProcessWrite(stream, track))
            return false;
        
        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_trak;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('k', 'e', 'y', 's');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'e', 't', 'a'))
        {
            std::cout << "Warning! Parent is not meta!" << std::endl;
            return false;
        }

        stream.seekg(4);    // Skip

        uint32_t count = stream.get_be32();

        for (int i = 0; i < count; ++i)
        {
            auto sz = stream.get_be32() - 8;

            auto ns = stream.get_be32();

            auto str_text = new char[sz];
            stream.read(str_text, sz);
            std::vector<uint8_t> text_data(str_text, str_text + sz);

            TinyMovMetadataKey key;
            key.Data(text_data);
            key.Namespace(ns);

            mov.Metadata().Keys().push_back(key);

            delete[] str_text;
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovFile& mov)
    {
        stream.push_atom(Type());

        // version
        stream.put_byte(0);

        // flags
        stream.put_byte(0);
        stream.put_byte(0);
        stream.put_byte(0);

        // count
        stream.put_be32(mov.Metadata().Keys().size());
        
        // keys
        for (auto it = mov.Metadata().Keys().begin(); it != mov.Metadata().Keys().end(); ++it)
        {
            stream.put_be32(it->Data().size() + 8);
            stream.put_be32(it->Namespace());
            stream.write((char*)it->Data().data(), it->Data().size());
        }

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_keys;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('d', 'a', 't', 'a');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
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

        uint8_t* text = new uint8_t[size];
        stream.read((char*)text, size);
        std::vector<uint8_t> text_data(text, text + size);

        TinyMovMetadataValue val;
        val.Type(type_indicator);
        val.Locale(locale_indicator);
        val.Data(text_data);

        delete[] text;

        mov.Metadata().Values().push_back(val);

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovMetadataValue& value)
    {
        stream.push_atom(Type());
        
        stream.put_be32(value.Type());
        stream.put_be32(value.Locale());
        stream.write((char*)value.Data().data(), value.Data().size());

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_data;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('i', 'l', 's', 't');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
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
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovFile& mov)
    {
        stream.push_atom(Type());

        auto& values = mov.Metadata().Values();

        for (int i = 0; i < values.size(); ++i)
        {
            stream.push_atom(bswap_32(i + 1));

            if(!TinyMovFileAtomProcessor_data.ProcessWrite(stream, values[i]))
                return false;

            stream.pop_atom();
        }

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_ilst;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('h', 'd', 'l', 'r');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'e', 't', 'a') &&
            stream.atom_parent_id() != MKTAG('m', 'd', 'i', 'a') &&
            stream.atom_parent_id() != MKTAG('m', 'i', 'n', 'f'))
        {
            std::cout << "Warning! Parent is not meta or mdia!" << std::endl;
            return false;
        }
        
        // version
        stream.get_byte();

        // flags
        stream.get_byte();
        stream.get_byte();
        stream.get_byte();

        // component type
        stream.get_be32();

        // component sub-type
        stream.get_le32();

        // component manufacturer
        stream.get_be32();

        // component flags
        stream.get_be32();

        // component flags mask
        stream.get_be32();

        // component name
        stream.get_byte();

        return true;
    }
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovFile& mov)
    {
        stream.push_atom(Type());
        
        // version
        stream.put_byte(0);

        // flags
        stream.put_byte(0);
        stream.put_byte(0);
        stream.put_byte(0);

        // component type
        stream.put_be32(0);

        // component sub-type
        stream.put_le32(MKTAG('m', 'd', 't', 'a'));

        // component manufacturer
        stream.put_be32(0);

        // component flags
        stream.put_be32(0);

        // component flags mask
        stream.put_be32(0);

        // component name
        stream.put_byte(0);

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_hdlr;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'e', 't', 'a');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('m', 'o', 'o', 'v'))
        {
            std::cout << "Warning! Parent is not moov!" << std::endl;
            return false;
        }
        
        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovFile& mov)
    {
        stream.push_atom(Type());

        // Add 'hdlr' atom
        if (!TinyMovFileAtomProcessor_hdlr.ProcessWrite(stream, mov))
            return false;
            
        // Add 'keys' atom
        if (!TinyMovFileAtomProcessor_keys.ProcessWrite(stream, mov))
            return false;

        // Add 'ilst' atom
        if (!TinyMovFileAtomProcessor_ilst.ProcessWrite(stream, mov))
            return false;

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_meta;

class : public TinyMovFileAtomProcessorBase
{
public:
    virtual uint32_t Type()
    {
        return MKTAG('m', 'o', 'o', 'v');
    }
    virtual bool ProcessRead(fstream_in_wrapper& stream, TinyMovFile& mov)
    {
        if (stream.atom_parent_id() != MKTAG('R', 'O', 'O', 'T'))
        {
            std::cout << "Warning! Parent is not ROOT!" << std::endl;
            return false;
        }

        return TinyMovFileAtomProcessor_default.ProcessRead(stream, mov);
    }
    bool ProcessWrite(fstream_out_wrapper& stream, TinyMovFile& mov)
    {
        stream.push_atom(Type());

        // Add 'mvhd' atom
        if (!TinyMovFileAtomProcessor_mvhd.ProcessWrite(stream, mov))
            return false;

        auto& tracks = mov.Tracks();
        for (int i = 0; i < tracks.size(); ++i)
        {
            // Add 'trak' atom
            if (!TinyMovFileAtomProcessor_trak.ProcessWrite(stream, tracks[i]))
                return false;
        }

        // Add 'meta' atom
        if (!TinyMovFileAtomProcessor_meta.ProcessWrite(stream, mov))
            return false;

        stream.pop_atom();
        return true;
    }
} TinyMovFileAtomProcessor_moov;

class TinyMovFileReader
{
public:
    TinyMovFile OpenMovFile(std::string path)
    {
        TinyMovFile mov;
        mov.Path(path);

        std::fstream f_in(path, std::ios::in | std::ios::binary);
        
        auto fsize = f_in.tellg();
        f_in.seekg(0, std::ios::end);
        fsize = f_in.tellg() - fsize;
        f_in.seekg(0, std::ios::beg);

        fstream_in_wrapper f_in_wrapper(f_in, fsize);
        TinyMovFileAtomProcessor_default.ProcessRead(f_in_wrapper, mov);

        return mov;
    }
};

class TinyMovFileWriter
{
public:
    bool SaveMovFile(TinyMovFile& mov, std::string path)
    {
        std::fstream f_out(path, std::ios::out | std::ios::binary);
        if (!f_out.is_open())
            return false;

        fstream_out_wrapper f_out_wrapper(f_out);

        // Add 'ftyp' atom
        if (!TinyMovFileAtomProcessor_ftyp.ProcessWrite(f_out_wrapper, mov))
        {
            f_out_wrapper.finalize_file();
            return false;
        }

        // Add 'moov' atom
        if (!TinyMovFileAtomProcessor_moov.ProcessWrite(f_out_wrapper, mov))
        {
            f_out_wrapper.finalize_file();
            return false;
        }

        // ('mdat' is written automatically)
        f_out_wrapper.finalize_file();

        return true;
    }
};

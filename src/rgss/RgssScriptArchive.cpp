#include <zlib.h>

#include "RgssScriptArchiveBridge.h"

namespace
{
class TMarshalReader
{
public:
    explicit TMarshalReader(const EasyRpgRgssScriptArchiveReader& source)
        : iSource(source), iPosition(0), iBufferStart(-1), iBufferLength(0)
    {
    }

    int Position() const
    {
        return iPosition;
    }

    int Size() const
    {
        return iSource.size;
    }

    int ReadByte(unsigned char& value)
    {
        if (iPosition < 0 || iPosition >= iSource.size)
            return ERgssScriptArchiveErrorCorrupt;
        if (iBufferStart < 0 || iPosition < iBufferStart ||
            iPosition >= iBufferStart + iBufferLength)
        {
            const int result = Fill();
            if (result != ERgssScriptArchiveOk)
                return result;
        }
        value = iBuffer[iPosition - iBufferStart];
        ++iPosition;
        return ERgssScriptArchiveOk;
    }

    int Skip(int bytes)
    {
        if (bytes < 0 || iPosition < 0 || iPosition > iSource.size ||
            bytes > iSource.size - iPosition)
            return ERgssScriptArchiveErrorCorrupt;
        iPosition += bytes;
        return ERgssScriptArchiveOk;
    }

    int ReadFixnumRaw(int& value)
    {
        unsigned char firstByte = 0;
        int result = ReadByte(firstByte);
        if (result != ERgssScriptArchiveOk)
            return result;
        int first = static_cast<int>(firstByte);
        if (first >= 128)
            first -= 256;
        if (first == 0)
        {
            value = 0;
            return ERgssScriptArchiveOk;
        }
        if (first > 4)
        {
            value = first - 5;
            return ERgssScriptArchiveOk;
        }
        if (first < -4)
        {
            value = first + 5;
            return ERgssScriptArchiveOk;
        }

        if (first > 0)
        {
            unsigned int raw = 0;
            for (int index = 0; index < first; ++index)
            {
                unsigned char byte = 0;
                result = ReadByte(byte);
                if (result != ERgssScriptArchiveOk)
                    return result;
                raw |= static_cast<unsigned int>(byte) << (index * 8);
            }
            if (raw > 0x7fffffffu)
                return ERgssScriptArchiveErrorUnsupported;
            value = static_cast<int>(raw);
            return ERgssScriptArchiveOk;
        }

        const int length = -first;
        unsigned int raw = 0xffffffffu;
        for (int index = 0; index < length; ++index)
        {
            unsigned char byte = 0;
            result = ReadByte(byte);
            if (result != ERgssScriptArchiveOk)
                return result;
            const unsigned int shift = static_cast<unsigned int>(index * 8);
            raw &= ~(0xffu << shift);
            raw |= static_cast<unsigned int>(byte) << shift;
        }
        value = static_cast<int>(raw);
        return ERgssScriptArchiveOk;
    }

private:
    int Fill()
    {
        if (!iSource.read || iPosition < 0 || iPosition >= iSource.size)
            return ERgssScriptArchiveErrorRead;
        int bytes = iSource.size - iPosition;
        if (bytes > static_cast<int>(sizeof(iBuffer)))
            bytes = static_cast<int>(sizeof(iBuffer));
        const int read = iSource.read(iSource.context, iPosition, iBuffer, bytes);
        if (read != bytes)
            return ERgssScriptArchiveErrorRead;
        iBufferStart = iPosition;
        iBufferLength = bytes;
        return ERgssScriptArchiveOk;
    }

private:
    EasyRpgRgssScriptArchiveReader iSource;
    int iPosition;
    int iBufferStart;
    int iBufferLength;
    unsigned char iBuffer[512];
};

int ReadLength(TMarshalReader& reader, int& length)
{
    const int result = reader.ReadFixnumRaw(length);
    if (result != ERgssScriptArchiveOk)
        return result;
    if (length < 0)
        return ERgssScriptArchiveErrorCorrupt;
    return ERgssScriptArchiveOk;
}

int SkipObject(TMarshalReader& reader);

int SkipArray(TMarshalReader& reader)
{
    int count = 0;
    int result = ReadLength(reader, count);
    if (result != ERgssScriptArchiveOk)
        return result;
    for (int index = 0; index < count; ++index)
    {
        result = SkipObject(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
    }
    return ERgssScriptArchiveOk;
}

int SkipHash(TMarshalReader& reader, int hasDefault)
{
    int count = 0;
    int result = ReadLength(reader, count);
    if (result != ERgssScriptArchiveOk)
        return result;
    for (int index = 0; index < count; ++index)
    {
        result = SkipObject(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
        result = SkipObject(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
    }
    return hasDefault ? SkipObject(reader) : ERgssScriptArchiveOk;
}

int SkipIvars(TMarshalReader& reader)
{
    int count = 0;
    int result = ReadLength(reader, count);
    if (result != ERgssScriptArchiveOk)
        return result;
    for (int index = 0; index < count; ++index)
    {
        result = SkipObject(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
        result = SkipObject(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
    }
    return ERgssScriptArchiveOk;
}

int SkipSizedBytes(TMarshalReader& reader)
{
    int length = 0;
    const int result = ReadLength(reader, length);
    if (result != ERgssScriptArchiveOk)
        return result;
    return reader.Skip(length);
}

int SkipObject(TMarshalReader& reader)
{
    unsigned char marker = 0;
    int result = reader.ReadByte(marker);
    if (result != ERgssScriptArchiveOk)
        return result;

    switch (marker)
    {
    case '0':
    case 'T':
    case 'F':
        return ERgssScriptArchiveOk;
    case 'i':
    {
        int value = 0;
        return reader.ReadFixnumRaw(value);
    }
    case '"':
    case ':':
    case 'f':
    case 'c':
    case 'm':
    case 'M':
        return SkipSizedBytes(reader);
    case ';':
    case '@':
    {
        int link = 0;
        return reader.ReadFixnumRaw(link);
    }
    case '[':
        return SkipArray(reader);
    case '{':
        return SkipHash(reader, 0);
    case '}':
        return SkipHash(reader, 1);
    case 'I':
        result = SkipObject(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
        return SkipIvars(reader);
    case '/':
        result = SkipSizedBytes(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
    {
        unsigned char options = 0;
        return reader.ReadByte(options);
    }
    case 'l':
    {
        unsigned char sign = 0;
        result = reader.ReadByte(sign);
        if (result != ERgssScriptArchiveOk || (sign != '+' && sign != '-'))
            return ERgssScriptArchiveErrorCorrupt;
        int words = 0;
        result = ReadLength(reader, words);
        if (result != ERgssScriptArchiveOk)
            return result;
        if (words > 0x3fffffff)
            return ERgssScriptArchiveErrorCorrupt;
        return reader.Skip(words * 2);
    }
    case 'o':
    case 'S':
    {
        result = SkipObject(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
        return SkipIvars(reader);
    }
    case 'C':
    case 'e':
    case 'U':
        result = SkipObject(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
        return SkipObject(reader);
    case 'u':
        result = SkipObject(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
        return SkipSizedBytes(reader);
    default:
        return ERgssScriptArchiveErrorUnsupported;
    }
}

int ReadIntegerObject(TMarshalReader& reader, int& value)
{
    unsigned char marker = 0;
    int result = reader.ReadByte(marker);
    if (result != ERgssScriptArchiveOk)
        return result;
    if (marker != 'i')
        return ERgssScriptArchiveErrorUnsupported;
    return reader.ReadFixnumRaw(value);
}

int ReadStringLike(TMarshalReader& reader, int& offset, int& length)
{
    unsigned char marker = 0;
    int result = reader.ReadByte(marker);
    if (result != ERgssScriptArchiveOk)
        return result;

    if (marker == 'I')
    {
        result = ReadStringLike(reader, offset, length);
        if (result != ERgssScriptArchiveOk)
            return result;
        return SkipIvars(reader);
    }
    if (marker != '"')
        return ERgssScriptArchiveErrorUnsupported;

    result = ReadLength(reader, length);
    if (result != ERgssScriptArchiveOk)
        return result;
    offset = reader.Position();
    return reader.Skip(length);
}

int ReadScriptEntry(TMarshalReader& reader, EasyRpgRgssScriptEntry& entry)
{
    unsigned char marker = 0;
    int result = reader.ReadByte(marker);
    if (result != ERgssScriptArchiveOk)
        return result;
    if (marker != '[')
        return ERgssScriptArchiveErrorCorrupt;

    int fields = 0;
    result = ReadLength(reader, fields);
    if (result != ERgssScriptArchiveOk)
        return result;
    if (fields < 3)
        return ERgssScriptArchiveErrorCorrupt;

    result = ReadIntegerObject(reader, entry.id);
    if (result != ERgssScriptArchiveOk)
        return result;
    result = ReadStringLike(reader, entry.name_offset, entry.name_length);
    if (result != ERgssScriptArchiveOk)
        return result;
    result = ReadStringLike(reader, entry.data_offset, entry.data_length);
    if (result != ERgssScriptArchiveOk)
        return result;
    for (int index = 3; index < fields; ++index)
    {
        result = SkipObject(reader);
        if (result != ERgssScriptArchiveOk)
            return result;
    }
    return ERgssScriptArchiveOk;
}
}

namespace
{
int InspectArchive(const EasyRpgRgssScriptArchiveReader* reader,
                   EasyRpgRgssScriptEntry* entries,
                   int capacity,
                   int* count,
                   int validateOnly)
{
    if (!reader || !reader->read || reader->size < 4 || capacity < 0 || !count)
        return ERgssScriptArchiveErrorInvalidArgument;

    TMarshalReader marshal(*reader);
    unsigned char major = 0;
    unsigned char minor = 0;
    unsigned char marker = 0;
    int result = marshal.ReadByte(major);
    if (result != ERgssScriptArchiveOk)
        return result;
    result = marshal.ReadByte(minor);
    if (result != ERgssScriptArchiveOk)
        return result;
    result = marshal.ReadByte(marker);
    if (result != ERgssScriptArchiveOk)
        return result;
    if (major != 4 || minor > 8 || marker != '[')
        return ERgssScriptArchiveErrorCorrupt;

    int scriptCount = 0;
    result = ReadLength(marshal, scriptCount);
    if (result != ERgssScriptArchiveOk)
        return result;
    if (scriptCount > 65535)
        return ERgssScriptArchiveErrorUnsupported;
    *count = scriptCount;
    if (!validateOnly &&
        (scriptCount > capacity || (scriptCount > 0 && !entries)))
        return ERgssScriptArchiveErrorCapacity;

    for (int index = 0; index < scriptCount; ++index)
    {
        EasyRpgRgssScriptEntry temporary;
        EasyRpgRgssScriptEntry& entry = validateOnly ? temporary : entries[index];
        result = ReadScriptEntry(marshal, entry);
        if (result != ERgssScriptArchiveOk)
            return result;
    }
    if (marshal.Position() != marshal.Size())
        return ERgssScriptArchiveErrorCorrupt;
    return ERgssScriptArchiveOk;
}

struct TInflateAllocator
{
    EasyRpgRgssScriptArchiveAllocFn allocate;
    EasyRpgRgssScriptArchiveFreeFn release;
    void* context;
};

voidpf ScriptZAlloc(voidpf opaque, uInt items, uInt size)
{
    TInflateAllocator* allocator = static_cast<TInflateAllocator*>(opaque);
    if (!allocator || !allocator->allocate || items == 0 || size == 0 ||
        items > 0xffffffffu / size)
        return Z_NULL;
    return allocator->allocate(allocator->context,
                               static_cast<unsigned int>(items) *
                               static_cast<unsigned int>(size));
}

void ScriptZFree(voidpf opaque, voidpf address)
{
    TInflateAllocator* allocator = static_cast<TInflateAllocator*>(opaque);
    if (allocator && allocator->release && address)
        allocator->release(allocator->context, address);
}

int GrowOutputBuffer(TInflateAllocator& allocator,
                     unsigned char** buffer,
                     int oldCapacity,
                     int minNewCapacity,
                     int hardLimit)
{
    int newCapacity = oldCapacity < 4096 ? 4096 : oldCapacity * 2;
    if (newCapacity < minNewCapacity)
        newCapacity = minNewCapacity;
    if (newCapacity > hardLimit)
        newCapacity = hardLimit;
    if (newCapacity <= oldCapacity)
        return 0;
    unsigned char* grown = static_cast<unsigned char*>(
        allocator.allocate(allocator.context, static_cast<unsigned int>(newCapacity)));
    if (!grown)
        return 0;
    if (oldCapacity > 0)
        for (int index = 0; index < oldCapacity; ++index)
            grown[index] = (*buffer)[index];
    if (*buffer)
        allocator.release(allocator.context, *buffer);
    *buffer = grown;
    return newCapacity;
}

int InflatePass(const EasyRpgRgssScriptArchiveReader& reader,
                const EasyRpgRgssScriptEntry& entry,
                TInflateAllocator& allocator,
                unsigned char** buffer,
                int& capacity,
                int maxOutputBytes,
                int& outputLength)
{
    unsigned char inputBuffer[1024];
    z_stream stream;
    stream.next_in = Z_NULL;
    stream.avail_in = 0;
    stream.total_in = 0;
    stream.next_out = Z_NULL;
    stream.avail_out = 0;
    stream.total_out = 0;
    stream.msg = Z_NULL;
    stream.state = Z_NULL;
    stream.zalloc = ScriptZAlloc;
    stream.zfree = ScriptZFree;
    stream.opaque = &allocator;
    stream.data_type = Z_BINARY;
    stream.adler = 0;
    stream.reserved = 0;

    const int initResult = inflateInit(&stream);
    if (initResult != Z_OK)
        return initResult == Z_MEM_ERROR
            ? ERgssScriptArchiveErrorNoMemory
            : ERgssScriptArchiveErrorCorrupt;

    int result = ERgssScriptArchiveOk;
    int inputPosition = 0;
    int done = 0;
    while (!done)
    {
        if (stream.avail_in == 0 && inputPosition < entry.data_length)
        {
            int chunk = entry.data_length - inputPosition;
            if (chunk > static_cast<int>(sizeof(inputBuffer)))
                chunk = static_cast<int>(sizeof(inputBuffer));
            const int read = reader.read(reader.context,
                                         entry.data_offset + inputPosition,
                                         inputBuffer,
                                         chunk);
            if (read != chunk)
            {
                result = ERgssScriptArchiveErrorRead;
                break;
            }
            inputPosition += chunk;
            stream.next_in = inputBuffer;
            stream.avail_in = static_cast<uInt>(chunk);
        }

        const unsigned long beforeOutput = stream.total_out;
        if (stream.total_out > static_cast<unsigned long>(capacity))
        {
            result = ERgssScriptArchiveErrorCorrupt;
            break;
        }
        int remaining = capacity - static_cast<int>(stream.total_out);
        if (remaining <= 0)
        {
            const int grown = GrowOutputBuffer(allocator,
                                               buffer,
                                               capacity,
                                               static_cast<int>(stream.total_out) + 1,
                                               maxOutputBytes + 1);
            if (!grown)
            {
                result = stream.total_out > static_cast<unsigned long>(maxOutputBytes)
                    ? ERgssScriptArchiveErrorCapacity
                    : ERgssScriptArchiveErrorNoMemory;
                break;
            }
            capacity = grown;
            remaining = capacity - static_cast<int>(stream.total_out);
        }
        stream.next_out = *buffer + stream.total_out;
        stream.avail_out = static_cast<uInt>(remaining);

        const int inflateResult = inflate(&stream, Z_NO_FLUSH);
        if (stream.total_out > static_cast<unsigned long>(maxOutputBytes))
        {
            result = ERgssScriptArchiveErrorCapacity;
            break;
        }
        if (inflateResult == Z_STREAM_END)
        {
            done = 1;
            break;
        }
        if (inflateResult == Z_MEM_ERROR)
        {
            result = ERgssScriptArchiveErrorNoMemory;
            break;
        }
        if (inflateResult != Z_OK)
        {
            result = ERgssScriptArchiveErrorCorrupt;
            break;
        }
        if (stream.total_out == beforeOutput &&
            stream.avail_in == 0 && inputPosition >= entry.data_length)
        {
            result = ERgssScriptArchiveErrorCorrupt;
            break;
        }
    }

    if (result == ERgssScriptArchiveOk &&
        (stream.total_in != static_cast<unsigned long>(entry.data_length) || !done))
        result = ERgssScriptArchiveErrorCorrupt;
    if (stream.total_out > 0x7fffffffu)
        result = ERgssScriptArchiveErrorCapacity;
    outputLength = static_cast<int>(stream.total_out);
    inflateEnd(&stream);
    return result;
}
}

extern "C" int EasyRpgRgssScriptArchiveInspect(
    const EasyRpgRgssScriptArchiveReader* reader,
    EasyRpgRgssScriptEntry* entries,
    int capacity,
    int* count)
{
    return InspectArchive(reader, entries, capacity, count, 0);
}

extern "C" int EasyRpgRgssScriptArchiveValidate(
    const EasyRpgRgssScriptArchiveReader* reader,
    int* count)
{
    return InspectArchive(reader, 0, 0, count, 1);
}

extern "C" int EasyRpgRgssScriptArchiveInflate(
    const EasyRpgRgssScriptArchiveReader* reader,
    const EasyRpgRgssScriptEntry* entry,
    EasyRpgRgssScriptArchiveAllocFn allocate,
    EasyRpgRgssScriptArchiveFreeFn release,
    void* allocation_context,
    int max_output_bytes,
    unsigned char** output,
    int* output_length)
{
    if (!reader || !reader->read || reader->size < 0 || !entry ||
        !allocate || !release || max_output_bytes <= 0 || !output || !output_length ||
        entry->data_offset < 0 || entry->data_length < 0 ||
        entry->data_offset > reader->size ||
        entry->data_length > reader->size - entry->data_offset)
        return ERgssScriptArchiveErrorInvalidArgument;

    *output = 0;
    *output_length = 0;
    TInflateAllocator allocator;
    allocator.allocate = allocate;
    allocator.release = release;
    allocator.context = allocation_context;

    // Single inflate pass: start from a compression-ratio estimate of the
    // final size and grow the buffer geometrically when it was too small.
    // The previous two-pass design (measure, then re-inflate) decoded every
    // script twice, which dominated boot time on large script archives.
    long estimate = static_cast<long>(entry->data_length) * 4 + 4096;
    if (estimate > max_output_bytes)
        estimate = max_output_bytes;
    if (estimate < 4096)
        estimate = 4096;
    int capacity = static_cast<int>(estimate) + 1;

    unsigned char* data = static_cast<unsigned char*>(
        allocate(allocation_context, static_cast<unsigned int>(capacity)));
    if (!data)
        return ERgssScriptArchiveErrorNoMemory;

    int actual = 0;
    int result = InflatePass(*reader,
                             *entry,
                             allocator,
                             &data,
                             capacity,
                             max_output_bytes,
                             actual);
    if (result == ERgssScriptArchiveOk && capacity < actual + 1)
    {
        const int grown = GrowOutputBuffer(allocator,
                                           &data,
                                           capacity,
                                           actual + 1,
                                           max_output_bytes + 1);
        result = grown ? ERgssScriptArchiveOk : ERgssScriptArchiveErrorNoMemory;
        if (grown)
            capacity = grown;
    }
    if (result != ERgssScriptArchiveOk || actual < 0)
    {
        release(allocation_context, data);
        return result != ERgssScriptArchiveOk
            ? result : ERgssScriptArchiveErrorCorrupt;
    }

    data[actual] = 0;
    *output = data;
    *output_length = actual;
    return ERgssScriptArchiveOk;
}

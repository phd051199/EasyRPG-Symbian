#include "easyrpgsymbian/CoreLcf.h"

namespace
{
struct ByteCursor
{
    const unsigned char* data;
    unsigned int size;
    unsigned int position;
};

bool ReadBer(ByteCursor& aCursor, unsigned int& aValue)
{
    unsigned int value = 0;
    for (int index = 0; index < 5; ++index)
    {
        if (aCursor.position >= aCursor.size)
            return false;

        const unsigned int octet = aCursor.data[aCursor.position++];
        if (value > 0x01ffffffu)
            return false;

        value = (value << 7) | (octet & 0x7fu);
        if ((octet & 0x80u) == 0)
        {
            aValue = value;
            return true;
        }
    }
    return false;
}

bool ReadChunk(ByteCursor& aCursor,
               unsigned int& aTag,
               const unsigned char*& aData,
               unsigned int& aLength,
               bool& aTerminator)
{
    aData = 0;
    aLength = 0;
    aTerminator = false;

    if (!ReadBer(aCursor, aTag))
        return false;
    if (aTag == 0)
    {
        aTerminator = true;
        return true;
    }

    if (!ReadBer(aCursor, aLength))
        return false;
    if (aLength > aCursor.size - aCursor.position)
        return false;

    aData = aCursor.data + aCursor.position;
    aCursor.position += aLength;
    return true;
}

bool ReadChunkInteger(const unsigned char* aData,
                      unsigned int aLength,
                      unsigned int& aValue)
{
    if (!aData || aLength == 0)
        return false;

    ByteCursor cursor = { aData, aLength, 0 };
    if (!ReadBer(cursor, aValue))
        return false;
    return cursor.position == cursor.size;
}

bool BytesEqual(const unsigned char* aLeft,
                const char* aRight,
                unsigned int aLength)
{
    for (unsigned int index = 0; index < aLength; ++index)
    {
        if (aLeft[index] != static_cast<unsigned char>(aRight[index]))
            return false;
    }
    return true;
}

unsigned int Fnv1a(const unsigned char* aData, unsigned int aSize)
{
    unsigned int hash = 2166136261u;
    for (unsigned int index = 0; index < aSize; ++index)
    {
        hash ^= aData[index];
        hash *= 16777619u;
    }
    return hash;
}

unsigned short ReadLittleEndian16(const unsigned char* aData)
{
    return static_cast<unsigned short>(
        static_cast<unsigned int>(aData[0]) |
        (static_cast<unsigned int>(aData[1]) << 8));
}

bool ReadHeader(ByteCursor& aCursor, const char* aExpected, unsigned int aExpectedLength)
{
    unsigned int headerLength = 0;
    if (!ReadBer(aCursor, headerLength))
        return false;
    if (headerLength != aExpectedLength)
        return false;
    if (headerLength > aCursor.size - aCursor.position)
        return false;
    if (!BytesEqual(aCursor.data + aCursor.position, aExpected, headerLength))
        return false;

    aCursor.position += headerLength;
    return true;
}

void CopyResourceName(unsigned char* aDestination,
                      unsigned int& aDestinationLength,
                      const unsigned char* aSource,
                      unsigned int aSourceLength)
{
    aDestinationLength = 0;
    if (!aDestination || !aSource || aSourceLength == 0)
        return;

    unsigned int copyLength = aSourceLength;
    if (copyLength >= EASYRPG_LCF_RESOURCE_NAME_CAPACITY)
        copyLength = EASYRPG_LCF_RESOURCE_NAME_CAPACITY - 1;

    for (unsigned int index = 0; index < copyLength; ++index)
        aDestination[index] = aSource[index];
    aDestination[copyLength] = 0;
    aDestinationLength = copyLength;
}

bool FindChipsetNameInArray(const unsigned char* aData,
                            unsigned int aLength,
                            unsigned int aChipsetId,
                            unsigned char* aName,
                            unsigned int aNameCapacity,
                            unsigned int& aNameLength)
{
    aNameLength = 0;
    if (!aData || aLength == 0 || !aName || aNameCapacity == 0)
        return false;

    ByteCursor cursor = { aData, aLength, 0 };
    unsigned int count = 0;
    if (!ReadBer(cursor, count))
        return false;

    for (unsigned int index = 0; index < count; ++index)
    {
        unsigned int entryId = 0;
        if (!ReadBer(cursor, entryId))
            return false;

        bool terminatorSeen = false;
        while (cursor.position < cursor.size)
        {
            unsigned int tag = 0;
            unsigned int length = 0;
            const unsigned char* data = 0;
            bool terminator = false;
            if (!ReadChunk(cursor, tag, data, length, terminator))
                return false;
            if (terminator)
            {
                terminatorSeen = true;
                break;
            }

            if (entryId == aChipsetId && tag == 0x02)
            {
                if (length == 0 || length >= aNameCapacity)
                    return false;
                for (unsigned int byteIndex = 0; byteIndex < length; ++byteIndex)
                    aName[byteIndex] = data[byteIndex];
                aName[length] = 0;
                aNameLength = length;
            }
        }

        if (!terminatorSeen)
            return false;
        if (entryId == aChipsetId && aNameLength > 0)
            return true;
    }

    return false;
}

bool ExtractChipsetInArray(const unsigned char* aData,
                           unsigned int aLength,
                           unsigned int aChipsetId,
                           EasyRpgCoreChipsetData& aChipset)
{
    if (!aData || aLength == 0 || aChipsetId == 0)
        return false;

    ByteCursor cursor = { aData, aLength, 0 };
    unsigned int count = 0;
    if (!ReadBer(cursor, count))
        return false;

    for (unsigned int index = 0; index < count; ++index)
    {
        unsigned int entryId = 0;
        if (!ReadBer(cursor, entryId) || entryId == 0)
            return false;

        bool terminatorSeen = false;
        while (cursor.position < cursor.size)
        {
            unsigned int tag = 0;
            unsigned int length = 0;
            const unsigned char* data = 0;
            bool terminator = false;
            if (!ReadChunk(cursor, tag, data, length, terminator))
                return false;
            if (terminator)
            {
                terminatorSeen = true;
                break;
            }

            if (entryId != aChipsetId)
                continue;

            if (tag == 0x02)
            {
                if (length == 0 || length >= EASYRPG_LCF_RESOURCE_NAME_CAPACITY)
                    return false;
                CopyResourceName(aChipset.name,
                                 aChipset.name_length,
                                 data,
                                 length);
            }
            else if (tag == 0x03)
            {
                if ((length & 1u) != 0 || length / 2u > 162u)
                    return false;
                aChipset.terrain_count = length / 2u;
                for (unsigned int valueIndex = 0;
                     valueIndex < aChipset.terrain_count;
                     ++valueIndex)
                {
                    aChipset.terrain[valueIndex] =
                        ReadLittleEndian16(data + valueIndex * 2u);
                }
            }
            else if (tag == 0x04)
            {
                if (length > 162u)
                    return false;
                aChipset.lower_passability_count = length;
                for (unsigned int valueIndex = 0; valueIndex < length; ++valueIndex)
                    aChipset.lower_passability[valueIndex] = data[valueIndex];
            }
            else if (tag == 0x05)
            {
                if (length > 144u)
                    return false;
                aChipset.upper_passability_count = length;
                for (unsigned int valueIndex = 0; valueIndex < length; ++valueIndex)
                    aChipset.upper_passability[valueIndex] = data[valueIndex];
            }
        }

        if (!terminatorSeen)
            return false;
        if (entryId == aChipsetId)
        {
            aChipset.chipset_id = entryId;
            return aChipset.name_length > 0;
        }
    }

    return false;
}

bool ParseSystemChunk(const unsigned char* aData,
                      unsigned int aLength,
                      EasyRpgCoreLcfSummary& aSummary)
{
    ByteCursor cursor = { aData, aLength, 0 };
    bool terminatorSeen = false;

    while (cursor.position < cursor.size)
    {
        unsigned int tag = 0;
        unsigned int length = 0;
        const unsigned char* data = 0;
        bool terminator = false;
        if (!ReadChunk(cursor, tag, data, length, terminator))
            return false;
        if (terminator)
        {
            terminatorSeen = true;
            break;
        }

        if (tag == 17)
            CopyResourceName(aSummary.title_name, aSummary.title_name_length, data, length);
        else if (tag == 19)
            CopyResourceName(aSummary.system_name, aSummary.system_name_length, data, length);
    }

    return terminatorSeen && cursor.position == cursor.size;
}

bool ParseDatabase(ByteCursor& aCursor, EasyRpgCoreLcfSummary& aSummary)
{
    while (aCursor.position < aCursor.size)
    {
        unsigned int tag = 0;
        unsigned int length = 0;
        const unsigned char* data = 0;
        bool terminator = false;
        if (!ReadChunk(aCursor, tag, data, length, terminator))
            return false;
        if (terminator)
        {
            aSummary.terminator_seen = 1;
            return aCursor.position == aCursor.size;
        }

        if (aSummary.record_count == 0)
            aSummary.first_tag = tag;
        aSummary.last_tag = tag;
        ++aSummary.record_count;

        if (tag == 22 && !ParseSystemChunk(data, length, aSummary))
            return false;
    }

    return true;
}

bool SkipTaggedStruct(ByteCursor& aCursor)
{
    while (aCursor.position < aCursor.size)
    {
        unsigned int tag = 0;
        unsigned int length = 0;
        const unsigned char* data = 0;
        bool terminator = false;
        if (!ReadChunk(aCursor, tag, data, length, terminator))
            return false;
        if (terminator)
            return true;
    }
    return false;
}

bool ParseStartStruct(ByteCursor& aCursor, EasyRpgCoreLcfSummary& aSummary)
{
    bool terminatorSeen = false;
    while (aCursor.position < aCursor.size)
    {
        unsigned int tag = 0;
        unsigned int length = 0;
        const unsigned char* data = 0;
        bool terminator = false;
        if (!ReadChunk(aCursor, tag, data, length, terminator))
            return false;
        if (terminator)
        {
            terminatorSeen = true;
            break;
        }

        unsigned int value = 0;
        if ((tag == 1 || tag == 2 || tag == 3) && !ReadChunkInteger(data, length, value))
            return false;

        if (tag == 1)
            aSummary.start_map_id = value;
        else if (tag == 2)
            aSummary.start_x = value;
        else if (tag == 3)
            aSummary.start_y = value;
    }

    aSummary.terminator_seen = terminatorSeen ? 1 : 0;
    return terminatorSeen;
}

bool ParseMapTree(ByteCursor& aCursor, EasyRpgCoreLcfSummary& aSummary)
{
    unsigned int mapCount = 0;
    if (!ReadBer(aCursor, mapCount) || mapCount == 0)
        return false;

    aSummary.record_count = mapCount;
    aSummary.first_tag = mapCount;
    aSummary.last_tag = mapCount;

    for (unsigned int index = 0; index < mapCount; ++index)
    {
        unsigned int mapId = 0;
        if (!ReadBer(aCursor, mapId))
            return false;
        if (!SkipTaggedStruct(aCursor))
            return false;
    }

    unsigned int treeOrderCount = 0;
    if (!ReadBer(aCursor, treeOrderCount))
        return false;
    for (unsigned int index = 0; index < treeOrderCount; ++index)
    {
        unsigned int mapId = 0;
        if (!ReadBer(aCursor, mapId))
            return false;
    }

    if (!ReadBer(aCursor, aSummary.active_node))
        return false;
    if (!ParseStartStruct(aCursor, aSummary))
        return false;

    return aCursor.position == aCursor.size;
}

bool CountEventPages(const unsigned char* aData,
                     unsigned int aLength,
                     unsigned int& aPageCount,
                     unsigned int& aCommandCount)
{
    if (!aData || aLength == 0)
        return false;

    ByteCursor cursor = { aData, aLength, 0 };
    unsigned int count = 0;
    if (!ReadBer(cursor, count))
        return false;

    for (unsigned int pageIndex = 0; pageIndex < count; ++pageIndex)
    {
        unsigned int pageId = 0;
        if (!ReadBer(cursor, pageId) || pageId == 0)
            return false;

        bool terminatorSeen = false;
        while (cursor.position < cursor.size)
        {
            unsigned int tag = 0;
            unsigned int length = 0;
            const unsigned char* data = 0;
            bool terminator = false;
            if (!ReadChunk(cursor, tag, data, length, terminator))
                return false;
            if (terminator)
            {
                terminatorSeen = true;
                break;
            }

            if (tag == 0x33)
            {
                unsigned int commandCount = 0;
                if (!ReadChunkInteger(data, length, commandCount))
                    return false;
                if (commandCount > 0xffffffffu - aCommandCount)
                    return false;
                aCommandCount += commandCount;
            }
        }

        if (!terminatorSeen)
            return false;
    }

    if (cursor.position != cursor.size)
        return false;
    if (count > 0xffffffffu - aPageCount)
        return false;
    aPageCount += count;
    return true;
}

bool CountEventArray(const unsigned char* aData,
                     unsigned int aLength,
                     unsigned int& aEventCount,
                     unsigned int& aPageCount,
                     unsigned int& aCommandCount)
{
    aEventCount = 0;
    aPageCount = 0;
    aCommandCount = 0;
    if (!aData || aLength == 0)
        return false;

    ByteCursor cursor = { aData, aLength, 0 };
    unsigned int count = 0;
    if (!ReadBer(cursor, count))
        return false;

    for (unsigned int index = 0; index < count; ++index)
    {
        unsigned int eventId = 0;
        if (!ReadBer(cursor, eventId) || eventId == 0)
            return false;

        bool terminatorSeen = false;
        while (cursor.position < cursor.size)
        {
            unsigned int tag = 0;
            unsigned int length = 0;
            const unsigned char* data = 0;
            bool terminator = false;
            if (!ReadChunk(cursor, tag, data, length, terminator))
                return false;
            if (terminator)
            {
                terminatorSeen = true;
                break;
            }

            if (tag == 0x05 &&
                !CountEventPages(data, length, aPageCount, aCommandCount))
                return false;
        }

        if (!terminatorSeen)
            return false;
    }

    if (cursor.position != cursor.size)
        return false;

    aEventCount = count;
    return true;
}

bool ParseMapUnit(ByteCursor& aCursor, EasyRpgCoreLcfSummary& aSummary)
{

    aSummary.map_chipset_id = 1;
    aSummary.map_width = 20;
    aSummary.map_height = 15;
    aSummary.map_scroll_type = 0;

    while (aCursor.position < aCursor.size)
    {
        unsigned int tag = 0;
        unsigned int length = 0;
        const unsigned char* data = 0;
        bool terminator = false;
        if (!ReadChunk(aCursor, tag, data, length, terminator))
            return false;
        if (terminator)
        {
            aSummary.terminator_seen = 1;
            break;
        }

        if (aSummary.record_count == 0)
            aSummary.first_tag = tag;
        aSummary.last_tag = tag;
        ++aSummary.record_count;

        unsigned int integerValue = 0;
        switch (tag)
        {
        case 0x01:
            if (!ReadChunkInteger(data, length, integerValue))
                return false;
            aSummary.map_chipset_id = integerValue;
            break;
        case 0x02:
            if (!ReadChunkInteger(data, length, integerValue) || integerValue == 0)
                return false;
            aSummary.map_width = integerValue;
            break;
        case 0x03:
            if (!ReadChunkInteger(data, length, integerValue) || integerValue == 0)
                return false;
            aSummary.map_height = integerValue;
            break;
        case 0x0B:
            if (!ReadChunkInteger(data, length, integerValue))
                return false;
            aSummary.map_scroll_type = integerValue;
            break;
        case 0x47:
            if ((length & 1u) != 0)
                return false;
            aSummary.map_lower_layer_tiles = length / 2u;
            break;
        case 0x48:
            if ((length & 1u) != 0)
                return false;
            aSummary.map_upper_layer_tiles = length / 2u;
            break;
        case 0x51:
            if (!CountEventArray(data,
                                 length,
                                 aSummary.map_event_count,
                                 aSummary.map_event_page_count,
                                 aSummary.map_event_command_count))
                return false;
            break;
        case 0x5B:
            if (!ReadChunkInteger(data, length, integerValue))
                return false;
            aSummary.map_save_count = integerValue;
            break;
        default:
            break;
        }
    }

    if (!aSummary.terminator_seen || aCursor.position != aCursor.size)
        return false;

    if (aSummary.map_width > 0xffffffffu / aSummary.map_height)
        return false;
    const unsigned int expectedTiles = aSummary.map_width * aSummary.map_height;
    if (aSummary.map_lower_layer_tiles != 0 &&
        aSummary.map_lower_layer_tiles != expectedTiles)
        return false;
    if (aSummary.map_upper_layer_tiles != 0 &&
        aSummary.map_upper_layer_tiles != expectedTiles)
        return false;

    return true;
}
}

extern "C" int EasyRpgCoreInspectLcf(int aFileType,
                                       const unsigned char* aData,
                                       unsigned int aSize,
                                       EasyRpgCoreLcfSummary* aSummary)
{
    if (!aData || !aSummary || aSize == 0)
        return -1;

    EasyRpgCoreLcfSummary summary = {};
    summary.file_size = aSize;
    summary.fnv1a = Fnv1a(aData, aSize);

    ByteCursor cursor = { aData, aSize, 0 };
    bool headerOk = false;

    switch (aFileType)
    {
    case EASYRPG_LCF_DATABASE:
        headerOk = ReadHeader(cursor, "LcfDataBase", 11);
        break;
    case EASYRPG_LCF_MAP_TREE:
        headerOk = ReadHeader(cursor, "LcfMapTree", 10);
        break;
    case EASYRPG_LCF_MAP_UNIT:
        headerOk = ReadHeader(cursor, "LcfMapUnit", 10);
        break;
    default:
        return -2;
    }

    if (!headerOk)
        return -3;

    summary.payload_offset = cursor.position;
    summary.payload_size = aSize - cursor.position;

    bool parsed = false;
    if (aFileType == EASYRPG_LCF_DATABASE)
        parsed = ParseDatabase(cursor, summary);
    else if (aFileType == EASYRPG_LCF_MAP_TREE)
        parsed = ParseMapTree(cursor, summary);
    else
        parsed = ParseMapUnit(cursor, summary);

    if (!parsed)
        return -4;

    *aSummary = summary;
    return 0;
}

extern "C" int EasyRpgCoreFindChipsetName(const unsigned char* aData,
                                             unsigned int aSize,
                                             unsigned int aChipsetId,
                                             unsigned char* aName,
                                             unsigned int aNameCapacity,
                                             unsigned int* aNameLength)
{
    if (!aData || aSize == 0 || aChipsetId == 0 ||
        !aName || aNameCapacity < 2 || !aNameLength)
        return -1;

    aName[0] = 0;
    *aNameLength = 0;

    ByteCursor cursor = { aData, aSize, 0 };
    if (!ReadHeader(cursor, "LcfDataBase", 11))
        return -2;

    while (cursor.position < cursor.size)
    {
        unsigned int tag = 0;
        unsigned int length = 0;
        const unsigned char* data = 0;
        bool terminator = false;
        if (!ReadChunk(cursor, tag, data, length, terminator))
            return -3;
        if (terminator)
            break;

        if (tag == 20)
        {
            unsigned int nameLength = 0;
            if (!FindChipsetNameInArray(data,
                                        length,
                                        aChipsetId,
                                        aName,
                                        aNameCapacity,
                                        nameLength))
                return -4;
            *aNameLength = nameLength;
            return 0;
        }
    }

    return -4;
}

extern "C" int EasyRpgCoreExtractMapLayers(const unsigned char* aData,
                                             unsigned int aSize,
                                             unsigned short* aLowerLayer,
                                             unsigned short* aUpperLayer,
                                             unsigned int aTileCapacity,
                                             unsigned int* aWidth,
                                             unsigned int* aHeight,
                                             unsigned int* aTileCount)
{
    if (!aData || aSize == 0 || !aLowerLayer || !aUpperLayer ||
        !aWidth || !aHeight || !aTileCount)
        return -1;

    *aWidth = 0;
    *aHeight = 0;
    *aTileCount = 0;

    EasyRpgCoreLcfSummary summary = {};
    const int inspectError = EasyRpgCoreInspectLcf(EASYRPG_LCF_MAP_UNIT,
                                                    aData,
                                                    aSize,
                                                    &summary);
    if (inspectError != 0)
        return -2;
    if (summary.map_width == 0 || summary.map_height == 0 ||
        summary.map_width > 0xffffffffu / summary.map_height)
        return -3;

    const unsigned int tileCount = summary.map_width * summary.map_height;
    if (tileCount > aTileCapacity)
        return -4;
    if (summary.map_lower_layer_tiles != tileCount ||
        summary.map_upper_layer_tiles != tileCount)
        return -5;

    ByteCursor cursor = { aData, aSize, 0 };
    if (!ReadHeader(cursor, "LcfMapUnit", 10))
        return -6;

    bool lowerSeen = false;
    bool upperSeen = false;
    while (cursor.position < cursor.size)
    {
        unsigned int tag = 0;
        unsigned int length = 0;
        const unsigned char* data = 0;
        bool terminator = false;
        if (!ReadChunk(cursor, tag, data, length, terminator))
            return -7;
        if (terminator)
            break;

        if (tag != 0x47 && tag != 0x48)
            continue;
        if (length != tileCount * 2u)
            return -8;

        unsigned short* destination = tag == 0x47 ? aLowerLayer : aUpperLayer;
        for (unsigned int index = 0; index < tileCount; ++index)
            destination[index] = ReadLittleEndian16(data + index * 2u);

        if (tag == 0x47)
            lowerSeen = true;
        else
            upperSeen = true;
    }

    if (!lowerSeen || !upperSeen)
        return -9;

    *aWidth = summary.map_width;
    *aHeight = summary.map_height;
    *aTileCount = tileCount;
    return 0;
}

extern "C" int EasyRpgCoreExtractChipset(const unsigned char* aData,
                                           unsigned int aSize,
                                           unsigned int aChipsetId,
                                           EasyRpgCoreChipsetData* aChipset)
{
    if (!aData || aSize == 0 || aChipsetId == 0 || !aChipset)
        return -1;

    EasyRpgCoreChipsetData chipset = {};
    ByteCursor cursor = { aData, aSize, 0 };
    if (!ReadHeader(cursor, "LcfDataBase", 11))
        return -2;

    while (cursor.position < cursor.size)
    {
        unsigned int tag = 0;
        unsigned int length = 0;
        const unsigned char* data = 0;
        bool terminator = false;
        if (!ReadChunk(cursor, tag, data, length, terminator))
            return -3;
        if (terminator)
            break;

        if (tag == 20)
        {
            if (!ExtractChipsetInArray(data, length, aChipsetId, chipset))
                return -4;
            *aChipset = chipset;
            return 0;
        }
    }

    return -4;
}

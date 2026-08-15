#include "RgssRgssad.h"

#include <cstdlib>
#include <cstring>

struct RgssadArchive
{
    int count;
    char** names;       /* normalized ('/'), lower-cased; owned */
    long* offsets;
    unsigned long* sizes;
    unsigned int* magics;
};

namespace {

const unsigned int KInitialMagic = 0xDEADCAFE;
const int KMaxEntryName = 1024;

unsigned int Advance(unsigned int& aMagic)
{
    const unsigned int old = aMagic;
    aMagic = aMagic * 7 + 3;
    return old;
}

unsigned int ReadLe32(const unsigned char* aBuffer)
{
    return static_cast<unsigned int>(aBuffer[0]) |
           (static_cast<unsigned int>(aBuffer[1]) << 8) |
           (static_cast<unsigned int>(aBuffer[2]) << 16) |
           (static_cast<unsigned int>(aBuffer[3]) << 24);
}

void ToLowerNormalized(char* aName)
{
    for (char* p = aName; *p; ++p)
    {
        if (*p == '\\')
            *p = '/';
        else if (*p >= 'A' && *p <= 'Z')
            *p = static_cast<char>(*p - 'A' + 'a');
    }
}

void FreeArchive(RgssadArchive* aArchive)
{
    if (!aArchive)
        return;
    for (int i = 0; i < aArchive->count; ++i)
        free(aArchive->names[i]);
    free(aArchive->names);
    free(aArchive->offsets);
    free(aArchive->sizes);
    free(aArchive->magics);
    free(aArchive);
}

/* Grows the entry table; returns the index of the next free slot or -1. */
int EnsureCapacity(RgssadArchive* aArchive, int aNeeded)
{
    if (aNeeded <= aArchive->count)
        return aArchive->count;
    const int growTo = aNeeded + 64;
    char** names = static_cast<char**>(
        realloc(aArchive->names, growTo * sizeof(char*)));
    if (!names)
        return -1;
    aArchive->names = names;
    long* offsets = static_cast<long*>(
        realloc(aArchive->offsets, growTo * sizeof(long)));
    if (!offsets)
        return -1;
    aArchive->offsets = offsets;
    unsigned long* sizes = static_cast<unsigned long*>(
        realloc(aArchive->sizes, growTo * sizeof(unsigned long)));
    if (!sizes)
        return -1;
    aArchive->sizes = sizes;
    unsigned int* magics = static_cast<unsigned int*>(
        realloc(aArchive->magics, growTo * sizeof(unsigned int)));
    if (!magics)
        return -1;
    aArchive->magics = magics;
    return aNeeded;
}

int PushEntry(RgssadArchive* aArchive, const char* aName,
              long aOffset, unsigned long aSize, unsigned int aMagic)
{
    if (EnsureCapacity(aArchive, aArchive->count + 1) < 0)
        return -1;
    char* name = static_cast<char*>(malloc(strlen(aName) + 1));
    if (!name)
        return -1;
    strcpy(name, aName);
    aArchive->names[aArchive->count] = name;
    aArchive->offsets[aArchive->count] = aOffset;
    aArchive->sizes[aArchive->count] = aSize;
    aArchive->magics[aArchive->count] = aMagic;
    ++aArchive->count;
    return 0;
}

int ParseV1(const unsigned char* aData, long aSize, RgssadArchive* aArchive)
{
    unsigned int magic = KInitialMagic;
    long pos = 8; /* "RGSSAD\0" + version */

    while (pos + 4 <= aSize)
    {
        unsigned int nameLen = ReadLe32(aData + pos) ^ Advance(magic);
        pos += 4;
        if (nameLen > static_cast<unsigned int>(KMaxEntryName) ||
            pos + static_cast<long>(nameLen) + 4 > aSize)
            return -1;

        char name[KMaxEntryName + 1];
        for (unsigned int i = 0; i < nameLen; ++i)
        {
            char c = static_cast<char>(aData[pos + i]);
            c ^= static_cast<char>(Advance(magic) & 0xFF);
            name[i] = c == '\\' ? '/' : c;
        }
        name[nameLen] = 0;
        pos += nameLen;

        const unsigned int entrySize = ReadLe32(aData + pos) ^ Advance(magic);
        pos += 4;

        if (PushEntry(aArchive, name, pos, entrySize, magic) != 0)
            return -1;
        pos += entrySize;
    }
    return 0;
}

int ParseV3(const unsigned char* aData, long aSize, RgssadArchive* aArchive)
{
    if (aSize < 12)
        return -1;

    unsigned int baseKey = ReadLe32(aData + 8);
    baseKey = baseKey * 9 + 3;
    long pos = 12;

    while (pos + 16 <= aSize)
    {
        const unsigned int offset = ReadLe32(aData + pos) ^ baseKey;
        if (offset == 0)
            return 0; /* end of entry table */
        const unsigned int size = ReadLe32(aData + pos + 4) ^ baseKey;
        const unsigned int magic = ReadLe32(aData + pos + 8) ^ baseKey;
        const unsigned int nameLen = ReadLe32(aData + pos + 12) ^ baseKey;
        pos += 16;

        if (nameLen > static_cast<unsigned int>(KMaxEntryName) ||
            pos + static_cast<long>(nameLen) > aSize)
            return -1;

        char name[KMaxEntryName + 1];
        for (unsigned int i = 0; i < nameLen; ++i)
        {
            char c = static_cast<char>(aData[pos + i]);
            c ^= static_cast<char>((baseKey >> (8 * (i % 4))) & 0xFF);
            name[i] = c == '\\' ? '/' : c;
        }
        name[nameLen] = 0;
        pos += nameLen;

        if (PushEntry(aArchive, name, offset, size, magic) != 0)
            return -1;
    }
    return 0;
}

} // namespace

extern "C" int RgssadParse(const unsigned char* aData, long aSize,
                           RgssadArchive** aOut)
{
    *aOut = 0;
    if (!aData || aSize < 8 || memcmp(aData, "RGSSAD\0", 7) != 0)
        return -1;

    RgssadArchive* archive =
        static_cast<RgssadArchive*>(calloc(1, sizeof(RgssadArchive)));
    if (!archive)
        return -1;

    int result = -1;
    if (aData[7] == 1)
        result = ParseV1(aData, aSize, archive);
    else if (aData[7] == 3)
        result = ParseV3(aData, aSize, archive);

    if (result != 0 || archive->count == 0)
    {
        FreeArchive(archive);
        return -1;
    }
    for (int i = 0; i < archive->count; ++i)
        ToLowerNormalized(archive->names[i]);

    *aOut = archive;
    return 0;
}

extern "C" void RgssadFree(RgssadArchive* aArchive)
{
    FreeArchive(aArchive);
}

extern "C" int RgssadEntryCount(const RgssadArchive* aArchive)
{
    return aArchive ? aArchive->count : 0;
}

extern "C" int RgssadFind(const RgssadArchive* aArchive, const char* aPath,
                          RgssadEntry* aOut)
{
    if (!aArchive || !aPath)
        return -1;

    char normalized[KMaxEntryName + 1];
    int i = 0;
    for (; aPath[i] && i < KMaxEntryName; ++i)
    {
        char c = aPath[i];
        if (c == '\\')
            c = '/';
        else if (c >= 'A' && c <= 'Z')
            c = static_cast<char>(c - 'A' + 'a');
        normalized[i] = c;
    }
    if (i >= KMaxEntryName)
        return -1;
    normalized[i] = 0;

    for (int e = 0; e < aArchive->count; ++e)
    {
        if (strcmp(aArchive->names[e], normalized) == 0)
        {
            if (aOut)
            {
                aOut->offset = aArchive->offsets[e];
                aOut->size = aArchive->sizes[e];
                aOut->startMagic = aArchive->magics[e];
            }
            return e;
        }
    }
    return -1;
}

extern "C" long RgssadReadEntry(const unsigned char* aArchiveData,
                                const RgssadEntry* aEntry,
                                unsigned char** aOut)
{
    *aOut = 0;
    if (!aArchiveData || !aEntry || aEntry->size == 0)
        return -1;

    unsigned char* out = static_cast<unsigned char*>(malloc(aEntry->size));
    if (!out)
        return -1;

    unsigned int magic = aEntry->startMagic;
    unsigned long i = 0;
    while (i < aEntry->size)
    {
        for (int b = 0; b < 4 && i < aEntry->size; ++b, ++i)
            out[i] = aArchiveData[aEntry->offset + i] ^
                     static_cast<unsigned char>((magic >> (8 * b)) & 0xFF);
        magic = magic * 7 + 3;
    }

    *aOut = out;
    return static_cast<long>(aEntry->size);
}

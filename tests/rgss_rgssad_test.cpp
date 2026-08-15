/* Host unit test for the portable .rgssad/.rgss3a reader. Fixtures are
 * built with the encryption side of the documented algorithm (byte-exact
 * port of mkxp-z src/crypto/rgssad.cpp), then parsed back. */

#include "rgss/core/RgssRgssad.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace {

unsigned int Advance(unsigned int& aMagic)
{
    const unsigned int old = aMagic;
    aMagic = aMagic * 7 + 3;
    return old;
}

void PutLe32(std::vector<unsigned char>& aOut, unsigned int aValue)
{
    aOut.push_back(static_cast<unsigned char>(aValue));
    aOut.push_back(static_cast<unsigned char>(aValue >> 8));
    aOut.push_back(static_cast<unsigned char>(aValue >> 16));
    aOut.push_back(static_cast<unsigned char>(aValue >> 24));
}

/* Encrypts a v1 (XP) archive: header + entries in order. */
std::vector<unsigned char> BuildV1(const char* const* aNames,
                                   const unsigned char* const* aPayloads,
                                   const unsigned int* aSizes, int aCount)
{
    std::vector<unsigned char> out;
    const char header[8] = {'R', 'G', 'S', 'S', 'A', 'D', 0, 1};
    out.insert(out.end(), header, header + 8);

    unsigned int magic = 0xDEADCAFE;
    for (int i = 0; i < aCount; ++i)
    {
        const unsigned int nameLen = static_cast<unsigned int>(strlen(aNames[i]));
        PutLe32(out, nameLen ^ Advance(magic));
        for (unsigned int c = 0; c < nameLen; ++c)
        {
            unsigned char byte = static_cast<unsigned char>(aNames[i][c]);
            out.push_back(byte ^ static_cast<unsigned char>(Advance(magic) & 0xFF));
        }
        PutLe32(out, aSizes[i] ^ Advance(magic));
        /* The parse-level magic chain does NOT advance across the payload
         * (mkxp RGSS_openArchive only advances for nameLen/name/size); the
         * payload itself is keyed off startMagic in a separate chain. */
        unsigned int payloadMagic = magic;
        for (unsigned int j = 0; j < aSizes[i]; ++j)
        {
            out.push_back(aPayloads[i][j] ^
                static_cast<unsigned char>((payloadMagic >> (8 * (j % 4))) & 0xFF));
            if (j % 4 == 3)
                Advance(payloadMagic);
        }
    }
    return out;
}

std::vector<unsigned char> BuildV3(const char* const* aNames,
                                   const unsigned char* const* aPayloads,
                                   const unsigned int* aSizes, int aCount)
{
    std::vector<unsigned char> out;
    const char header[8] = {'R', 'G', 'S', 'S', 'A', 'D', 0, 3};
    out.insert(out.end(), header, header + 8);

    const unsigned int baseRaw = 0x11223344;
    PutLe32(out, baseRaw);
    const unsigned int baseKey = baseRaw * 9 + 3;

    /* Entry table first (fixed size per entry: 16 + nameLen). */
    unsigned int offset = 8 + 4 + aCount * 16;
    for (int i = 0; i < aCount; ++i)
        offset += static_cast<unsigned int>(strlen(aNames[i]));
    std::vector<unsigned char> table;
    std::vector<unsigned char> names;
    std::vector<unsigned char> data;
    std::vector<unsigned int> magics;

    for (int i = 0; i < aCount; ++i)
    {
        const unsigned int entryMagic = 0xA0 + static_cast<unsigned int>(i);
        magics.push_back(entryMagic);
        const unsigned int nameLen = static_cast<unsigned int>(strlen(aNames[i]));
        PutLe32(table, offset ^ baseKey);
        PutLe32(table, aSizes[i] ^ baseKey);
        PutLe32(table, entryMagic ^ baseKey);
        PutLe32(table, nameLen ^ baseKey);
        for (unsigned int c = 0; c < nameLen; ++c)
            names.push_back(static_cast<unsigned char>(aNames[i][c]) ^
                            static_cast<unsigned char>((baseKey >> (8 * (c % 4))) & 0xFF));
        offset += aSizes[i];
    }

    for (int i = 0; i < aCount; ++i)
    {
        unsigned int magic = magics[i];
        for (unsigned int j = 0; j < aSizes[i]; ++j)
        {
            data.push_back(aPayloads[i][j] ^
                static_cast<unsigned char>((magic >> (8 * (j % 4))) & 0xFF));
            if (j % 4 == 3)
                Advance(magic);
        }
    }

    out.insert(out.end(), table.begin(), table.end());
    out.insert(out.end(), names.begin(), names.end());
    out.insert(out.end(), data.begin(), data.end());
    return out;
}

void TestV1()
{
    const unsigned char scripts[] = {0xde, 0xad, 0xbe, 0xef, 1, 2, 3, 4, 5, 6};
    const unsigned char graphic[] = {'P', 'N', 'G', '!', 0, 0, 0};
    const char* names[] = {"Data\\Scripts.rxdata", "Graphics/picture.png"};
    const unsigned char* payloads[] = {scripts, graphic};
    const unsigned int sizes[] = {sizeof(scripts), sizeof(graphic)};

    std::vector<unsigned char> archive =
        BuildV1(names, payloads, sizes, 2);

    RgssadArchive* parsed = 0;
    assert(RgssadParse(&archive[0], static_cast<long>(archive.size()), &parsed) == 0);
    assert(RgssadEntryCount(parsed) == 2);

    RgssadEntry entry;
    assert(RgssadFind(parsed, "data/scripts.rxdata", &entry) >= 0); /* ci + mixed slashes */
    unsigned char* out = 0;
    assert(RgssadReadEntry(&archive[0], &entry, &out) == 10);
    assert(memcmp(out, scripts, 10) == 0);
    free(out);

    /* Non-dword-aligned entry size exercises the tail of the XOR stream. */
    assert(RgssadFind(parsed, "Graphics/Picture.PNG", &entry) >= 0);
    assert(RgssadReadEntry(&archive[0], &entry, &out) == 7);
    assert(memcmp(out, graphic, 7) == 0);
    free(out);

    assert(RgssadFind(parsed, "Data/Nope.rxdata", 0) < 0);
    RgssadFree(parsed);
    printf("rgssad v1 ok\n");
}

void TestV3()
{
    const unsigned char map[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 255, 254};
    const char* names[] = {"Data\\Map001.rvdata2"};
    const unsigned char* payloads[] = {map};
    const unsigned int sizes[] = {sizeof(map)};

    std::vector<unsigned char> archive = BuildV3(names, payloads, sizes, 1);

    RgssadArchive* parsed = 0;
    assert(RgssadParse(&archive[0], static_cast<long>(archive.size()), &parsed) == 0);

    RgssadEntry entry;
    assert(RgssadFind(parsed, "data/map001.rvdata2", &entry) >= 0);
    unsigned char* out = 0;
    assert(RgssadReadEntry(&archive[0], &entry, &out) == 12);
    assert(memcmp(out, map, 12) == 0);
    free(out);
    RgssadFree(parsed);
    printf("rgssad v3 ok\n");
}

void TestRejectsGarbage()
{
    RgssadArchive* parsed = reinterpret_cast<RgssadArchive*>(1);
    const unsigned char junk[64] = {0};
    assert(RgssadParse(junk, sizeof(junk), &parsed) != 0);
    assert(parsed == 0);
    printf("rgssad reject ok\n");
}

} // namespace

int main()
{
    TestV1();
    TestV3();
    TestRejectsGarbage();
    printf("rgss_rgssad_test: all ok\n");
    return 0;
}

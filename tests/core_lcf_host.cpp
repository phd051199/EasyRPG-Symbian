#include "easyrpgsymbian/CoreLcf.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace
{
std::vector<unsigned char> ReadFile(const std::string& path)
{
    std::ifstream input(path.c_str(), std::ios::binary);
    if (!input)
        return std::vector<unsigned char>();

    input.seekg(0, std::ios::end);
    const std::streamoff size = input.tellg();
    input.seekg(0, std::ios::beg);
    if (size <= 0)
        return std::vector<unsigned char>();

    std::vector<unsigned char> data(static_cast<std::size_t>(size));
    input.read(reinterpret_cast<char*>(&data[0]), size);
    if (!input)
        return std::vector<unsigned char>();
    return data;
}

bool Inspect(const std::string& root,
             const char* fileName,
             int fileType,
             EasyRpgCoreLcfSummary& summary)
{
    const std::vector<unsigned char> data = ReadFile(root + "/" + fileName);
    if (data.empty())
    {
        std::cerr << "cannot read " << fileName << "\n";
        return false;
    }

    const int rc = EasyRpgCoreInspectLcf(fileType,
                                         &data[0],
                                         static_cast<unsigned int>(data.size()),
                                         &summary);
    if (rc != 0)
    {
        std::cerr << fileName << ": parser rc=" << rc << "\n";
        return false;
    }
    return true;
}

bool Expect(bool condition, const char* message)
{
    if (!condition)
        std::cerr << "FAIL: " << message << "\n";
    return condition;
}

bool EqualBytes(const unsigned char* actual,
                unsigned int actualLength,
                const unsigned char* expected,
                unsigned int expectedLength)
{
    if (actualLength != expectedLength)
        return false;
    for (unsigned int index = 0; index < expectedLength; ++index)
    {
        if (actual[index] != expected[index])
            return false;
    }
    return true;
}

unsigned int HashWords(const std::vector<unsigned short>& values)
{
    unsigned int hash = 2166136261u;
    for (std::size_t index = 0; index < values.size(); ++index)
    {
        const unsigned short value = values[index];
        hash ^= static_cast<unsigned char>(value & 0xffu);
        hash *= 16777619u;
        hash ^= static_cast<unsigned char>((value >> 8) & 0xffu);
        hash *= 16777619u;
    }
    return hash;
}
}

int main(int argc, char** argv)
{
    const std::string root = argc > 1 ? argv[1] : ".games/NobihazaVH";
    bool ok = true;

    EasyRpgCoreLcfSummary database = {};
    ok &= Inspect(root, "RPG_RT.ldb", EASYRPG_LCF_DATABASE, database);
    static const unsigned char kTitleName[] =
        { 0x83, 0x5e, 0x83, 0x43, 0x83, 0x67, 0x83, 0x8b }; // タイトル (CP932)
    static const unsigned char kSystemName[] =
        { 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80 }; // システム (CP932)
    if (ok)
    {
        ok &= Expect(database.record_count == 15, "LDB top-level record count must be 15");
        ok &= Expect(database.fnv1a == 0x507ca2d7u, "LDB checksum changed");
        ok &= Expect(EqualBytes(database.title_name,
                                database.title_name_length,
                                kTitleName,
                                sizeof(kTitleName)),
                     "LDB System.title_name must be CP932 タイトル");
        ok &= Expect(EqualBytes(database.system_name,
                                database.system_name_length,
                                kSystemName,
                                sizeof(kSystemName)),
                     "LDB System.system_name must be CP932 システム");
    }

    const std::vector<unsigned char> databaseBytes = ReadFile(root + "/RPG_RT.ldb");
    unsigned char chipsetName[EASYRPG_LCF_RESOURCE_NAME_CAPACITY] = {};
    unsigned int chipsetNameLength = 0;
    const int chipsetRc = databaseBytes.empty()
        ? -99
        : EasyRpgCoreFindChipsetName(&databaseBytes[0],
                                     static_cast<unsigned int>(databaseBytes.size()),
                                     1,
                                     chipsetName,
                                     sizeof(chipsetName),
                                     &chipsetNameLength);
    static const unsigned char kChipsetName[] = { 0x8a, 0xee, 0x96, 0x7b }; // 基本 (CP932)
    ok &= Expect(chipsetRc == 0, "chipset #1 lookup must succeed");
    ok &= Expect(EqualBytes(chipsetName,
                            chipsetNameLength,
                            kChipsetName,
                            sizeof(kChipsetName)),
                 "chipset #1 resource must be CP932 基本");

    EasyRpgCoreChipsetData chipset = {};
    const int chipsetDataRc = databaseBytes.empty()
        ? -99
        : EasyRpgCoreExtractChipset(&databaseBytes[0],
                                    static_cast<unsigned int>(databaseBytes.size()),
                                    1,
                                    &chipset);
    ok &= Expect(chipsetDataRc == 0, "chipset #1 data extraction must succeed");
    ok &= Expect(chipset.terrain_count == 162, "chipset #1 terrain table must contain 162 entries");
    ok &= Expect(chipset.lower_passability_count == 162, "chipset #1 lower passability must contain 162 entries");
    ok &= Expect(chipset.upper_passability_count == 144, "chipset #1 upper passability must contain 144 entries");

    EasyRpgCoreLcfSummary mapTree = {};
    ok &= Inspect(root, "RPG_RT.lmt", EASYRPG_LCF_MAP_TREE, mapTree);
    if (ok)
    {
        ok &= Expect(mapTree.record_count == 203, "LMT map entry count must be 203");
        ok &= Expect(mapTree.fnv1a == 0x08f8ecd1u, "LMT checksum changed");
        ok &= Expect(mapTree.active_node == 53, "LMT active node must be 53");
        ok &= Expect(mapTree.start_map_id == 1, "new game must start on Map0001");
        ok &= Expect(mapTree.start_x == 1, "new game start X must be 1");
        ok &= Expect(mapTree.start_y == 0, "new game start Y must be 0");
    }

    EasyRpgCoreLcfSummary mapUnit = {};
    ok &= Inspect(root, "Map0001.lmu", EASYRPG_LCF_MAP_UNIT, mapUnit);
    const std::vector<unsigned char> mapBytes = ReadFile(root + "/Map0001.lmu");
    std::vector<unsigned short> lowerLayer(300);
    std::vector<unsigned short> upperLayer(300);
    unsigned int layerWidth = 0;
    unsigned int layerHeight = 0;
    unsigned int layerTileCount = 0;
    const int layerRc = mapBytes.empty()
        ? -99
        : EasyRpgCoreExtractMapLayers(&mapBytes[0],
                                      static_cast<unsigned int>(mapBytes.size()),
                                      &lowerLayer[0],
                                      &upperLayer[0],
                                      static_cast<unsigned int>(lowerLayer.size()),
                                      &layerWidth,
                                      &layerHeight,
                                      &layerTileCount);
    ok &= Expect(layerRc == 0, "Map0001 layer extraction must succeed");
    ok &= Expect(layerWidth == 20 && layerHeight == 15, "Map0001 extracted dimensions must be 20x15");
    ok &= Expect(layerTileCount == 300, "Map0001 extracted tile count must be 300");
    ok &= Expect(HashWords(lowerLayer) == 0x3202eee5u, "Map0001 lower layer checksum changed");
    ok &= Expect(HashWords(upperLayer) == 0xe1fb9c55u, "Map0001 upper layer checksum changed");
    if (ok)
    {
        ok &= Expect(mapUnit.record_count == 5, "Map0001 top-level record count must be 5");
        ok &= Expect(mapUnit.fnv1a == 0x36dd67f4u, "Map0001 checksum changed");
        ok &= Expect(mapUnit.terminator_seen != 0, "Map0001 must end with LCF terminator");
        ok &= Expect(mapUnit.map_chipset_id == 1, "Map0001 default chipset id must be 1");
        ok &= Expect(mapUnit.map_width == 20, "Map0001 default width must be 20");
        ok &= Expect(mapUnit.map_height == 15, "Map0001 default height must be 15");
        ok &= Expect(mapUnit.map_lower_layer_tiles == 300, "Map0001 lower layer must contain 300 tiles");
        ok &= Expect(mapUnit.map_upper_layer_tiles == 300, "Map0001 upper layer must contain 300 tiles");
        ok &= Expect(mapUnit.map_event_count == 1, "Map0001 must contain one event");
        ok &= Expect(mapUnit.map_event_page_count == 2, "Map0001 event must contain two pages");
        ok &= Expect(mapUnit.map_event_command_count == 121, "Map0001 event pages must declare 121 commands total");
        ok &= Expect(mapUnit.map_save_count == 196, "Map0001 save count must be 196");
    }

    if (!ok)
        return EXIT_FAILURE;

    std::cout << "NobihazaVH LCF regression OK: "
              << "LDB=" << database.record_count
              << " LMT=" << mapTree.record_count
              << " start=M" << mapTree.start_map_id
              << "(" << mapTree.start_x << "," << mapTree.start_y << ")"
              << " LMU=" << mapUnit.record_count
              << " map=" << mapUnit.map_width << "x" << mapUnit.map_height
              << " chipset=" << mapUnit.map_chipset_id
              << " events=" << mapUnit.map_event_count
              << " pages=" << mapUnit.map_event_page_count
              << " commands=" << mapUnit.map_event_command_count
              << " saveCount=" << mapUnit.map_save_count
              << " lowerHash=0x" << std::hex << HashWords(lowerLayer)
              << " upperHash=0x" << HashWords(upperLayer) << std::dec
              << "\n";
    return EXIT_SUCCESS;
}

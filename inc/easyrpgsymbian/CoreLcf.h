#ifndef EASYRPG_SYMBIAN_CORE_LCF_H
#define EASYRPG_SYMBIAN_CORE_LCF_H

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgCoreLcfFileType
{
    EASYRPG_LCF_DATABASE = 1,
    EASYRPG_LCF_MAP_TREE = 2,
    EASYRPG_LCF_MAP_UNIT = 3
};

enum
{
    EASYRPG_LCF_RESOURCE_NAME_CAPACITY = 96
};

typedef struct EasyRpgCoreLcfSummary
{
    unsigned int file_size;
    unsigned int payload_offset;
    unsigned int payload_size;
    unsigned int record_count;
    unsigned int first_tag;
    unsigned int last_tag;
    unsigned int fnv1a;
    int terminator_seen;

    unsigned int active_node;
    unsigned int start_map_id;
    unsigned int start_x;
    unsigned int start_y;

    unsigned int map_chipset_id;
    unsigned int map_width;
    unsigned int map_height;
    unsigned int map_scroll_type;
    unsigned int map_lower_layer_tiles;
    unsigned int map_upper_layer_tiles;
    unsigned int map_event_count;
    unsigned int map_event_page_count;
    unsigned int map_event_command_count;
    unsigned int map_save_count;

    unsigned int title_name_length;
    unsigned char title_name[EASYRPG_LCF_RESOURCE_NAME_CAPACITY];
    unsigned int system_name_length;
    unsigned char system_name[EASYRPG_LCF_RESOURCE_NAME_CAPACITY];
} EasyRpgCoreLcfSummary;

int EasyRpgCoreInspectLcf(int aFileType,
                          const unsigned char* aData,
                          unsigned int aSize,
                          EasyRpgCoreLcfSummary* aSummary);

int EasyRpgCoreFindChipsetName(const unsigned char* aData,
                               unsigned int aSize,
                               unsigned int aChipsetId,
                               unsigned char* aName,
                               unsigned int aNameCapacity,
                               unsigned int* aNameLength);

int EasyRpgCoreExtractMapLayers(const unsigned char* aData,
                                unsigned int aSize,
                                unsigned short* aLowerLayer,
                                unsigned short* aUpperLayer,
                                unsigned int aTileCapacity,
                                unsigned int* aWidth,
                                unsigned int* aHeight,
                                unsigned int* aTileCount);

typedef struct EasyRpgCoreChipsetData
{
    unsigned int chipset_id;
    unsigned int name_length;
    unsigned char name[EASYRPG_LCF_RESOURCE_NAME_CAPACITY];
    unsigned int terrain_count;
    unsigned short terrain[162];
    unsigned int lower_passability_count;
    unsigned char lower_passability[162];
    unsigned int upper_passability_count;
    unsigned char upper_passability[144];
} EasyRpgCoreChipsetData;

int EasyRpgCoreExtractChipset(const unsigned char* aData,
                              unsigned int aSize,
                              unsigned int aChipsetId,
                              EasyRpgCoreChipsetData* aChipset);

#ifdef __cplusplus
}
#endif

#endif

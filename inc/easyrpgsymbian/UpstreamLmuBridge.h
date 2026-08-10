#ifndef EASYRPG_SYMBIAN_UPSTREAM_LMU_BRIDGE_H
#define EASYRPG_SYMBIAN_UPSTREAM_LMU_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EasyRpgUpstreamLmtSummary {
    unsigned int map_count;
    unsigned int tree_order_count;
    int active_node;
    int start_map_id;
    int start_x;
    int start_y;
} EasyRpgUpstreamLmtSummary;

typedef struct EasyRpgUpstreamLdbSummary {
    unsigned int actor_count;
    unsigned int chipset_count;
    unsigned int common_event_count;
} EasyRpgUpstreamLdbSummary;

typedef struct EasyRpgUpstreamLmuSummary {
    int width;
    int height;
    int chipset_id;
    unsigned int lower_layer_count;
    unsigned int upper_layer_count;
    unsigned int event_count;
    unsigned int event_page_count;
    unsigned int event_command_count;
    unsigned int save_count;
} EasyRpgUpstreamLmuSummary;

int EasyRpgUpstreamLoadLmtSummary(const char* filename,
                                  const char* encoding,
                                  EasyRpgUpstreamLmtSummary* summary);

int EasyRpgUpstreamLoadLdbSummary(const char* filename,
                                  const char* encoding,
                                  int selected_chipset_id,
                                  EasyRpgUpstreamLdbSummary* summary,
                                  unsigned char* title_name,
                                  unsigned int title_name_capacity,
                                  unsigned int* title_name_length,
                                  unsigned char* system_name,
                                  unsigned int system_name_capacity,
                                  unsigned int* system_name_length,
                                  unsigned char* chipset_name,
                                  unsigned int chipset_name_capacity,
                                  unsigned int* chipset_name_length);

int EasyRpgUpstreamReadLmuSummary(const char* filename,
                                  const char* encoding,
                                  EasyRpgUpstreamLmuSummary* summary);

int EasyRpgUpstreamReadLmuLayers(const char* filename,
                                 const char* encoding,
                                 short* lower_layer,
                                 unsigned int lower_layer_capacity,
                                 unsigned int* lower_layer_length,
                                 short* upper_layer,
                                 unsigned int upper_layer_capacity,
                                 unsigned int* upper_layer_length);

#ifdef __cplusplus
}
#endif

#endif

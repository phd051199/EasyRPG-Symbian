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
    int initial_actor_id;
    int initial_actor_character_index;
    int initial_actor_transparent;
    unsigned int initial_actor_character_name_length;
    unsigned char initial_actor_character_name[96];
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

typedef struct EasyRpgUpstreamBootSequence {
    int wait_before_picture;
    int picture_x;
    int picture_y;
    int picture_magnify;
    int picture_opacity;
    int first_move_x;
    int first_move_y;
    int first_move_magnify;
    int first_move_opacity;
    int first_move_duration;
    int wait_after_first_move;
    int second_move_x;
    int second_move_y;
    int second_move_magnify;
    int second_move_opacity;
    int second_move_duration;
    int wait_after_second_move;
    int target_map_id;
    int target_x;
    int target_y;
    unsigned int picture_name_length;
    unsigned char picture_name[96];
} EasyRpgUpstreamBootSequence;

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

int EasyRpgUpstreamReadBootSequence(const char* filename,
                                    const char* encoding,
                                    EasyRpgUpstreamBootSequence* sequence);

int EasyRpgUpstreamGetChipsetName(int chipset_id,
                                  unsigned char* name,
                                  unsigned int capacity,
                                  unsigned int* length);

#ifdef __cplusplus
}
#endif

#endif

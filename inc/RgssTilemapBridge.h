#ifndef EASYRPG_RGSS_TILEMAP_BRIDGE_H
#define EASYRPG_RGSS_TILEMAP_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssTilemapResult
{
    ERgssTilemapOk = 0,
    ERgssTilemapErrorInvalidArgument = -130,
    ERgssTilemapErrorNotInitialized = -131,
    ERgssTilemapErrorNoMemory = -132,
    ERgssTilemapErrorCapacity = -133,
    ERgssTilemapErrorInvalidHandle = -134,
    ERgssTilemapErrorResource = -135,
    ERgssTilemapErrorRender = -136
};

typedef void* (*EasyRpgRgssTilemapAllocFn)(void* context, unsigned int bytes);
typedef void (*EasyRpgRgssTilemapFreeFn)(void* context, void* memory);

typedef struct EasyRpgRgssTilemapHost
{
    void* context;
    EasyRpgRgssTilemapAllocFn allocate;
    EasyRpgRgssTilemapFreeFn release;
    int max_tilemaps;
} EasyRpgRgssTilemapHost;

typedef struct EasyRpgRgssTilemapState
{
    int viewport_handle;
    int tileset_handle;
    int autotile_handles[7];
    int map_data_handle;
    int flash_data_handle;
    int priorities_handle;
    int bitmap_handles[9];
    int flags_handle;
    int ox;
    int oy;
    int visible;
    int update_count;
} EasyRpgRgssTilemapState;

int EasyRpgRgssTilemapInitialize(const EasyRpgRgssTilemapHost* host);
void EasyRpgRgssTilemapShutdown(void);
int EasyRpgRgssTilemapCreate(int viewport_handle, int* handle);
int EasyRpgRgssTilemapDestroy(int handle);
int EasyRpgRgssTilemapGetState(int handle, EasyRpgRgssTilemapState* state);
int EasyRpgRgssTilemapSetViewport(int handle, int viewport_handle);
int EasyRpgRgssTilemapSetTileset(int handle, int bitmap_handle);
int EasyRpgRgssTilemapSetAutotile(int handle, int index, int bitmap_handle);
int EasyRpgRgssTilemapSetMapData(int handle, int table_handle);
int EasyRpgRgssTilemapSetFlashData(int handle, int table_handle);
int EasyRpgRgssTilemapSetPriorities(int handle, int table_handle);
int EasyRpgRgssTilemapSetBitmap(int handle, int index, int bitmap_handle);
int EasyRpgRgssTilemapSetFlags(int handle, int table_handle);
int EasyRpgRgssTilemapSetOrigin(int handle, int ox, int oy);
int EasyRpgRgssTilemapSetVisible(int handle, int visible);
int EasyRpgRgssTilemapUpdate(int handle);
void EasyRpgRgssTilemapInvalidateAll(void);
int EasyRpgRgssTilemapCount(void);

#ifdef __cplusplus
}
#endif

#endif

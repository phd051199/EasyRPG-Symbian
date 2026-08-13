#ifndef EASYRPG_RGSS_SPRITE_BRIDGE_H
#define EASYRPG_RGSS_SPRITE_BRIDGE_H

#include "RgssCoreBridge.h"

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssSpriteResult
{
    ERgssSpriteOk = 0,
    ERgssSpriteErrorInvalidArgument = -60,
    ERgssSpriteErrorNotInitialized = -61,
    ERgssSpriteErrorNoMemory = -62,
    ERgssSpriteErrorCapacity = -63,
    ERgssSpriteErrorInvalidHandle = -64,
    ERgssSpriteErrorBitmap = -65,
    ERgssSpriteErrorRender = -66
};

typedef void* (*EasyRpgRgssSpriteAllocFn)(void* context, unsigned int bytes);
typedef void (*EasyRpgRgssSpriteFreeFn)(void* context, void* memory);

typedef struct EasyRpgRgssSpriteHost
{
    void* context;
    EasyRpgRgssSpriteAllocFn allocate;
    EasyRpgRgssSpriteFreeFn release;
    int max_sprites;
} EasyRpgRgssSpriteHost;

typedef struct EasyRpgRgssSpriteState
{
    int bitmap_handle;
    int viewport_handle;
    EasyRpgRgssRect source_rect;
    int x;
    int y;
    int z;
    int ox;
    int oy;
    int zoom_x_milli;
    int zoom_y_milli;
    int opacity;
    int visible;
    int mirror;
    int blend_type;
} EasyRpgRgssSpriteState;

int EasyRpgRgssSpriteInitialize(const EasyRpgRgssSpriteHost* host);
void EasyRpgRgssSpriteShutdown(void);
int EasyRpgRgssSpriteCreate(int* handle);
int EasyRpgRgssSpriteDestroy(int handle);
int EasyRpgRgssSpriteGetState(int handle, EasyRpgRgssSpriteState* state);
int EasyRpgRgssSpriteSetState(int handle, const EasyRpgRgssSpriteState* state);
int EasyRpgRgssSpriteSetBitmap(int handle, int bitmap_handle);
int EasyRpgRgssSpriteSetViewport(int handle, int viewport_handle);
int EasyRpgRgssSpriteSetSourceRect(int handle, const EasyRpgRgssRect* source_rect);
int EasyRpgRgssSpriteSetPosition(int handle, int x, int y, int z);
int EasyRpgRgssSpriteSetOrigin(int handle, int ox, int oy);
int EasyRpgRgssSpriteSetZoom(int handle, int zoom_x_milli, int zoom_y_milli);
int EasyRpgRgssSpriteSetOpacity(int handle, int opacity);
int EasyRpgRgssSpriteSetVisible(int handle, int visible);
int EasyRpgRgssSpriteSetMirror(int handle, int mirror);
int EasyRpgRgssSpriteSetBlendType(int handle, int blend_type);
int EasyRpgRgssSpriteLogicalBounds(int handle, EasyRpgRgssRect* bounds);
int EasyRpgRgssSpriteRenderRange(int minimum_z,
                                 int maximum_z,
                                 EasyRpgRgssRect* output_dirty);
int EasyRpgRgssSpriteCount(void);

#ifdef __cplusplus
}
#endif

#endif

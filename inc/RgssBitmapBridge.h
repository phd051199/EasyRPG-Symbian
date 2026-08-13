#ifndef EASYRPG_RGSS_BITMAP_BRIDGE_H
#define EASYRPG_RGSS_BITMAP_BRIDGE_H

#include "RgssCoreBridge.h"

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssBitmapBlend
{
    ERgssBitmapBlendNormal = 0,
    ERgssBitmapBlendAdd = 1,
    ERgssBitmapBlendSubtract = 2
};

enum EasyRpgRgssBitmapResult
{
    ERgssBitmapOk = 0,
    ERgssBitmapErrorInvalidArgument = -40,
    ERgssBitmapErrorNotInitialized = -41,
    ERgssBitmapErrorNoMemory = -42,
    ERgssBitmapErrorCapacity = -43,
    ERgssBitmapErrorInvalidHandle = -44,
    ERgssBitmapErrorFramebuffer = -45
};

typedef void* (*EasyRpgRgssAllocFn)(void* context, unsigned int bytes);
typedef void (*EasyRpgRgssFreeFn)(void* context, void* memory);

typedef struct EasyRpgRgssBitmapHost
{
    void* context;
    EasyRpgRgssAllocFn allocate;
    EasyRpgRgssFreeFn release;
    int max_bitmaps;
} EasyRpgRgssBitmapHost;

int EasyRpgRgssBitmapInitialize(const EasyRpgRgssBitmapHost* host);
void EasyRpgRgssBitmapShutdown(void);
int EasyRpgRgssBitmapCreate(int width, int height, int* handle);
int EasyRpgRgssBitmapDestroy(int handle);
int EasyRpgRgssBitmapWidth(int handle);
int EasyRpgRgssBitmapHeight(int handle);
int EasyRpgRgssBitmapClear(int handle);
int EasyRpgRgssBitmapFillRect(int handle,
                              int x,
                              int y,
                              int width,
                              int height,
                              int red,
                              int green,
                              int blue,
                              int alpha);
int EasyRpgRgssBitmapSetPixel(int handle,
                              int x,
                              int y,
                              int red,
                              int green,
                              int blue,
                              int alpha);
int EasyRpgRgssBitmapGetPixel(int handle,
                              int x,
                              int y,
                              int* red,
                              int* green,
                              int* blue,
                              int* alpha);
int EasyRpgRgssBitmapLock(int handle,
                          unsigned short** rgb565,
                          unsigned char** alpha,
                          int* pitch_pixels);
int EasyRpgRgssBitmapBlit(int handle,
                          const EasyRpgRgssRect* source_rect,
                          int destination_x,
                          int destination_y,
                          int opacity,
                          EasyRpgRgssRect* output_dirty);
int EasyRpgRgssBitmapBlitEx(int handle,
                            const EasyRpgRgssRect* source_rect,
                            const EasyRpgRgssRect* destination_rect,
                            int opacity,
                            int mirror,
                            int blend_type,
                            EasyRpgRgssRect* output_dirty);
int EasyRpgRgssBitmapBlitExClipped(int handle,
                                   const EasyRpgRgssRect* source_rect,
                                   const EasyRpgRgssRect* destination_rect,
                                   const EasyRpgRgssRect* clip_rect,
                                   int opacity,
                                   int mirror,
                                   int blend_type,
                                   EasyRpgRgssRect* output_dirty);
int EasyRpgRgssBitmapTile(int handle,
                          const EasyRpgRgssRect* source_rect,
                          const EasyRpgRgssRect* destination_rect,
                          int offset_x,
                          int offset_y,
                          int zoom_x_milli,
                          int zoom_y_milli,
                          int opacity,
                          int blend_type,
                          EasyRpgRgssRect* output_dirty);

#ifdef __cplusplus
}
#endif

#endif

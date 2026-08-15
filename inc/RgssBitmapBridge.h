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
typedef int (*EasyRpgRgssGlyphFn)(void* context,
                                  unsigned int codepoint,
                                  const unsigned short** rows,
                                  int* width,
                                  int* height,
                                  int* advance);

typedef struct EasyRpgRgssBlitEffects
{
    int tone_red;
    int tone_green;
    int tone_blue;
    int tone_gray;
    int color_red;
    int color_green;
    int color_blue;
    int color_alpha;
    int bush_depth;
    int bush_opacity;
} EasyRpgRgssBlitEffects;

typedef struct EasyRpgRgssTextStyle
{
    int font_size;
    int red;
    int green;
    int blue;
    int alpha;
    int bold;
    int italic;
    int shadow;
    int align;
} EasyRpgRgssTextStyle;

typedef struct EasyRpgRgssBitmapHost
{
    void* context;
    EasyRpgRgssAllocFn allocate;
    EasyRpgRgssFreeFn release;
    EasyRpgRgssGlyphFn glyph;
    int max_bitmaps;
} EasyRpgRgssBitmapHost;

int EasyRpgRgssBitmapInitialize(const EasyRpgRgssBitmapHost* host);
void EasyRpgRgssBitmapShutdown(void);
void EasyRpgRgssBitmapSetGlyphProvider(void* context, EasyRpgRgssGlyphFn glyph);
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
int EasyRpgRgssBitmapClearRect(int handle,
                               int x,
                               int y,
                               int width,
                               int height);
int EasyRpgRgssBitmapGradientFillRect(int handle,
                                      int x,
                                      int y,
                                      int width,
                                      int height,
                                      int red1,
                                      int green1,
                                      int blue1,
                                      int alpha1,
                                      int red2,
                                      int green2,
                                      int blue2,
                                      int alpha2,
                                      int vertical);
int EasyRpgRgssBitmapSetPixel(int handle,
                              int x,
                              int y,
                              int red,
                              int green,
                              int blue,
                              int alpha);
int EasyRpgRgssBitmapBlt(int destination_handle,
                         int destination_x,
                         int destination_y,
                         int source_handle,
                         const EasyRpgRgssRect* source_rect,
                         int opacity);
int EasyRpgRgssBitmapStretchBlt(int destination_handle,
                                const EasyRpgRgssRect* destination_rect,
                                int source_handle,
                                const EasyRpgRgssRect* source_rect,
                                int opacity);
int EasyRpgRgssBitmapHueChange(int handle, int hue_degrees);
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
int EasyRpgRgssBitmapBlitEffects(int handle,
                                 const EasyRpgRgssRect* source_rect,
                                 const EasyRpgRgssRect* destination_rect,
                                 const EasyRpgRgssRect* clip_rect,
                                 int opacity,
                                 int mirror,
                                 int blend_type,
                                 const EasyRpgRgssBlitEffects* effects,
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
int EasyRpgRgssBitmapTextSize(int handle,
                              const unsigned int* codepoints,
                              int count,
                              const EasyRpgRgssTextStyle* style,
                              int* width,
                              int* height);
int EasyRpgRgssBitmapDrawText(int handle,
                              const EasyRpgRgssRect* rect,
                              const unsigned int* codepoints,
                              int count,
                              const EasyRpgRgssTextStyle* style);

#ifdef __cplusplus
}
#endif

#endif

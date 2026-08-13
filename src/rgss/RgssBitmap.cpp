#include "RgssBitmapBridge.h"

namespace
{
struct TBitmapSlot
{
    unsigned short* color;
    unsigned char* alpha;
    int width;
    int height;
    unsigned short generation;
    unsigned char used;
};

struct TBitmapState
{
    EasyRpgRgssBitmapHost host;
    TBitmapSlot* slots;
    int initialized;
};

TBitmapState gBitmaps;

#if defined(__arm__)
extern "C" int __aeabi_idiv(int numerator, int denominator);
#endif

int DivideInt(int numerator, int denominator)
{
#if defined(__arm__)
    return __aeabi_idiv(numerator, denominator);
#else
    return numerator / denominator;
#endif
}

int ClampByte(int value)
{
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
}

unsigned short Pack565(int red, int green, int blue)
{
    const unsigned int r = static_cast<unsigned int>(ClampByte(red));
    const unsigned int g = static_cast<unsigned int>(ClampByte(green));
    const unsigned int b = static_cast<unsigned int>(ClampByte(blue));
    return static_cast<unsigned short>(((r >> 3) << 11) |
                                       ((g >> 2) << 5) |
                                       (b >> 3));
}

void Unpack565(unsigned short value, int& red, int& green, int& blue)
{
    const int r = (value >> 11) & 31;
    const int g = (value >> 5) & 63;
    const int b = value & 31;
    red = (r << 3) | (r >> 2);
    green = (g << 2) | (g >> 4);
    blue = (b << 3) | (b >> 2);
}

int Divide255Round(int value)
{
    const int rounded = value + 128;
    return (rounded + (rounded >> 8)) >> 8;
}

unsigned short Blend565(unsigned short destination,
                        unsigned short source,
                        int alpha)
{
    if (alpha <= 0)
        return destination;
    if (alpha >= 255)
        return source;

    const int srcR = (source >> 11) & 31;
    const int srcG = (source >> 5) & 63;
    const int srcB = source & 31;
    const int dstR = (destination >> 11) & 31;
    const int dstG = (destination >> 5) & 63;
    const int dstB = destination & 31;
    const int inverse = 255 - alpha;
    const int outR = Divide255Round(srcR * alpha + dstR * inverse);
    const int outG = Divide255Round(srcG * alpha + dstG * inverse);
    const int outB = Divide255Round(srcB * alpha + dstB * inverse);
    return static_cast<unsigned short>((outR << 11) | (outG << 5) | outB);
}

unsigned short Composite565(unsigned short destination,
                            unsigned short source,
                            int alpha,
                            int blendType)
{
    if (blendType == ERgssBitmapBlendNormal)
        return Blend565(destination, source, alpha);
    if (alpha <= 0)
        return destination;

    const int srcR = (source >> 11) & 31;
    const int srcG = (source >> 5) & 63;
    const int srcB = source & 31;
    const int dstR = (destination >> 11) & 31;
    const int dstG = (destination >> 5) & 63;
    const int dstB = destination & 31;
    const int amountR = Divide255Round(srcR * alpha);
    const int amountG = Divide255Round(srcG * alpha);
    const int amountB = Divide255Round(srcB * alpha);
    int outR = dstR;
    int outG = dstG;
    int outB = dstB;
    if (blendType == ERgssBitmapBlendAdd)
    {
        outR += amountR;
        outG += amountG;
        outB += amountB;
        if (outR > 31)
            outR = 31;
        if (outG > 63)
            outG = 63;
        if (outB > 31)
            outB = 31;
    }
    else
    {
        outR -= amountR;
        outG -= amountG;
        outB -= amountB;
        if (outR < 0)
            outR = 0;
        if (outG < 0)
            outG = 0;
        if (outB < 0)
            outB = 0;
    }
    return static_cast<unsigned short>((outR << 11) | (outG << 5) | outB);
}

TBitmapSlot* FindSlot(int handle)
{
    if (!gBitmaps.initialized || handle <= 0)
        return 0;
    const int encodedIndex = handle & 0xffff;
    const int generation = (handle >> 16) & 0x7fff;
    if (encodedIndex <= 0 || encodedIndex > gBitmaps.host.max_bitmaps || generation <= 0)
        return 0;
    TBitmapSlot* slot = &gBitmaps.slots[encodedIndex - 1];
    if (!slot->used || slot->generation != generation)
        return 0;
    return slot;
}

int MakeHandle(int index, unsigned short generation)
{
    return (static_cast<int>(generation & 0x7fff) << 16) | (index + 1);
}

void ReleaseSlot(TBitmapSlot& slot)
{
    if (slot.color)
        gBitmaps.host.release(gBitmaps.host.context, slot.color);
    if (slot.alpha)
        gBitmaps.host.release(gBitmaps.host.context, slot.alpha);
    slot.color = 0;
    slot.alpha = 0;
    slot.width = 0;
    slot.height = 0;
    slot.used = 0;
}

void ZeroDirty(EasyRpgRgssRect* rect)
{
    if (!rect)
        return;
    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;
}
}

extern "C" int EasyRpgRgssBitmapInitialize(const EasyRpgRgssBitmapHost* host)
{
    if (!host || !host->allocate || !host->release ||
        host->max_bitmaps <= 0 || host->max_bitmaps > 65535)
        return ERgssBitmapErrorInvalidArgument;

    EasyRpgRgssBitmapShutdown();
    const unsigned int bytes = static_cast<unsigned int>(host->max_bitmaps) *
                               static_cast<unsigned int>(sizeof(TBitmapSlot));
    TBitmapSlot* slots = static_cast<TBitmapSlot*>(host->allocate(host->context, bytes));
    if (!slots)
        return ERgssBitmapErrorNoMemory;

    gBitmaps.host = *host;
    gBitmaps.slots = slots;
    gBitmaps.initialized = 1;
    for (int index = 0; index < host->max_bitmaps; ++index)
    {
        slots[index].color = 0;
        slots[index].alpha = 0;
        slots[index].width = 0;
        slots[index].height = 0;
        slots[index].generation = 0;
        slots[index].used = 0;
    }
    return ERgssBitmapOk;
}

extern "C" void EasyRpgRgssBitmapShutdown(void)
{
    if (!gBitmaps.initialized)
        return;
    for (int index = 0; index < gBitmaps.host.max_bitmaps; ++index)
    {
        if (gBitmaps.slots[index].used)
            ReleaseSlot(gBitmaps.slots[index]);
    }
    gBitmaps.host.release(gBitmaps.host.context, gBitmaps.slots);
    gBitmaps.slots = 0;
    gBitmaps.host.context = 0;
    gBitmaps.host.allocate = 0;
    gBitmaps.host.release = 0;
    gBitmaps.host.max_bitmaps = 0;
    gBitmaps.initialized = 0;
}

extern "C" int EasyRpgRgssBitmapCreate(int width, int height, int* handle)
{
    if (!gBitmaps.initialized)
        return ERgssBitmapErrorNotInitialized;
    if (!handle || width <= 0 || height <= 0 || width > 32767 || height > 32767)
        return ERgssBitmapErrorInvalidArgument;

    const unsigned int w = static_cast<unsigned int>(width);
    const unsigned int h = static_cast<unsigned int>(height);
    if (w > 0xffffffffu / h)
        return ERgssBitmapErrorNoMemory;
    const unsigned int pixels = w * h;
    if (pixels > 0xffffffffu / 2u)
        return ERgssBitmapErrorNoMemory;

    int slotIndex = -1;
    for (int index = 0; index < gBitmaps.host.max_bitmaps; ++index)
    {
        if (!gBitmaps.slots[index].used)
        {
            slotIndex = index;
            break;
        }
    }
    if (slotIndex < 0)
        return ERgssBitmapErrorCapacity;

    unsigned short* color = static_cast<unsigned short*>(
        gBitmaps.host.allocate(gBitmaps.host.context, pixels * 2u));
    if (!color)
        return ERgssBitmapErrorNoMemory;
    unsigned char* alpha = static_cast<unsigned char*>(
        gBitmaps.host.allocate(gBitmaps.host.context, pixels));
    if (!alpha)
    {
        gBitmaps.host.release(gBitmaps.host.context, color);
        return ERgssBitmapErrorNoMemory;
    }

    for (unsigned int pixel = 0; pixel < pixels; ++pixel)
    {
        color[pixel] = 0;
        alpha[pixel] = 0;
    }

    TBitmapSlot& slot = gBitmaps.slots[slotIndex];
    unsigned short generation = static_cast<unsigned short>((slot.generation + 1u) & 0x7fffu);
    if (generation == 0)
        generation = 1;
    slot.color = color;
    slot.alpha = alpha;
    slot.width = width;
    slot.height = height;
    slot.generation = generation;
    slot.used = 1;
    *handle = MakeHandle(slotIndex, generation);
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapDestroy(int handle)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return gBitmaps.initialized ? ERgssBitmapErrorInvalidHandle : ERgssBitmapErrorNotInitialized;
    ReleaseSlot(*slot);
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapWidth(int handle)
{
    TBitmapSlot* slot = FindSlot(handle);
    return slot ? slot->width : ERgssBitmapErrorInvalidHandle;
}

extern "C" int EasyRpgRgssBitmapHeight(int handle)
{
    TBitmapSlot* slot = FindSlot(handle);
    return slot ? slot->height : ERgssBitmapErrorInvalidHandle;
}

extern "C" int EasyRpgRgssBitmapClear(int handle)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    const unsigned int pixels = static_cast<unsigned int>(slot->width) *
                                static_cast<unsigned int>(slot->height);
    for (unsigned int pixel = 0; pixel < pixels; ++pixel)
    {
        slot->color[pixel] = 0;
        slot->alpha[pixel] = 0;
    }
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapFillRect(int handle,
                                          int x,
                                          int y,
                                          int width,
                                          int height,
                                          int red,
                                          int green,
                                          int blue,
                                          int alpha)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (width < 0 || height < 0)
        return ERgssBitmapErrorInvalidArgument;
    if (width == 0 || height == 0)
        return ERgssBitmapOk;

    int left = x < 0 ? 0 : x;
    int top = y < 0 ? 0 : y;
    int right = x + width;
    int bottom = y + height;
    if (right > slot->width)
        right = slot->width;
    if (bottom > slot->height)
        bottom = slot->height;
    if (right <= left || bottom <= top)
        return ERgssBitmapOk;

    const unsigned short color = Pack565(red, green, blue);
    const unsigned char opacity = static_cast<unsigned char>(ClampByte(alpha));
    for (int row = top; row < bottom; ++row)
    {
        const int base = row * slot->width;
        for (int column = left; column < right; ++column)
        {
            slot->color[base + column] = color;
            slot->alpha[base + column] = opacity;
        }
    }
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapSetPixel(int handle,
                                          int x,
                                          int y,
                                          int red,
                                          int green,
                                          int blue,
                                          int alpha)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (x < 0 || y < 0 || x >= slot->width || y >= slot->height)
        return ERgssBitmapOk;
    const int index = y * slot->width + x;
    slot->color[index] = Pack565(red, green, blue);
    slot->alpha[index] = static_cast<unsigned char>(ClampByte(alpha));
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapGetPixel(int handle,
                                          int x,
                                          int y,
                                          int* red,
                                          int* green,
                                          int* blue,
                                          int* alpha)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!red || !green || !blue || !alpha)
        return ERgssBitmapErrorInvalidArgument;
    if (x < 0 || y < 0 || x >= slot->width || y >= slot->height)
    {
        *red = 0;
        *green = 0;
        *blue = 0;
        *alpha = 0;
        return ERgssBitmapOk;
    }
    const int index = y * slot->width + x;
    Unpack565(slot->color[index], *red, *green, *blue);
    *alpha = slot->alpha[index];
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapLock(int handle,
                                      unsigned short** rgb565,
                                      unsigned char** alpha,
                                      int* pitch_pixels)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!rgb565 || !alpha || !pitch_pixels)
        return ERgssBitmapErrorInvalidArgument;
    *rgb565 = slot->color;
    *alpha = slot->alpha;
    *pitch_pixels = slot->width;
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapBlit(int handle,
                                      const EasyRpgRgssRect* source_rect,
                                      int destination_x,
                                      int destination_y,
                                      int opacity,
                                      EasyRpgRgssRect* output_dirty)
{
    ZeroDirty(output_dirty);
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!source_rect || source_rect->width < 0 || source_rect->height < 0)
        return ERgssBitmapErrorInvalidArgument;
    if (source_rect->width == 0 || source_rect->height == 0 || opacity <= 0)
        return ERgssBitmapOk;

    int sourceX = source_rect->x;
    int sourceY = source_rect->y;
    int sourceWidth = source_rect->width;
    int sourceHeight = source_rect->height;
    if (sourceX < 0)
    {
        const int skip = -sourceX;
        sourceX = 0;
        sourceWidth -= skip;
        destination_x += skip;
    }
    if (sourceY < 0)
    {
        const int skip = -sourceY;
        sourceY = 0;
        sourceHeight -= skip;
        destination_y += skip;
    }
    if (sourceX + sourceWidth > slot->width)
        sourceWidth = slot->width - sourceX;
    if (sourceY + sourceHeight > slot->height)
        sourceHeight = slot->height - sourceY;
    if (sourceWidth <= 0 || sourceHeight <= 0)
        return ERgssBitmapOk;

    EasyRpgRgssConfig config;
    if (EasyRpgRgssCoreGetConfig(&config) != ERgssOk)
        return ERgssBitmapErrorFramebuffer;

    int logicalX = destination_x;
    int logicalY = destination_y;
    int logicalWidth = sourceWidth;
    int logicalHeight = sourceHeight;
    if (logicalX < 0)
    {
        const int skip = -logicalX;
        logicalX = 0;
        logicalWidth -= skip;
        sourceX += skip;
    }
    if (logicalY < 0)
    {
        const int skip = -logicalY;
        logicalY = 0;
        logicalHeight -= skip;
        sourceY += skip;
    }
    if (logicalX + logicalWidth > config.logical_width)
        logicalWidth = config.logical_width - logicalX;
    if (logicalY + logicalHeight > config.logical_height)
        logicalHeight = config.logical_height - logicalY;
    if (logicalWidth <= 0 || logicalHeight <= 0)
        return ERgssBitmapOk;

    EasyRpgRgssRect logicalRect;
    logicalRect.x = logicalX;
    logicalRect.y = logicalY;
    logicalRect.width = logicalWidth;
    logicalRect.height = logicalHeight;
    EasyRpgRgssRect physicalRect;
    if (EasyRpgRgssCoreMapRect(&logicalRect, &physicalRect) != ERgssOk ||
        physicalRect.width <= 0 || physicalRect.height <= 0)
        return ERgssBitmapErrorFramebuffer;

    void* framebuffer = 0;
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    int framebufferPitch = 0;
    int framebufferFormat = 0;
    if (EasyRpgRgssCoreGetFramebuffer(&framebuffer,
                                       &framebufferWidth,
                                       &framebufferHeight,
                                       &framebufferPitch,
                                       &framebufferFormat) != ERgssOk ||
        framebufferFormat != ERgssPixelRgb565)
        return ERgssBitmapErrorFramebuffer;

    const int globalOpacity = ClampByte(opacity);
    unsigned char* targetBytes = static_cast<unsigned char*>(framebuffer);
    for (int py = 0; py < physicalRect.height; ++py)
    {
        const int sourceOffsetY = (py * logicalHeight) / physicalRect.height;
        const int sy = sourceY + sourceOffsetY;
        unsigned short* target = reinterpret_cast<unsigned short*>(
            targetBytes + (physicalRect.y + py) * framebufferPitch);
        for (int px = 0; px < physicalRect.width; ++px)
        {
            const int sourceOffsetX = (px * logicalWidth) / physicalRect.width;
            const int sx = sourceX + sourceOffsetX;
            const int sourceIndex = sy * slot->width + sx;
            const int effectiveAlpha = Divide255Round(
                static_cast<int>(slot->alpha[sourceIndex]) * globalOpacity);
            const int targetX = physicalRect.x + px;
            if (targetX < 0 || targetX >= framebufferWidth ||
                physicalRect.y + py < 0 || physicalRect.y + py >= framebufferHeight)
                continue;
            target[targetX] = Blend565(target[targetX],
                                       slot->color[sourceIndex],
                                       effectiveAlpha);
        }
    }

    if (output_dirty)
        *output_dirty = physicalRect;
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapBlitExClipped(int handle,
                                               const EasyRpgRgssRect* source_rect,
                                               const EasyRpgRgssRect* destination_rect,
                                               const EasyRpgRgssRect* clip_rect,
                                               int opacity,
                                               int mirror,
                                               int blend_type,
                                               EasyRpgRgssRect* output_dirty)
{
    ZeroDirty(output_dirty);
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!source_rect || !destination_rect ||
        source_rect->width < 0 || source_rect->height < 0 ||
        destination_rect->width < 0 || destination_rect->height < 0 ||
        (clip_rect && (clip_rect->width < 0 || clip_rect->height < 0)) ||
        blend_type < ERgssBitmapBlendNormal ||
        blend_type > ERgssBitmapBlendSubtract)
        return ERgssBitmapErrorInvalidArgument;
    if (source_rect->width == 0 || source_rect->height == 0 ||
        destination_rect->width == 0 || destination_rect->height == 0 ||
        opacity <= 0)
        return ERgssBitmapOk;

    EasyRpgRgssConfig config;
    if (EasyRpgRgssCoreGetConfig(&config) != ERgssOk)
        return ERgssBitmapErrorFramebuffer;

    EasyRpgRgssRect clippedDestination = *destination_rect;
    if (clippedDestination.x < 0)
    {
        clippedDestination.width += clippedDestination.x;
        clippedDestination.x = 0;
    }
    if (clippedDestination.y < 0)
    {
        clippedDestination.height += clippedDestination.y;
        clippedDestination.y = 0;
    }
    if (clippedDestination.x + clippedDestination.width > config.logical_width)
        clippedDestination.width = config.logical_width - clippedDestination.x;
    if (clippedDestination.y + clippedDestination.height > config.logical_height)
        clippedDestination.height = config.logical_height - clippedDestination.y;
    if (clip_rect)
    {
        const int left = clippedDestination.x > clip_rect->x
            ? clippedDestination.x : clip_rect->x;
        const int top = clippedDestination.y > clip_rect->y
            ? clippedDestination.y : clip_rect->y;
        const int destinationRight = clippedDestination.x + clippedDestination.width;
        const int destinationBottom = clippedDestination.y + clippedDestination.height;
        const int clipRight = clip_rect->x + clip_rect->width;
        const int clipBottom = clip_rect->y + clip_rect->height;
        const int right = destinationRight < clipRight ? destinationRight : clipRight;
        const int bottom = destinationBottom < clipBottom ? destinationBottom : clipBottom;
        clippedDestination.x = left;
        clippedDestination.y = top;
        clippedDestination.width = right - left;
        clippedDestination.height = bottom - top;
    }
    if (clippedDestination.width <= 0 || clippedDestination.height <= 0)
        return ERgssBitmapOk;

    EasyRpgRgssRect physicalRect;
    if (EasyRpgRgssCoreMapRect(&clippedDestination, &physicalRect) != ERgssOk ||
        physicalRect.width <= 0 || physicalRect.height <= 0)
        return ERgssBitmapErrorFramebuffer;

    void* framebuffer = 0;
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    int framebufferPitch = 0;
    int framebufferFormat = 0;
    if (EasyRpgRgssCoreGetFramebuffer(&framebuffer,
                                       &framebufferWidth,
                                       &framebufferHeight,
                                       &framebufferPitch,
                                       &framebufferFormat) != ERgssOk ||
        framebufferFormat != ERgssPixelRgb565)
        return ERgssBitmapErrorFramebuffer;

    const int globalOpacity = ClampByte(opacity);
    unsigned char* targetBytes = static_cast<unsigned char*>(framebuffer);
    for (int py = 0; py < physicalRect.height; ++py)
    {
        const int logicalY = clippedDestination.y +
            (py * clippedDestination.height) / physicalRect.height;
        const int relativeY = logicalY - destination_rect->y;
        const int sourceOffsetY =
            (relativeY * source_rect->height) / destination_rect->height;
        const int sourceY = source_rect->y + sourceOffsetY;
        const int targetY = physicalRect.y + py;
        if (sourceY < 0 || sourceY >= slot->height ||
            targetY < 0 || targetY >= framebufferHeight)
            continue;
        unsigned short* target = reinterpret_cast<unsigned short*>(
            targetBytes + targetY * framebufferPitch);
        for (int px = 0; px < physicalRect.width; ++px)
        {
            const int logicalX = clippedDestination.x +
                (px * clippedDestination.width) / physicalRect.width;
            const int relativeX = logicalX - destination_rect->x;
            int sourceOffsetX =
                (relativeX * source_rect->width) / destination_rect->width;
            if (mirror)
                sourceOffsetX = source_rect->width - 1 - sourceOffsetX;
            const int sourceX = source_rect->x + sourceOffsetX;
            const int targetX = physicalRect.x + px;
            if (sourceX < 0 || sourceX >= slot->width ||
                targetX < 0 || targetX >= framebufferWidth)
                continue;
            const int sourceIndex = sourceY * slot->width + sourceX;
            const int effectiveAlpha = Divide255Round(
                static_cast<int>(slot->alpha[sourceIndex]) * globalOpacity);
            target[targetX] = Composite565(target[targetX],
                                           slot->color[sourceIndex],
                                           effectiveAlpha,
                                           blend_type);
        }
    }

    if (output_dirty)
        *output_dirty = physicalRect;
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapBlitEx(int handle,
                                        const EasyRpgRgssRect* source_rect,
                                        const EasyRpgRgssRect* destination_rect,
                                        int opacity,
                                        int mirror,
                                        int blend_type,
                                        EasyRpgRgssRect* output_dirty)
{
    return EasyRpgRgssBitmapBlitExClipped(handle,
                                          source_rect,
                                          destination_rect,
                                          0,
                                          opacity,
                                          mirror,
                                          blend_type,
                                          output_dirty);
}

extern "C" int EasyRpgRgssBitmapTile(int handle,
                                      const EasyRpgRgssRect* source_rect,
                                      const EasyRpgRgssRect* destination_rect,
                                      int offset_x,
                                      int offset_y,
                                      int zoom_x_milli,
                                      int zoom_y_milli,
                                      int opacity,
                                      int blend_type,
                                      EasyRpgRgssRect* output_dirty)
{
    ZeroDirty(output_dirty);
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!source_rect || !destination_rect ||
        source_rect->x < 0 || source_rect->y < 0 ||
        source_rect->width <= 0 || source_rect->height <= 0 ||
        source_rect->x + source_rect->width > slot->width ||
        source_rect->y + source_rect->height > slot->height ||
        destination_rect->width < 0 || destination_rect->height < 0 ||
        zoom_x_milli <= 0 || zoom_y_milli <= 0 ||
        zoom_x_milli > 16000 || zoom_y_milli > 16000 ||
        blend_type < ERgssBitmapBlendNormal ||
        blend_type > ERgssBitmapBlendSubtract)
        return ERgssBitmapErrorInvalidArgument;
    if (destination_rect->width == 0 || destination_rect->height == 0 || opacity <= 0)
        return ERgssBitmapOk;
    if (source_rect->width > (2147483647 - 999) / zoom_x_milli ||
        source_rect->height > (2147483647 - 999) / zoom_y_milli)
        return ERgssBitmapErrorInvalidArgument;

    const int periodX = DivideInt(source_rect->width * zoom_x_milli + 999, 1000);
    const int periodY = DivideInt(source_rect->height * zoom_y_milli + 999, 1000);
    if (periodX <= 0 || periodY <= 0)
        return ERgssBitmapOk;
    int normalizedOffsetX = offset_x % periodX;
    int normalizedOffsetY = offset_y % periodY;
    if (normalizedOffsetX < 0)
        normalizedOffsetX += periodX;
    if (normalizedOffsetY < 0)
        normalizedOffsetY += periodY;

    EasyRpgRgssConfig config;
    if (EasyRpgRgssCoreGetConfig(&config) != ERgssOk)
        return ERgssBitmapErrorFramebuffer;

    EasyRpgRgssRect clippedDestination = *destination_rect;
    if (clippedDestination.x < 0)
    {
        clippedDestination.width += clippedDestination.x;
        clippedDestination.x = 0;
    }
    if (clippedDestination.y < 0)
    {
        clippedDestination.height += clippedDestination.y;
        clippedDestination.y = 0;
    }
    if (clippedDestination.x + clippedDestination.width > config.logical_width)
        clippedDestination.width = config.logical_width - clippedDestination.x;
    if (clippedDestination.y + clippedDestination.height > config.logical_height)
        clippedDestination.height = config.logical_height - clippedDestination.y;
    if (clippedDestination.width <= 0 || clippedDestination.height <= 0)
        return ERgssBitmapOk;

    EasyRpgRgssRect physicalRect;
    if (EasyRpgRgssCoreMapRect(&clippedDestination, &physicalRect) != ERgssOk ||
        physicalRect.width <= 0 || physicalRect.height <= 0)
        return ERgssBitmapErrorFramebuffer;

    void* framebuffer = 0;
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    int framebufferPitch = 0;
    int framebufferFormat = 0;
    if (EasyRpgRgssCoreGetFramebuffer(&framebuffer,
                                       &framebufferWidth,
                                       &framebufferHeight,
                                       &framebufferPitch,
                                       &framebufferFormat) != ERgssOk ||
        framebufferFormat != ERgssPixelRgb565)
        return ERgssBitmapErrorFramebuffer;

    const int globalOpacity = ClampByte(opacity);
    unsigned char* targetBytes = static_cast<unsigned char*>(framebuffer);
    for (int py = 0; py < physicalRect.height; ++py)
    {
        const int logicalY = clippedDestination.y +
            (py * clippedDestination.height) / physicalRect.height;
        int tileY = (logicalY - destination_rect->y + normalizedOffsetY) % periodY;
        if (tileY < 0)
            tileY += periodY;
        int sourceOffsetY = (tileY * 1000) / zoom_y_milli;
        if (sourceOffsetY >= source_rect->height)
            sourceOffsetY = source_rect->height - 1;
        const int sourceY = source_rect->y + sourceOffsetY;
        const int targetY = physicalRect.y + py;
        if (targetY < 0 || targetY >= framebufferHeight)
            continue;
        unsigned short* target = reinterpret_cast<unsigned short*>(
            targetBytes + targetY * framebufferPitch);
        for (int px = 0; px < physicalRect.width; ++px)
        {
            const int logicalX = clippedDestination.x +
                (px * clippedDestination.width) / physicalRect.width;
            int tileX = (logicalX - destination_rect->x + normalizedOffsetX) % periodX;
            if (tileX < 0)
                tileX += periodX;
            int sourceOffsetX = (tileX * 1000) / zoom_x_milli;
            if (sourceOffsetX >= source_rect->width)
                sourceOffsetX = source_rect->width - 1;
            const int sourceX = source_rect->x + sourceOffsetX;
            const int targetX = physicalRect.x + px;
            if (targetX < 0 || targetX >= framebufferWidth)
                continue;
            const int sourceIndex = sourceY * slot->width + sourceX;
            const int effectiveAlpha = Divide255Round(
                static_cast<int>(slot->alpha[sourceIndex]) * globalOpacity);
            target[targetX] = Composite565(target[targetX],
                                           slot->color[sourceIndex],
                                           effectiveAlpha,
                                           blend_type);
        }
    }

    if (output_dirty)
        *output_dirty = physicalRect;
    return ERgssBitmapOk;
}

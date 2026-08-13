#include "RgssBitmapBridge.h"
#include "RgssRenderBridge.h"
#include "RgssSpriteBridge.h"
#include "RgssViewportBridge.h"

namespace
{
struct TSpriteSlot
{
    EasyRpgRgssSpriteState state;
    unsigned int serial;
    int render_handle;
    unsigned short generation;
    unsigned char used;
};

struct TSpriteState
{
    EasyRpgRgssSpriteHost host;
    TSpriteSlot* slots;
    int* order;
    int order_count;
    int order_dirty;
    int active_count;
    unsigned int next_serial;
    unsigned int viewport_revision;
    int initialized;
};

TSpriteState gSprites;

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

void ZeroRect(EasyRpgRgssRect* rect)
{
    if (!rect)
        return;
    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;
}

void DefaultState(EasyRpgRgssSpriteState& state)
{
    state.bitmap_handle = 0;
    state.viewport_handle = 0;
    state.source_rect.x = 0;
    state.source_rect.y = 0;
    state.source_rect.width = 0;
    state.source_rect.height = 0;
    state.x = 0;
    state.y = 0;
    state.z = 0;
    state.ox = 0;
    state.oy = 0;
    state.zoom_x_milli = 1000;
    state.zoom_y_milli = 1000;
    state.opacity = 255;
    state.visible = 1;
    state.mirror = 0;
    state.blend_type = ERgssBitmapBlendNormal;
}

TSpriteSlot* FindSlot(int handle)
{
    if (!gSprites.initialized || handle <= 0)
        return 0;
    const int encodedIndex = handle & 0xffff;
    const int generation = (handle >> 16) & 0x7fff;
    if (encodedIndex <= 0 || encodedIndex > gSprites.host.max_sprites || generation <= 0)
        return 0;
    TSpriteSlot* slot = &gSprites.slots[encodedIndex - 1];
    if (!slot->used || slot->generation != generation)
        return 0;
    return slot;
}

int MakeHandle(int index, unsigned short generation)
{
    return (static_cast<int>(generation & 0x7fff) << 16) | (index + 1);
}

int ValidateState(const EasyRpgRgssSpriteState& state)
{
    if (state.source_rect.width < 0 || state.source_rect.height < 0 ||
        state.zoom_x_milli < 0 || state.zoom_y_milli < 0 ||
        state.zoom_x_milli > 16000 || state.zoom_y_milli > 16000 ||
        state.opacity < 0 || state.opacity > 255 ||
        state.blend_type < ERgssBitmapBlendNormal ||
        state.blend_type > ERgssBitmapBlendSubtract)
        return ERgssSpriteErrorInvalidArgument;
    if (state.bitmap_handle != 0 &&
        (EasyRpgRgssBitmapWidth(state.bitmap_handle) < 0 ||
         EasyRpgRgssBitmapHeight(state.bitmap_handle) < 0))
        return ERgssSpriteErrorBitmap;
    if (state.viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(state.viewport_handle, &viewport) != ERgssViewportOk)
            return ERgssSpriteErrorInvalidArgument;
    }
    return ERgssSpriteOk;
}

int ScaleValue(int value, int scale, int& result)
{
    if (value < 0 || scale < 0)
        return 0;
    if (scale != 0 && value > (2147483647 - 999) / scale)
        return 0;
    result = DivideInt(value * scale + 999, 1000);
    return 1;
}

int ScaleOrigin(int value, int scale, int& result)
{
    if (scale < 0)
        return 0;
    if (scale != 0 &&
        (value > 2147483647 / scale ||
         value < (-2147483647 - 1) / scale))
        return 0;
    result = DivideInt(value * scale, 1000);
    return 1;
}

int ComputeBounds(const EasyRpgRgssSpriteState& state, EasyRpgRgssRect& bounds)
{
    bounds.x = state.x;
    bounds.y = state.y;
    bounds.width = 0;
    bounds.height = 0;
    if (!state.visible || state.opacity <= 0 || state.bitmap_handle == 0 ||
        state.source_rect.width <= 0 || state.source_rect.height <= 0 ||
        state.zoom_x_milli <= 0 || state.zoom_y_milli <= 0)
        return ERgssSpriteOk;

    int width = 0;
    int height = 0;
    int originX = 0;
    int originY = 0;
    if (!ScaleValue(state.source_rect.width, state.zoom_x_milli, width) ||
        !ScaleValue(state.source_rect.height, state.zoom_y_milli, height) ||
        !ScaleOrigin(state.ox, state.zoom_x_milli, originX) ||
        !ScaleOrigin(state.oy, state.zoom_y_milli, originY))
        return ERgssSpriteErrorInvalidArgument;
    bounds.x = state.x - originX;
    bounds.y = state.y - originY;
    bounds.width = width;
    bounds.height = height;
    return ERgssSpriteOk;
}

int LayerZ(const EasyRpgRgssSpriteState& state)
{
    long long value = state.z;
    if (state.viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(state.viewport_handle, &viewport) == ERgssViewportOk)
            value += viewport.z;
    }
    if (value > 2147483647LL)
        return 2147483647;
    if (value < -2147483647LL - 1LL)
        return -2147483647 - 1;
    return static_cast<int>(value);
}

int ComputeRenderBounds(const EasyRpgRgssSpriteState& state,
                        EasyRpgRgssRect& bounds,
                        EasyRpgRgssRect& clip,
                        int& clipped)
{
    clipped = 0;
    const int result = ComputeBounds(state, bounds);
    if (result != ERgssSpriteOk || bounds.width <= 0 || bounds.height <= 0)
        return result;
    if (state.viewport_handle == 0)
        return ERgssSpriteOk;

    EasyRpgRgssViewportState viewport;
    if (EasyRpgRgssViewportGetState(state.viewport_handle, &viewport) != ERgssViewportOk ||
        !viewport.visible || viewport.rect.width <= 0 || viewport.rect.height <= 0)
    {
        bounds.width = 0;
        bounds.height = 0;
        return ERgssSpriteOk;
    }

    bounds.x += viewport.rect.x - viewport.ox;
    bounds.y += viewport.rect.y - viewport.oy;
    clip = viewport.rect;
    clipped = 1;
    return ERgssSpriteOk;
}

int ComesBefore(int leftIndex, int rightIndex)
{
    const TSpriteSlot& left = gSprites.slots[leftIndex];
    const TSpriteSlot& right = gSprites.slots[rightIndex];
    const int leftZ = LayerZ(left.state);
    const int rightZ = LayerZ(right.state);
    if (leftZ != rightZ)
        return leftZ < rightZ;
    return left.serial < right.serial;
}

void RebuildOrder()
{
    const unsigned int viewportRevision = EasyRpgRgssViewportRevision();
    if (viewportRevision != gSprites.viewport_revision)
        gSprites.order_dirty = 1;
    if (!gSprites.order_dirty)
        return;
    int count = 0;
    for (int index = 0; index < gSprites.host.max_sprites; ++index)
    {
        if (!gSprites.slots[index].used)
            continue;
        int position = count;
        while (position > 0 && ComesBefore(index, gSprites.order[position - 1]))
        {
            gSprites.order[position] = gSprites.order[position - 1];
            --position;
        }
        gSprites.order[position] = index;
        ++count;
    }
    gSprites.order_count = count;
    gSprites.viewport_revision = viewportRevision;
    gSprites.order_dirty = 0;
}

int RenderState(const EasyRpgRgssSpriteState& state, EasyRpgRgssRect* outputDirty)
{
    ZeroRect(outputDirty);
    EasyRpgRgssRect bounds;
    EasyRpgRgssRect clip;
    int clipped = 0;
    const int boundsResult = ComputeRenderBounds(state, bounds, clip, clipped);
    if (boundsResult != ERgssSpriteOk)
        return boundsResult;
    if (bounds.width <= 0 || bounds.height <= 0)
        return ERgssSpriteOk;
    const int result = clipped
        ? EasyRpgRgssBitmapBlitExClipped(state.bitmap_handle,
                                         &state.source_rect,
                                         &bounds,
                                         &clip,
                                         state.opacity,
                                         state.mirror,
                                         state.blend_type,
                                         outputDirty)
        : EasyRpgRgssBitmapBlitEx(state.bitmap_handle,
                                  &state.source_rect,
                                  &bounds,
                                  state.opacity,
                                  state.mirror,
                                  state.blend_type,
                                  outputDirty);
    return result == ERgssBitmapOk ? ERgssSpriteOk : ERgssSpriteErrorRender;
}

int RenderGetZ(void*, int objectHandle, int* z)
{
    TSpriteSlot* slot = FindSlot(objectHandle);
    if (!slot || !z)
        return ERgssSpriteErrorInvalidHandle;
    *z = LayerZ(slot->state);
    return ERgssSpriteOk;
}

int RenderDraw(void*, int objectHandle, EasyRpgRgssRect* outputDirty)
{
    TSpriteSlot* slot = FindSlot(objectHandle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    return RenderState(slot->state, outputDirty);
}

void UnionDirty(EasyRpgRgssRect& total, const EasyRpgRgssRect& rect)
{
    if (rect.width <= 0 || rect.height <= 0)
        return;
    if (total.width <= 0 || total.height <= 0)
    {
        total = rect;
        return;
    }
    const int left = total.x < rect.x ? total.x : rect.x;
    const int top = total.y < rect.y ? total.y : rect.y;
    const int totalRight = total.x + total.width;
    const int rectRight = rect.x + rect.width;
    const int totalBottom = total.y + total.height;
    const int rectBottom = rect.y + rect.height;
    const int right = totalRight > rectRight ? totalRight : rectRight;
    const int bottom = totalBottom > rectBottom ? totalBottom : rectBottom;
    total.x = left;
    total.y = top;
    total.width = right - left;
    total.height = bottom - top;
}
}

extern "C" int EasyRpgRgssSpriteInitialize(const EasyRpgRgssSpriteHost* host)
{
    if (!host || !host->allocate || !host->release ||
        host->max_sprites <= 0 || host->max_sprites > 65535)
        return ERgssSpriteErrorInvalidArgument;
    if (EasyRpgRgssRenderCount() < 0)
        return ERgssSpriteErrorNotInitialized;

    EasyRpgRgssSpriteShutdown();
    const unsigned int slotBytes = static_cast<unsigned int>(host->max_sprites) *
                                   static_cast<unsigned int>(sizeof(TSpriteSlot));
    const unsigned int orderBytes = static_cast<unsigned int>(host->max_sprites) *
                                    static_cast<unsigned int>(sizeof(int));
    TSpriteSlot* slots = static_cast<TSpriteSlot*>(host->allocate(host->context, slotBytes));
    if (!slots)
        return ERgssSpriteErrorNoMemory;
    int* order = static_cast<int*>(host->allocate(host->context, orderBytes));
    if (!order)
    {
        host->release(host->context, slots);
        return ERgssSpriteErrorNoMemory;
    }

    gSprites.host = *host;
    gSprites.slots = slots;
    gSprites.order = order;
    gSprites.order_count = 0;
    gSprites.order_dirty = 1;
    gSprites.active_count = 0;
    gSprites.next_serial = 1;
    gSprites.viewport_revision = EasyRpgRgssViewportRevision();
    gSprites.initialized = 1;
    for (int index = 0; index < host->max_sprites; ++index)
    {
        DefaultState(slots[index].state);
        slots[index].serial = 0;
        slots[index].render_handle = 0;
        slots[index].generation = 0;
        slots[index].used = 0;
        order[index] = 0;
    }
    return ERgssSpriteOk;
}

extern "C" void EasyRpgRgssSpriteShutdown(void)
{
    if (!gSprites.initialized)
        return;
    for (int index = 0; index < gSprites.host.max_sprites; ++index)
    {
        if (gSprites.slots[index].used && gSprites.slots[index].render_handle > 0)
            EasyRpgRgssRenderUnregister(gSprites.slots[index].render_handle);
        gSprites.slots[index].render_handle = 0;
    }
    gSprites.host.release(gSprites.host.context, gSprites.order);
    gSprites.host.release(gSprites.host.context, gSprites.slots);
    gSprites.slots = 0;
    gSprites.order = 0;
    gSprites.order_count = 0;
    gSprites.order_dirty = 0;
    gSprites.active_count = 0;
    gSprites.next_serial = 0;
    gSprites.viewport_revision = 0;
    gSprites.host.context = 0;
    gSprites.host.allocate = 0;
    gSprites.host.release = 0;
    gSprites.host.max_sprites = 0;
    gSprites.initialized = 0;
}

extern "C" int EasyRpgRgssSpriteCreate(int* handle)
{
    if (!gSprites.initialized)
        return ERgssSpriteErrorNotInitialized;
    if (!handle)
        return ERgssSpriteErrorInvalidArgument;
    for (int index = 0; index < gSprites.host.max_sprites; ++index)
    {
        TSpriteSlot& slot = gSprites.slots[index];
        if (slot.used)
            continue;
        unsigned short generation = static_cast<unsigned short>((slot.generation + 1u) & 0x7fffu);
        if (generation == 0)
            generation = 1;
        DefaultState(slot.state);
        slot.generation = generation;
        slot.serial = gSprites.next_serial++;
        if (gSprites.next_serial == 0)
            gSprites.next_serial = 1;
        slot.used = 1;
        ++gSprites.active_count;
        gSprites.order_dirty = 1;
        const int spriteHandle = MakeHandle(index, generation);
        const int renderResult = EasyRpgRgssRenderRegister(0,
                                                           spriteHandle,
                                                           &RenderGetZ,
                                                           &RenderDraw,
                                                           &slot.render_handle);
        if (renderResult != ERgssRenderOk)
        {
            slot.used = 0;
            slot.serial = 0;
            slot.render_handle = 0;
            --gSprites.active_count;
            return renderResult == ERgssRenderErrorCapacity
                ? ERgssSpriteErrorCapacity : ERgssSpriteErrorRender;
        }
        *handle = spriteHandle;
        return ERgssSpriteOk;
    }
    return ERgssSpriteErrorCapacity;
}

extern "C" int EasyRpgRgssSpriteDestroy(int handle)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return gSprites.initialized ? ERgssSpriteErrorInvalidHandle : ERgssSpriteErrorNotInitialized;
    if (slot->render_handle > 0)
        EasyRpgRgssRenderUnregister(slot->render_handle);
    slot->used = 0;
    DefaultState(slot->state);
    slot->serial = 0;
    slot->render_handle = 0;
    --gSprites.active_count;
    gSprites.order_dirty = 1;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteGetState(int handle, EasyRpgRgssSpriteState* state)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    if (!state)
        return ERgssSpriteErrorInvalidArgument;
    *state = slot->state;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetState(int handle, const EasyRpgRgssSpriteState* state)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    if (!state)
        return ERgssSpriteErrorInvalidArgument;
    const int validation = ValidateState(*state);
    if (validation != ERgssSpriteOk)
        return validation;
    if (slot->state.z != state->z ||
        slot->state.viewport_handle != state->viewport_handle)
    {
        gSprites.order_dirty = 1;
        EasyRpgRgssRenderMarkOrderDirty();
    }
    slot->state = *state;
    slot->state.visible = state->visible ? 1 : 0;
    slot->state.mirror = state->mirror ? 1 : 0;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetBitmap(int handle, int bitmap_handle)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    if (bitmap_handle == 0)
    {
        slot->state.bitmap_handle = 0;
        slot->state.source_rect.x = 0;
        slot->state.source_rect.y = 0;
        slot->state.source_rect.width = 0;
        slot->state.source_rect.height = 0;
        return ERgssSpriteOk;
    }
    const int width = EasyRpgRgssBitmapWidth(bitmap_handle);
    const int height = EasyRpgRgssBitmapHeight(bitmap_handle);
    if (width < 0 || height < 0)
        return ERgssSpriteErrorBitmap;
    slot->state.bitmap_handle = bitmap_handle;
    slot->state.source_rect.x = 0;
    slot->state.source_rect.y = 0;
    slot->state.source_rect.width = width;
    slot->state.source_rect.height = height;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetViewport(int handle, int viewport_handle)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    if (viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(viewport_handle, &viewport) != ERgssViewportOk)
            return ERgssSpriteErrorInvalidArgument;
    }
    if (slot->state.viewport_handle != viewport_handle)
    {
        slot->state.viewport_handle = viewport_handle;
        gSprites.order_dirty = 1;
        EasyRpgRgssRenderMarkOrderDirty();
    }
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetSourceRect(int handle,
                                               const EasyRpgRgssRect* source_rect)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    if (!source_rect || source_rect->width < 0 || source_rect->height < 0)
        return ERgssSpriteErrorInvalidArgument;
    slot->state.source_rect = *source_rect;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetPosition(int handle, int x, int y, int z)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    if (slot->state.z != z)
    {
        gSprites.order_dirty = 1;
        EasyRpgRgssRenderMarkOrderDirty();
    }
    slot->state.x = x;
    slot->state.y = y;
    slot->state.z = z;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetOrigin(int handle, int ox, int oy)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    slot->state.ox = ox;
    slot->state.oy = oy;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetZoom(int handle, int zoom_x_milli, int zoom_y_milli)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    if (zoom_x_milli < 0 || zoom_y_milli < 0 || zoom_x_milli > 16000 || zoom_y_milli > 16000)
        return ERgssSpriteErrorInvalidArgument;
    slot->state.zoom_x_milli = zoom_x_milli;
    slot->state.zoom_y_milli = zoom_y_milli;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetOpacity(int handle, int opacity)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    if (opacity < 0 || opacity > 255)
        return ERgssSpriteErrorInvalidArgument;
    slot->state.opacity = opacity;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetVisible(int handle, int visible)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    slot->state.visible = visible ? 1 : 0;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetMirror(int handle, int mirror)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    slot->state.mirror = mirror ? 1 : 0;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteSetBlendType(int handle, int blend_type)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    if (blend_type < ERgssBitmapBlendNormal || blend_type > ERgssBitmapBlendSubtract)
        return ERgssSpriteErrorInvalidArgument;
    slot->state.blend_type = blend_type;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteLogicalBounds(int handle, EasyRpgRgssRect* bounds)
{
    TSpriteSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssSpriteErrorInvalidHandle;
    if (!bounds)
        return ERgssSpriteErrorInvalidArgument;
    EasyRpgRgssRect clip;
    int clipped = 0;
    return ComputeRenderBounds(slot->state, *bounds, clip, clipped);
}

extern "C" int EasyRpgRgssSpriteRenderRange(int minimum_z,
                                             int maximum_z,
                                             EasyRpgRgssRect* output_dirty)
{
    ZeroRect(output_dirty);
    if (!gSprites.initialized)
        return ERgssSpriteErrorNotInitialized;
    if (minimum_z > maximum_z)
        return ERgssSpriteErrorInvalidArgument;
    RebuildOrder();
    EasyRpgRgssRect total;
    total.x = 0;
    total.y = 0;
    total.width = 0;
    total.height = 0;
    for (int orderIndex = 0; orderIndex < gSprites.order_count; ++orderIndex)
    {
        TSpriteSlot& slot = gSprites.slots[gSprites.order[orderIndex]];
        const int layerZ = LayerZ(slot.state);
        if (layerZ < minimum_z)
            continue;
        if (layerZ > maximum_z)
            break;
        EasyRpgRgssRect bounds;
        EasyRpgRgssRect clip;
        int clipped = 0;
        const int boundsResult = ComputeRenderBounds(slot.state, bounds, clip, clipped);
        if (boundsResult != ERgssSpriteOk)
            return boundsResult;
        if (bounds.width <= 0 || bounds.height <= 0)
            continue;
        EasyRpgRgssRect dirty;
        const int result = clipped
            ? EasyRpgRgssBitmapBlitExClipped(slot.state.bitmap_handle,
                                             &slot.state.source_rect,
                                             &bounds,
                                             &clip,
                                             slot.state.opacity,
                                             slot.state.mirror,
                                             slot.state.blend_type,
                                             &dirty)
            : EasyRpgRgssBitmapBlitEx(slot.state.bitmap_handle,
                                      &slot.state.source_rect,
                                      &bounds,
                                      slot.state.opacity,
                                      slot.state.mirror,
                                      slot.state.blend_type,
                                      &dirty);
        if (result != ERgssBitmapOk)
            return ERgssSpriteErrorRender;
        UnionDirty(total, dirty);
    }
    if (output_dirty)
        *output_dirty = total;
    return ERgssSpriteOk;
}

extern "C" int EasyRpgRgssSpriteCount(void)
{
    return gSprites.initialized ? gSprites.active_count : ERgssSpriteErrorNotInitialized;
}

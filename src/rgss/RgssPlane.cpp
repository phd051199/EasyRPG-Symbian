#include "RgssBitmapBridge.h"
#include "RgssCoreBridge.h"
#include "RgssPlaneBridge.h"
#include "RgssRenderBridge.h"
#include "RgssViewportBridge.h"

namespace
{
struct TPlaneSlot
{
    EasyRpgRgssPlaneState state;
    int render_handle;
    unsigned short generation;
    unsigned char used;
};

struct TPlaneRegistry
{
    EasyRpgRgssPlaneHost host;
    TPlaneSlot* slots;
    int active_count;
    int initialized;
};

TPlaneRegistry gPlanes;

void ZeroRect(EasyRpgRgssRect* rect)
{
    if (!rect)
        return;
    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;
}

void DefaultState(EasyRpgRgssPlaneState& state)
{
    state.bitmap_handle = 0;
    state.viewport_handle = 0;
    state.z = 0;
    state.ox = 0;
    state.oy = 0;
    state.zoom_x_milli = 1000;
    state.zoom_y_milli = 1000;
    state.opacity = 255;
    state.visible = 1;
    state.blend_type = ERgssBitmapBlendNormal;
}

int ValidateState(const EasyRpgRgssPlaneState& state)
{
    if (state.zoom_x_milli < 0 || state.zoom_y_milli < 0 ||
        state.zoom_x_milli > 16000 || state.zoom_y_milli > 16000 ||
        state.opacity < 0 || state.opacity > 255 ||
        state.blend_type < ERgssBitmapBlendNormal ||
        state.blend_type > ERgssBitmapBlendSubtract)
        return ERgssPlaneErrorInvalidArgument;
    if (state.bitmap_handle != 0 &&
        (EasyRpgRgssBitmapWidth(state.bitmap_handle) <= 0 ||
         EasyRpgRgssBitmapHeight(state.bitmap_handle) <= 0))
        return ERgssPlaneErrorBitmap;
    if (state.viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(state.viewport_handle, &viewport) != ERgssViewportOk)
            return ERgssPlaneErrorInvalidArgument;
    }
    return ERgssPlaneOk;
}

int MakeHandle(int index, unsigned short generation)
{
    return (static_cast<int>(generation & 0x7fff) << 16) | (index + 1);
}

TPlaneSlot* FindSlot(int handle)
{
    if (!gPlanes.initialized || handle <= 0)
        return 0;
    const int encodedIndex = handle & 0xffff;
    const int generation = (handle >> 16) & 0x7fff;
    if (encodedIndex <= 0 || encodedIndex > gPlanes.host.max_planes || generation <= 0)
        return 0;
    TPlaneSlot* slot = &gPlanes.slots[encodedIndex - 1];
    if (!slot->used || slot->generation != generation)
        return 0;
    return slot;
}

int ClampAdd(int left, int right)
{
    const long long value = static_cast<long long>(left) + static_cast<long long>(right);
    if (value > 2147483647LL)
        return 2147483647;
    if (value < -2147483647LL - 1LL)
        return -2147483647 - 1;
    return static_cast<int>(value);
}

int EffectiveZ(const EasyRpgRgssPlaneState& state)
{
    int z = state.z;
    if (state.viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(state.viewport_handle, &viewport) == ERgssViewportOk)
            z = ClampAdd(z, viewport.z);
    }
    return z;
}

int RenderGetZ(void*, int objectHandle, int* z)
{
    TPlaneSlot* slot = FindSlot(objectHandle);
    if (!slot || !z)
        return ERgssPlaneErrorInvalidHandle;
    *z = EffectiveZ(slot->state);
    return ERgssPlaneOk;
}

int RenderDraw(void*, int objectHandle, EasyRpgRgssRect* outputDirty)
{
    ZeroRect(outputDirty);
    TPlaneSlot* slot = FindSlot(objectHandle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    const EasyRpgRgssPlaneState& state = slot->state;
    if (!state.visible || state.opacity <= 0 || state.bitmap_handle == 0 ||
        state.zoom_x_milli <= 0 || state.zoom_y_milli <= 0)
        return ERgssPlaneOk;

    const int width = EasyRpgRgssBitmapWidth(state.bitmap_handle);
    const int height = EasyRpgRgssBitmapHeight(state.bitmap_handle);
    if (width <= 0 || height <= 0)
        return ERgssPlaneErrorBitmap;

    EasyRpgRgssConfig config;
    if (EasyRpgRgssCoreGetConfig(&config) != ERgssOk)
        return ERgssPlaneErrorRender;

    EasyRpgRgssRect source;
    source.x = 0;
    source.y = 0;
    source.width = width;
    source.height = height;

    EasyRpgRgssRect destination;
    destination.x = 0;
    destination.y = 0;
    destination.width = config.logical_width;
    destination.height = config.logical_height;
    int offsetX = state.ox;
    int offsetY = state.oy;
    if (state.viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(state.viewport_handle, &viewport) != ERgssViewportOk ||
            !viewport.visible || viewport.rect.width <= 0 || viewport.rect.height <= 0)
            return ERgssPlaneOk;
        destination = viewport.rect;
        offsetX = ClampAdd(offsetX, viewport.ox);
        offsetY = ClampAdd(offsetY, viewport.oy);
    }

    const int result = EasyRpgRgssBitmapTile(state.bitmap_handle,
                                             &source,
                                             &destination,
                                             offsetX,
                                             offsetY,
                                             state.zoom_x_milli,
                                             state.zoom_y_milli,
                                             state.opacity,
                                             state.blend_type,
                                             outputDirty);
    return result == ERgssBitmapOk ? ERgssPlaneOk : ERgssPlaneErrorRender;
}
}

extern "C" int EasyRpgRgssPlaneInitialize(const EasyRpgRgssPlaneHost* host)
{
    if (!host || !host->allocate || !host->release ||
        host->max_planes <= 0 || host->max_planes > 65535)
        return ERgssPlaneErrorInvalidArgument;
    if (EasyRpgRgssRenderCount() < 0)
        return ERgssPlaneErrorNotInitialized;

    EasyRpgRgssPlaneShutdown();
    const unsigned int bytes = static_cast<unsigned int>(host->max_planes) *
                               static_cast<unsigned int>(sizeof(TPlaneSlot));
    TPlaneSlot* slots = static_cast<TPlaneSlot*>(host->allocate(host->context, bytes));
    if (!slots)
        return ERgssPlaneErrorNoMemory;

    gPlanes.host = *host;
    gPlanes.slots = slots;
    gPlanes.active_count = 0;
    gPlanes.initialized = 1;
    for (int index = 0; index < host->max_planes; ++index)
    {
        DefaultState(slots[index].state);
        slots[index].render_handle = 0;
        slots[index].generation = 0;
        slots[index].used = 0;
    }
    return ERgssPlaneOk;
}

extern "C" void EasyRpgRgssPlaneShutdown(void)
{
    if (!gPlanes.initialized)
        return;
    for (int index = 0; index < gPlanes.host.max_planes; ++index)
    {
        if (gPlanes.slots[index].used && gPlanes.slots[index].render_handle > 0)
            EasyRpgRgssRenderUnregister(gPlanes.slots[index].render_handle);
        gPlanes.slots[index].render_handle = 0;
    }
    gPlanes.host.release(gPlanes.host.context, gPlanes.slots);
    gPlanes.slots = 0;
    gPlanes.active_count = 0;
    gPlanes.host.context = 0;
    gPlanes.host.allocate = 0;
    gPlanes.host.release = 0;
    gPlanes.host.max_planes = 0;
    gPlanes.initialized = 0;
}

extern "C" int EasyRpgRgssPlaneCreate(int* handle)
{
    if (!gPlanes.initialized)
        return ERgssPlaneErrorNotInitialized;
    if (!handle)
        return ERgssPlaneErrorInvalidArgument;

    for (int index = 0; index < gPlanes.host.max_planes; ++index)
    {
        TPlaneSlot& slot = gPlanes.slots[index];
        if (slot.used)
            continue;
        unsigned short generation = static_cast<unsigned short>((slot.generation + 1u) & 0x7fffu);
        if (generation == 0)
            generation = 1;
        DefaultState(slot.state);
        slot.generation = generation;
        slot.used = 1;
        const int planeHandle = MakeHandle(index, generation);
        const int renderResult = EasyRpgRgssRenderRegister(0,
                                                           planeHandle,
                                                           &RenderGetZ,
                                                           &RenderDraw,
                                                           &slot.render_handle);
        if (renderResult != ERgssRenderOk)
        {
            slot.used = 0;
            slot.render_handle = 0;
            return renderResult == ERgssRenderErrorCapacity
                ? ERgssPlaneErrorCapacity : ERgssPlaneErrorRender;
        }
        ++gPlanes.active_count;
        *handle = planeHandle;
        return ERgssPlaneOk;
    }
    return ERgssPlaneErrorCapacity;
}

extern "C" int EasyRpgRgssPlaneDestroy(int handle)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return gPlanes.initialized ? ERgssPlaneErrorInvalidHandle : ERgssPlaneErrorNotInitialized;
    if (slot->render_handle > 0)
        EasyRpgRgssRenderUnregister(slot->render_handle);
    slot->used = 0;
    slot->render_handle = 0;
    DefaultState(slot->state);
    --gPlanes.active_count;
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneGetState(int handle, EasyRpgRgssPlaneState* state)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    if (!state)
        return ERgssPlaneErrorInvalidArgument;
    *state = slot->state;
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneSetState(int handle, const EasyRpgRgssPlaneState* state)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    if (!state)
        return ERgssPlaneErrorInvalidArgument;
    const int validation = ValidateState(*state);
    if (validation != ERgssPlaneOk)
        return validation;
    if (slot->state.z != state->z || slot->state.viewport_handle != state->viewport_handle)
        EasyRpgRgssRenderMarkOrderDirty();
    slot->state = *state;
    slot->state.visible = state->visible ? 1 : 0;
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneSetBitmap(int handle, int bitmap_handle)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    if (bitmap_handle != 0 &&
        (EasyRpgRgssBitmapWidth(bitmap_handle) <= 0 ||
         EasyRpgRgssBitmapHeight(bitmap_handle) <= 0))
        return ERgssPlaneErrorBitmap;
    slot->state.bitmap_handle = bitmap_handle;
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneSetViewport(int handle, int viewport_handle)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    if (viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(viewport_handle, &viewport) != ERgssViewportOk)
            return ERgssPlaneErrorInvalidArgument;
    }
    if (slot->state.viewport_handle != viewport_handle)
    {
        slot->state.viewport_handle = viewport_handle;
        EasyRpgRgssRenderMarkOrderDirty();
    }
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneSetZ(int handle, int z)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    if (slot->state.z != z)
    {
        slot->state.z = z;
        EasyRpgRgssRenderMarkOrderDirty();
    }
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneSetOrigin(int handle, int ox, int oy)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    slot->state.ox = ox;
    slot->state.oy = oy;
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneSetZoom(int handle, int zoom_x_milli, int zoom_y_milli)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    if (zoom_x_milli < 0 || zoom_y_milli < 0 ||
        zoom_x_milli > 16000 || zoom_y_milli > 16000)
        return ERgssPlaneErrorInvalidArgument;
    slot->state.zoom_x_milli = zoom_x_milli;
    slot->state.zoom_y_milli = zoom_y_milli;
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneSetOpacity(int handle, int opacity)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    if (opacity < 0 || opacity > 255)
        return ERgssPlaneErrorInvalidArgument;
    slot->state.opacity = opacity;
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneSetVisible(int handle, int visible)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    slot->state.visible = visible ? 1 : 0;
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneSetBlendType(int handle, int blend_type)
{
    TPlaneSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssPlaneErrorInvalidHandle;
    if (blend_type < ERgssBitmapBlendNormal || blend_type > ERgssBitmapBlendSubtract)
        return ERgssPlaneErrorInvalidArgument;
    slot->state.blend_type = blend_type;
    return ERgssPlaneOk;
}

extern "C" int EasyRpgRgssPlaneCount(void)
{
    return gPlanes.initialized ? gPlanes.active_count : ERgssPlaneErrorNotInitialized;
}

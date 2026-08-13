#include "RgssRenderBridge.h"
#include "RgssViewportBridge.h"

namespace
{
struct TViewportSlot
{
    EasyRpgRgssViewportState state;
    unsigned short generation;
    unsigned char used;
};

struct TViewportRegistry
{
    EasyRpgRgssViewportHost host;
    TViewportSlot* slots;
    int active_count;
    unsigned int revision;
    int initialized;
};

TViewportRegistry gViewports;

void DefaultState(EasyRpgRgssViewportState& state)
{
    state.rect.x = 0;
    state.rect.y = 0;
    state.rect.width = 0;
    state.rect.height = 0;
    state.ox = 0;
    state.oy = 0;
    state.z = 0;
    state.visible = 1;
}

int ValidateState(const EasyRpgRgssViewportState& state)
{
    if (state.rect.width < 0 || state.rect.height < 0)
        return ERgssViewportErrorInvalidArgument;
    return ERgssViewportOk;
}

int MakeHandle(int index, unsigned short generation)
{
    return (static_cast<int>(generation & 0x7fff) << 16) | (index + 1);
}

TViewportSlot* FindSlot(int handle)
{
    if (!gViewports.initialized || handle <= 0)
        return 0;
    const int encodedIndex = handle & 0xffff;
    const int generation = (handle >> 16) & 0x7fff;
    if (encodedIndex <= 0 || encodedIndex > gViewports.host.max_viewports || generation <= 0)
        return 0;
    TViewportSlot* slot = &gViewports.slots[encodedIndex - 1];
    if (!slot->used || slot->generation != generation)
        return 0;
    return slot;
}
}

extern "C" int EasyRpgRgssViewportInitialize(const EasyRpgRgssViewportHost* host)
{
    if (!host || !host->allocate || !host->release ||
        host->max_viewports <= 0 || host->max_viewports > 65535)
        return ERgssViewportErrorInvalidArgument;

    EasyRpgRgssViewportShutdown();
    const unsigned int bytes = static_cast<unsigned int>(host->max_viewports) *
                               static_cast<unsigned int>(sizeof(TViewportSlot));
    TViewportSlot* slots = static_cast<TViewportSlot*>(host->allocate(host->context, bytes));
    if (!slots)
        return ERgssViewportErrorNoMemory;

    gViewports.host = *host;
    gViewports.slots = slots;
    gViewports.active_count = 0;
    gViewports.revision = 1;
    gViewports.initialized = 1;
    for (int index = 0; index < host->max_viewports; ++index)
    {
        DefaultState(slots[index].state);
        slots[index].generation = 0;
        slots[index].used = 0;
    }
    return ERgssViewportOk;
}

extern "C" void EasyRpgRgssViewportShutdown(void)
{
    if (!gViewports.initialized)
        return;
    gViewports.host.release(gViewports.host.context, gViewports.slots);
    gViewports.slots = 0;
    gViewports.active_count = 0;
    gViewports.revision = 0;
    gViewports.host.context = 0;
    gViewports.host.allocate = 0;
    gViewports.host.release = 0;
    gViewports.host.max_viewports = 0;
    gViewports.initialized = 0;
}

extern "C" int EasyRpgRgssViewportCreate(const EasyRpgRgssRect* rect, int* handle)
{
    if (!gViewports.initialized)
        return ERgssViewportErrorNotInitialized;
    if (!rect || !handle || rect->width < 0 || rect->height < 0)
        return ERgssViewportErrorInvalidArgument;

    for (int index = 0; index < gViewports.host.max_viewports; ++index)
    {
        TViewportSlot& slot = gViewports.slots[index];
        if (slot.used)
            continue;
        unsigned short generation = static_cast<unsigned short>((slot.generation + 1u) & 0x7fffu);
        if (generation == 0)
            generation = 1;
        DefaultState(slot.state);
        slot.state.rect = *rect;
        slot.generation = generation;
        slot.used = 1;
        ++gViewports.active_count;
        ++gViewports.revision;
        if (gViewports.revision == 0)
            gViewports.revision = 1;
        EasyRpgRgssRenderMarkOrderDirty();
        *handle = MakeHandle(index, generation);
        return ERgssViewportOk;
    }
    return ERgssViewportErrorCapacity;
}

extern "C" int EasyRpgRgssViewportDestroy(int handle)
{
    TViewportSlot* slot = FindSlot(handle);
    if (!slot)
        return gViewports.initialized ? ERgssViewportErrorInvalidHandle : ERgssViewportErrorNotInitialized;
    slot->used = 0;
    DefaultState(slot->state);
    --gViewports.active_count;
    ++gViewports.revision;
    if (gViewports.revision == 0)
        gViewports.revision = 1;
    EasyRpgRgssRenderMarkOrderDirty();
    return ERgssViewportOk;
}

extern "C" int EasyRpgRgssViewportGetState(int handle, EasyRpgRgssViewportState* state)
{
    TViewportSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssViewportErrorInvalidHandle;
    if (!state)
        return ERgssViewportErrorInvalidArgument;
    *state = slot->state;
    return ERgssViewportOk;
}

extern "C" int EasyRpgRgssViewportSetState(int handle, const EasyRpgRgssViewportState* state)
{
    TViewportSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssViewportErrorInvalidHandle;
    if (!state)
        return ERgssViewportErrorInvalidArgument;
    const int result = ValidateState(*state);
    if (result != ERgssViewportOk)
        return result;
    const int oldZ = slot->state.z;
    slot->state = *state;
    slot->state.visible = state->visible ? 1 : 0;
    if (oldZ != slot->state.z)
    {
        ++gViewports.revision;
        if (gViewports.revision == 0)
            gViewports.revision = 1;
        EasyRpgRgssRenderMarkOrderDirty();
    }
    return ERgssViewportOk;
}

extern "C" int EasyRpgRgssViewportSetRect(int handle, const EasyRpgRgssRect* rect)
{
    TViewportSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssViewportErrorInvalidHandle;
    if (!rect || rect->width < 0 || rect->height < 0)
        return ERgssViewportErrorInvalidArgument;
    slot->state.rect = *rect;
    return ERgssViewportOk;
}

extern "C" int EasyRpgRgssViewportSetOrigin(int handle, int ox, int oy)
{
    TViewportSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssViewportErrorInvalidHandle;
    slot->state.ox = ox;
    slot->state.oy = oy;
    return ERgssViewportOk;
}

extern "C" int EasyRpgRgssViewportSetZ(int handle, int z)
{
    TViewportSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssViewportErrorInvalidHandle;
    if (slot->state.z != z)
    {
        slot->state.z = z;
        ++gViewports.revision;
        if (gViewports.revision == 0)
            gViewports.revision = 1;
        EasyRpgRgssRenderMarkOrderDirty();
    }
    return ERgssViewportOk;
}

extern "C" int EasyRpgRgssViewportSetVisible(int handle, int visible)
{
    TViewportSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssViewportErrorInvalidHandle;
    slot->state.visible = visible ? 1 : 0;
    return ERgssViewportOk;
}

extern "C" int EasyRpgRgssViewportCount(void)
{
    return gViewports.initialized ? gViewports.active_count : ERgssViewportErrorNotInitialized;
}

extern "C" unsigned int EasyRpgRgssViewportRevision(void)
{
    return gViewports.initialized ? gViewports.revision : 0;
}

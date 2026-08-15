#include "RgssRenderBridge.h"

// Provided by RgssBitmap.cpp: refreshes the per-pass config/framebuffer
// snapshot once so the per-command blits skip repeated struct copies.
extern "C" int EasyRpgRgssBitmapBeginFrame(void);

namespace
{
struct TRenderSlot
{
    void* context;
    EasyRpgRgssRenderGetZFn get_z;
    EasyRpgRgssRenderDrawFn draw;
    int object_handle;
    int sort_z;
    unsigned int serial;
    unsigned short generation;
    unsigned char used;
};

struct TRenderRegistry
{
    EasyRpgRgssRenderHost host;
    TRenderSlot* slots;
    int* order;
    int order_count;
    int active_count;
    int order_dirty;
    unsigned int next_serial;
    int initialized;
};

TRenderRegistry gRender;

void ZeroRect(EasyRpgRgssRect* rect)
{
    if (!rect)
        return;
    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;
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

int MakeHandle(int index, unsigned short generation)
{
    return (static_cast<int>(generation & 0x7fff) << 16) | (index + 1);
}

TRenderSlot* FindSlot(int handle)
{
    if (!gRender.initialized || handle <= 0)
        return 0;
    const int encodedIndex = handle & 0xffff;
    const int generation = (handle >> 16) & 0x7fff;
    if (encodedIndex <= 0 || encodedIndex > gRender.host.max_renderables || generation <= 0)
        return 0;
    TRenderSlot* slot = &gRender.slots[encodedIndex - 1];
    if (!slot->used || slot->generation != generation)
        return 0;
    return slot;
}

int ComesBefore(int leftIndex, int rightIndex)
{
    const TRenderSlot& left = gRender.slots[leftIndex];
    const TRenderSlot& right = gRender.slots[rightIndex];
    if (left.sort_z != right.sort_z)
        return left.sort_z < right.sort_z;
    return left.serial < right.serial;
}

int RebuildOrder()
{
    if (!gRender.order_dirty)
        return ERgssRenderOk;

    int count = 0;
    for (int index = 0; index < gRender.host.max_renderables; ++index)
    {
        TRenderSlot& slot = gRender.slots[index];
        if (!slot.used)
            continue;
        int z = 0;
        if (!slot.get_z || slot.get_z(slot.context, slot.object_handle, &z) != 0)
            return ERgssRenderErrorObject;
        slot.sort_z = z;
        int position = count;
        while (position > 0 && ComesBefore(index, gRender.order[position - 1]))
        {
            gRender.order[position] = gRender.order[position - 1];
            --position;
        }
        gRender.order[position] = index;
        ++count;
    }
    gRender.order_count = count;
    gRender.order_dirty = 0;
    return ERgssRenderOk;
}
}

extern "C" int EasyRpgRgssRenderInitialize(const EasyRpgRgssRenderHost* host)
{
    if (!host || !host->allocate || !host->release ||
        host->max_renderables <= 0 || host->max_renderables > 65535)
        return ERgssRenderErrorInvalidArgument;

    EasyRpgRgssRenderShutdown();
    const unsigned int slotBytes = static_cast<unsigned int>(host->max_renderables) *
                                   static_cast<unsigned int>(sizeof(TRenderSlot));
    const unsigned int orderBytes = static_cast<unsigned int>(host->max_renderables) *
                                    static_cast<unsigned int>(sizeof(int));
    TRenderSlot* slots = static_cast<TRenderSlot*>(host->allocate(host->context, slotBytes));
    if (!slots)
        return ERgssRenderErrorNoMemory;
    int* order = static_cast<int*>(host->allocate(host->context, orderBytes));
    if (!order)
    {
        host->release(host->context, slots);
        return ERgssRenderErrorNoMemory;
    }

    gRender.host = *host;
    gRender.slots = slots;
    gRender.order = order;
    gRender.order_count = 0;
    gRender.active_count = 0;
    gRender.order_dirty = 1;
    gRender.next_serial = 1;
    gRender.initialized = 1;
    for (int index = 0; index < host->max_renderables; ++index)
    {
        slots[index].context = 0;
        slots[index].get_z = 0;
        slots[index].draw = 0;
        slots[index].object_handle = 0;
        slots[index].sort_z = 0;
        slots[index].serial = 0;
        slots[index].generation = 0;
        slots[index].used = 0;
        order[index] = 0;
    }
    return ERgssRenderOk;
}

extern "C" void EasyRpgRgssRenderShutdown(void)
{
    if (!gRender.initialized)
        return;
    gRender.host.release(gRender.host.context, gRender.order);
    gRender.host.release(gRender.host.context, gRender.slots);
    gRender.slots = 0;
    gRender.order = 0;
    gRender.order_count = 0;
    gRender.active_count = 0;
    gRender.order_dirty = 0;
    gRender.next_serial = 0;
    gRender.host.context = 0;
    gRender.host.allocate = 0;
    gRender.host.release = 0;
    gRender.host.max_renderables = 0;
    gRender.initialized = 0;
}

extern "C" int EasyRpgRgssRenderRegister(void* context,
                                          int object_handle,
                                          EasyRpgRgssRenderGetZFn get_z,
                                          EasyRpgRgssRenderDrawFn draw,
                                          int* render_handle)
{
    if (!gRender.initialized)
        return ERgssRenderErrorNotInitialized;
    if (object_handle <= 0 || !get_z || !draw || !render_handle)
        return ERgssRenderErrorInvalidArgument;

    for (int index = 0; index < gRender.host.max_renderables; ++index)
    {
        TRenderSlot& slot = gRender.slots[index];
        if (slot.used)
            continue;
        unsigned short generation = static_cast<unsigned short>((slot.generation + 1u) & 0x7fffu);
        if (generation == 0)
            generation = 1;
        slot.context = context;
        slot.get_z = get_z;
        slot.draw = draw;
        slot.object_handle = object_handle;
        slot.sort_z = 0;
        slot.serial = gRender.next_serial++;
        if (gRender.next_serial == 0)
            gRender.next_serial = 1;
        slot.generation = generation;
        slot.used = 1;
        ++gRender.active_count;
        gRender.order_dirty = 1;
        *render_handle = MakeHandle(index, generation);
        return ERgssRenderOk;
    }
    return ERgssRenderErrorCapacity;
}

extern "C" int EasyRpgRgssRenderUnregister(int render_handle)
{
    TRenderSlot* slot = FindSlot(render_handle);
    if (!slot)
        return gRender.initialized ? ERgssRenderErrorInvalidHandle : ERgssRenderErrorNotInitialized;
    slot->context = 0;
    slot->get_z = 0;
    slot->draw = 0;
    slot->object_handle = 0;
    slot->sort_z = 0;
    slot->serial = 0;
    slot->used = 0;
    --gRender.active_count;
    gRender.order_dirty = 1;
    return ERgssRenderOk;
}

extern "C" int EasyRpgRgssRenderMarkOrderDirty(void)
{
    if (!gRender.initialized)
        return ERgssRenderErrorNotInitialized;
    gRender.order_dirty = 1;
    return ERgssRenderOk;
}

extern "C" int EasyRpgRgssRenderRange(int minimum_z,
                                       int maximum_z,
                                       EasyRpgRgssRect* output_dirty)
{
    ZeroRect(output_dirty);
    if (!gRender.initialized)
        return ERgssRenderErrorNotInitialized;
    if (minimum_z > maximum_z)
        return ERgssRenderErrorInvalidArgument;
    const int rebuildResult = RebuildOrder();
    if (rebuildResult != ERgssRenderOk)
        return rebuildResult;
    // One snapshot per render pass; individual draw callbacks then hit the
    // cached config/framebuffer instead of re-querying the core per command.
    EasyRpgRgssBitmapBeginFrame();

    EasyRpgRgssRect total;
    total.x = 0;
    total.y = 0;
    total.width = 0;
    total.height = 0;
    for (int orderIndex = 0; orderIndex < gRender.order_count; ++orderIndex)
    {
        TRenderSlot& slot = gRender.slots[gRender.order[orderIndex]];
        if (slot.sort_z < minimum_z)
            continue;
        if (slot.sort_z > maximum_z)
            break;
        EasyRpgRgssRect dirty;
        ZeroRect(&dirty);
        if (!slot.draw || slot.draw(slot.context, slot.object_handle, &dirty) != 0)
            return ERgssRenderErrorObject;
        UnionDirty(total, dirty);
    }
    if (output_dirty)
        *output_dirty = total;
    return ERgssRenderOk;
}

extern "C" int EasyRpgRgssRenderAll(EasyRpgRgssRect* output_dirty)
{
    return EasyRpgRgssRenderRange((-2147483647 - 1), 2147483647, output_dirty);
}

extern "C" int EasyRpgRgssRenderCount(void)
{
    return gRender.initialized ? gRender.active_count : ERgssRenderErrorNotInitialized;
}

#include "RgssBitmapBridge.h"
#include "RgssCoreBridge.h"
#include "RgssRenderBridge.h"
#include "RgssTableBridge.h"
#include "RgssTilemapBridge.h"
#include "RgssViewportBridge.h"

namespace
{
const int KAutotileCount = 7;
const int KPriorityLayerCount = 24;
const int KRenderLayerCount = KPriorityLayerCount + 1;
const int KMaxCommands = 6144;

const unsigned char KAutotilePieces[48 * 4 * 2] = {
    2,4, 3,4, 2,5, 3,5, 4,0, 3,4, 2,5, 3,5, 2,4, 5,0, 2,5, 3,5, 4,0, 5,0, 2,5, 3,5,
    2,4, 3,4, 2,5, 5,1, 4,0, 3,4, 2,5, 5,1, 2,4, 5,0, 2,5, 5,1, 4,0, 5,0, 2,5, 5,1,
    2,4, 3,4, 4,1, 3,5, 4,0, 3,4, 4,1, 3,5, 2,4, 5,0, 4,1, 3,5, 4,0, 5,0, 4,1, 3,5,
    2,4, 3,4, 4,1, 5,1, 4,0, 3,4, 4,1, 5,1, 2,4, 5,0, 4,1, 5,1, 4,0, 5,0, 4,1, 5,1,
    0,4, 1,4, 0,5, 1,5, 0,4, 5,0, 0,5, 1,5, 0,4, 1,4, 0,5, 5,1, 0,4, 5,0, 0,5, 5,1,
    2,2, 3,2, 2,3, 3,3, 2,2, 3,2, 2,3, 5,1, 2,2, 3,2, 4,1, 3,3, 2,2, 3,2, 4,1, 5,1,
    4,4, 5,4, 4,5, 5,5, 4,4, 5,4, 4,1, 5,5, 4,0, 5,4, 4,5, 5,5, 4,0, 5,4, 4,1, 5,5,
    2,6, 3,6, 2,7, 3,7, 4,0, 3,6, 2,7, 3,7, 2,6, 5,0, 2,7, 3,7, 4,0, 5,0, 2,7, 3,7,
    0,4, 5,4, 0,5, 5,5, 2,2, 3,2, 2,7, 3,7, 0,2, 1,2, 0,3, 1,3, 0,2, 1,2, 0,3, 5,1,
    4,2, 5,2, 4,3, 5,3, 4,2, 5,2, 4,1, 5,3, 4,6, 5,6, 4,7, 5,7, 4,0, 5,6, 4,7, 5,7,
    0,6, 1,6, 0,7, 1,7, 0,6, 5,0, 0,7, 1,7, 0,2, 5,2, 0,3, 5,3, 0,2, 1,2, 0,7, 1,7,
    0,6, 5,6, 0,7, 5,7, 4,2, 5,2, 4,7, 5,7, 0,2, 5,2, 0,7, 5,7, 0,0, 1,0, 0,1, 1,1
};

struct TTileCommand
{
    int bitmap_handle;
    short source_x;
    short source_y;
    short destination_x;
    short destination_y;
    short next;
    unsigned char size;
    unsigned char autotile;
};

struct TTilemapSlot;

struct TTilemapLayer
{
    TTilemapSlot* owner;
    int index;
    int render_handle;
};

struct TTilemapSlot
{
    EasyRpgRgssTilemapState state;
    TTileCommand* commands;
    int command_count;
    short heads[KRenderLayerCount];
    short tails[KRenderLayerCount];
    TTilemapLayer layers[KRenderLayerCount];
    unsigned int map_revision;
    unsigned int priorities_revision;
    unsigned int viewport_revision;
    int animation_tick;
    int animation_frame;
    unsigned short generation;
    unsigned char used;
    unsigned char commands_dirty;
};

struct TTilemapRegistry
{
    EasyRpgRgssTilemapHost host;
    TTilemapSlot* slots;
    int active_count;
    int initialized;
};

TTilemapRegistry gTilemaps;

void ZeroRect(EasyRpgRgssRect* rect)
{
    if (!rect)
        return;
    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;
}

void UnionRect(EasyRpgRgssRect& target, const EasyRpgRgssRect& value)
{
    if (value.width <= 0 || value.height <= 0)
        return;
    if (target.width <= 0 || target.height <= 0)
    {
        target = value;
        return;
    }
    const int left = target.x < value.x ? target.x : value.x;
    const int top = target.y < value.y ? target.y : value.y;
    const int targetRight = target.x + target.width;
    const int valueRight = value.x + value.width;
    const int targetBottom = target.y + target.height;
    const int valueBottom = value.y + value.height;
    const int right = targetRight > valueRight ? targetRight : valueRight;
    const int bottom = targetBottom > valueBottom ? targetBottom : valueBottom;
    target.x = left;
    target.y = top;
    target.width = right - left;
    target.height = bottom - top;
}

void DefaultState(EasyRpgRgssTilemapState& state)
{
    state.viewport_handle = 0;
    state.tileset_handle = 0;
    for (int index = 0; index < KAutotileCount; ++index)
        state.autotile_handles[index] = 0;
    state.map_data_handle = 0;
    state.flash_data_handle = 0;
    state.priorities_handle = 0;
    state.ox = 0;
    state.oy = 0;
    state.visible = 1;
}

int MakeHandle(int index, unsigned short generation)
{
    return (static_cast<int>(generation & 0x7fff) << 16) | (index + 1);
}

TTilemapSlot* FindSlot(int handle)
{
    if (!gTilemaps.initialized || handle <= 0)
        return 0;
    const int encodedIndex = handle & 0xffff;
    const int generation = (handle >> 16) & 0x7fff;
    if (encodedIndex <= 0 || encodedIndex > gTilemaps.host.max_tilemaps || generation <= 0)
        return 0;
    TTilemapSlot* slot = &gTilemaps.slots[encodedIndex - 1];
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

int FloorDiv32(int value)
{
    if (value >= 0)
        return value / 32;
    return -static_cast<int>((static_cast<unsigned int>(-value) + 31u) / 32u);
}

int FloorMod32(int value)
{
    const int quotient = FloorDiv32(value);
    return value - quotient * 32;
}

int Wrap(int value, int range)
{
    if (range <= 0)
        return 0;
    int result = value % range;
    if (result < 0)
        result += range;
    return result;
}

int ValidBitmap(int handle)
{
    return handle == 0 ||
        (EasyRpgRgssBitmapWidth(handle) > 0 && EasyRpgRgssBitmapHeight(handle) > 0);
}

int ValidTable(int handle)
{
    return handle == 0 || EasyRpgRgssTableDimensions(handle) > 0;
}

void ResetCommands(TTilemapSlot& slot)
{
    slot.command_count = 0;
    for (int index = 0; index < KRenderLayerCount; ++index)
    {
        slot.heads[index] = -1;
        slot.tails[index] = -1;
    }
}

int AddCommand(TTilemapSlot& slot,
               int layer,
               int bitmapHandle,
               int sourceX,
               int sourceY,
               int destinationX,
               int destinationY,
               int size,
               int autotile)
{
    if (layer < 0 || layer >= KRenderLayerCount || bitmapHandle <= 0)
        return ERgssTilemapErrorInvalidArgument;
    if (slot.command_count >= KMaxCommands)
        return ERgssTilemapErrorCapacity;
    if (sourceX < -32768 || sourceX > 32767 || sourceY < -32768 || sourceY > 32767 ||
        destinationX < -32768 || destinationX > 32767 ||
        destinationY < -32768 || destinationY > 32767)
        return ERgssTilemapErrorInvalidArgument;

    const int index = slot.command_count++;
    TTileCommand& command = slot.commands[index];
    command.bitmap_handle = bitmapHandle;
    command.source_x = static_cast<short>(sourceX);
    command.source_y = static_cast<short>(sourceY);
    command.destination_x = static_cast<short>(destinationX);
    command.destination_y = static_cast<short>(destinationY);
    command.next = -1;
    command.size = static_cast<unsigned char>(size);
    command.autotile = autotile ? 1 : 0;

    if (slot.tails[layer] >= 0)
        slot.commands[slot.tails[layer]].next = static_cast<short>(index);
    else
        slot.heads[layer] = static_cast<short>(index);
    slot.tails[layer] = static_cast<short>(index);
    return ERgssTilemapOk;
}

int TableValue(int handle, int x, int y, int z, int defaultValue)
{
    if (handle == 0)
        return defaultValue;
    int value = 0;
    return EasyRpgRgssTableGet(handle, x, y, z, &value) == ERgssTableOk
        ? value : defaultValue;
}

int TilePriority(const TTilemapSlot& slot, int tileId)
{
    if (slot.state.priorities_handle == 0)
        return 0;
    const int size = EasyRpgRgssTableXSize(slot.state.priorities_handle);
    if (size <= 0 || tileId < 0 || tileId >= size)
        return 0;
    const int priority = TableValue(slot.state.priorities_handle, tileId, 0, 0, 0);
    return priority >= 0 && priority <= 5 ? priority : -1;
}

int AddNormalTile(TTilemapSlot& slot,
                  int layer,
                  int tileId,
                  int destinationX,
                  int destinationY)
{
    const int bitmap = slot.state.tileset_handle;
    if (bitmap == 0)
        return ERgssTilemapOk;
    const int width = EasyRpgRgssBitmapWidth(bitmap);
    const int height = EasyRpgRgssBitmapHeight(bitmap);
    if (width <= 0 || height <= 0)
        return ERgssTilemapOk;
    const int index = tileId - 384;
    if (index < 0)
        return ERgssTilemapOk;
    const int sourceX = (index % 8) * 32;
    const int sourceY = (index / 8) * 32;
    if (sourceX + 32 > width || sourceY + 32 > height)
        return ERgssTilemapOk;
    return AddCommand(slot,
                      layer,
                      bitmap,
                      sourceX,
                      sourceY,
                      destinationX,
                      destinationY,
                      32,
                      0);
}

int AddAutotile(TTilemapSlot& slot,
                int layer,
                int tileId,
                int destinationX,
                int destinationY)
{
    const int autotileIndex = tileId / 48 - 1;
    const int pattern = tileId % 48;
    if (autotileIndex < 0 || autotileIndex >= KAutotileCount || pattern < 0)
        return ERgssTilemapOk;
    const int bitmap = slot.state.autotile_handles[autotileIndex];
    if (bitmap == 0)
        return ERgssTilemapOk;
    const int width = EasyRpgRgssBitmapWidth(bitmap);
    const int height = EasyRpgRgssBitmapHeight(bitmap);
    if (width < 96 || height < 128)
        return ERgssTilemapOk;

    for (int piece = 0; piece < 4; ++piece)
    {
        const int coordinate = (pattern * 4 + piece) * 2;
        const int sourceX = static_cast<int>(KAutotilePieces[coordinate]) * 16;
        const int sourceY = static_cast<int>(KAutotilePieces[coordinate + 1]) * 16;
        const int pieceX = piece & 1;
        const int pieceY = piece >> 1;
        const int result = AddCommand(slot,
                                      layer,
                                      bitmap,
                                      sourceX,
                                      sourceY,
                                      destinationX + pieceX * 16,
                                      destinationY + pieceY * 16,
                                      16,
                                      1);
        if (result != ERgssTilemapOk)
            return result;
    }
    return ERgssTilemapOk;
}

int BuildCommands(TTilemapSlot& slot)
{
    ResetCommands(slot);
    if (!slot.state.visible || slot.state.map_data_handle == 0)
        return ERgssTilemapOk;

    const int mapWidth = EasyRpgRgssTableXSize(slot.state.map_data_handle);
    const int mapHeight = EasyRpgRgssTableYSize(slot.state.map_data_handle);
    const int mapDepth = EasyRpgRgssTableZSize(slot.state.map_data_handle);
    if (mapWidth <= 0 || mapHeight <= 0 || mapDepth <= 0)
        return ERgssTilemapOk;

    EasyRpgRgssConfig config;
    if (EasyRpgRgssCoreGetConfig(&config) != ERgssOk)
        return ERgssTilemapErrorRender;

    EasyRpgRgssRect view;
    view.x = 0;
    view.y = 0;
    view.width = config.logical_width;
    view.height = config.logical_height;
    int viewportOx = 0;
    int viewportOy = 0;
    if (slot.state.viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(slot.state.viewport_handle, &viewport) != ERgssViewportOk)
            return ERgssTilemapErrorResource;
        if (!viewport.visible || viewport.rect.width <= 0 || viewport.rect.height <= 0)
            return ERgssTilemapOk;
        view = viewport.rect;
        viewportOx = viewport.ox;
        viewportOy = viewport.oy;
    }

    const int scrollX = ClampAdd(slot.state.ox, viewportOx);
    const int scrollY = ClampAdd(slot.state.oy, viewportOy);
    const int firstMapX = FloorDiv32(scrollX);
    const int firstMapY = FloorDiv32(scrollY);
    const int offsetX = FloorMod32(scrollX);
    const int offsetY = FloorMod32(scrollY);
    const int columns = view.width / 32 + 3;
    const int rows = view.height / 32 + 3;
    const int depth = mapDepth < 3 ? mapDepth : 3;

    for (int localX = 0; localX < columns; ++localX)
    {
        const int mapX = Wrap(firstMapX + localX, mapWidth);
        const int destinationX = view.x - offsetX + localX * 32;
        for (int localY = 0; localY < rows; ++localY)
        {
            const int mapY = Wrap(firstMapY + localY, mapHeight);
            const int destinationY = view.y - offsetY + localY * 32;
            for (int depthIndex = 0; depthIndex < depth; ++depthIndex)
            {
                const int tileId = TableValue(slot.state.map_data_handle,
                                              mapX,
                                              mapY,
                                              depthIndex,
                                              0);
                if (tileId < 48)
                    continue;
                const int priority = TilePriority(slot, tileId);
                if (priority < 0)
                    continue;
                int layer = 0;
                if (priority > 0)
                {
                    const int priorityLayer = localY + priority;
                    if (priorityLayer < 0 || priorityLayer >= KPriorityLayerCount)
                        continue;
                    layer = priorityLayer + 1;
                }
                const int result = tileId < 384
                    ? AddAutotile(slot, layer, tileId, destinationX, destinationY)
                    : AddNormalTile(slot, layer, tileId, destinationX, destinationY);
                if (result != ERgssTilemapOk)
                    return result;
            }
        }
    }
    return ERgssTilemapOk;
}

int EnsureCommands(TTilemapSlot& slot)
{
    const unsigned int mapRevision = slot.state.map_data_handle != 0
        ? EasyRpgRgssTableRevision(slot.state.map_data_handle) : 0;
    const unsigned int prioritiesRevision = slot.state.priorities_handle != 0
        ? EasyRpgRgssTableRevision(slot.state.priorities_handle) : 0;
    const unsigned int viewportRevision = EasyRpgRgssViewportRevision();
    if (!slot.commands_dirty &&
        slot.map_revision == mapRevision &&
        slot.priorities_revision == prioritiesRevision &&
        slot.viewport_revision == viewportRevision)
        return ERgssTilemapOk;

    const int result = BuildCommands(slot);
    if (result != ERgssTilemapOk)
        return result;
    slot.map_revision = mapRevision;
    slot.priorities_revision = prioritiesRevision;
    slot.viewport_revision = viewportRevision;
    slot.commands_dirty = 0;
    return ERgssTilemapOk;
}

int ViewportZ(const TTilemapSlot& slot)
{
    if (slot.state.viewport_handle == 0)
        return 0;
    EasyRpgRgssViewportState viewport;
    return EasyRpgRgssViewportGetState(slot.state.viewport_handle, &viewport) == ERgssViewportOk
        ? viewport.z : 0;
}

int LayerZ(const TTilemapLayer& layer)
{
    if (!layer.owner)
        return 0;
    const TTilemapSlot& slot = *layer.owner;
    const int viewportZ = ViewportZ(slot);
    if (layer.index == 0)
        return viewportZ;
    int viewportOy = 0;
    if (slot.state.viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(slot.state.viewport_handle, &viewport) == ERgssViewportOk)
            viewportOy = viewport.oy;
    }
    const int scrollY = ClampAdd(slot.state.oy, viewportOy);
    const int localLayer = layer.index - 1;
    return ClampAdd(viewportZ, 32 * (localLayer + 1) - FloorMod32(scrollY));
}

int RenderGetZ(void* context, int, int* z)
{
    TTilemapLayer* layer = static_cast<TTilemapLayer*>(context);
    if (!layer || !layer->owner || !layer->owner->used || !z)
        return ERgssTilemapErrorInvalidHandle;
    *z = LayerZ(*layer);
    return ERgssTilemapOk;
}

int RenderDraw(void* context, int, EasyRpgRgssRect* outputDirty)
{
    ZeroRect(outputDirty);
    TTilemapLayer* layer = static_cast<TTilemapLayer*>(context);
    if (!layer || !layer->owner || !layer->owner->used)
        return ERgssTilemapErrorInvalidHandle;
    TTilemapSlot& slot = *layer->owner;
    if (!slot.state.visible)
        return ERgssTilemapOk;

    EasyRpgRgssRect clip;
    EasyRpgRgssConfig config;
    if (EasyRpgRgssCoreGetConfig(&config) != ERgssOk)
        return ERgssTilemapErrorRender;
    clip.x = 0;
    clip.y = 0;
    clip.width = config.logical_width;
    clip.height = config.logical_height;
    if (slot.state.viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(slot.state.viewport_handle, &viewport) != ERgssViewportOk)
            return ERgssTilemapErrorResource;
        if (!viewport.visible)
            return ERgssTilemapOk;
        clip = viewport.rect;
    }

    const int ensureResult = EnsureCommands(slot);
    if (ensureResult != ERgssTilemapOk)
        return ensureResult;

    EasyRpgRgssRect total;
    ZeroRect(&total);
    int commandIndex = slot.heads[layer->index];
    while (commandIndex >= 0)
    {
        if (commandIndex >= slot.command_count)
            return ERgssTilemapErrorRender;
        const TTileCommand& command = slot.commands[commandIndex];
        EasyRpgRgssRect source;
        source.x = command.source_x;
        source.y = command.source_y;
        source.width = command.size;
        source.height = command.size;
        if (command.autotile && EasyRpgRgssBitmapWidth(command.bitmap_handle) > 96)
            source.x += slot.animation_frame * 96;
        EasyRpgRgssRect destination;
        destination.x = command.destination_x;
        destination.y = command.destination_y;
        destination.width = command.size;
        destination.height = command.size;
        EasyRpgRgssRect dirty;
        const int result = EasyRpgRgssBitmapBlitExClipped(command.bitmap_handle,
                                                           &source,
                                                           &destination,
                                                           &clip,
                                                           255,
                                                           0,
                                                           ERgssBitmapBlendNormal,
                                                           &dirty);
        if (result != ERgssBitmapOk)
            return ERgssTilemapErrorRender;
        UnionRect(total, dirty);
        commandIndex = command.next;
    }
    if (outputDirty)
        *outputDirty = total;
    return ERgssTilemapOk;
}

void ReleaseSlot(TTilemapSlot& slot)
{
    for (int layer = 0; layer < KRenderLayerCount; ++layer)
    {
        if (slot.layers[layer].render_handle > 0)
            EasyRpgRgssRenderUnregister(slot.layers[layer].render_handle);
        slot.layers[layer].render_handle = 0;
        slot.layers[layer].owner = 0;
        slot.layers[layer].index = layer;
    }
    if (slot.commands)
        gTilemaps.host.release(gTilemaps.host.context, slot.commands);
    slot.commands = 0;
    slot.command_count = 0;
    slot.used = 0;
    slot.commands_dirty = 1;
    DefaultState(slot.state);
}
}

extern "C" int EasyRpgRgssTilemapInitialize(const EasyRpgRgssTilemapHost* host)
{
    if (!host || !host->allocate || !host->release ||
        host->max_tilemaps <= 0 || host->max_tilemaps > 64)
        return ERgssTilemapErrorInvalidArgument;
    if (EasyRpgRgssRenderCount() < 0)
        return ERgssTilemapErrorNotInitialized;

    EasyRpgRgssTilemapShutdown();
    const unsigned int bytes = static_cast<unsigned int>(host->max_tilemaps) *
                               static_cast<unsigned int>(sizeof(TTilemapSlot));
    TTilemapSlot* slots = static_cast<TTilemapSlot*>(host->allocate(host->context, bytes));
    if (!slots)
        return ERgssTilemapErrorNoMemory;

    gTilemaps.host = *host;
    gTilemaps.slots = slots;
    gTilemaps.active_count = 0;
    gTilemaps.initialized = 1;
    for (int index = 0; index < host->max_tilemaps; ++index)
    {
        TTilemapSlot& slot = slots[index];
        DefaultState(slot.state);
        slot.commands = 0;
        slot.command_count = 0;
        slot.map_revision = 0;
        slot.priorities_revision = 0;
        slot.viewport_revision = 0;
        slot.animation_tick = 0;
        slot.animation_frame = 0;
        slot.generation = 0;
        slot.used = 0;
        slot.commands_dirty = 1;
        ResetCommands(slot);
        for (int layer = 0; layer < KRenderLayerCount; ++layer)
        {
            slot.layers[layer].owner = 0;
            slot.layers[layer].index = layer;
            slot.layers[layer].render_handle = 0;
        }
    }
    return ERgssTilemapOk;
}

extern "C" void EasyRpgRgssTilemapShutdown(void)
{
    if (!gTilemaps.initialized)
        return;
    for (int index = 0; index < gTilemaps.host.max_tilemaps; ++index)
    {
        if (gTilemaps.slots[index].used)
            ReleaseSlot(gTilemaps.slots[index]);
    }
    gTilemaps.host.release(gTilemaps.host.context, gTilemaps.slots);
    gTilemaps.slots = 0;
    gTilemaps.active_count = 0;
    gTilemaps.host.context = 0;
    gTilemaps.host.allocate = 0;
    gTilemaps.host.release = 0;
    gTilemaps.host.max_tilemaps = 0;
    gTilemaps.initialized = 0;
}

extern "C" int EasyRpgRgssTilemapCreate(int viewport_handle, int* handle)
{
    if (!gTilemaps.initialized)
        return ERgssTilemapErrorNotInitialized;
    if (!handle)
        return ERgssTilemapErrorInvalidArgument;
    if (viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(viewport_handle, &viewport) != ERgssViewportOk)
            return ERgssTilemapErrorInvalidArgument;
    }

    for (int index = 0; index < gTilemaps.host.max_tilemaps; ++index)
    {
        TTilemapSlot& slot = gTilemaps.slots[index];
        if (slot.used)
            continue;
        TTileCommand* commands = static_cast<TTileCommand*>(
            gTilemaps.host.allocate(gTilemaps.host.context,
                                    static_cast<unsigned int>(sizeof(TTileCommand) * KMaxCommands)));
        if (!commands)
            return ERgssTilemapErrorNoMemory;
        unsigned short generation = static_cast<unsigned short>((slot.generation + 1u) & 0x7fffu);
        if (generation == 0)
            generation = 1;
        DefaultState(slot.state);
        slot.state.viewport_handle = viewport_handle;
        slot.commands = commands;
        slot.command_count = 0;
        slot.map_revision = 0;
        slot.priorities_revision = 0;
        slot.viewport_revision = 0;
        slot.animation_tick = 0;
        slot.animation_frame = 0;
        slot.generation = generation;
        slot.used = 1;
        slot.commands_dirty = 1;
        ResetCommands(slot);
        const int tilemapHandle = MakeHandle(index, generation);

        for (int layer = 0; layer < KRenderLayerCount; ++layer)
        {
            slot.layers[layer].owner = &slot;
            slot.layers[layer].index = layer;
            slot.layers[layer].render_handle = 0;
            const int result = EasyRpgRgssRenderRegister(&slot.layers[layer],
                                                         tilemapHandle,
                                                         &RenderGetZ,
                                                         &RenderDraw,
                                                         &slot.layers[layer].render_handle);
            if (result != ERgssRenderOk)
            {
                ReleaseSlot(slot);
                return result == ERgssRenderErrorCapacity
                    ? ERgssTilemapErrorCapacity : ERgssTilemapErrorRender;
            }
        }
        ++gTilemaps.active_count;
        *handle = tilemapHandle;
        return ERgssTilemapOk;
    }
    return ERgssTilemapErrorCapacity;
}

extern "C" int EasyRpgRgssTilemapDestroy(int handle)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return gTilemaps.initialized ? ERgssTilemapErrorInvalidHandle : ERgssTilemapErrorNotInitialized;
    ReleaseSlot(*slot);
    --gTilemaps.active_count;
    return ERgssTilemapOk;
}

extern "C" int EasyRpgRgssTilemapGetState(int handle, EasyRpgRgssTilemapState* state)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTilemapErrorInvalidHandle;
    if (!state)
        return ERgssTilemapErrorInvalidArgument;
    *state = slot->state;
    return ERgssTilemapOk;
}

extern "C" int EasyRpgRgssTilemapSetViewport(int handle, int viewport_handle)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTilemapErrorInvalidHandle;
    if (viewport_handle != 0)
    {
        EasyRpgRgssViewportState viewport;
        if (EasyRpgRgssViewportGetState(viewport_handle, &viewport) != ERgssViewportOk)
            return ERgssTilemapErrorInvalidArgument;
    }
    if (slot->state.viewport_handle != viewport_handle)
    {
        slot->state.viewport_handle = viewport_handle;
        slot->commands_dirty = 1;
        EasyRpgRgssRenderMarkOrderDirty();
    }
    return ERgssTilemapOk;
}

extern "C" int EasyRpgRgssTilemapSetTileset(int handle, int bitmap_handle)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTilemapErrorInvalidHandle;
    if (!ValidBitmap(bitmap_handle))
        return ERgssTilemapErrorResource;
    if (slot->state.tileset_handle != bitmap_handle)
    {
        slot->state.tileset_handle = bitmap_handle;
        slot->commands_dirty = 1;
    }
    return ERgssTilemapOk;
}

extern "C" int EasyRpgRgssTilemapSetAutotile(int handle, int index, int bitmap_handle)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTilemapErrorInvalidHandle;
    if (index < 0 || index >= KAutotileCount || !ValidBitmap(bitmap_handle))
        return ERgssTilemapErrorInvalidArgument;
    if (slot->state.autotile_handles[index] != bitmap_handle)
    {
        slot->state.autotile_handles[index] = bitmap_handle;
        slot->commands_dirty = 1;
    }
    return ERgssTilemapOk;
}

extern "C" int EasyRpgRgssTilemapSetMapData(int handle, int table_handle)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTilemapErrorInvalidHandle;
    if (!ValidTable(table_handle))
        return ERgssTilemapErrorResource;
    if (slot->state.map_data_handle != table_handle)
    {
        slot->state.map_data_handle = table_handle;
        slot->commands_dirty = 1;
    }
    return ERgssTilemapOk;
}

extern "C" int EasyRpgRgssTilemapSetFlashData(int handle, int table_handle)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTilemapErrorInvalidHandle;
    if (!ValidTable(table_handle))
        return ERgssTilemapErrorResource;
    slot->state.flash_data_handle = table_handle;
    return ERgssTilemapOk;
}

extern "C" int EasyRpgRgssTilemapSetPriorities(int handle, int table_handle)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTilemapErrorInvalidHandle;
    if (!ValidTable(table_handle))
        return ERgssTilemapErrorResource;
    if (slot->state.priorities_handle != table_handle)
    {
        slot->state.priorities_handle = table_handle;
        slot->commands_dirty = 1;
    }
    return ERgssTilemapOk;
}

extern "C" int EasyRpgRgssTilemapSetOrigin(int handle, int ox, int oy)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTilemapErrorInvalidHandle;
    if (slot->state.ox != ox || slot->state.oy != oy)
    {
        const int oldY = slot->state.oy;
        slot->state.ox = ox;
        slot->state.oy = oy;
        slot->commands_dirty = 1;
        if (oldY != oy)
            EasyRpgRgssRenderMarkOrderDirty();
    }
    return ERgssTilemapOk;
}

extern "C" int EasyRpgRgssTilemapSetVisible(int handle, int visible)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTilemapErrorInvalidHandle;
    slot->state.visible = visible ? 1 : 0;
    return ERgssTilemapOk;
}

extern "C" int EasyRpgRgssTilemapUpdate(int handle)
{
    TTilemapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTilemapErrorInvalidHandle;
    ++slot->animation_tick;
    if (slot->animation_tick >= 16)
    {
        slot->animation_tick = 0;
        slot->animation_frame = (slot->animation_frame + 1) & 3;
    }
    return ERgssTilemapOk;
}

extern "C" void EasyRpgRgssTilemapInvalidateAll(void)
{
    if (!gTilemaps.initialized)
        return;
    for (int index = 0; index < gTilemaps.host.max_tilemaps; ++index)
    {
        if (gTilemaps.slots[index].used)
            gTilemaps.slots[index].commands_dirty = 1;
    }
}

extern "C" int EasyRpgRgssTilemapCount(void)
{
    return gTilemaps.initialized ? gTilemaps.active_count : ERgssTilemapErrorNotInitialized;
}

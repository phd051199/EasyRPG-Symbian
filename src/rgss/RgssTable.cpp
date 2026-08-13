#include "RgssTableBridge.h"

namespace
{
struct TTableSlot
{
    short* data;
    int dimensions;
    int x_size;
    int y_size;
    int z_size;
    int count;
    unsigned int revision;
    unsigned short generation;
    unsigned char used;
};

struct TTableState
{
    EasyRpgRgssTableHost host;
    TTableSlot* slots;
    int active_count;
    int initialized;
};

TTableState gTables;

TTableSlot* FindSlot(int handle)
{
    if (!gTables.initialized || handle <= 0)
        return 0;
    const int encodedIndex = handle & 0xffff;
    const int generation = (handle >> 16) & 0x7fff;
    if (encodedIndex <= 0 || encodedIndex > gTables.host.max_tables || generation <= 0)
        return 0;
    TTableSlot* slot = &gTables.slots[encodedIndex - 1];
    if (!slot->used || slot->generation != generation)
        return 0;
    return slot;
}

int MakeHandle(int index, unsigned short generation)
{
    return (static_cast<int>(generation & 0x7fff) << 16) | (index + 1);
}

int ValidateShape(int dimensions,
                  int xSize,
                  int ySize,
                  int zSize,
                  int& normalizedY,
                  int& normalizedZ,
                  int& count)
{
    if (dimensions < 1 || dimensions > 3 || xSize <= 0)
        return 0;
    normalizedY = dimensions >= 2 ? ySize : 1;
    normalizedZ = dimensions >= 3 ? zSize : 1;
    if (normalizedY <= 0 || normalizedZ <= 0)
        return 0;
    if (xSize > 1073741823 / normalizedY)
        return 0;
    const int xy = xSize * normalizedY;
    if (xy > 1073741823 / normalizedZ)
        return 0;
    count = xy * normalizedZ;
    return count > 0;
}

int IndexOf(const TTableSlot& slot, int x, int y, int z, int& index)
{
    if (x < 0 || y < 0 || z < 0 ||
        x >= slot.x_size || y >= slot.y_size || z >= slot.z_size)
        return 0;
    index = x + slot.x_size * (y + slot.y_size * z);
    return 1;
}

void ResetSlot(TTableSlot& slot)
{
    slot.data = 0;
    slot.dimensions = 0;
    slot.x_size = 0;
    slot.y_size = 0;
    slot.z_size = 0;
    slot.count = 0;
    slot.revision = 0;
    slot.used = 0;
}

short* AllocateData(int count)
{
    const unsigned int bytes = static_cast<unsigned int>(count) *
                               static_cast<unsigned int>(sizeof(short));
    short* data = static_cast<short*>(gTables.host.allocate(gTables.host.context, bytes));
    if (!data)
        return 0;
    for (int index = 0; index < count; ++index)
        data[index] = 0;
    return data;
}
}

extern "C" int EasyRpgRgssTableInitialize(const EasyRpgRgssTableHost* host)
{
    if (!host || !host->allocate || !host->release ||
        host->max_tables <= 0 || host->max_tables > 65535)
        return ERgssTableErrorInvalidArgument;

    EasyRpgRgssTableShutdown();
    const unsigned int bytes = static_cast<unsigned int>(host->max_tables) *
                               static_cast<unsigned int>(sizeof(TTableSlot));
    TTableSlot* slots = static_cast<TTableSlot*>(host->allocate(host->context, bytes));
    if (!slots)
        return ERgssTableErrorNoMemory;

    gTables.host = *host;
    gTables.slots = slots;
    gTables.active_count = 0;
    gTables.initialized = 1;
    for (int index = 0; index < host->max_tables; ++index)
    {
        slots[index].generation = 0;
        ResetSlot(slots[index]);
    }
    return ERgssTableOk;
}

extern "C" void EasyRpgRgssTableShutdown(void)
{
    if (!gTables.initialized)
        return;
    for (int index = 0; index < gTables.host.max_tables; ++index)
    {
        TTableSlot& slot = gTables.slots[index];
        if (slot.used && slot.data)
            gTables.host.release(gTables.host.context, slot.data);
        ResetSlot(slot);
    }
    gTables.host.release(gTables.host.context, gTables.slots);
    gTables.slots = 0;
    gTables.active_count = 0;
    gTables.host.context = 0;
    gTables.host.allocate = 0;
    gTables.host.release = 0;
    gTables.host.max_tables = 0;
    gTables.initialized = 0;
}

extern "C" int EasyRpgRgssTableCreate(int dimensions,
                                       int x_size,
                                       int y_size,
                                       int z_size,
                                       int* handle)
{
    if (!gTables.initialized)
        return ERgssTableErrorNotInitialized;
    if (!handle)
        return ERgssTableErrorInvalidArgument;

    int normalizedY = 0;
    int normalizedZ = 0;
    int count = 0;
    if (!ValidateShape(dimensions,
                       x_size,
                       y_size,
                       z_size,
                       normalizedY,
                       normalizedZ,
                       count))
        return ERgssTableErrorInvalidArgument;

    int slotIndex = -1;
    for (int index = 0; index < gTables.host.max_tables; ++index)
    {
        if (!gTables.slots[index].used)
        {
            slotIndex = index;
            break;
        }
    }
    if (slotIndex < 0)
        return ERgssTableErrorCapacity;

    short* data = AllocateData(count);
    if (!data)
        return ERgssTableErrorNoMemory;

    TTableSlot& slot = gTables.slots[slotIndex];
    unsigned short generation = static_cast<unsigned short>((slot.generation + 1u) & 0x7fffu);
    if (generation == 0)
        generation = 1;
    slot.data = data;
    slot.dimensions = dimensions;
    slot.x_size = x_size;
    slot.y_size = normalizedY;
    slot.z_size = normalizedZ;
    slot.count = count;
    slot.revision = 1;
    slot.generation = generation;
    slot.used = 1;
    ++gTables.active_count;
    *handle = MakeHandle(slotIndex, generation);
    return ERgssTableOk;
}

extern "C" int EasyRpgRgssTableDestroy(int handle)
{
    TTableSlot* slot = FindSlot(handle);
    if (!slot)
        return gTables.initialized ? ERgssTableErrorInvalidHandle : ERgssTableErrorNotInitialized;
    if (slot->data)
        gTables.host.release(gTables.host.context, slot->data);
    ResetSlot(*slot);
    --gTables.active_count;
    return ERgssTableOk;
}

extern "C" int EasyRpgRgssTableDimensions(int handle)
{
    TTableSlot* slot = FindSlot(handle);
    return slot ? slot->dimensions : ERgssTableErrorInvalidHandle;
}

extern "C" int EasyRpgRgssTableXSize(int handle)
{
    TTableSlot* slot = FindSlot(handle);
    return slot ? slot->x_size : ERgssTableErrorInvalidHandle;
}

extern "C" int EasyRpgRgssTableYSize(int handle)
{
    TTableSlot* slot = FindSlot(handle);
    return slot ? slot->y_size : ERgssTableErrorInvalidHandle;
}

extern "C" int EasyRpgRgssTableZSize(int handle)
{
    TTableSlot* slot = FindSlot(handle);
    return slot ? slot->z_size : ERgssTableErrorInvalidHandle;
}

extern "C" int EasyRpgRgssTableGet(int handle,
                                    int x,
                                    int y,
                                    int z,
                                    int* value)
{
    TTableSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTableErrorInvalidHandle;
    if (!value)
        return ERgssTableErrorInvalidArgument;
    int index = 0;
    if (!IndexOf(*slot, x, y, z, index))
        return ERgssTableErrorOutOfBounds;
    *value = static_cast<int>(slot->data[index]);
    return ERgssTableOk;
}

extern "C" int EasyRpgRgssTableSet(int handle,
                                    int x,
                                    int y,
                                    int z,
                                    int value)
{
    TTableSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTableErrorInvalidHandle;
    if (value < -32768 || value > 32767)
        return ERgssTableErrorInvalidArgument;
    int index = 0;
    if (!IndexOf(*slot, x, y, z, index))
        return ERgssTableErrorOutOfBounds;
    const short next = static_cast<short>(value);
    if (slot->data[index] != next)
    {
        slot->data[index] = next;
        ++slot->revision;
        if (slot->revision == 0)
            slot->revision = 1;
    }
    return ERgssTableOk;
}

extern "C" int EasyRpgRgssTableResize(int handle,
                                       int dimensions,
                                       int x_size,
                                       int y_size,
                                       int z_size)
{
    TTableSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTableErrorInvalidHandle;

    int normalizedY = 0;
    int normalizedZ = 0;
    int count = 0;
    if (!ValidateShape(dimensions,
                       x_size,
                       y_size,
                       z_size,
                       normalizedY,
                       normalizedZ,
                       count))
        return ERgssTableErrorInvalidArgument;

    short* data = AllocateData(count);
    if (!data)
        return ERgssTableErrorNoMemory;

    const int copyX = slot->x_size < x_size ? slot->x_size : x_size;
    const int copyY = slot->y_size < normalizedY ? slot->y_size : normalizedY;
    const int copyZ = slot->z_size < normalizedZ ? slot->z_size : normalizedZ;
    for (int z = 0; z < copyZ; ++z)
    {
        for (int y = 0; y < copyY; ++y)
        {
            const int oldBase = slot->x_size * (y + slot->y_size * z);
            const int newBase = x_size * (y + normalizedY * z);
            for (int x = 0; x < copyX; ++x)
                data[newBase + x] = slot->data[oldBase + x];
        }
    }

    gTables.host.release(gTables.host.context, slot->data);
    slot->data = data;
    slot->dimensions = dimensions;
    slot->x_size = x_size;
    slot->y_size = normalizedY;
    slot->z_size = normalizedZ;
    slot->count = count;
    ++slot->revision;
    if (slot->revision == 0)
        slot->revision = 1;
    return ERgssTableOk;
}

extern "C" int EasyRpgRgssTableLock(int handle,
                                     short** data,
                                     int* count)
{
    TTableSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTableErrorInvalidHandle;
    if (!data || !count)
        return ERgssTableErrorInvalidArgument;
    *data = slot->data;
    *count = slot->count;
    return ERgssTableOk;
}

extern "C" int EasyRpgRgssTableLoadRaw(int handle,
                                        const short* data,
                                        int count)
{
    TTableSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssTableErrorInvalidHandle;
    if (!data || count != slot->count)
        return ERgssTableErrorInvalidArgument;
    for (int index = 0; index < count; ++index)
        slot->data[index] = data[index];
    ++slot->revision;
    if (slot->revision == 0)
        slot->revision = 1;
    return ERgssTableOk;
}

extern "C" unsigned int EasyRpgRgssTableRevision(int handle)
{
    TTableSlot* slot = FindSlot(handle);
    return slot ? slot->revision : 0;
}

extern "C" int EasyRpgRgssTableCount(void)
{
    return gTables.initialized ? gTables.active_count : ERgssTableErrorNotInitialized;
}

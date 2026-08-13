#ifndef EASYRPG_RGSS_TABLE_BRIDGE_H
#define EASYRPG_RGSS_TABLE_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssTableResult
{
    ERgssTableOk = 0,
    ERgssTableErrorInvalidArgument = -80,
    ERgssTableErrorNotInitialized = -81,
    ERgssTableErrorNoMemory = -82,
    ERgssTableErrorCapacity = -83,
    ERgssTableErrorInvalidHandle = -84,
    ERgssTableErrorOutOfBounds = -85
};

typedef void* (*EasyRpgRgssTableAllocFn)(void* context, unsigned int bytes);
typedef void (*EasyRpgRgssTableFreeFn)(void* context, void* memory);

typedef struct EasyRpgRgssTableHost
{
    void* context;
    EasyRpgRgssTableAllocFn allocate;
    EasyRpgRgssTableFreeFn release;
    int max_tables;
} EasyRpgRgssTableHost;

int EasyRpgRgssTableInitialize(const EasyRpgRgssTableHost* host);
void EasyRpgRgssTableShutdown(void);
int EasyRpgRgssTableCreate(int dimensions,
                           int x_size,
                           int y_size,
                           int z_size,
                           int* handle);
int EasyRpgRgssTableDestroy(int handle);
int EasyRpgRgssTableDimensions(int handle);
int EasyRpgRgssTableXSize(int handle);
int EasyRpgRgssTableYSize(int handle);
int EasyRpgRgssTableZSize(int handle);
int EasyRpgRgssTableGet(int handle,
                        int x,
                        int y,
                        int z,
                        int* value);
int EasyRpgRgssTableSet(int handle,
                        int x,
                        int y,
                        int z,
                        int value);
int EasyRpgRgssTableResize(int handle,
                           int dimensions,
                           int x_size,
                           int y_size,
                           int z_size);
int EasyRpgRgssTableLock(int handle,
                         short** data,
                         int* count);
int EasyRpgRgssTableLoadRaw(int handle,
                            const short* data,
                            int count);
int EasyRpgRgssTableCount(void);

#ifdef __cplusplus
}
#endif

#endif

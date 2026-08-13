#ifndef EASYRPG_RGSS_VIEWPORT_BRIDGE_H
#define EASYRPG_RGSS_VIEWPORT_BRIDGE_H

#include "RgssCoreBridge.h"

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssViewportResult
{
    ERgssViewportOk = 0,
    ERgssViewportErrorInvalidArgument = -70,
    ERgssViewportErrorNotInitialized = -71,
    ERgssViewportErrorNoMemory = -72,
    ERgssViewportErrorCapacity = -73,
    ERgssViewportErrorInvalidHandle = -74
};

typedef void* (*EasyRpgRgssViewportAllocFn)(void* context, unsigned int bytes);
typedef void (*EasyRpgRgssViewportFreeFn)(void* context, void* memory);

typedef struct EasyRpgRgssViewportHost
{
    void* context;
    EasyRpgRgssViewportAllocFn allocate;
    EasyRpgRgssViewportFreeFn release;
    int max_viewports;
} EasyRpgRgssViewportHost;

typedef struct EasyRpgRgssViewportState
{
    EasyRpgRgssRect rect;
    int ox;
    int oy;
    int z;
    int visible;
} EasyRpgRgssViewportState;

int EasyRpgRgssViewportInitialize(const EasyRpgRgssViewportHost* host);
void EasyRpgRgssViewportShutdown(void);
int EasyRpgRgssViewportCreate(const EasyRpgRgssRect* rect, int* handle);
int EasyRpgRgssViewportDestroy(int handle);
int EasyRpgRgssViewportGetState(int handle, EasyRpgRgssViewportState* state);
int EasyRpgRgssViewportSetState(int handle, const EasyRpgRgssViewportState* state);
int EasyRpgRgssViewportSetRect(int handle, const EasyRpgRgssRect* rect);
int EasyRpgRgssViewportSetOrigin(int handle, int ox, int oy);
int EasyRpgRgssViewportSetZ(int handle, int z);
int EasyRpgRgssViewportSetVisible(int handle, int visible);
int EasyRpgRgssViewportCount(void);
unsigned int EasyRpgRgssViewportRevision(void);

#ifdef __cplusplus
}
#endif

#endif

#ifndef EASYRPG_RGSS_RENDER_BRIDGE_H
#define EASYRPG_RGSS_RENDER_BRIDGE_H

#include "RgssCoreBridge.h"

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssRenderResult
{
    ERgssRenderOk = 0,
    ERgssRenderErrorInvalidArgument = -100,
    ERgssRenderErrorNotInitialized = -101,
    ERgssRenderErrorNoMemory = -102,
    ERgssRenderErrorCapacity = -103,
    ERgssRenderErrorInvalidHandle = -104,
    ERgssRenderErrorObject = -105
};

typedef void* (*EasyRpgRgssRenderAllocFn)(void* context, unsigned int bytes);
typedef void (*EasyRpgRgssRenderFreeFn)(void* context, void* memory);
typedef int (*EasyRpgRgssRenderGetZFn)(void* context, int object_handle, int* z);
typedef int (*EasyRpgRgssRenderDrawFn)(void* context,
                                       int object_handle,
                                       EasyRpgRgssRect* output_dirty);

typedef struct EasyRpgRgssRenderHost
{
    void* context;
    EasyRpgRgssRenderAllocFn allocate;
    EasyRpgRgssRenderFreeFn release;
    int max_renderables;
} EasyRpgRgssRenderHost;

int EasyRpgRgssRenderInitialize(const EasyRpgRgssRenderHost* host);
void EasyRpgRgssRenderShutdown(void);
int EasyRpgRgssRenderRegister(void* context,
                              int object_handle,
                              EasyRpgRgssRenderGetZFn get_z,
                              EasyRpgRgssRenderDrawFn draw,
                              int* render_handle);
int EasyRpgRgssRenderUnregister(int render_handle);
int EasyRpgRgssRenderMarkOrderDirty(void);
int EasyRpgRgssRenderRange(int minimum_z,
                           int maximum_z,
                           EasyRpgRgssRect* output_dirty);
int EasyRpgRgssRenderAll(EasyRpgRgssRect* output_dirty);
int EasyRpgRgssRenderCount(void);

#ifdef __cplusplus
}
#endif

#endif

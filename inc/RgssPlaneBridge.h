#ifndef EASYRPG_RGSS_PLANE_BRIDGE_H
#define EASYRPG_RGSS_PLANE_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssPlaneResult
{
    ERgssPlaneOk = 0,
    ERgssPlaneErrorInvalidArgument = -110,
    ERgssPlaneErrorNotInitialized = -111,
    ERgssPlaneErrorNoMemory = -112,
    ERgssPlaneErrorCapacity = -113,
    ERgssPlaneErrorInvalidHandle = -114,
    ERgssPlaneErrorBitmap = -115,
    ERgssPlaneErrorRender = -116
};

typedef void* (*EasyRpgRgssPlaneAllocFn)(void* context, unsigned int bytes);
typedef void (*EasyRpgRgssPlaneFreeFn)(void* context, void* memory);

typedef struct EasyRpgRgssPlaneHost
{
    void* context;
    EasyRpgRgssPlaneAllocFn allocate;
    EasyRpgRgssPlaneFreeFn release;
    int max_planes;
} EasyRpgRgssPlaneHost;

typedef struct EasyRpgRgssPlaneState
{
    int bitmap_handle;
    int viewport_handle;
    int z;
    int ox;
    int oy;
    int zoom_x_milli;
    int zoom_y_milli;
    int opacity;
    int visible;
    int blend_type;
} EasyRpgRgssPlaneState;

int EasyRpgRgssPlaneInitialize(const EasyRpgRgssPlaneHost* host);
void EasyRpgRgssPlaneShutdown(void);
int EasyRpgRgssPlaneCreate(int* handle);
int EasyRpgRgssPlaneDestroy(int handle);
int EasyRpgRgssPlaneGetState(int handle, EasyRpgRgssPlaneState* state);
int EasyRpgRgssPlaneSetState(int handle, const EasyRpgRgssPlaneState* state);
int EasyRpgRgssPlaneSetBitmap(int handle, int bitmap_handle);
int EasyRpgRgssPlaneSetViewport(int handle, int viewport_handle);
int EasyRpgRgssPlaneSetZ(int handle, int z);
int EasyRpgRgssPlaneSetOrigin(int handle, int ox, int oy);
int EasyRpgRgssPlaneSetZoom(int handle, int zoom_x_milli, int zoom_y_milli);
int EasyRpgRgssPlaneSetOpacity(int handle, int opacity);
int EasyRpgRgssPlaneSetVisible(int handle, int visible);
int EasyRpgRgssPlaneSetBlendType(int handle, int blend_type);
int EasyRpgRgssPlaneCount(void);

#ifdef __cplusplus
}
#endif

#endif

#include "RgssVmBridge.h"

namespace
{
EasyRpgRgssVmHost gHost;
int gInitialized = 0;

void ResetHost()
{
    gHost.context = 0;
    gHost.file_size = 0;
    gHost.file_read = 0;
    gHost.ticks = 0;
    gHost.input = 0;
    gHost.bitmap_load = 0;
    gHost.frame_sync = 0;
    gHost.dirty = 0;
    gInitialized = 0;
}
}

extern "C" int EasyRpgRgssVmAvailable(void)
{
    return 0;
}

extern "C" int EasyRpgRgssVmInitialize(int version,
                                        const EasyRpgRgssVmHost* host)
{
    if (version < 1 || version > 3 || !host ||
        !host->file_size || !host->file_read || !host->ticks ||
        !host->input || !host->bitmap_load || !host->frame_sync || !host->dirty)
        return ERgssVmErrorInvalidArgument;

    ResetHost();
    gHost = *host;
    return ERgssVmErrorUnavailable;
}

extern "C" int EasyRpgRgssVmLoadScripts(const char*)
{
    return gInitialized ? ERgssVmErrorUnavailable : ERgssVmErrorNotInitialized;
}

extern "C" int EasyRpgRgssVmTick(void)
{
    return gInitialized ? ERgssVmErrorUnavailable : ERgssVmErrorNotInitialized;
}

extern "C" int EasyRpgRgssVmCollect(void)
{
    return gInitialized ? ERgssVmErrorUnavailable : ERgssVmErrorNotInitialized;
}

extern "C" int EasyRpgRgssVmHeapBytes(void)
{
    return gInitialized ? 0 : ERgssVmErrorNotInitialized;
}

extern "C" void EasyRpgRgssVmShutdown(void)
{
    ResetHost();
}

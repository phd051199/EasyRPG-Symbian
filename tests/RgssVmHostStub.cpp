#include "RgssVmBridge.h"

// Host-test stub for the VM entry points. The real implementation lives in
// RgssVmMri.cpp and requires the Ruby 3.0 static runtime, which is only
// built for the Symbian target. Host builds report "unavailable", matching
// the expectations in rgss_core_test.cpp.
extern "C" int EasyRpgRgssVmAvailable(void)
{
    return 0;
}

extern "C" int EasyRpgRgssVmInitialize(int, const EasyRpgRgssVmHost*)
{
    return ERgssVmErrorUnavailable;
}

extern "C" int EasyRpgRgssVmLoadScripts(const char*)
{
    return ERgssVmErrorNotInitialized;
}

extern "C" int EasyRpgRgssVmTick(void)
{
    return ERgssVmErrorNotInitialized;
}

extern "C" int EasyRpgRgssVmCollect(void)
{
    return ERgssVmErrorNotInitialized;
}

extern "C" int EasyRpgRgssVmHeapBytes(void)
{
    return ERgssVmErrorNotInitialized;
}

extern "C" int EasyRpgRgssVmLastError(char*, int, int*)
{
    return ERgssVmErrorNotInitialized;
}

extern "C" void EasyRpgRgssVmShutdown(void)
{
}

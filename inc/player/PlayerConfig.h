#ifndef EASYRPG_PLAYER_CONFIG_H
#define EASYRPG_PLAYER_CONFIG_H

#include <e32std.h>

namespace EasyRpgPlayerConfig
{
enum
{
    KGameWidth = 320,
    KGameHeight = 240,
    KLogicFps = 60,
    KRenderFps = 30,
    KLogicTicksPerRender = KLogicFps / KRenderFps,
    KMaxCatchUpLogicFrames = 2,
    KMaximumScreenUpdateRegions = 6
};

const TInt KLowMemoryBytes = 12 * 1024 * 1024;
const TInt KCriticalMemoryBytes = 6 * 1024 * 1024;
const TInt KLowMemoryRecoveryBytes = 16 * 1024 * 1024;
const TInt KCriticalMemoryRecoveryBytes = 9 * 1024 * 1024;
const TUint32 KLowMemoryTrimFrames = 180;
const TUint32 KCriticalMemoryTrimFrames = 60;
const TUint32 KHeapCompressFrames = 300;
const TInt KHeapCompressMinFreedBytes = 128 * 1024;
}

#endif

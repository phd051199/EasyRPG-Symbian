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
    // Keep short render stalls from slowing game logic/input. A missed render
    // stays due and is retried on the next logic tick while 60 Hz game logic
    // keeps advancing independently.
    KMaxCatchUpLogicFrames = 4,
    KIdleMaintenanceIntervalFrames = 16,
    KIdleMaintenanceMinSlackUs = 4000,
    // Upstream damage-tracking cap; presentation batching limits
    // (KMaximumPresentRegions, KDirtyRegionCapacity) live in DisplaySurface.cpp.
    KMaximumScreenUpdateRegions = 8
};

const TInt KLowMemoryBytes = 12 * 1024 * 1024;
const TInt KCriticalMemoryBytes = 6 * 1024 * 1024;
const TInt KLowMemoryRecoveryBytes = 16 * 1024 * 1024;
const TInt KCriticalMemoryRecoveryBytes = 9 * 1024 * 1024;
}

#endif

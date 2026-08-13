#include "RgssGraphicsBridge.h"

namespace
{
struct TGraphicsState
{
    EasyRpgRgssGraphicsHost host;
    int frame_rate;
    int frame_count;
    int initialized;
};

TGraphicsState gGraphics;
}

extern "C" int EasyRpgRgssGraphicsInitialize(const EasyRpgRgssGraphicsHost* host)
{
    if (!host || !host->frame_sync ||
        host->default_frame_rate <= 0 || host->default_frame_rate > 120)
        return ERgssGraphicsErrorInvalidArgument;

    gGraphics.host = *host;
    gGraphics.frame_rate = host->default_frame_rate;
    gGraphics.frame_count = 0;
    gGraphics.initialized = 1;
    return ERgssGraphicsOk;
}

extern "C" void EasyRpgRgssGraphicsShutdown(void)
{
    gGraphics.host.context = 0;
    gGraphics.host.frame_sync = 0;
    gGraphics.host.default_frame_rate = 0;
    gGraphics.frame_rate = 0;
    gGraphics.frame_count = 0;
    gGraphics.initialized = 0;
}

extern "C" int EasyRpgRgssGraphicsUpdate(void)
{
    if (!gGraphics.initialized)
        return ERgssGraphicsErrorNotInitialized;
    if (gGraphics.frame_count < 2147483647)
        ++gGraphics.frame_count;
    else
        gGraphics.frame_count = 0;
    const int result = gGraphics.host.frame_sync(gGraphics.host.context,
                                                  gGraphics.frame_rate,
                                                  gGraphics.frame_count);
    return result == 0 ? ERgssGraphicsOk : ERgssGraphicsErrorSync;
}

extern "C" int EasyRpgRgssGraphicsWait(int duration)
{
    if (!gGraphics.initialized)
        return ERgssGraphicsErrorNotInitialized;
    if (duration < 0)
        return ERgssGraphicsErrorInvalidArgument;
    for (int index = 0; index < duration; ++index)
    {
        const int result = EasyRpgRgssGraphicsUpdate();
        if (result != ERgssGraphicsOk)
            return result;
    }
    return ERgssGraphicsOk;
}

extern "C" int EasyRpgRgssGraphicsFrameRate(void)
{
    return gGraphics.initialized
        ? gGraphics.frame_rate
        : ERgssGraphicsErrorNotInitialized;
}

extern "C" int EasyRpgRgssGraphicsSetFrameRate(int frame_rate)
{
    if (!gGraphics.initialized)
        return ERgssGraphicsErrorNotInitialized;
    if (frame_rate <= 0 || frame_rate > 120)
        return ERgssGraphicsErrorInvalidArgument;
    gGraphics.frame_rate = frame_rate;
    return ERgssGraphicsOk;
}

extern "C" int EasyRpgRgssGraphicsFrameCount(void)
{
    return gGraphics.initialized
        ? gGraphics.frame_count
        : ERgssGraphicsErrorNotInitialized;
}

extern "C" int EasyRpgRgssGraphicsSetFrameCount(int frame_count)
{
    if (!gGraphics.initialized)
        return ERgssGraphicsErrorNotInitialized;
    if (frame_count < 0)
        return ERgssGraphicsErrorInvalidArgument;
    gGraphics.frame_count = frame_count;
    return ERgssGraphicsOk;
}

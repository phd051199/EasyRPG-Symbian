#include "RgssInputBridge.h"

namespace
{
volatile unsigned int gRawMask = 0;
unsigned int gCurrentMask = 0;
unsigned int gPreviousMask = 0;
unsigned int gTriggerMask = 0;
unsigned short gHeldFrames[32];
int gRepeatDelay = 24;
int gRepeatInterval = 6;
}

extern "C" void EasyRpgRgssInputReset(void)
{
    gRawMask = 0;
    gCurrentMask = 0;
    gPreviousMask = 0;
    gTriggerMask = 0;
    gRepeatDelay = 24;
    gRepeatInterval = 6;
    for (int index = 0; index < 32; ++index)
        gHeldFrames[index] = 0;
}

extern "C" void EasyRpgRgssInputSetMask(unsigned int mask)
{
    gRawMask = mask;
}

extern "C" unsigned int EasyRpgRgssInputRawMask(void)
{
    return gRawMask;
}

extern "C" unsigned int EasyRpgRgssInputCurrentMask(void)
{
    return gCurrentMask;
}

extern "C" int EasyRpgRgssInputUpdate(void)
{
    gPreviousMask = gCurrentMask;
    gCurrentMask = gRawMask;
    gTriggerMask = gCurrentMask & ~gPreviousMask;
    for (int index = 0; index < 32; ++index)
    {
        const unsigned int bit = 1u << index;
        if ((gCurrentMask & bit) == 0)
        {
            gHeldFrames[index] = 0;
            continue;
        }
        if (gHeldFrames[index] < 65535)
            ++gHeldFrames[index];
    }
    return ERgssInputOk;
}

extern "C" int EasyRpgRgssInputPress(unsigned int mask)
{
    if (mask == 0)
        return 0;
    return (gCurrentMask & mask) != 0 ? 1 : 0;
}

extern "C" int EasyRpgRgssInputTrigger(unsigned int mask)
{
    if (mask == 0)
        return 0;
    return (gTriggerMask & mask) != 0 ? 1 : 0;
}

extern "C" int EasyRpgRgssInputRepeat(unsigned int mask)
{
    if (mask == 0)
        return 0;
    if ((gTriggerMask & mask) != 0)
        return 1;
    for (int index = 0; index < 32; ++index)
    {
        const unsigned int bit = 1u << index;
        if ((mask & bit) == 0 || (gCurrentMask & bit) == 0)
            continue;
        const int held = gHeldFrames[index];
        if (held >= gRepeatDelay &&
            ((held - gRepeatDelay) % gRepeatInterval) == 0)
            return 1;
    }
    return 0;
}

extern "C" int EasyRpgRgssInputSetRepeatTiming(int delay_frames,
                                                int interval_frames)
{
    if (delay_frames <= 0 || interval_frames <= 0 ||
        delay_frames > 65535 || interval_frames > 65535)
        return ERgssInputErrorInvalidArgument;
    gRepeatDelay = delay_frames;
    gRepeatInterval = interval_frames;
    return ERgssInputOk;
}

extern "C" int EasyRpgRgssInputHeldFrames(unsigned int mask)
{
    if (mask == 0)
        return 0;
    int result = 0;
    for (int index = 0; index < 32; ++index)
    {
        const unsigned int bit = 1u << index;
        if ((mask & bit) != 0 && gHeldFrames[index] > result)
            result = gHeldFrames[index];
    }
    return result;
}

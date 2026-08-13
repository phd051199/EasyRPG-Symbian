#ifndef EASYRPG_RGSS_INPUT_BRIDGE_H
#define EASYRPG_RGSS_INPUT_BRIDGE_H

#include "RgssVmBridge.h"

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssInputResult
{
    ERgssInputOk = 0,
    ERgssInputErrorInvalidArgument = -100
};

void EasyRpgRgssInputReset(void);
void EasyRpgRgssInputSetMask(unsigned int mask);
unsigned int EasyRpgRgssInputRawMask(void);
unsigned int EasyRpgRgssInputCurrentMask(void);
int EasyRpgRgssInputUpdate(void);
int EasyRpgRgssInputPress(unsigned int mask);
int EasyRpgRgssInputTrigger(unsigned int mask);
int EasyRpgRgssInputRepeat(unsigned int mask);
int EasyRpgRgssInputSetRepeatTiming(int delay_frames, int interval_frames);
int EasyRpgRgssInputHeldFrames(unsigned int mask);

#ifdef __cplusplus
}
#endif

#endif

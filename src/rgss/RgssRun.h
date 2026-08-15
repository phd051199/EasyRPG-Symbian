#ifndef RGSS_RUN_H
#define RGSS_RUN_H

/*
 * Portable entry point of the RGSS runtime. The host implementation
 * (inc/RgssHost.h) must be installed before calling. Replaces the
 * ScriptVmBridge one-shot flow for the rewritten lane.
 */

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    RgssRunOk = 0,
    RgssRunGeneral = -2,
    RgssRunNoMemory = -4,
    RgssRunCorrupt = -20,
    RgssRunScriptError = -30
};

/* Boots Ruby, binds the RGSS classes, evals module_rpgN + Data/Scripts.*
 * for the generation (1=XP, 2=VX, 3=VXAce). Blocking: runs rgss_main. */
int RgssRun(int aGeneration, char* aError, int aErrorCapacity);

/* Evaluate one line/snippet after RgssRun (host harness tests). */
int RgssEval(const char* aCode, char* aError, int aErrorCapacity);

#ifdef __cplusplus
}
#endif

#endif

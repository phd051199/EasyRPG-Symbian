#ifndef EASYRPG_SCRIPT_VM_BRIDGE_H
#define EASYRPG_SCRIPT_VM_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    EScriptVmOk = 0,
    EScriptVmGeneral = -2,
    EScriptVmNoMemory = -4,
    EScriptVmNotSupported = -5,
    EScriptVmCorrupt = -20
};

int EasyRpgScriptVmCreate(const unsigned char* aArchive,
                          int aArchiveSize,
                          int aGeneration,
                          char* aError,
                          int aErrorCapacity);
int EasyRpgScriptVmExecute(char* aError, int aErrorCapacity);
void EasyRpgScriptVmDestroy(void);

#ifdef __cplusplus
}
#endif

#endif

#ifndef EASYRPG_RGSS_VM_BRIDGE_H
#define EASYRPG_RGSS_VM_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssInputMask
{
    ERgssInputUp = 1u << 0,
    ERgssInputDown = 1u << 1,
    ERgssInputLeft = 1u << 2,
    ERgssInputRight = 1u << 3,
    ERgssInputDecision = 1u << 4,
    ERgssInputCancel = 1u << 5,
    ERgssInputShift = 1u << 6,
    ERgssInputA = 1u << 7,
    ERgssInputX = 1u << 8,
    ERgssInputY = 1u << 9,
    ERgssInputZ = 1u << 10,
    ERgssInputL = 1u << 11,
    ERgssInputR = 1u << 12
};

enum EasyRpgRgssVmResult
{
    ERgssVmOk = 0,
    ERgssVmErrorInvalidArgument = -20,
    ERgssVmErrorUnavailable = -21,
    ERgssVmErrorNotInitialized = -22,
    ERgssVmErrorFile = -23,
    ERgssVmErrorScript = -24,
    ERgssVmErrorRuntime = -25
};

enum EasyRpgRgssAudioCommand
{
    ERgssAudioBgmPlay = 1,
    ERgssAudioBgmStop,
    ERgssAudioBgmFade,
    ERgssAudioBgsPlay,
    ERgssAudioBgsStop,
    ERgssAudioBgsFade,
    ERgssAudioMePlay,
    ERgssAudioMeStop,
    ERgssAudioMeFade,
    ERgssAudioSePlay,
    ERgssAudioSeStop
};

typedef int (*EasyRpgRgssVmFileSizeFn)(void* context,
                                       const char* relative_path);
typedef int (*EasyRpgRgssVmFileReadFn)(void* context,
                                       const char* relative_path,
                                       int offset,
                                       void* buffer,
                                       int bytes);
typedef int (*EasyRpgRgssVmFileWriteFn)(void* context,
                                        const char* relative_path,
                                        int offset,
                                        const void* buffer,
                                        int bytes,
                                        int truncate);
typedef unsigned int (*EasyRpgRgssVmTicksFn)(void* context);
typedef unsigned int (*EasyRpgRgssVmInputFn)(void* context,
                                             int word);
typedef int (*EasyRpgRgssVmBitmapLoadFn)(void* context,
                                         const char* relative_path,
                                         int* bitmap_handle);
typedef int (*EasyRpgRgssVmTextDecodeFn)(void* context,
                                         const unsigned char* text,
                                         int text_length,
                                         unsigned int* codepoints,
                                         int capacity);
typedef int (*EasyRpgRgssVmFrameSyncFn)(void* context,
                                        int frame_rate,
                                        int frame_count);
typedef int (*EasyRpgRgssVmScriptCountFn)(void* context);
typedef int (*EasyRpgRgssVmScriptInfoFn)(void* context,
                                         int index,
                                         int* id,
                                         int* name_length);
typedef int (*EasyRpgRgssVmScriptNameFn)(void* context,
                                         int index,
                                         void* buffer,
                                         int capacity);
typedef int (*EasyRpgRgssVmScriptInflateFn)(void* context,
                                            int index,
                                            unsigned char** source,
                                            int* source_length);
typedef void (*EasyRpgRgssVmReleaseFn)(void* context, void* memory);
typedef void (*EasyRpgRgssVmDirtyFn)(void* context,
                                     int x,
                                     int y,
                                     int width,
                                     int height);
typedef int (*EasyRpgRgssVmAudioFn)(void* context,
                                    int command,
                                    const char* relative_path,
                                    int volume,
                                    int pitch,
                                    int duration);
typedef void (*EasyRpgRgssVmBootProgressFn)(void* context,
                                            int phase,
                                            int percent);

/* Boot phases reported through EasyRpgRgssVmHost.boot_progress so the engine
   thread can drive a loading gauge while the VM thread is still booting. */
enum EasyRpgRgssVmBootPhase
{
    ERgssVmBootMounting = 1,
    ERgssVmBootRubyInit = 2,
    ERgssVmBootBootstrap = 3,
    ERgssVmBootScripts = 4,
    ERgssVmBootRunning = 5
};

typedef struct EasyRpgRgssVmHost
{
    void* context;
    EasyRpgRgssVmFileSizeFn file_size;
    EasyRpgRgssVmFileReadFn file_read;
    EasyRpgRgssVmFileWriteFn file_write;
    EasyRpgRgssVmTicksFn ticks;
    EasyRpgRgssVmInputFn input;
    EasyRpgRgssVmBitmapLoadFn bitmap_load;
    EasyRpgRgssVmTextDecodeFn text_decode;
    EasyRpgRgssVmFrameSyncFn frame_sync;
    EasyRpgRgssVmScriptCountFn script_count;
    EasyRpgRgssVmScriptInfoFn script_info;
    EasyRpgRgssVmScriptNameFn script_name;
    EasyRpgRgssVmScriptInflateFn script_inflate;
    EasyRpgRgssVmReleaseFn release;
    EasyRpgRgssVmDirtyFn dirty;
    EasyRpgRgssVmAudioFn audio;
    EasyRpgRgssVmBootProgressFn boot_progress;
} EasyRpgRgssVmHost;

int EasyRpgRgssVmAvailable(void);
int EasyRpgRgssVmInitialize(int version,
                            const EasyRpgRgssVmHost* host);
int EasyRpgRgssVmLoadScripts(const char* relative_path);
int EasyRpgRgssVmTick(void);
int EasyRpgRgssVmCollect(void);
int EasyRpgRgssVmHeapBytes(void);
int EasyRpgRgssVmLastError(char* buffer, int capacity, int* script_index);
void EasyRpgRgssVmShutdown(void);

#ifdef __cplusplus
}
#endif

#endif

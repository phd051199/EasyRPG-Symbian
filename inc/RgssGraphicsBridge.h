#ifndef EASYRPG_RGSS_GRAPHICS_BRIDGE_H
#define EASYRPG_RGSS_GRAPHICS_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssGraphicsResult
{
    ERgssGraphicsOk = 0,
    ERgssGraphicsErrorInvalidArgument = -90,
    ERgssGraphicsErrorNotInitialized = -91,
    ERgssGraphicsErrorSync = -92
};

typedef int (*EasyRpgRgssGraphicsFrameSyncFn)(void* context,
                                               int frame_rate,
                                               int frame_count);

typedef struct EasyRpgRgssGraphicsHost
{
    void* context;
    EasyRpgRgssGraphicsFrameSyncFn frame_sync;
    int default_frame_rate;
} EasyRpgRgssGraphicsHost;

int EasyRpgRgssGraphicsInitialize(const EasyRpgRgssGraphicsHost* host);
void EasyRpgRgssGraphicsShutdown(void);
int EasyRpgRgssGraphicsUpdate(void);
int EasyRpgRgssGraphicsWait(int duration);
int EasyRpgRgssGraphicsFrameRate(void);
int EasyRpgRgssGraphicsSetFrameRate(int frame_rate);
int EasyRpgRgssGraphicsFrameCount(void);
int EasyRpgRgssGraphicsSetFrameCount(int frame_count);

#ifdef __cplusplus
}
#endif

#endif

#ifndef EASYRPG_UPSTREAM_PLAYER_BRIDGE_H
#define EASYRPG_UPSTREAM_PLAYER_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

int EasyRpgUpstreamPlayerBindFramebuffer(void* pixels,
                                         int width,
                                         int height,
                                         int pitch_bytes);
int EasyRpgUpstreamPlayerInit(const char* project_path,
                              const char* save_path,
                              int width,
                              int height);
void EasyRpgUpstreamPlayerShutdown(void);
void EasyRpgUpstreamPlayerPause(void);
void EasyRpgUpstreamPlayerResume(void);
int EasyRpgUpstreamPlayerTick(unsigned int ticks,
                              unsigned int keys_0,
                              unsigned int keys_1,
                              unsigned int keys_2,
                              unsigned int keys_3,
                              int render_frame);
int EasyRpgUpstreamPlayerShouldRender(void);
void EasyRpgUpstreamPlayerRefreshAudio(void);
int EasyRpgUpstreamPlayerTrimMemory(int pressure_level);
int EasyRpgUpstreamPlayerIdleMaintenance(int pressure_level);
int EasyRpgPlatformFreeRamBytes(void);
int EasyRpgPlatformPrepareAllocation(int bytes);
unsigned int EasyRpgPlatformFastCounter(void);
unsigned int EasyRpgPlatformFastCounterFrequency(void);
int EasyRpgPlatformFastCounterCountsUp(void);
enum EasyRpgDirectImageFormat
{
    EEasyRpgDirectImageNone = 0,
    EEasyRpgDirectImageRgb565 = 1,
    EEasyRpgDirectImageA1555 = 2,
    EEasyRpgDirectImageBgra8888 = 3
};
int EasyRpgPlatformProbeDirectImage(void* stream,
                                    int transparent,
                                    unsigned int flags,
                                    int* width,
                                    int* height,
                                    int* pixel_format);
int EasyRpgPlatformDecodeDirectImage(void* stream,
                                     int transparent,
                                     int pixel_format,
                                     int width,
                                     int height,
                                     void* pixels,
                                     int pitch_bytes);
int EasyRpgUpstreamPlayerDirtyRegionCount(void);
int EasyRpgUpstreamPlayerDirtyRegion(int index,
                                     int* x,
                                     int* y,
                                     int* width,
                                     int* height);
int EasyRpgUpstreamPlayerPresentScroll(int* x, int* y);
void EasyRpgUpstreamPlayerConsumeDirtyRegions(void);

#ifdef __cplusplus
}
#endif

#endif

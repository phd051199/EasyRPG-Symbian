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
int EasyRpgPlatformFreeRamBytes(void);
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

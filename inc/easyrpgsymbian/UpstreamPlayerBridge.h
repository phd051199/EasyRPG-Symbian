#ifndef EASYRPG_SYMBIAN_UPSTREAM_PLAYER_BRIDGE_H
#define EASYRPG_SYMBIAN_UPSTREAM_PLAYER_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

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
const unsigned int* EasyRpgUpstreamPlayerPixels(void);
int EasyRpgUpstreamPlayerWidth(void);
int EasyRpgUpstreamPlayerHeight(void);
int EasyRpgUpstreamPlayerPitchPixels(void);

#ifdef __cplusplus
}
#endif

#endif

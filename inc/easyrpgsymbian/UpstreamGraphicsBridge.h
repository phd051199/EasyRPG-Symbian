#ifndef EASYRPG_SYMBIAN_UPSTREAM_GRAPHICS_BRIDGE_H
#define EASYRPG_SYMBIAN_UPSTREAM_GRAPHICS_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

int EasyRpgUpstreamGraphicsInit(int width, int height);
void EasyRpgUpstreamGraphicsShutdown(void);
int EasyRpgUpstreamGraphicsRenderFrame(void);
int EasyRpgUpstreamGraphicsLoadMap(const char* lmu_path,
                                   const unsigned char* chipset_data,
                                   unsigned int chipset_size,
                                   int chipset_id,
                                   int start_x,
                                   int start_y);
int EasyRpgUpstreamGraphicsLoadPlayer(const unsigned char* character_data,
                                      unsigned int character_size,
                                      int character_index,
                                      int transparent,
                                      int start_x,
                                      int start_y);
int EasyRpgUpstreamGraphicsShowPicture(const unsigned char* picture_data,
                                       unsigned int picture_size,
                                       int x,
                                       int y,
                                       int magnify_percent,
                                       int opacity_percent);
int EasyRpgUpstreamGraphicsMovePicture(int x,
                                       int y,
                                       int magnify_percent,
                                       int opacity_percent,
                                       int duration_frames);
void EasyRpgUpstreamGraphicsErasePicture(void);
void EasyRpgUpstreamGraphicsSetTicks(unsigned int ticks);
void EasyRpgUpstreamGraphicsSetKeys(unsigned int keys_lo, unsigned int keys_hi);
const unsigned int* EasyRpgUpstreamGraphicsPixels(void);
int EasyRpgUpstreamGraphicsWidth(void);
int EasyRpgUpstreamGraphicsHeight(void);
int EasyRpgUpstreamGraphicsPitchPixels(void);

#ifdef __cplusplus
}
#endif

#endif

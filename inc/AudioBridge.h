#ifndef EASYRPG_AUDIO_BRIDGE_H
#define EASYRPG_AUDIO_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

int EasyRpgAudioPreloadSe(const char* path, int pitch);
int EasyRpgAudioPlaySe(const char* path, int volume, int pitch);
void EasyRpgAudioStopSe(void);
int EasyRpgAudioPlayBgm(const char* path, int volume, int pitch, int fadein);
void EasyRpgAudioStopBgm(void);
void EasyRpgAudioPauseBgm(void);
void EasyRpgAudioResumeBgm(void);
void EasyRpgAudioSetBgmVolume(int volume);
void EasyRpgAudioSetBgmPitch(int pitch);
void EasyRpgAudioFadeBgm(int fade);
int EasyRpgAudioIsBgmPlaying(void);
unsigned int EasyRpgAudioBgmTicks(void);
void EasyRpgAudioUpdate(void);
int EasyRpgAudioConsumeError(void);
void EasyRpgAudioShutdown(void);

#ifdef __cplusplus
}
#endif

#endif

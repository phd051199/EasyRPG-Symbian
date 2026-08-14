#ifndef EASYRPG_AUDIO_BRIDGE_H
#define EASYRPG_AUDIO_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

void EasyRpgAudioSetEnabled(int enabled);
int EasyRpgAudioIsEnabled(void);
int EasyRpgAudioPreloadSe(const char* path, int pitch);
int EasyRpgAudioWarmSe(const char* path, int pitch);
int EasyRpgAudioPlaySe(const char* path, int volume, int pitch);
void EasyRpgAudioStopSe(void);
int EasyRpgAudioPlayBgm(const char* path, int volume, int pitch, int fadein);
void EasyRpgAudioStopBgm(void);
int EasyRpgAudioPlayBgs(const char* path, int volume, int pitch);
void EasyRpgAudioStopBgs(void);
void EasyRpgAudioFadeBgs(int fade);
int EasyRpgAudioPlayMe(const char* path, int volume, int pitch);
void EasyRpgAudioStopMe(void);
void EasyRpgAudioFadeMe(int fade);
void EasyRpgAudioPauseBgm(void);
void EasyRpgAudioResumeBgm(void);
void EasyRpgAudioSetBgmVolume(int volume);
void EasyRpgAudioSetMasterVolume(int volume);
void EasyRpgAudioSetBgmPitch(int pitch);
void EasyRpgAudioFadeBgm(int fade);
int EasyRpgAudioIsBgmPlaying(void);
int EasyRpgAudioBgmPlayedOnce(void);
unsigned int EasyRpgAudioBgmTicks(void);
void EasyRpgAudioUpdate(void);
int EasyRpgAudioConsumeError(void);
void EasyRpgAudioShutdown(void);

#ifdef __cplusplus
}
#endif

#endif

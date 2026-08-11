#ifndef EASYRPG_SYMBIAN_AUDIO_BRIDGE_H
#define EASYRPG_SYMBIAN_AUDIO_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

int EasyRpgSymbianAudioPlaySe(const char* path, int volume, int pitch);
void EasyRpgSymbianAudioStopSe(void);
int EasyRpgSymbianAudioPlayBgm(const char* path, int volume, int pitch, int fadein);
void EasyRpgSymbianAudioStopBgm(void);
void EasyRpgSymbianAudioPauseBgm(void);
void EasyRpgSymbianAudioResumeBgm(void);
void EasyRpgSymbianAudioSetBgmVolume(int volume);
void EasyRpgSymbianAudioSetBgmPitch(int pitch);
void EasyRpgSymbianAudioFadeBgm(int fade);
int EasyRpgSymbianAudioIsBgmPlaying(void);
unsigned int EasyRpgSymbianAudioBgmTicks(void);
void EasyRpgSymbianAudioUpdate(void);
int EasyRpgSymbianAudioConsumeError(void);
void EasyRpgSymbianAudioShutdown(void);

#ifdef __cplusplus
}
#endif

#endif

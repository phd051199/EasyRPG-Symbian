#include "easyrpgsymbian/SymbianAudio.h"
#include "easyrpgsymbian/SymbianAudioBridge.h"
#include "output.h"

namespace {
class SymbianAudio : public AudioInterface {
public:
    SymbianAudio()
        : bgm_playing(false) {
    }

    void Update() override {
        EasyRpgSymbianAudioUpdate();
        const int error = EasyRpgSymbianAudioConsumeError();
        if (error != 0)
            Output::Warning("Audio backend async error (%d)", error);
    }

    void BGM_Play(std::string const& file, int volume, int pitch, int fadein) override {
        const int result = EasyRpgSymbianAudioPlayBgm(file.c_str(), volume, pitch, fadein);
        bgm_playing = result == 0;
        if (result != 0)
            Output::Warning("BGM backend failed: %s (%d)", file.c_str(), result);
    }

    void BGM_Stop() override {
        EasyRpgSymbianAudioStopBgm();
        bgm_playing = false;
    }

    bool BGM_PlayedOnce() const override {
        return EasyRpgSymbianAudioBgmTicks() > 5000;
    }

    bool BGM_IsPlaying() const override {
        return bgm_playing && EasyRpgSymbianAudioIsBgmPlaying() != 0;
    }

    unsigned BGM_GetTicks() const override {
        return EasyRpgSymbianAudioBgmTicks();
    }

    void BGM_Fade(int fade) override {
        EasyRpgSymbianAudioFadeBgm(fade);
    }

    void BGM_Pause() override {
        EasyRpgSymbianAudioPauseBgm();
    }

    void BGM_Resume() override {
        EasyRpgSymbianAudioResumeBgm();
    }

    void BGM_Volume(int volume) override {
        EasyRpgSymbianAudioSetBgmVolume(volume);
    }

    void BGM_Pitch(int pitch) override {
        EasyRpgSymbianAudioSetBgmPitch(pitch);
    }

    void SE_Play(std::string const& file, int volume, int pitch) override {
        const int result = EasyRpgSymbianAudioPlaySe(file.c_str(), volume, pitch);
        if (result != 0)
            Output::Warning("SE backend failed: %s (%d)", file.c_str(), result);
    }

    void SE_Stop() override {
        EasyRpgSymbianAudioStopSe();
    }

private:
    bool bgm_playing;
};
}

AudioInterface& EasyRpgSymbianAudio() {
    static SymbianAudio audio;
    return audio;
}

#include "AudioBridge.h"
#include "audio.h"
#include "output.h"

namespace
{
class AudioBackend : public AudioInterface
{
  public:
    AudioBackend()
        : bgm_playing(false)
    {
    }

    void Update() override
    {
        if (!EasyRpgAudioEnabled())
            return;
        EasyRpgAudioUpdate();
        const int error = EasyRpgAudioConsumeError();
        if (error != 0)
            Output::Warning("Audio backend async error (%d)", error);
    }

    void BGM_Play(std::string const &file, int volume, int pitch, int fadein) override
    {
        const int result = EasyRpgAudioPlayBgm(file.c_str(), volume, pitch, fadein);
        bgm_playing = result == 0;
        if (result != 0)
            Output::Warning("BGM backend failed: %s (%d)", file.c_str(), result);
    }

    void BGM_Stop() override
    {
        EasyRpgAudioStopBgm();
        bgm_playing = false;
    }

    bool BGM_PlayedOnce() const override
    {
        return EasyRpgAudioBgmPlayedOnce() != 0;
    }

    bool BGM_IsPlaying() const override
    {
        return bgm_playing && EasyRpgAudioIsBgmPlaying() != 0;
    }

    unsigned BGM_GetTicks() const override
    {
        return EasyRpgAudioBgmTicks();
    }

    void BGM_Fade(int fade) override
    {
        EasyRpgAudioFadeBgm(fade);
    }

    void BGM_Pause() override
    {
        EasyRpgAudioPauseBgm();
    }

    void BGM_Resume() override
    {
        EasyRpgAudioResumeBgm();
    }

    void BGM_Volume(int volume) override
    {
        EasyRpgAudioSetBgmVolume(volume);
    }

    void BGM_Pitch(int pitch) override
    {
        EasyRpgAudioSetBgmPitch(pitch);
    }

    void SE_Play(std::string const &file, int volume, int pitch) override
    {
        const int result = EasyRpgAudioPlaySe(file.c_str(), volume, pitch);
        if (result != 0)
            Output::Warning("SE backend failed: %s (%d)", file.c_str(), result);
    }

    void SE_Stop() override
    {
        EasyRpgAudioStopSe();
    }

  private:
    bool bgm_playing;
};
} // namespace

AudioInterface &EasyRpgAudio()
{
    static AudioBackend audio;
    return audio;
}

bool EasyRpgAudioEnabled()
{
    return EasyRpgAudioIsEnabled() != 0;
}

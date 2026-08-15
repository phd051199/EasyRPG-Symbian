#include "RgssBindingUtil.h"

#include "RgssHost.h"

#include <string.h>

namespace {

VALUE mAudio = Qnil;

int Clamp(int aValue, int aMin, int aMax)
{
    if (aValue < aMin)
        return aMin;
    if (aValue > aMax)
        return aMax;
    return aValue;
}

bool CopyNormalized(const char* aInput, char* aOutput, int aCapacity)
{
    if (!aInput || !aOutput || aCapacity <= 1)
        return false;
    const size_t length = strlen(aInput);
    if (length == 0 || length >= static_cast<size_t>(aCapacity))
        return false;
    for (size_t i = 0; i < length; ++i)
        aOutput[i] = aInput[i] == '\\' ? '/' : aInput[i];
    aOutput[length] = 0;
    return true;
}

bool HasExtension(const char* aPath)
{
    const char* slash = strrchr(aPath, '/');
    const char* dot = strrchr(aPath, '.');
    return dot && (!slash || dot > slash);
}

bool ResolveAudioPath(const char* aInput, char* aOutput, int aCapacity)
{
    char base[1024];
    if (!CopyNormalized(aInput, base, sizeof(base)))
        return false;

    const RgssHostVtable* host = RgssHost();
    if (!host || !host->FileExists)
        return false;

    if (host->FileExists(host->ctx, base))
    {
        if (strlen(base) >= static_cast<size_t>(aCapacity))
            return false;
        strcpy(aOutput, base);
        return true;
    }
    if (HasExtension(base))
        return false;

    static const char* kExtensions[] = {
        ".ogg", ".OGG", ".mp3", ".MP3", ".wav", ".WAV",
        ".mid", ".MID", ".midi", ".MIDI", ".wma", ".WMA"
    };
    for (unsigned i = 0; i < sizeof(kExtensions) / sizeof(kExtensions[0]); ++i)
    {
        const size_t baseLength = strlen(base);
        const size_t extLength = strlen(kExtensions[i]);
        if (baseLength + extLength >= sizeof(base) ||
            baseLength + extLength >= static_cast<size_t>(aCapacity))
            continue;
        char candidate[1024];
        strcpy(candidate, base);
        strcat(candidate, kExtensions[i]);
        if (!host->FileExists(host->ctx, candidate))
            continue;
        strcpy(aOutput, candidate);
        return true;
    }
    return false;
}

void ParsePlayArgs(int aArgc, VALUE* aArgv, char* aPath, int aPathCapacity,
                   int* aVolume, int* aPitch)
{
    VALUE filename = Qnil, volume = Qnil, pitch = Qnil;
    rb_scan_args(aArgc, aArgv, "12", &filename, &volume, &pitch);
    StringValue(filename);
    const char* rawPath = StringValueCStr(filename);
    if (!ResolveAudioPath(rawPath, aPath, aPathCapacity))
    {
        VALUE errnoModule = rb_const_get(rb_cObject, rb_intern("Errno"));
        VALUE enoent = rb_const_get(errnoModule, rb_intern("ENOENT"));
        rb_raise(enoent, "No such audio file - %s", rawPath);
    }
    *aVolume = NIL_P(volume) ? 100 : Clamp(NUM2INT(volume), 0, 100);
    *aPitch = NIL_P(pitch) ? 100 : Clamp(NUM2INT(pitch), 50, 150);
}

VALUE AudioBgmPlay(int aArgc, VALUE* aArgv, VALUE)
{
    char path[1024];
    int volume, pitch;
    ParsePlayArgs(aArgc, aArgv, path, sizeof(path), &volume, &pitch);
    const RgssHostVtable* host = RgssHost();
    if (!host || !host->BgmPlay || host->BgmPlay(host->ctx, path, volume, pitch) != 0)
        rgssRaise("BGM playback failed");
    return Qnil;
}

VALUE AudioBgmStop(VALUE)
{
    const RgssHostVtable* host = RgssHost();
    if (host && host->BgmStop)
        host->BgmStop(host->ctx);
    return Qnil;
}

VALUE AudioBgmFade(VALUE, VALUE aMilliseconds)
{
    const RgssHostVtable* host = RgssHost();
    if (host && host->BgmFade)
        host->BgmFade(host->ctx, Clamp(NUM2INT(aMilliseconds), 0, 0x7fffffff));
    return Qnil;
}

VALUE AudioBgmPos(VALUE)
{
    const RgssHostVtable* host = RgssHost();
    const int pos = host && host->BgmPos ? host->BgmPos(host->ctx) : -1;
    return pos < 0 ? INT2FIX(0) : INT2NUM(pos);
}

VALUE AudioBgmPause(VALUE)
{
    const RgssHostVtable* host = RgssHost();
    if (host && host->BgmPause)
        host->BgmPause(host->ctx);
    return Qnil;
}

VALUE AudioBgmResume(VALUE)
{
    const RgssHostVtable* host = RgssHost();
    if (host && host->BgmResume)
        host->BgmResume(host->ctx);
    return Qnil;
}

VALUE AudioBgsPlay(int aArgc, VALUE* aArgv, VALUE)
{
    char path[1024];
    int volume, pitch;
    ParsePlayArgs(aArgc, aArgv, path, sizeof(path), &volume, &pitch);
    const RgssHostVtable* host = RgssHost();
    if (!host || !host->BgsPlay || host->BgsPlay(host->ctx, path, volume, pitch) != 0)
        rgssRaise("BGS playback failed");
    return Qnil;
}

VALUE AudioBgsStop(VALUE)
{
    const RgssHostVtable* host = RgssHost();
    if (host && host->BgsStop)
        host->BgsStop(host->ctx);
    return Qnil;
}

VALUE AudioBgsFade(VALUE, VALUE aMilliseconds)
{
    const RgssHostVtable* host = RgssHost();
    if (host && host->BgsFade)
        host->BgsFade(host->ctx, Clamp(NUM2INT(aMilliseconds), 0, 0x7fffffff));
    return Qnil;
}

VALUE AudioMePlay(int aArgc, VALUE* aArgv, VALUE)
{
    char path[1024];
    int volume, pitch;
    ParsePlayArgs(aArgc, aArgv, path, sizeof(path), &volume, &pitch);
    const RgssHostVtable* host = RgssHost();
    if (!host || !host->MePlay || host->MePlay(host->ctx, path, volume, pitch) != 0)
        rgssRaise("ME playback failed");
    return Qnil;
}

VALUE AudioMeStop(VALUE)
{
    const RgssHostVtable* host = RgssHost();
    if (host && host->MeStop)
        host->MeStop(host->ctx);
    return Qnil;
}

VALUE AudioMeFade(VALUE, VALUE aMilliseconds)
{
    const RgssHostVtable* host = RgssHost();
    if (host && host->MeFade)
        host->MeFade(host->ctx, Clamp(NUM2INT(aMilliseconds), 0, 0x7fffffff));
    return Qnil;
}

VALUE AudioSePlay(int aArgc, VALUE* aArgv, VALUE)
{
    char path[1024];
    int volume, pitch;
    ParsePlayArgs(aArgc, aArgv, path, sizeof(path), &volume, &pitch);
    const RgssHostVtable* host = RgssHost();
    if (!host || !host->SePlay || host->SePlay(host->ctx, path, volume, pitch) != 0)
        rgssRaise("SE playback failed");
    return Qnil;
}

VALUE AudioSeStop(VALUE)
{
    const RgssHostVtable* host = RgssHost();
    if (host && host->SeStop)
        host->SeStop(host->ctx);
    return Qnil;
}

} // namespace

void RgssAudioBindingInit()
{
    mAudio = rb_define_module("Audio");
    rb_define_module_function(mAudio, "bgm_play", RUBY_METHOD_FUNC(AudioBgmPlay), -1);
    rb_define_module_function(mAudio, "bgm_stop", RUBY_METHOD_FUNC(AudioBgmStop), 0);
    rb_define_module_function(mAudio, "bgm_fade", RUBY_METHOD_FUNC(AudioBgmFade), 1);
    rb_define_module_function(mAudio, "bgm_pos", RUBY_METHOD_FUNC(AudioBgmPos), 0);
    rb_define_module_function(mAudio, "bgm_pause", RUBY_METHOD_FUNC(AudioBgmPause), 0);
    rb_define_module_function(mAudio, "bgm_resume", RUBY_METHOD_FUNC(AudioBgmResume), 0);
    rb_define_module_function(mAudio, "bgs_play", RUBY_METHOD_FUNC(AudioBgsPlay), -1);
    rb_define_module_function(mAudio, "bgs_stop", RUBY_METHOD_FUNC(AudioBgsStop), 0);
    rb_define_module_function(mAudio, "bgs_fade", RUBY_METHOD_FUNC(AudioBgsFade), 1);
    rb_define_module_function(mAudio, "me_play", RUBY_METHOD_FUNC(AudioMePlay), -1);
    rb_define_module_function(mAudio, "me_stop", RUBY_METHOD_FUNC(AudioMeStop), 0);
    rb_define_module_function(mAudio, "me_fade", RUBY_METHOD_FUNC(AudioMeFade), 1);
    rb_define_module_function(mAudio, "se_play", RUBY_METHOD_FUNC(AudioSePlay), -1);
    rb_define_module_function(mAudio, "se_stop", RUBY_METHOD_FUNC(AudioSeStop), 0);
}

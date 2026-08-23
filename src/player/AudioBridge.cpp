#include <e32base.h>
#include "AudioBridge.h"
#include "AudioDecode.h"
#include "AudioMixer.h"
#include "DirentBridge.h"
#include "Encoding.h"
#include "MidiLiteBridge.h"
#include "NativeAudio.h"
#include "PlayerConfig.h"
#include "UpstreamPlayerBridge.h"

namespace
{
struct TSoftwareBgmState
{
    HBufC8* iPcm;
    void* iMidiDecoder;
    TInt iFrameCount;
    TInt iSampleRate;
    TInt iChannels;
    TInt iPitch;
    TInt iVolume;
    TInt64 iPhase;
    TInt64 iStep;
    TUint64 iFramesQueued;
    TUint32 iLoopCount;
    TUint32 iFadeStartMs;
    TInt iFadeDurationMs;
    TInt iFadeStartVolume;
    TBool iPlaying;
    TBool iPaused;
};

struct TNativeChannelState
{
    TInt iBgsVolume;
    TUint32 iBgsFadeStartMs;
    TInt iBgsFadeDurationMs;
    TInt iBgsFadeStartVolume;
    TInt iMeVolume;
    TUint32 iMeFadeStartMs;
    TInt iMeFadeDurationMs;
    TInt iMeFadeStartVolume;
    TBool iMeActive;
    TBool iResumeBgmAfterMe;
};

struct TAudioRuntimeState
{
    CAudioMixer* iMixer;
    CIdle* iIdlePump;
    TSoftwareBgmState iBgm;
    TNativeChannelState iNativeChannels;
    TInt iAsyncError;
    TInt iMasterVolumePercent;
    TBool iEnabled;
};

TAudioRuntimeState gAudio = {
    NULL,
    NULL,
    {
        NULL, NULL,
        0, 0, 0,
        100, 100,
        0, 0,
        0, 0,
        0, 0, 100,
        EFalse, EFalse
    },
    {
        100, 0, 0, 100,
        100, 0, 0, 100,
        EFalse, EFalse
    },
    KErrNone,
    50,
    ETrue
};

const TInt KBgmRenderPumpFrames = 256;
const TInt KBgmLogicPumpFrames = 768;
const TInt KBgmIdlePumpFrames = 256;
const TInt KBgmInitialPumpFrames = 1024;
const TInt KBgmMinimumQueuedFrames = 1024;
const TInt KBgmTargetFrames = 3072;
const TInt KSoftwareBgmWaveMaxBytes = 768 * 1024;
const TInt KLowMemorySoftwareBgmWaveMaxBytes = 256 * 1024;
const TInt KCriticalMemorySoftwareBgmWaveMaxBytes = 128 * 1024;
const TInt KSoftwareSeWaveMaxBytes = 2 * 1024 * 1024;
const TInt KLowMemorySoftwareSeWaveMaxBytes = 512 * 1024;
const TInt KCriticalMemorySoftwareSeWaveMaxBytes = 256 * 1024;
const TInt KSoftwareMidiFileMaxBytes = 2 * 1024 * 1024;

_LIT(KWavExtension, ".wav");
_LIT(KMidExtension, ".mid");
_LIT(KMidiExtension, ".midi");

void StartAudioIdlePump();

void DecodeResourcePathL(const char* aPath, TDes& aDestination)
{
    if (!aPath || !aPath[0])
        User::Leave(KErrArgument);
    TInt length = 0;
    while (aPath[length])
        ++length;
    TPtrC8 source(reinterpret_cast<const TUint8*>(aPath), length);
    TRAPD(error, TEncoding::Utf8ToUnicodeL(source, aDestination));
    if (error == KErrNone)
        return;
    TEncoding::ShiftJisToUnicodeL(source, aDestination);
}

void EnsureMixerL()
{
    if (!gAudio.iMixer)
    {
        gAudio.iMixer = CAudioMixer::NewL();
        gAudio.iMixer->SetMasterVolume(gAudio.iMasterVolumePercent);
    }
    if (!gAudio.iIdlePump)
        TRAP_IGNORE(gAudio.iIdlePump = CIdle::NewL(CActive::EPriorityLow));
}

void UpdateBgmStep()
{
    if (gAudio.iBgm.iSampleRate <= 0 || gAudio.iBgm.iPitch <= 0)
    {
        gAudio.iBgm.iStep = 0;
        return;
    }
    gAudio.iBgm.iStep = (static_cast<TInt64>(gAudio.iBgm.iSampleRate) * gAudio.iBgm.iPitch << 16) /
               (CAudioMixer::OutputSampleRate() * 100);
    if (gAudio.iBgm.iStep <= 0)
        gAudio.iBgm.iStep = 1;
}

TBool HasExtension(const TDesC& aPath, const TDesC& aExtension)
{
    return aPath.Length() >= aExtension.Length() &&
           aPath.Right(aExtension.Length()).CompareF(aExtension) == 0;
}

TInt AdaptiveAudioReadLimit(TInt aNormalBytes, TInt aLowBytes, TInt aCriticalBytes)
{
    const TInt freeRam = EasyRpgPlatformFreeRamBytes();
    if (freeRam >= 0 && freeRam < EasyRpgPlayerConfig::KCriticalMemoryBytes)
        return aCriticalBytes;
    if (freeRam >= 0 && freeRam < EasyRpgPlayerConfig::KLowMemoryBytes)
        return aLowBytes;
    return aNormalBytes;
}

void BuildCacheKeyL(const TDesC& aPath, TInt aPitch, TDes& aKey)
{
    aKey.Zero();
    if (aPath.Length() + 12 > aKey.MaxLength())
        User::Leave(KErrOverflow);
    aKey.Copy(aPath);
    aKey.Append('#');
    aKey.AppendNum(aPitch);
}

void StopBgmNow()
{
    if (gAudio.iIdlePump)
        gAudio.iIdlePump->Cancel();
    if (gAudio.iMixer)
        gAudio.iMixer->ClearStream(CAudioMixer::EStreamBgm);
    TNativeAudio::StopBgm();
    EasyRpgMidiLiteDestroy(gAudio.iBgm.iMidiDecoder);
    gAudio.iBgm.iMidiDecoder = NULL;
    delete gAudio.iBgm.iPcm;
    gAudio.iBgm.iPcm = NULL;
    gAudio.iBgm.iFrameCount = 0;
    gAudio.iBgm.iSampleRate = 0;
    gAudio.iBgm.iChannels = 0;
    gAudio.iBgm.iPhase = 0;
    gAudio.iBgm.iStep = 0;
    gAudio.iBgm.iFramesQueued = 0;
    gAudio.iBgm.iLoopCount = 0;
    gAudio.iBgm.iFadeDurationMs = 0;
    gAudio.iBgm.iPlaying = EFalse;
    gAudio.iBgm.iPaused = EFalse;
}

TInt BgmSample(TInt aFrame, TInt aChannel)
{
    const TInt16* samples = reinterpret_cast<const TInt16*>(gAudio.iBgm.iPcm->Ptr());
    const TInt sourceChannel = gAudio.iBgm.iChannels == 1 ? 0 : aChannel;
    return samples[aFrame * gAudio.iBgm.iChannels + sourceChannel];
}

void PumpBgm(TInt aMaxFrames)
{
    if (!gAudio.iMixer || !gAudio.iBgm.iPlaying || gAudio.iBgm.iPaused || aMaxFrames <= 0)
        return;

    const TInt queuedFrames = gAudio.iMixer->StreamQueuedFrames(CAudioMixer::EStreamBgm);
    TInt freeFrames = gAudio.iMixer->StreamFreeFrames(CAudioMixer::EStreamBgm);
    TInt framesToWrite = Min(aMaxFrames, KBgmTargetFrames - queuedFrames);
    if (framesToWrite <= 0)
        return;
    freeFrames = Min(freeFrames, framesToWrite);

    if (gAudio.iBgm.iMidiDecoder)
    {
        while (freeFrames > 0)
        {
            const TInt batch = Min(freeFrames, 512);
            TInt16 output[512 * 2];
            const TInt rendered = EasyRpgMidiLiteRender(gAudio.iBgm.iMidiDecoder, output, batch);
            if (rendered <= 0)
            {
                gAudio.iAsyncError = KErrCorrupt;
                StopBgmNow();
                return;
            }
            const TInt written = gAudio.iMixer->WriteStreamPcm16(CAudioMixer::EStreamBgm,
                                                           output,
                                                           rendered);
            if (written <= 0)
                break;
            gAudio.iBgm.iFramesQueued += static_cast<TUint64>(written);
            gAudio.iBgm.iLoopCount = EasyRpgMidiLiteLoopCount(gAudio.iBgm.iMidiDecoder);
            freeFrames -= written;
            if (written != rendered || rendered != batch)
                break;
        }
        return;
    }

    if (!gAudio.iBgm.iPcm || gAudio.iBgm.iFrameCount <= 0 || gAudio.iBgm.iSampleRate <= 0)
        return;
    if (gAudio.iBgm.iStep <= 0)
        UpdateBgmStep();
    const TInt64 totalPhase = static_cast<TInt64>(gAudio.iBgm.iFrameCount) << 16;

    // Single resample path: at unity pitch the interpolation fraction is 0
    // and the lerp degenerates to an exact copy, so no fast path is needed.
    while (freeFrames > 0)
    {
        const TInt batch = Min(freeFrames, 512);
        TInt16 output[512 * 2];
        TInt frame = 0;
        for (frame = 0; frame < batch; ++frame)
        {
            TInt sourceFrame = static_cast<TInt>(gAudio.iBgm.iPhase >> 16);
            if (sourceFrame >= gAudio.iBgm.iFrameCount)
                sourceFrame = 0;
            TInt nextFrame = sourceFrame + 1;
            if (nextFrame >= gAudio.iBgm.iFrameCount)
                nextFrame = 0;
            const TInt fraction = static_cast<TInt>((gAudio.iBgm.iPhase & 0xffff) >> 1);

            TInt channel = 0;
            for (channel = 0; channel < 2; ++channel)
            {
                const TInt first = BgmSample(sourceFrame, channel);
                const TInt second = BgmSample(nextFrame, channel);
                output[frame * 2 + channel] = static_cast<TInt16>(
                    first + (((second - first) * fraction) >> 15));
            }

            gAudio.iBgm.iPhase += gAudio.iBgm.iStep;
            while (gAudio.iBgm.iPhase >= totalPhase)
            {
                gAudio.iBgm.iPhase -= totalPhase;
                ++gAudio.iBgm.iLoopCount;
            }
        }

        const TInt written = gAudio.iMixer->WriteStreamPcm16(CAudioMixer::EStreamBgm,
                                                       output,
                                                       batch);
        if (written <= 0)
            break;
        gAudio.iBgm.iFramesQueued += static_cast<TUint64>(written);
        freeFrames -= written;
        if (written != batch)
            break;
    }
}

TInt AudioIdlePump(TAny*)
{
    if (!gAudio.iMixer || !gAudio.iBgm.iPlaying || gAudio.iBgm.iPaused ||
        (!gAudio.iBgm.iMidiDecoder && !gAudio.iBgm.iPcm))
        return EFalse;

    PumpBgm(KBgmIdlePumpFrames);
    if (gAudio.iMixer)
        gAudio.iMixer->CommitStreams();

    if (!gAudio.iMixer || !gAudio.iBgm.iPlaying || gAudio.iBgm.iPaused ||
        (!gAudio.iBgm.iMidiDecoder && !gAudio.iBgm.iPcm))
        return EFalse;
    return gAudio.iMixer->StreamQueuedFrames(CAudioMixer::EStreamBgm) < KBgmTargetFrames &&
           gAudio.iMixer->StreamFreeFrames(CAudioMixer::EStreamBgm) > 0;
}

void StartAudioIdlePump()
{
    if (!gAudio.iIdlePump || gAudio.iIdlePump->IsActive() ||
        !gAudio.iMixer || !gAudio.iBgm.iPlaying || gAudio.iBgm.iPaused ||
        (!gAudio.iBgm.iMidiDecoder && !gAudio.iBgm.iPcm))
        return;
    gAudio.iIdlePump->Start(TCallBack(AudioIdlePump, NULL));
}

void PlayBgmL(const char* aPath, TInt aVolume, TInt aPitch, TInt aFadeIn)
{
    if (aPitch < 50 || aPitch > 200)
        User::Leave(KErrArgument);

    StopBgmNow();
    gAudio.iBgm.iPitch = aPitch;
    gAudio.iBgm.iVolume = aVolume;
    gAudio.iBgm.iFadeDurationMs = 0;

    TFileName path;
    DecodeResourcePathL(aPath, path);
    if (HasExtension(path, KMidExtension) || HasExtension(path, KMidiExtension))
    {
        EnsureMixerL();
        HBufC8* file = TNativeAudio::ReadAudioFileLC(path, KSoftwareMidiFileMaxBytes);
        gAudio.iBgm.iMidiDecoder = EasyRpgMidiLiteCreate(file->Ptr(),
                                                         file->Length(),
                                                         CAudioMixer::OutputSampleRate(),
                                                         aPitch);
        CleanupStack::PopAndDestroy(file);
        if (!gAudio.iBgm.iMidiDecoder)
            User::Leave(KErrCorrupt);
        gAudio.iBgm.iFramesQueued = 0;
        gAudio.iBgm.iLoopCount = 0;
        gAudio.iBgm.iPlaying = ETrue;
        gAudio.iBgm.iPaused = EFalse;
        gAudio.iMixer->SetStreamVolume(CAudioMixer::EStreamBgm, aVolume);
        PumpBgm(KBgmInitialPumpFrames);
        gAudio.iMixer->CommitStreams();
        StartAudioIdlePump();
        return;
    }
    if (!HasExtension(path, KWavExtension))
    {
        TNativeAudio::PlayBgmL(path, aVolume, aPitch, aFadeIn);
        return;
    }

    EnsureMixerL();
    HBufC8* file = NULL;
    TRAPD(readError,
        file = TNativeAudio::ReadAudioFileLC(path,
                                             AdaptiveAudioReadLimit(KSoftwareBgmWaveMaxBytes,
                                                                    KLowMemorySoftwareBgmWaveMaxBytes,
                                                                    KCriticalMemorySoftwareBgmWaveMaxBytes));
        CleanupStack::Pop(file);
    );
    if (readError == KErrTooBig)
    {
        TNativeAudio::PlayBgmL(path, aVolume, aPitch, aFadeIn);
        return;
    }
    User::LeaveIfError(readError);
    CleanupStack::PushL(file);
    TInt frameCount = 0;
    TInt sampleRate = 0;
    TInt channels = 0;
    HBufC8* pcm = TAudioDecode::DecodeWavePcm16LC(*file,
                                                  frameCount,
                                                  sampleRate,
                                                  channels);

    gAudio.iBgm.iPcm = pcm;
    CleanupStack::Pop(pcm);
    CleanupStack::PopAndDestroy(file);
    gAudio.iBgm.iFrameCount = frameCount;
    gAudio.iBgm.iSampleRate = sampleRate;
    gAudio.iBgm.iChannels = channels;
    gAudio.iBgm.iPhase = 0;
    UpdateBgmStep();
    gAudio.iBgm.iFramesQueued = 0;
    gAudio.iBgm.iLoopCount = 0;
    gAudio.iBgm.iPlaying = ETrue;
    gAudio.iBgm.iPaused = EFalse;
    gAudio.iMixer->SetStreamVolume(CAudioMixer::EStreamBgm, aVolume);
    PumpBgm(KBgmInitialPumpFrames);
    gAudio.iMixer->CommitStreams();
    StartAudioIdlePump();
}

void PlayBgsL(const char* aPath, TInt aVolume, TInt aPitch)
{
    if (aPitch < 50 || aPitch > 200)
        User::Leave(KErrArgument);
    TFileName path;
    DecodeResourcePathL(aPath, path);
    TNativeAudio::PlayBgsL(path, aVolume, aPitch);
    gAudio.iNativeChannels.iBgsVolume = aVolume;
    gAudio.iNativeChannels.iBgsFadeDurationMs = 0;
}

void PlayMeL(const char* aPath, TInt aVolume, TInt aPitch)
{
    if (aPitch < 50 || aPitch > 200)
        User::Leave(KErrArgument);
    const TBool resumeBgm = EasyRpgAudioIsBgmPlaying() != 0;
    TNativeAudio::StopMe();
    gAudio.iNativeChannels.iMeActive = EFalse;
    if (resumeBgm)
        EasyRpgAudioPauseBgm();

    TFileName path;
    TRAPD(error,
        DecodeResourcePathL(aPath, path);
        TNativeAudio::PlayMeL(path, aVolume, aPitch);
    );
    if (error != KErrNone)
    {
        if (resumeBgm)
            EasyRpgAudioResumeBgm();
        User::Leave(error);
    }
    gAudio.iNativeChannels.iMeVolume = aVolume;
    gAudio.iNativeChannels.iMeFadeDurationMs = 0;
    gAudio.iNativeChannels.iResumeBgmAfterMe = resumeBgm;
    gAudio.iNativeChannels.iMeActive = ETrue;
}

void CacheSeL(const TDesC& aPath, TInt aPitch, TBool aPinned)
{
    TBuf<160> cacheKey;
    BuildCacheKeyL(aPath, aPitch, cacheKey);
    if (gAudio.iMixer->IsCached(cacheKey))
    {
        if (aPinned)
            gAudio.iMixer->SetCachedPinned(cacheKey, ETrue);
        return;
    }

    HBufC8* file = TNativeAudio::ReadAudioFileLC(aPath,
                                                 AdaptiveAudioReadLimit(KSoftwareSeWaveMaxBytes,
                                                                        KLowMemorySoftwareSeWaveMaxBytes,
                                                                        KCriticalMemorySoftwareSeWaveMaxBytes));
    TInt frameCount = 0;
    TInt sampleRate = 0;
    TInt channels = 0;
    HBufC8* pcm = TAudioDecode::DecodeWavePcm16LC(*file,
                                                  frameCount,
                                                  sampleRate,
                                                  channels);
    CleanupStack::Pop(pcm);
    CleanupStack::PopAndDestroy(file);
    CleanupStack::PushL(pcm);

    const TInt64 adjustedRate64 = static_cast<TInt64>(sampleRate) * aPitch / 100;
    if (adjustedRate64 <= 0 || adjustedRate64 > KMaxTInt)
        User::Leave(KErrArgument);

    gAudio.iMixer->CachePcm16L(cacheKey,
                               reinterpret_cast<const TInt16*>(pcm->Ptr()),
                               frameCount,
                               static_cast<TInt>(adjustedRate64),
                               channels,
                               aPinned);

    CleanupStack::PopAndDestroy(pcm);
}

void PlaySeL(const char* aPath, TInt aVolume, TInt aPitch)
{
    if (aPitch < 50 || aPitch > 200)
        User::Leave(KErrArgument);

    TFileName path;
    DecodeResourcePathL(aPath, path);

    _LIT(KSoundBackslash, "Sound\\");
    _LIT(KSoundSlash, "Sound/");
    const TBool isRpg2kSound = HasExtension(path, KWavExtension) &&
        (path.FindF(KSoundBackslash) >= 0 || path.FindF(KSoundSlash) >= 0);
    if (isRpg2kSound)
    {
        EnsureMixerL();
        TBuf<160> cacheKey;
        BuildCacheKeyL(path, aPitch, cacheKey);
        if (!gAudio.iMixer->IsCached(cacheKey))
        {
            TRAPD(cacheError, CacheSeL(path, aPitch, EFalse));
            if (cacheError != KErrNone)
            {
                if (aPitch == 100 &&
                    (cacheError == KErrTooBig || cacheError == KErrNoMemory || cacheError == KErrInUse))
                {
                    TNativeAudio::PlaySeL(path, aVolume);
                    return;
                }
                User::Leave(cacheError);
            }
        }
        if (!gAudio.iMixer->PlaySe(cacheKey, aVolume))
            User::Leave(KErrNotReady);
        gAudio.iMixer->CommitStreams();
        return;
    }

    TNativeAudio::PlaySeL(path, aVolume);
}

TBool FadeVolumeStep(TUint32 aStartMs, TInt aDurationMs, TInt aStartVolume, TInt& aVolume)
{
    const TUint32 elapsed = TNativeAudio::NowMilliseconds() - aStartMs;
    if (elapsed >= static_cast<TUint32>(aDurationMs))
        return EFalse;
    aVolume = static_cast<TInt>(
        static_cast<TInt64>(aStartVolume) *
        (aDurationMs - static_cast<TInt>(elapsed)) /
        aDurationMs);
    return ETrue;
}
}

extern "C" void EasyRpgAudioSetEnabled(int aEnabled)
{
    const TBool enabled = aEnabled != 0;
    if (gAudio.iEnabled == enabled)
        return;
    if (!enabled)
        EasyRpgAudioShutdown();
    gAudio.iEnabled = enabled;
}

extern "C" int EasyRpgAudioIsEnabled(void)
{
    return gAudio.iEnabled ? 1 : 0;
}

extern "C" int EasyRpgAudioPlaySe(const char* aPath, int aVolume, int aPitch)
{
    if (!EasyRpgAudioIsEnabled())
        return KErrNone;
    TRAPD(error, PlaySeL(aPath, aVolume, aPitch));
    return error;
}

extern "C" void EasyRpgAudioStopSe(void)
{
    if (gAudio.iMixer)
        gAudio.iMixer->StopSe();
    TNativeAudio::StopSe();
}

extern "C" int EasyRpgAudioPlayBgm(const char* aPath, int aVolume, int aPitch, int aFadeIn)
{
    if (!EasyRpgAudioIsEnabled())
        return KErrNone;
    TRAPD(error, PlayBgmL(aPath, aVolume, aPitch, aFadeIn));
    return error;
}

extern "C" void EasyRpgAudioStopBgm(void)
{
    StopBgmNow();
}

extern "C" int EasyRpgAudioPlayBgs(const char* aPath, int aVolume, int aPitch)
{
    if (!EasyRpgAudioIsEnabled())
        return KErrNone;
    TRAPD(error, PlayBgsL(aPath, aVolume, aPitch));
    return error;
}

extern "C" void EasyRpgAudioStopBgs(void)
{
    TNativeAudio::StopBgs();
    gAudio.iNativeChannels.iBgsFadeDurationMs = 0;
}

extern "C" void EasyRpgAudioFadeBgs(int aFade)
{
    if (!TNativeAudio::IsBgsActive())
        return;
    if (aFade <= 0)
    {
        EasyRpgAudioStopBgs();
        return;
    }
    gAudio.iNativeChannels.iBgsFadeStartMs = TNativeAudio::NowMilliseconds();
    gAudio.iNativeChannels.iBgsFadeDurationMs = aFade;
    gAudio.iNativeChannels.iBgsFadeStartVolume = gAudio.iNativeChannels.iBgsVolume;
}

extern "C" int EasyRpgAudioPlayMe(const char* aPath, int aVolume, int aPitch)
{
    if (!EasyRpgAudioIsEnabled())
        return KErrNone;
    TRAPD(error, PlayMeL(aPath, aVolume, aPitch));
    return error;
}

extern "C" void EasyRpgAudioStopMe(void)
{
    TNativeAudio::StopMe();
    gAudio.iNativeChannels.iMeActive = EFalse;
    gAudio.iNativeChannels.iMeFadeDurationMs = 0;
    const TBool resume = gAudio.iNativeChannels.iResumeBgmAfterMe;
    gAudio.iNativeChannels.iResumeBgmAfterMe = EFalse;
    if (resume)
        EasyRpgAudioResumeBgm();
}

extern "C" void EasyRpgAudioFadeMe(int aFade)
{
    if (!gAudio.iNativeChannels.iMeActive || !TNativeAudio::IsMeActive())
        return;
    if (aFade <= 0)
    {
        EasyRpgAudioStopMe();
        return;
    }
    gAudio.iNativeChannels.iMeFadeStartMs = TNativeAudio::NowMilliseconds();
    gAudio.iNativeChannels.iMeFadeDurationMs = aFade;
    gAudio.iNativeChannels.iMeFadeStartVolume = gAudio.iNativeChannels.iMeVolume;
}

extern "C" void EasyRpgAudioPauseBgm(void)
{
    if (TNativeAudio::HasBgm())
    {
        TNativeAudio::PauseBgm();
        return;
    }
    if (!gAudio.iBgm.iPlaying || gAudio.iBgm.iPaused)
        return;
    gAudio.iBgm.iPaused = ETrue;
    if (gAudio.iIdlePump)
        gAudio.iIdlePump->Cancel();
    if (gAudio.iMixer)
        gAudio.iMixer->ClearStream(CAudioMixer::EStreamBgm);
}

extern "C" void EasyRpgAudioResumeBgm(void)
{
    if (TNativeAudio::HasBgm())
    {
        TNativeAudio::ResumeBgm();
        return;
    }
    if (!gAudio.iBgm.iPlaying || !gAudio.iBgm.iPaused)
        return;
    gAudio.iBgm.iPaused = EFalse;
    PumpBgm(KBgmInitialPumpFrames);
    if (gAudio.iMixer)
        gAudio.iMixer->CommitStreams();
    StartAudioIdlePump();
}

extern "C" void EasyRpgAudioSetBgmVolume(int aVolume)
{
    gAudio.iBgm.iVolume = aVolume;
    if (TNativeAudio::HasBgm())
        TNativeAudio::SetBgmVolume(aVolume);
    if (gAudio.iMixer)
        gAudio.iMixer->SetStreamVolume(CAudioMixer::EStreamBgm, aVolume);
}

extern "C" void EasyRpgAudioSetMasterVolume(int aVolume)
{
    const TInt clamped = TNativeAudio::ClampVolume(aVolume);
    gAudio.iMasterVolumePercent = clamped;
    if (gAudio.iMixer)
        gAudio.iMixer->SetMasterVolume(clamped);
    TNativeAudio::SetMasterVolume(clamped);
}

extern "C" void EasyRpgAudioSetBgmPitch(int aPitch)
{
    if (aPitch >= 50 && aPitch <= 200)
    {
        gAudio.iBgm.iPitch = aPitch;
        UpdateBgmStep();
        EasyRpgMidiLiteSetPitch(gAudio.iBgm.iMidiDecoder, aPitch);
    }
}

extern "C" void EasyRpgAudioFadeBgm(int aFade)
{
    if (!gAudio.iBgm.iPlaying && !TNativeAudio::HasBgm())
        return;
    if (aFade <= 0)
    {
        StopBgmNow();
        return;
    }
    gAudio.iBgm.iFadeStartMs = TNativeAudio::NowMilliseconds();
    gAudio.iBgm.iFadeDurationMs = aFade;
    gAudio.iBgm.iFadeStartVolume = gAudio.iBgm.iVolume;
}

extern "C" int EasyRpgAudioIsBgmPlaying(void)
{
    if (TNativeAudio::HasBgm())
        return TNativeAudio::IsBgmPlaying() ? 1 : 0;
    return gAudio.iBgm.iPlaying ? 1 : 0;
}

extern "C" int EasyRpgAudioBgmPlayedOnce(void)
{
    if (TNativeAudio::HasBgm())
        return TNativeAudio::BgmPlayedOnce() ? 1 : 0;
    if (gAudio.iBgm.iMidiDecoder)
        return EasyRpgMidiLiteLoopCount(gAudio.iBgm.iMidiDecoder) > 0 ? 1 : 0;
    return gAudio.iBgm.iLoopCount > 0 ? 1 : 0;
}

extern "C" unsigned int EasyRpgAudioBgmTicks(void)
{
    if (TNativeAudio::HasBgm())
        return TNativeAudio::BgmTicks();
    if (gAudio.iBgm.iMidiDecoder)
        return EasyRpgMidiLiteTicks(gAudio.iBgm.iMidiDecoder);
    if (!gAudio.iBgm.iPlaying)
        return 0;
    const TUint64 ticks = gAudio.iBgm.iFramesQueued * 1000u /
                          static_cast<TUint64>(CAudioMixer::OutputSampleRate());
    return ticks > 0xffffffffu ? 0xffffffffu : static_cast<unsigned int>(ticks);
}

extern "C" void EasyRpgAudioUpdate(void)
{
    if (!EasyRpgAudioIsEnabled())
        return;
    if (gAudio.iMixer)
    {
        const TInt mixerError = gAudio.iMixer->ConsumeError();
        if (mixerError != KErrNone)
            gAudio.iAsyncError = mixerError;
    }
    if (gAudio.iBgm.iFadeDurationMs > 0 &&
        (gAudio.iBgm.iPlaying || TNativeAudio::HasBgm()))
    {
        TInt volume = 0;
        if (!FadeVolumeStep(gAudio.iBgm.iFadeStartMs,
                            gAudio.iBgm.iFadeDurationMs,
                            gAudio.iBgm.iFadeStartVolume,
                            volume))
        {
            StopBgmNow();
        }
        else
        {
            if (TNativeAudio::HasBgm())
                TNativeAudio::SetBgmVolumeSmoothed(volume);
            if (gAudio.iMixer)
                gAudio.iMixer->SetStreamVolume(CAudioMixer::EStreamBgm, volume);
        }
    }
    if (gAudio.iNativeChannels.iBgsFadeDurationMs > 0 && TNativeAudio::IsBgsActive())
    {
        TInt volume = 0;
        if (!FadeVolumeStep(gAudio.iNativeChannels.iBgsFadeStartMs,
                            gAudio.iNativeChannels.iBgsFadeDurationMs,
                            gAudio.iNativeChannels.iBgsFadeStartVolume,
                            volume))
            EasyRpgAudioStopBgs();
        else
            TNativeAudio::SetBgsVolumeSmoothed(volume);
    }
    if (gAudio.iNativeChannels.iMeActive)
    {
        if (gAudio.iNativeChannels.iMeFadeDurationMs > 0 && TNativeAudio::IsMeActive())
        {
            TInt volume = 0;
            if (!FadeVolumeStep(gAudio.iNativeChannels.iMeFadeStartMs,
                                gAudio.iNativeChannels.iMeFadeDurationMs,
                                gAudio.iNativeChannels.iMeFadeStartVolume,
                                volume))
                EasyRpgAudioStopMe();
            else
                TNativeAudio::SetMeVolumeSmoothed(volume);
        }
        if (gAudio.iNativeChannels.iMeActive && !TNativeAudio::IsMeActive())
        {
            gAudio.iNativeChannels.iMeActive = EFalse;
            gAudio.iNativeChannels.iMeFadeDurationMs = 0;
            const TBool resume = gAudio.iNativeChannels.iResumeBgmAfterMe;
            gAudio.iNativeChannels.iResumeBgmAfterMe = EFalse;
            if (resume)
                EasyRpgAudioResumeBgm();
        }
    }
    if (gAudio.iMixer && gAudio.iBgm.iPlaying && !gAudio.iBgm.iPaused &&
        (gAudio.iBgm.iMidiDecoder || gAudio.iBgm.iPcm))
    {
        if (gAudio.iIdlePump)
        {
            const TInt queuedFrames = gAudio.iMixer->StreamQueuedFrames(CAudioMixer::EStreamBgm);
            if (queuedFrames < KBgmMinimumQueuedFrames)
            {
                PumpBgm(KBgmMinimumQueuedFrames - queuedFrames);
                gAudio.iMixer->CommitStreams();
            }
            StartAudioIdlePump();
        }
        else
        {
#ifdef EASYRPG_SCRIPT_PROCESS
            PumpBgm(KBgmLogicPumpFrames);
#else
            PumpBgm(EasyRpgUpstreamPlayerShouldRender() ? KBgmRenderPumpFrames : KBgmLogicPumpFrames);
#endif
            gAudio.iMixer->CommitStreams();
        }
    }
}

extern "C" int EasyRpgAudioConsumeError(void)
{
    if (!EasyRpgAudioIsEnabled())
        return KErrNone;
    if (gAudio.iAsyncError != KErrNone)
    {
        const TInt error = gAudio.iAsyncError;
        gAudio.iAsyncError = KErrNone;
        return error;
    }
    return TNativeAudio::ConsumeError();
}

extern "C" void EasyRpgAudioSetMemoryPressure(int aLevel)
{
    if (aLevel <= 0)
    {
        if (gAudio.iMixer)
            gAudio.iMixer->TrimCache(4 * 1024 * 1024);
        return;
    }

    if (gAudio.iMixer)
        gAudio.iMixer->TrimCache(aLevel >= 2 ? 512 * 1024 : 2 * 1024 * 1024);
}

extern "C" void EasyRpgAudioShutdown(void)
{
    StopBgmNow();
    TNativeAudio::Shutdown();
    gAudio.iNativeChannels.iBgsFadeDurationMs = 0;
    gAudio.iNativeChannels.iMeFadeDurationMs = 0;
    gAudio.iNativeChannels.iMeActive = EFalse;
    gAudio.iNativeChannels.iResumeBgmAfterMe = EFalse;
    delete gAudio.iIdlePump;
    gAudio.iIdlePump = NULL;
    delete gAudio.iMixer;
    gAudio.iMixer = NULL;
}

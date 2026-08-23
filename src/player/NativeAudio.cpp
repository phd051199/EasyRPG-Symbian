#include <e32base.h>
#include <mda/common/resource.h>
#include <mdaaudiosampleplayer.h>

#include "NativeAudio.h"
#include "DirentBridge.h"

namespace TNativeAudio
{

HBufC8* ReadAudioFileLC(const TDesC& aPath, TInt aMaxBytes)
{
    if (aMaxBytes <= 0)
        User::Leave(KErrArgument);
    if (EasyRpgIsVirtualPath(aPath))
        return EasyRpgReadVirtualFileLC(aPath, aMaxBytes);

    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);

    RFile file;
    User::LeaveIfError(file.Open(fs, aPath, EFileRead | EFileShareReadersOnly));
    CleanupClosePushL(file);

    TInt size = 0;
    User::LeaveIfError(file.Size(size));
    if (size <= 0 || size > aMaxBytes)
        User::Leave(KErrTooBig);

    HBufC8* data = HBufC8::NewLC(size);
    TPtr8 bytes = data->Des();
    User::LeaveIfError(file.Read(bytes));
    if (bytes.Length() != size)
        User::Leave(KErrEof);

    CleanupStack::Pop(data);
    CleanupStack::PopAndDestroy(&file);
    CleanupStack::PopAndDestroy(&fs);
    CleanupStack::PushL(data);
    return data;
}

}

namespace
{
const TInt KNativeSeVoiceCount = 4;
const TInt KNativeAudioMaxFileBytes = 8 * 1024 * 1024;

class CNativeBgmPlayer;
class CNativeSeVoice;

struct TNativeAudioState
{
    CNativeBgmPlayer* iBgm;
    CNativeBgmPlayer* iBgs;
    CNativeBgmPlayer* iMe;
    CNativeSeVoice* iSeVoices[KNativeSeVoiceCount];
    TInt iSeCursor;
    TInt iAsyncError;
    TInt iMasterVolumePercent;
};

TNativeAudioState gState = {NULL, NULL, NULL, {NULL, NULL, NULL, NULL}, 0, KErrNone, 50};

void SetAsyncError(TInt aError)
{
    if (aError != KErrNone)
        gState.iAsyncError = aError;
}

class CNativeBgmPlayer : public CBase,
                         public MMdaAudioPlayerCallback
{
public:
    static CNativeBgmPlayer* NewL()
    {
        return new (ELeave) CNativeBgmPlayer;
    }

    ~CNativeBgmPlayer()
    {
        Stop();
        delete iAudio;
        iAudio = NULL;
    }

    void PlaySampledFileL(const TDesC& aPath, TInt aVolume, TInt aFadeIn, TBool aRepeat = ETrue)
    {
        if (aPath.Length() == 0)
            User::Leave(KErrArgument);
        Stop();
        if (!iAudio)
            iAudio = CMdaAudioPlayerUtility::NewL(*this,
                                                  EMdaPriorityNormal,
                                                  EMdaPriorityPreferenceTime);
        iActive = ETrue;
        iVolume = TNativeAudio::ClampVolume(aVolume);
        iFadeIn = aFadeIn;
        iRepeat = aRepeat;
        iLastPosition = 0;
        iLoopCount = 0;
        iPending = ETrue;
        TRAPD(error, iAudio->OpenFileL(aPath));
        if (error != KErrNone)
        {
            Stop();
            User::Leave(error);
        }
    }

    void PlaySampledDesL(HBufC8* aData, TInt aVolume, TInt aFadeIn, TBool aRepeat = ETrue)
    {
        if (!aData || aData->Length() == 0)
        {
            delete aData;
            User::Leave(KErrArgument);
        }
        Stop();
        if (!iAudio)
            iAudio = CMdaAudioPlayerUtility::NewL(*this,
                                                  EMdaPriorityNormal,
                                                  EMdaPriorityPreferenceTime);
        iSource = aData;
        iActive = ETrue;
        iVolume = TNativeAudio::ClampVolume(aVolume);
        iFadeIn = aFadeIn;
        iRepeat = aRepeat;
        iLastPosition = 0;
        iLoopCount = 0;
        iPending = ETrue;
        TRAPD(error, iAudio->OpenDesL(*iSource));
        if (error != KErrNone)
        {
            Stop();
            User::Leave(error);
        }
    }

    void Stop()
    {
        if (iAudio && iActive)
            iAudio->Stop();
        ResetPlaybackState(ETrue);
    }

    void Pause()
    {
        if (!iPlaying || iPaused || !iActive || !iAudio)
            return;
        if (iAudio->Pause() == KErrNone)
            iPaused = ETrue;
    }

    void Resume()
    {
        if (!iPlaying || !iPaused || !iActive || !iAudio)
            return;
        iAudio->Play();
        iPaused = EFalse;
    }

    void SetVolume(TInt aVolume)
    {
        iVolume = TNativeAudio::ClampVolume(aVolume);
        iVolumePushValid = EFalse;
        if (!iActive || !iAudio)
            return;
        const TInt maxVolume = iAudio->MaxVolume();
        SetAsyncError(iAudio->SetVolume(maxVolume * iVolume * gState.iMasterVolumePercent / 10000));
    }

    void SetVolumeSmoothed(TInt aVolume)
    {
        iVolume = TNativeAudio::ClampVolume(aVolume);
        if (!iActive || !iAudio)
        {
            iVolumePushValid = EFalse;
            return;
        }

        const TUint32 now = TNativeAudio::NowMilliseconds();
        const TInt delta = iVolume - iLastPushedVolume;
        if (!iVolumePushValid || iVolume == 0 || iVolume >= 100 ||
            delta >= 2 || delta <= -2 || now - iLastVolumePushMs >= 100)
        {
            const TInt maxVolume = iAudio->MaxVolume();
            SetAsyncError(iAudio->SetVolume(maxVolume * iVolume * gState.iMasterVolumePercent / 10000));
            iLastPushedVolume = iVolume;
            iLastVolumePushMs = now;
            iVolumePushValid = ETrue;
        }
    }

    void RefreshVolume()
    {
        if (iActive && (iPlaying || iPaused))
            SetVolume(iVolume);
    }

    TBool IsPlaying() const
    {
        return iPlaying;
    }

    TBool IsActive() const
    {
        return iPending || iPlaying;
    }

    TBool HasSample() const
    {
        return iActive;
    }

    TBool PlayedOnce() const
    {
        Ticks();
        return iLoopCount > 0;
    }

    TUint32 Ticks() const
    {
        TInt64 microseconds = 0;
        if (iActive && iAudio)
        {
            TTimeIntervalMicroSeconds position;
            if (iAudio->GetPosition(position) != KErrNone)
                return 0;
            microseconds = position.Int64();
        }
        if (microseconds <= 0)
            return 0;
        if (!iPaused && iLastPosition > 0 && microseconds < iLastPosition)
            ++iLoopCount;
        iLastPosition = microseconds;
        const TUint64 milliseconds = static_cast<TUint64>(microseconds) / 1000u;
        return milliseconds > 0xffffffffu ? 0xffffffffu : static_cast<TUint32>(milliseconds);
    }

private:
    CNativeBgmPlayer()
        : iAudio(NULL),
          iSource(NULL),
          iActive(EFalse),
          iVolume(100),
          iFadeIn(0),
          iRepeat(ETrue),
          iLastPosition(0),
          iLoopCount(0),
          iPending(EFalse),
          iPlaying(EFalse),
          iPaused(EFalse),
          iLastPushedVolume(100),
          iLastVolumePushMs(0),
          iVolumePushValid(EFalse)
    {
    }

    void ResetPlaybackState(TBool aCloseAudio)
    {
        if (aCloseAudio && iAudio)
            iAudio->Close();
        iActive = EFalse;
        iPending = EFalse;
        iPlaying = EFalse;
        iPaused = EFalse;
        iLastPosition = 0;
        iLoopCount = 0;
        iVolumePushValid = EFalse;
        delete iSource;
        iSource = NULL;
    }

    void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration)
    {
        if (!iActive || !iPending)
            return;
        iPending = EFalse;
        if (aError != KErrNone)
        {
            SetAsyncError(aError);
            ResetPlaybackState(ETrue);
            return;
        }
        (void)aDuration;
        if (iRepeat)
            iAudio->SetRepeats(KMdaRepeatForever, TTimeIntervalMicroSeconds(0));
        SetVolume(iVolume);
        if (iFadeIn > 0)
            iAudio->SetVolumeRamp(TTimeIntervalMicroSeconds(static_cast<TInt64>(iFadeIn) * 1000));
        iAudio->Play();
        iPlaying = ETrue;
    }

    void MapcPlayComplete(TInt aError)
    {
        if (!iActive)
            return;
        if (aError != KErrNone && aError != KErrAbort)
            SetAsyncError(aError);
        if (aError != KErrNone || !iRepeat)
            ResetPlaybackState(ETrue);
    }

private:
    CMdaAudioPlayerUtility* iAudio;
    HBufC8* iSource;
    TBool iActive;
    TInt iVolume;
    TInt iFadeIn;
    TBool iRepeat;
    mutable TInt64 iLastPosition;
    mutable TUint32 iLoopCount;
    TBool iPending;
    TBool iPlaying;
    TBool iPaused;
    TInt iLastPushedVolume;
    TUint32 iLastVolumePushMs;
    TBool iVolumePushValid;
};

class CNativeSeVoice : public CBase,
                       public MMdaAudioPlayerCallback
{
public:
    static CNativeSeVoice* NewL()
    {
        CNativeSeVoice* self = new (ELeave) CNativeSeVoice;
        CleanupStack::PushL(self);
        self->iAudio = CMdaAudioPlayerUtility::NewL(*self,
                                                    EMdaPriorityNormal,
                                                    EMdaPriorityPreferenceTime);
        CleanupStack::Pop(self);
        return self;
    }

    ~CNativeSeVoice()
    {
        Stop();
        delete iAudio;
        iAudio = NULL;
    }

    void PlayL(HBufC8* aSource, TInt aVolume)
    {
        if (!aSource)
            User::Leave(KErrArgument);
        Stop();
        iSource = aSource;
        iVolume = TNativeAudio::ClampVolume(aVolume);
        iPending = ETrue;
        TRAPD(error, iAudio->OpenDesL(*iSource));
        if (error != KErrNone)
        {
            Stop();
            User::Leave(error);
        }
    }

    void Stop()
    {
        if (iAudio)
        {
            iAudio->Stop();
            iAudio->Close();
        }
        iPending = EFalse;
        iPlaying = EFalse;
        ReleaseSource();
    }

    TBool IsBusy() const
    {
        return iPending || iPlaying;
    }

    void RefreshVolume()
    {
        if (!iAudio || !iPlaying)
            return;
        const TInt maxVolume = iAudio->MaxVolume();
        SetAsyncError(iAudio->SetVolume(maxVolume * iVolume * gState.iMasterVolumePercent / 10000));
    }

private:
    CNativeSeVoice()
        : iAudio(NULL),
          iSource(NULL),
          iVolume(100),
          iPending(EFalse),
          iPlaying(EFalse)
    {
    }

    void ReleaseSource()
    {
        delete iSource;
        iSource = NULL;
    }

    void FinishPlayback()
    {
        if (iAudio)
            iAudio->Close();
        iPending = EFalse;
        iPlaying = EFalse;
        ReleaseSource();
    }

    void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds&)
    {
        if (!iPending)
            return;
        iPending = EFalse;
        if (aError != KErrNone)
        {
            SetAsyncError(aError);
            FinishPlayback();
            return;
        }
        const TInt maxVolume = iAudio->MaxVolume();
        const TInt volumeError = iAudio->SetVolume(maxVolume * iVolume * gState.iMasterVolumePercent / 10000);
        if (volumeError != KErrNone)
        {
            SetAsyncError(volumeError);
            FinishPlayback();
            return;
        }
        iAudio->Play();
        iPlaying = ETrue;
    }

    void MapcPlayComplete(TInt aError)
    {
        if (aError != KErrNone && aError != KErrAbort)
            SetAsyncError(aError);
        FinishPlayback();
    }

private:
    CMdaAudioPlayerUtility* iAudio;
    HBufC8* iSource;
    TInt iVolume;
    TBool iPending;
    TBool iPlaying;
};

void EnsureBgmL()
{
    if (!gState.iBgm)
        gState.iBgm = CNativeBgmPlayer::NewL();
}

void EnsureBgsL()
{
    if (!gState.iBgs)
        gState.iBgs = CNativeBgmPlayer::NewL();
}

void EnsureMeL()
{
    if (!gState.iMe)
        gState.iMe = CNativeBgmPlayer::NewL();
}

void PlaySampledL(CNativeBgmPlayer& aPlayer,
                  const TDesC& aPath,
                  TInt aVolume,
                  TInt aFadeIn,
                  TBool aRepeat)
{
    if (!EasyRpgIsVirtualPath(aPath))
    {
        aPlayer.PlaySampledFileL(aPath, aVolume, aFadeIn, aRepeat);
        return;
    }

    aPlayer.Stop();
    HBufC8* data = TNativeAudio::ReadAudioFileLC(aPath, KNativeAudioMaxFileBytes);
    CleanupStack::Pop(data);
    aPlayer.PlaySampledDesL(data, aVolume, aFadeIn, aRepeat);
}
}

namespace TNativeAudio
{
void PlayBgmL(const TDesC& aPath, TInt aVolume, TInt aPitch, TInt aFadeIn)
{
    EnsureBgmL();
    PlaySampledL(*gState.iBgm, aPath, aVolume, aFadeIn, ETrue);
}

void StopBgm()
{
    if (gState.iBgm)
        gState.iBgm->Stop();
}

void PauseBgm()
{
    if (gState.iBgm)
        gState.iBgm->Pause();
}

void ResumeBgm()
{
    if (gState.iBgm)
        gState.iBgm->Resume();
}

void SetBgmVolume(TInt aVolume)
{
    if (gState.iBgm)
        gState.iBgm->SetVolume(aVolume);
}

void SetBgmVolumeSmoothed(TInt aVolume)
{
    if (gState.iBgm)
        gState.iBgm->SetVolumeSmoothed(aVolume);
}

TBool HasBgm()
{
    return gState.iBgm && gState.iBgm->HasSample();
}

TBool IsBgmPlaying()
{
    return HasBgm() && gState.iBgm->IsPlaying();
}

TBool BgmPlayedOnce()
{
    return HasBgm() && gState.iBgm->PlayedOnce();
}

TUint32 BgmTicks()
{
    return HasBgm() ? gState.iBgm->Ticks() : 0;
}

void PlayBgsL(const TDesC& aPath, TInt aVolume, TInt aPitch)
{
    EnsureBgsL();
    PlaySampledL(*gState.iBgs, aPath, aVolume, 0, ETrue);
}

void StopBgs()
{
    if (gState.iBgs)
        gState.iBgs->Stop();
}

void SetBgsVolumeSmoothed(TInt aVolume)
{
    if (gState.iBgs)
        gState.iBgs->SetVolumeSmoothed(aVolume);
}

TBool IsBgsActive()
{
    return gState.iBgs && gState.iBgs->IsActive();
}

void PlayMeL(const TDesC& aPath, TInt aVolume, TInt aPitch)
{
    EnsureMeL();
    PlaySampledL(*gState.iMe, aPath, aVolume, 0, EFalse);
}

void StopMe()
{
    if (gState.iMe)
        gState.iMe->Stop();
}

void SetMeVolumeSmoothed(TInt aVolume)
{
    if (gState.iMe)
        gState.iMe->SetVolumeSmoothed(aVolume);
}

TBool IsMeActive()
{
    return gState.iMe && gState.iMe->IsActive();
}

void PlaySeL(const TDesC& aPath, TInt aVolume)
{
    TInt selected = -1;
    for (TInt index = 0; index < KNativeSeVoiceCount; ++index)
    {
        if (!gState.iSeVoices[index] || !gState.iSeVoices[index]->IsBusy())
        {
            selected = index;
            break;
        }
    }
    if (selected < 0)
        selected = gState.iSeCursor;
    if (!gState.iSeVoices[selected])
        gState.iSeVoices[selected] = CNativeSeVoice::NewL();

    HBufC8* data = ReadAudioFileLC(aPath, KNativeAudioMaxFileBytes);
    CleanupStack::Pop(data);
    TRAPD(error, gState.iSeVoices[selected]->PlayL(data, aVolume));
    User::LeaveIfError(error);
    gState.iSeCursor = (selected + 1) % KNativeSeVoiceCount;
}

void StopSe()
{
    for (TInt index = 0; index < KNativeSeVoiceCount; ++index)
        if (gState.iSeVoices[index])
            gState.iSeVoices[index]->Stop();
}

void SetMasterVolume(TInt aVolume)
{
    const TInt clamped = ClampVolume(aVolume);
    gState.iMasterVolumePercent = clamped;

    if (gState.iBgm)
        gState.iBgm->RefreshVolume();
    if (gState.iBgs)
        gState.iBgs->RefreshVolume();
    if (gState.iMe)
        gState.iMe->RefreshVolume();
    for (TInt index = 0; index < KNativeSeVoiceCount; ++index)
        if (gState.iSeVoices[index])
            gState.iSeVoices[index]->RefreshVolume();
}

TInt ConsumeError()
{
    const TInt error = gState.iAsyncError;
    gState.iAsyncError = KErrNone;
    return error;
}

void Shutdown()
{
    StopBgm();
    StopBgs();
    StopMe();
    StopSe();
    for (TInt index = 0; index < KNativeSeVoiceCount; ++index)
    {
        delete gState.iSeVoices[index];
        gState.iSeVoices[index] = NULL;
    }
    delete gState.iBgm;
    gState.iBgm = NULL;
    delete gState.iBgs;
    gState.iBgs = NULL;
    delete gState.iMe;
    gState.iMe = NULL;
    gState.iSeCursor = 0;
    gState.iAsyncError = KErrNone;
}
}

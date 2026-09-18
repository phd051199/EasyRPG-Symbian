#ifndef EASYRPG_NATIVE_AUDIO_H
#define EASYRPG_NATIVE_AUDIO_H

#include <e32base.h>
#include <e32hal.h>
#include <f32file.h>
#include "DirentBridge.h"

namespace TNativeAudio
{
inline TUint32 NowMilliseconds()
{
    static TInt tickMicroseconds = 0;
    if (tickMicroseconds <= 0)
    {
        TTimeIntervalMicroSeconds32 period;
        if (UserHal::TickPeriod(period) == KErrNone && period.Int() > 0)
            tickMicroseconds = period.Int();
        else
            tickMicroseconds = 1000;
    }
    return static_cast<TUint32>(
        (static_cast<TUint64>(User::TickCount()) * static_cast<TUint64>(tickMicroseconds)) / 1000u);
}

inline TInt ClampVolume(TInt aVolume)
{
    if (aVolume < 0)
        return 0;
    if (aVolume > 100)
        return 100;
    return aVolume;
}

HBufC8 *ReadAudioFileLC(const TDesC &aPath, TInt aMaxBytes);

void PlayBgmL(const TDesC &aPath, TInt aVolume, TInt aPitch, TInt aFadeIn);
void StopBgm();
void PauseBgm();
void ResumeBgm();
void SetBgmVolume(TInt aVolume);
void SetBgmVolumeSmoothed(TInt aVolume);
TBool HasBgm();
TBool IsBgmPlaying();
TBool BgmPlayedOnce();
TUint32 BgmTicks();

void PlayBgsL(const TDesC &aPath, TInt aVolume, TInt aPitch);
void StopBgs();
void SetBgsVolumeSmoothed(TInt aVolume);
TBool IsBgsActive();

void PlayMeL(const TDesC &aPath, TInt aVolume, TInt aPitch);
void StopMe();
void SetMeVolumeSmoothed(TInt aVolume);
TBool IsMeActive();

void PlaySeL(const TDesC &aPath, TInt aVolume);
void StopSe();

void SetMasterVolume(TInt aVolume);
TInt ConsumeError();
void Shutdown();
} // namespace TNativeAudio

#endif

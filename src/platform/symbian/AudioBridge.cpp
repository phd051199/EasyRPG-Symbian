#include <e32base.h>
#include <e32hal.h>
#include <f32file.h>
#include <mdaaudiosampleplayer.h>

#include "AudioBridge.h"
#include "AudioMixer.h"

namespace
{
CAudioMixer* gMixer = NULL;
HBufC8* gBgmPcm = NULL;
TInt gBgmFrameCount = 0;
TInt gBgmSampleRate = 0;
TInt gBgmChannels = 0;
TInt gBgmPitch = 100;
TInt gBgmVolume = 100;
TInt64 gBgmPhase = 0;
TInt64 gBgmStep = 0;
TUint64 gBgmFramesQueued = 0;
TUint32 gBgmStartMs = 0;
TUint32 gFadeStartMs = 0;
TInt gFadeDurationMs = 0;
TInt gFadeStartVolume = 100;
TBool gBgmPlaying = EFalse;
TBool gBgmPaused = EFalse;
TInt gAudioAsyncError = KErrNone;
const TInt KMasterVolumePercent = 20;

TUint16 Read16(const TUint8* aData)
{
    return static_cast<TUint16>(aData[0]) |
           static_cast<TUint16>(static_cast<TUint16>(aData[1]) << 8);
}

TUint32 Read32(const TUint8* aData)
{
    return static_cast<TUint32>(aData[0]) |
           (static_cast<TUint32>(aData[1]) << 8) |
           (static_cast<TUint32>(aData[2]) << 16) |
           (static_cast<TUint32>(aData[3]) << 24);
}

void AsciiPathL(const char* aPath, TDes& aDestination)
{
    if (!aPath || !aPath[0])
        User::Leave(KErrArgument);
    aDestination.Zero();
    TInt index = 0;
    while (aPath[index])
    {
        if (aDestination.Length() >= aDestination.MaxLength())
            User::Leave(KErrOverflow);
        const TUint8 value = static_cast<TUint8>(aPath[index]);
        if (value > 0x7fu)
            User::Leave(KErrNotSupported);
        aDestination.Append(static_cast<TUint16>(value));
        ++index;
    }
}

void EnsureMixerL()
{
    if (!gMixer)
        gMixer = CAudioMixer::NewL();
}

void UpdateBgmStep()
{
    if (gBgmSampleRate <= 0 || gBgmPitch <= 0)
    {
        gBgmStep = 0;
        return;
    }
    gBgmStep = (static_cast<TInt64>(gBgmSampleRate) * gBgmPitch << 16) /
               (CAudioMixer::OutputSampleRate() * 100);
    if (gBgmStep <= 0)
        gBgmStep = 1;
}

class CNativeBgmPlayer : public CBase,
                         public MMdaAudioPlayerCallback
{
public:
    enum TType
    {
        ENone,
        ESampled
    };

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

    void PlaySampledL(const TDesC& aPath, TInt aVolume, TInt aPitch, TInt aFadeIn)
    {
        Stop();
        if (!iAudio)
            iAudio = CMdaAudioPlayerUtility::NewL(*this,
                                                  EMdaPriorityNormal,
                                                  EMdaPriorityPreferenceTime);
        iType = ESampled;
        iVolume = ClampVolume(aVolume);
        iPitch = aPitch;
        iFadeIn = aFadeIn;
        iPending = ETrue;
        iAudio->OpenFileL(aPath);
    }

    void Stop()
    {
        if (iAudio && iType == ESampled)
        {
            iAudio->Stop();
            iAudio->Close();
        }
        iType = ENone;
        iPending = EFalse;
        iPlaying = EFalse;
        iPaused = EFalse;
    }

    void Pause()
    {
        if (!iPlaying || iPaused)
            return;
        if (iAudio && iAudio->Pause() == KErrNone)
            iPaused = ETrue;
    }

    void Resume()
    {
        if (!iPlaying || !iPaused)
            return;
        if (iAudio)
        {
            iAudio->Play();
            iPaused = EFalse;
        }
    }

    void SetVolume(TInt aVolume)
    {
        iVolume = ClampVolume(aVolume);
        if (iAudio)
        {
            const TInt maxVolume = iAudio->MaxVolume();
            const TInt error = iAudio->SetVolume(maxVolume * iVolume * KMasterVolumePercent / 10000);
            if (error != KErrNone)
                gAudioAsyncError = error;
        }
    }

    void SetPitch(TInt aPitch)
    {
        if (aPitch < 50 || aPitch > 200)
            return;
        iPitch = aPitch;
    }

    TBool IsPlaying() const
    {
        return iPlaying;
    }

    TUint32 Ticks() const
    {
        TInt64 microseconds = 0;
        if (iAudio)
        {
            TTimeIntervalMicroSeconds position;
            if (iAudio->GetPosition(position) != KErrNone)
                return 0;
            microseconds = position.Int64();
        }
        if (microseconds <= 0)
            return 0;
        const TUint64 milliseconds = static_cast<TUint64>(microseconds) / 1000u;
        return milliseconds > 0xffffffffu ? 0xffffffffu : static_cast<TUint32>(milliseconds);
    }

    TType Type() const
    {
        return iType;
    }

private:
    CNativeBgmPlayer()
        : iAudio(NULL),
          iType(ENone),
          iVolume(100),
          iPitch(100),
          iFadeIn(0),
          iPending(EFalse),
          iPlaying(EFalse),
          iPaused(EFalse)
    {
    }

    static TInt ClampVolume(TInt aVolume)
    {
        if (aVolume < 0)
            return 0;
        if (aVolume > 100)
            return 100;
        return aVolume;
    }

    void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds&)
    {
        if (iType != ESampled || !iPending)
            return;
        iPending = EFalse;
        if (aError != KErrNone)
        {
            gAudioAsyncError = aError;
            iPlaying = EFalse;
            return;
        }
        iAudio->SetRepeats(KMdaRepeatForever, TTimeIntervalMicroSeconds(0));
        SetVolume(iVolume);
        if (iFadeIn > 0)
            iAudio->SetVolumeRamp(TTimeIntervalMicroSeconds(static_cast<TInt64>(iFadeIn) * 1000));
        iAudio->Play();
        iPlaying = ETrue;
    }

    void MapcPlayComplete(TInt aError)
    {
        if (aError != KErrNone && aError != KErrAbort)
            gAudioAsyncError = aError;
        if (aError != KErrNone)
            iPlaying = EFalse;
    }

private:
    CMdaAudioPlayerUtility* iAudio;
    TType iType;
    TInt iVolume;
    TInt iPitch;
    TInt iFadeIn;
    TBool iPending;
    TBool iPlaying;
    TBool iPaused;
};

const TInt KNativeSeVoiceCount = 4;

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

    void PlayL(const TDesC& aPath, TInt aVolume)
    {
        Stop();
        iVolume = ClampVolume(aVolume);
        iPending = ETrue;
        iAudio->OpenFileL(aPath);
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
    }

    TBool IsBusy() const
    {
        return iPending || iPlaying;
    }

private:
    CNativeSeVoice()
        : iAudio(NULL),
          iVolume(100),
          iPending(EFalse),
          iPlaying(EFalse)
    {
    }

    static TInt ClampVolume(TInt aVolume)
    {
        if (aVolume < 0)
            return 0;
        if (aVolume > 100)
            return 100;
        return aVolume;
    }

    void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds&)
    {
        if (!iPending)
            return;
        iPending = EFalse;
        if (aError != KErrNone)
        {
            gAudioAsyncError = aError;
            return;
        }
        const TInt maxVolume = iAudio->MaxVolume();
        const TInt volumeError = iAudio->SetVolume(maxVolume * iVolume * KMasterVolumePercent / 10000);
        if (volumeError != KErrNone)
        {
            gAudioAsyncError = volumeError;
            return;
        }
        iAudio->Play();
        iPlaying = ETrue;
    }

    void MapcPlayComplete(TInt aError)
    {
        if (aError != KErrNone && aError != KErrAbort)
            gAudioAsyncError = aError;
        iPlaying = EFalse;
    }

private:
    CMdaAudioPlayerUtility* iAudio;
    TInt iVolume;
    TBool iPending;
    TBool iPlaying;
};

CNativeBgmPlayer* gNativeBgm = NULL;
CNativeSeVoice* gNativeSeVoices[KNativeSeVoiceCount] = {NULL, NULL, NULL, NULL};
TInt gNativeSeCursor = 0;

void EnsureNativeBgmL()
{
    if (!gNativeBgm)
        gNativeBgm = CNativeBgmPlayer::NewL();
}

void PlayNativeSeL(const TDesC& aPath, TInt aVolume)
{
    TInt selected = -1;
    for (TInt index = 0; index < KNativeSeVoiceCount; ++index)
    {
        if (!gNativeSeVoices[index] || !gNativeSeVoices[index]->IsBusy())
        {
            selected = index;
            break;
        }
    }
    if (selected < 0)
        selected = gNativeSeCursor;
    if (!gNativeSeVoices[selected])
        gNativeSeVoices[selected] = CNativeSeVoice::NewL();
    gNativeSeVoices[selected]->PlayL(aPath, aVolume);
    gNativeSeCursor = (selected + 1) % KNativeSeVoiceCount;
}

void StopNativeSe()
{
    for (TInt index = 0; index < KNativeSeVoiceCount; ++index)
        if (gNativeSeVoices[index])
            gNativeSeVoices[index]->Stop();
}

TBool HasExtension(const TDesC& aPath, const TDesC& aExtension)
{
    return aPath.Length() >= aExtension.Length() &&
           aPath.Right(aExtension.Length()).CompareF(aExtension) == 0;
}

HBufC8* ReadFileLC(const TDesC& aPath)
{
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);

    RFile file;
    User::LeaveIfError(file.Open(fs, aPath, EFileRead | EFileShareReadersOnly));
    CleanupClosePushL(file);

    TInt size = 0;
    User::LeaveIfError(file.Size(size));
    if (size <= 0 || size > 2 * 1024 * 1024)
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

TInt ClampPcm(TInt aValue)
{
    if (aValue > 32767)
        return 32767;
    if (aValue < -32768)
        return -32768;
    return aValue;
}

struct TAdpcmState
{
    TInt iDelta;
    TInt iSample1;
    TInt iSample2;
    TInt iCoef1;
    TInt iCoef2;
};

TInt DecodeAdpcmNibble(TAdpcmState& aState, TUint8 aNibble)
{
    static const TInt KAdaptation[16] =
    {
        230, 230, 230, 230, 307, 409, 512, 614,
        768, 614, 512, 409, 307, 230, 230, 230
    };
    const TInt signedNibble = (aNibble & 8u) ? static_cast<TInt>(aNibble) - 16 : aNibble;
    const TInt prediction = ClampPcm(
        (aState.iSample1 * aState.iCoef1 + aState.iSample2 * aState.iCoef2) / 256 +
        signedNibble * aState.iDelta);
    aState.iSample2 = aState.iSample1;
    aState.iSample1 = prediction;
    aState.iDelta = (KAdaptation[aNibble & 15u] * aState.iDelta) / 256;
    if (aState.iDelta < 16)
        aState.iDelta = 16;
    return prediction;
}

void WriteSample(TUint8* aDestination, TInt& aSampleIndex, TInt aValue)
{
    const TUint16 encoded = static_cast<TUint16>(static_cast<TInt16>(aValue));
    aDestination[aSampleIndex * 2] = static_cast<TUint8>(encoded & 0xffu);
    aDestination[aSampleIndex * 2 + 1] = static_cast<TUint8>(encoded >> 8);
    ++aSampleIndex;
}

void DecodeMsAdpcmLC(const TUint8* aFormat,
                     TUint32 aFormatBytes,
                     const TUint8* aData,
                     TUint32 aDataBytes,
                     HBufC8*& aPcm,
                     TInt& aFrameCount,
                     TInt& aSampleRate,
                     TInt& aChannels)
{
    if (!aFormat || aFormatBytes < 22 || !aData || aDataBytes == 0)
        User::Leave(KErrCorrupt);

    const TInt channels = Read16(aFormat + 2);
    const TUint32 sampleRate = Read32(aFormat + 4);
    const TInt blockAlign = Read16(aFormat + 12);
    const TInt bits = Read16(aFormat + 14);
    const TInt samplesPerBlock = Read16(aFormat + 18);
    const TInt coefficientCount = Read16(aFormat + 20);
    if ((channels != 1 && channels != 2) || sampleRate == 0 || blockAlign <= channels * 7 ||
        bits != 4 || samplesPerBlock < 2 || coefficientCount <= 0 ||
        22u + static_cast<TUint32>(coefficientCount) * 4u > aFormatBytes)
        User::Leave(KErrNotSupported);

    const TUint32 blockCount = (aDataBytes + static_cast<TUint32>(blockAlign) - 1u) /
                               static_cast<TUint32>(blockAlign);
    const TInt64 maxFrames64 = static_cast<TInt64>(blockCount) * samplesPerBlock;
    if (maxFrames64 <= 0 || maxFrames64 > KMaxTInt / (channels * 2))
        User::Leave(KErrTooBig);

    HBufC8* pcm = HBufC8::NewLC(static_cast<TInt>(maxFrames64) * channels * 2);
    TPtr8 output = pcm->Des();
    output.SetLength(output.MaxLength());
    TUint8* destination = const_cast<TUint8*>(output.Ptr());
    TInt writeSample = 0;
    TUint32 dataOffset = 0;

    while (dataOffset < aDataBytes)
    {
        TUint32 blockBytes = aDataBytes - dataOffset;
        if (blockBytes > static_cast<TUint32>(blockAlign))
            blockBytes = blockAlign;
        const TUint8* block = aData + dataOffset;
        const TInt headerBytes = channels * 7;
        if (blockBytes < static_cast<TUint32>(headerBytes))
            User::Leave(KErrCorrupt);

        TAdpcmState state[2];
        TInt channel = 0;
        for (channel = 0; channel < channels; ++channel)
        {
            const TInt predictor = block[channel];
            if (predictor < 0 || predictor >= coefficientCount)
                User::Leave(KErrCorrupt);
            const TUint8* coefficient = aFormat + 22 + predictor * 4;
            state[channel].iCoef1 = static_cast<TInt16>(Read16(coefficient));
            state[channel].iCoef2 = static_cast<TInt16>(Read16(coefficient + 2));
        }

        TInt cursor = channels;
        for (channel = 0; channel < channels; ++channel)
        {
            state[channel].iDelta = static_cast<TInt16>(Read16(block + cursor));
            if (state[channel].iDelta < 16)
                state[channel].iDelta = 16;
            cursor += 2;
        }
        for (channel = 0; channel < channels; ++channel)
        {
            state[channel].iSample1 = static_cast<TInt16>(Read16(block + cursor));
            cursor += 2;
        }
        for (channel = 0; channel < channels; ++channel)
        {
            state[channel].iSample2 = static_cast<TInt16>(Read16(block + cursor));
            cursor += 2;
        }

        for (channel = 0; channel < channels; ++channel)
            WriteSample(destination, writeSample, state[channel].iSample2);
        for (channel = 0; channel < channels; ++channel)
            WriteSample(destination, writeSample, state[channel].iSample1);

        TInt framesWritten = 2;
        if (channels == 2)
        {
            while (framesWritten < samplesPerBlock && cursor < static_cast<TInt>(blockBytes))
            {
                const TUint8 value = block[cursor++];
                WriteSample(destination, writeSample, DecodeAdpcmNibble(state[0], value >> 4));
                WriteSample(destination, writeSample, DecodeAdpcmNibble(state[1], value & 15u));
                ++framesWritten;
            }
        }
        else
        {
            while (framesWritten < samplesPerBlock && cursor < static_cast<TInt>(blockBytes))
            {
                const TUint8 value = block[cursor++];
                WriteSample(destination, writeSample, DecodeAdpcmNibble(state[0], value >> 4));
                ++framesWritten;
                if (framesWritten < samplesPerBlock)
                {
                    WriteSample(destination, writeSample, DecodeAdpcmNibble(state[0], value & 15u));
                    ++framesWritten;
                }
            }
        }

        if (framesWritten < 2)
            User::Leave(KErrCorrupt);
        dataOffset += blockBytes;
    }

    output.SetLength(writeSample * 2);
    aPcm = pcm;
    aFrameCount = writeSample / channels;
    aSampleRate = static_cast<TInt>(sampleRate);
    aChannels = channels;
}

void DecodePcmWavLC(const TDesC8& aFile,
                     HBufC8*& aPcm,
                     TInt& aFrameCount,
                     TInt& aSampleRate,
                     TInt& aChannels)
{
    aPcm = NULL;
    aFrameCount = 0;
    aSampleRate = 0;
    aChannels = 0;

    if (aFile.Length() < 12 ||
        Mem::Compare(aFile.Ptr(), 4, reinterpret_cast<const TUint8*>("RIFF"), 4) != 0 ||
        Mem::Compare(aFile.Ptr() + 8, 4, reinterpret_cast<const TUint8*>("WAVE"), 4) != 0)
        User::Leave(KErrCorrupt);

    TUint16 format = 0;
    const TUint8* formatData = NULL;
    TUint32 formatBytes = 0;
    TUint16 channels = 0;
    TUint32 sampleRate = 0;
    TUint16 blockAlign = 0;
    TUint16 bits = 0;
    const TUint8* sampleData = NULL;
    TUint32 sampleBytes = 0;

    TInt position = 12;
    while (position <= aFile.Length() - 8)
    {
        const TUint8* chunk = aFile.Ptr() + position;
        const TUint32 chunkSize = Read32(chunk + 4);
        position += 8;
        if (chunkSize > static_cast<TUint32>(aFile.Length() - position))
            User::Leave(KErrCorrupt);

        if (Mem::Compare(chunk, 4, reinterpret_cast<const TUint8*>("fmt "), 4) == 0)
        {
            if (chunkSize < 16)
                User::Leave(KErrCorrupt);
            const TUint8* fmt = aFile.Ptr() + position;
            formatData = fmt;
            formatBytes = chunkSize;
            format = Read16(fmt);
            channels = Read16(fmt + 2);
            sampleRate = Read32(fmt + 4);
            blockAlign = Read16(fmt + 12);
            bits = Read16(fmt + 14);
        }
        else if (Mem::Compare(chunk, 4, reinterpret_cast<const TUint8*>("data"), 4) == 0)
        {
            sampleData = aFile.Ptr() + position;
            sampleBytes = chunkSize;
        }

        const TUint32 advance = chunkSize + (chunkSize & 1u);
        if (advance > static_cast<TUint32>(aFile.Length() - position))
        {
            if (chunkSize == static_cast<TUint32>(aFile.Length() - position))
                position = aFile.Length();
            else
                User::Leave(KErrCorrupt);
        }
        else
        {
            position += static_cast<TInt>(advance);
        }
    }

    if (format == 2)
    {
        DecodeMsAdpcmLC(formatData,
                        formatBytes,
                        sampleData,
                        sampleBytes,
                        aPcm,
                        aFrameCount,
                        aSampleRate,
                        aChannels);
        return;
    }

    if (format != 1 || !sampleData ||
        (channels != 1 && channels != 2) ||
        (bits != 8 && bits != 16) ||
        sampleRate == 0)
        User::Leave(KErrNotSupported);

    const TInt expectedAlign = channels * (bits / 8);
    if (blockAlign != expectedAlign || blockAlign == 0 || sampleBytes % blockAlign != 0)
        User::Leave(KErrCorrupt);

    const TUint32 frames = sampleBytes / blockAlign;
    if (frames == 0 || frames > static_cast<TUint32>(KMaxTInt / (channels * 2)))
        User::Leave(KErrTooBig);

    const TInt pcmBytes = static_cast<TInt>(frames) * channels * 2;
    HBufC8* pcm = HBufC8::NewLC(pcmBytes);
    TPtr8 output = pcm->Des();
    output.SetLength(pcmBytes);
    TUint8* destination = const_cast<TUint8*>(output.Ptr());
    const TInt sampleCount = static_cast<TInt>(frames) * channels;

    TInt sample = 0;
    if (bits == 16)
    {
        for (sample = 0; sample < sampleCount; ++sample)
        {
            destination[sample * 2] = sampleData[sample * 2];
            destination[sample * 2 + 1] = sampleData[sample * 2 + 1];
        }
    }
    else
    {
        for (sample = 0; sample < sampleCount; ++sample)
        {
            const TInt value = (static_cast<TInt>(sampleData[sample]) - 128) << 8;
            const TUint16 encoded = static_cast<TUint16>(static_cast<TInt16>(value));
            destination[sample * 2] = static_cast<TUint8>(encoded & 0xffu);
            destination[sample * 2 + 1] = static_cast<TUint8>(encoded >> 8);
        }
    }

    aPcm = pcm;
    aFrameCount = static_cast<TInt>(frames);
    aSampleRate = static_cast<TInt>(sampleRate);
    aChannels = channels;
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

TUint32 NowMilliseconds()
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

void StopBgmNow()
{
    if (gMixer)
        gMixer->ClearStream(CAudioMixer::EStreamBgm);
    if (gNativeBgm)
        gNativeBgm->Stop();
    delete gBgmPcm;
    gBgmPcm = NULL;
    gBgmFrameCount = 0;
    gBgmSampleRate = 0;
    gBgmChannels = 0;
    gBgmPhase = 0;
    gBgmStep = 0;
    gBgmFramesQueued = 0;
    gFadeDurationMs = 0;
    gBgmPlaying = EFalse;
    gBgmPaused = EFalse;
}

TInt BgmSample(TInt aFrame, TInt aChannel)
{
    const TInt16* samples = reinterpret_cast<const TInt16*>(gBgmPcm->Ptr());
    const TInt sourceChannel = gBgmChannels == 1 ? 0 : aChannel;
    return samples[aFrame * gBgmChannels + sourceChannel];
}

void PumpBgm(TInt aMaxFrames)
{
    if (!gMixer || !gBgmPcm || !gBgmPlaying || gBgmPaused ||
        gBgmFrameCount <= 0 || gBgmSampleRate <= 0 || aMaxFrames <= 0)
        return;

    if (gBgmStep <= 0)
        UpdateBgmStep();
    const TInt64 totalPhase = static_cast<TInt64>(gBgmFrameCount) << 16;
    const TInt targetFrames = 3072;
    const TInt queuedFrames = gMixer->StreamQueuedFrames(CAudioMixer::EStreamBgm);
    TInt freeFrames = gMixer->StreamFreeFrames(CAudioMixer::EStreamBgm);
    TInt framesToWrite = Min(aMaxFrames, targetFrames - queuedFrames);
    if (framesToWrite <= 0)
        return;
    freeFrames = Min(freeFrames, framesToWrite);
    while (freeFrames > 0)
    {
        const TInt batch = Min(freeFrames, 512);
        TInt16 output[512 * 2];
        TInt frame = 0;
        for (frame = 0; frame < batch; ++frame)
        {
            TInt sourceFrame = static_cast<TInt>(gBgmPhase >> 16);
            if (sourceFrame >= gBgmFrameCount)
                sourceFrame = 0;
            TInt nextFrame = sourceFrame + 1;
            if (nextFrame >= gBgmFrameCount)
                nextFrame = 0;
            const TInt fraction = static_cast<TInt>((gBgmPhase & 0xffff) >> 1);

            TInt channel = 0;
            for (channel = 0; channel < 2; ++channel)
            {
                const TInt first = BgmSample(sourceFrame, channel);
                const TInt second = BgmSample(nextFrame, channel);
                output[frame * 2 + channel] = static_cast<TInt16>(
                    first + (((second - first) * fraction) >> 15));
            }

            gBgmPhase += gBgmStep;
            while (gBgmPhase >= totalPhase)
                gBgmPhase -= totalPhase;
        }

        const TInt written = gMixer->WriteStreamPcm16(CAudioMixer::EStreamBgm,
                                                       output,
                                                       batch);
        if (written <= 0)
            break;
        gBgmFramesQueued += static_cast<TUint64>(written);
        freeFrames -= written;
        if (written != batch)
            break;
    }
    gMixer->CommitStreams();
}

void PlayBgmL(const char* aPath, TInt aVolume, TInt aPitch, TInt aFadeIn)
{
    if (aPitch < 50 || aPitch > 200)
        User::Leave(KErrArgument);

    StopBgmNow();
    gBgmPitch = aPitch;
    gBgmVolume = aVolume;
    gBgmStartMs = NowMilliseconds();
    gFadeDurationMs = 0;

    TFileName path;
    AsciiPathL(aPath, path);
    _LIT(KWav, ".wav");
    if (!HasExtension(path, KWav))
    {
        EnsureNativeBgmL();
        gNativeBgm->PlaySampledL(path, aVolume, aPitch, aFadeIn);
        return;
    }

    EnsureMixerL();
    HBufC8* file = ReadFileLC(path);
    HBufC8* pcm = NULL;
    TInt frameCount = 0;
    TInt sampleRate = 0;
    TInt channels = 0;
    DecodePcmWavLC(*file, pcm, frameCount, sampleRate, channels);

    gBgmPcm = pcm;
    CleanupStack::Pop(pcm);
    CleanupStack::PopAndDestroy(file);
    gBgmFrameCount = frameCount;
    gBgmSampleRate = sampleRate;
    gBgmChannels = channels;
    gBgmPhase = 0;
    UpdateBgmStep();
    gBgmFramesQueued = 0;
    gBgmPlaying = ETrue;
    gBgmPaused = EFalse;
    gMixer->SetStreamVolume(CAudioMixer::EStreamBgm, aVolume);
    PumpBgm(3072);
}

void CacheSeL(const TDesC& aPath, TInt aPitch, TBool aPinned)
{
    TBuf<160> cacheKey;
    BuildCacheKeyL(aPath, aPitch, cacheKey);
    if (gMixer->IsCached(cacheKey))
    {
        if (aPinned)
            gMixer->SetCachedPinned(cacheKey, ETrue);
        return;
    }

    HBufC8* file = ReadFileLC(aPath);
    HBufC8* pcm = NULL;
    TInt frameCount = 0;
    TInt sampleRate = 0;
    TInt channels = 0;
    DecodePcmWavLC(*file, pcm, frameCount, sampleRate, channels);

    const TInt64 adjustedRate64 = static_cast<TInt64>(sampleRate) * aPitch / 100;
    if (adjustedRate64 <= 0 || adjustedRate64 > KMaxTInt)
        User::Leave(KErrArgument);

    gMixer->CachePcm16L(cacheKey,
                        reinterpret_cast<const TInt16*>(pcm->Ptr()),
                        frameCount,
                        static_cast<TInt>(adjustedRate64),
                        channels,
                        aPinned);

    CleanupStack::PopAndDestroy(pcm);
    CleanupStack::PopAndDestroy(file);
}

void PreloadSeL(const char* aPath, TInt aPitch)
{
    if (aPitch < 50 || aPitch > 200)
        User::Leave(KErrArgument);
    EnsureMixerL();

    TFileName path;
    AsciiPathL(aPath, path);
    _LIT(KWav, ".wav");
    if (!HasExtension(path, KWav))
        return;
    CacheSeL(path, aPitch, ETrue);
}

void WarmSeL(const char* aPath, TInt aPitch)
{
    if (aPitch < 50 || aPitch > 200)
        User::Leave(KErrArgument);
    EnsureMixerL();

    TFileName path;
    AsciiPathL(aPath, path);
    _LIT(KWav, ".wav");
    if (!HasExtension(path, KWav))
        return;
    CacheSeL(path, aPitch, EFalse);
}

void PlaySeL(const char* aPath, TInt aVolume, TInt aPitch)
{
    if (aPitch < 50 || aPitch > 200)
        User::Leave(KErrArgument);
    EnsureMixerL();

    TFileName path;
    AsciiPathL(aPath, path);
    _LIT(KWav, ".wav");
    if (!HasExtension(path, KWav))
    {
        PlayNativeSeL(path, aVolume);
        return;
    }
    TBuf<160> cacheKey;
    BuildCacheKeyL(path, aPitch, cacheKey);
    if (!gMixer->IsCached(cacheKey))
        CacheSeL(path, aPitch, EFalse);
    if (!gMixer->PlaySe(cacheKey, aVolume))
        User::Leave(KErrNotReady);
}
}

extern "C" int EasyRpgAudioPreloadSe(const char* aPath, int aPitch)
{
    TRAPD(error, PreloadSeL(aPath, aPitch));
    return error;
}

extern "C" int EasyRpgAudioWarmSe(const char* aPath, int aPitch)
{
    TRAPD(error, WarmSeL(aPath, aPitch));
    return error;
}

extern "C" int EasyRpgAudioPlaySe(const char* aPath, int aVolume, int aPitch)
{
    TRAPD(error, PlaySeL(aPath, aVolume, aPitch));
    return error;
}

extern "C" void EasyRpgAudioStopSe(void)
{
    if (gMixer)
        gMixer->StopSe();
    StopNativeSe();
}

extern "C" int EasyRpgAudioPlayBgm(const char* aPath, int aVolume, int aPitch, int aFadeIn)
{
    TRAPD(error, PlayBgmL(aPath, aVolume, aPitch, aFadeIn));
    return error;
}

extern "C" void EasyRpgAudioStopBgm(void)
{
    StopBgmNow();
}

extern "C" void EasyRpgAudioPauseBgm(void)
{
    if (gNativeBgm && gNativeBgm->Type() != CNativeBgmPlayer::ENone)
    {
        gNativeBgm->Pause();
        return;
    }
    if (!gBgmPlaying || gBgmPaused)
        return;
    gBgmPaused = ETrue;
    if (gMixer)
        gMixer->ClearStream(CAudioMixer::EStreamBgm);
}

extern "C" void EasyRpgAudioResumeBgm(void)
{
    if (gNativeBgm && gNativeBgm->Type() != CNativeBgmPlayer::ENone)
    {
        gNativeBgm->Resume();
        return;
    }
    if (!gBgmPlaying || !gBgmPaused)
        return;
    gBgmPaused = EFalse;
    PumpBgm(3072);
}

extern "C" void EasyRpgAudioSetBgmVolume(int aVolume)
{
    gBgmVolume = aVolume;
    if (gNativeBgm && gNativeBgm->Type() != CNativeBgmPlayer::ENone)
        gNativeBgm->SetVolume(aVolume);
    if (gMixer)
        gMixer->SetStreamVolume(CAudioMixer::EStreamBgm, aVolume);
}

extern "C" void EasyRpgAudioSetBgmPitch(int aPitch)
{
    if (aPitch >= 50 && aPitch <= 200)
    {
        gBgmPitch = aPitch;
        UpdateBgmStep();
        if (gNativeBgm && gNativeBgm->Type() != CNativeBgmPlayer::ENone)
            gNativeBgm->SetPitch(aPitch);
    }
}

extern "C" void EasyRpgAudioFadeBgm(int aFade)
{
    if (!gBgmPlaying && (!gNativeBgm || gNativeBgm->Type() == CNativeBgmPlayer::ENone))
        return;
    if (aFade <= 0)
    {
        StopBgmNow();
        return;
    }
    gFadeStartMs = NowMilliseconds();
    gFadeDurationMs = aFade;
    gFadeStartVolume = gBgmVolume;
}

extern "C" int EasyRpgAudioIsBgmPlaying(void)
{
    if (gNativeBgm && gNativeBgm->Type() != CNativeBgmPlayer::ENone)
        return gNativeBgm->IsPlaying() ? 1 : 0;
    return gBgmPlaying ? 1 : 0;
}

extern "C" unsigned int EasyRpgAudioBgmTicks(void)
{
    if (gNativeBgm && gNativeBgm->Type() != CNativeBgmPlayer::ENone)
        return gNativeBgm->Ticks();
    if (!gBgmPlaying)
        return 0;
    const TUint64 ticks = gBgmFramesQueued * 1000u /
                          static_cast<TUint64>(CAudioMixer::OutputSampleRate());
    return ticks > 0xffffffffu ? 0xffffffffu : static_cast<unsigned int>(ticks);
}

extern "C" void EasyRpgAudioUpdate(void)
{
    if (gMixer)
    {
        const TInt mixerError = gMixer->ConsumeError();
        if (mixerError != KErrNone)
            gAudioAsyncError = mixerError;
    }
    if (gFadeDurationMs > 0 &&
        (gBgmPlaying || (gNativeBgm && gNativeBgm->Type() != CNativeBgmPlayer::ENone)))
    {
        const TUint32 elapsed = NowMilliseconds() - gFadeStartMs;
        if (elapsed >= static_cast<TUint32>(gFadeDurationMs))
        {
            StopBgmNow();
            return;
        }
        const TInt volume = static_cast<TInt>(
            static_cast<TInt64>(gFadeStartVolume) *
            (gFadeDurationMs - static_cast<TInt>(elapsed)) /
            gFadeDurationMs);
        if (gNativeBgm && gNativeBgm->Type() != CNativeBgmPlayer::ENone)
            gNativeBgm->SetVolume(volume);
        if (gMixer)
            gMixer->SetStreamVolume(CAudioMixer::EStreamBgm, volume);
    }
    PumpBgm(1024);
    if (gMixer)
        gMixer->CommitStreams();
}

extern "C" int EasyRpgAudioConsumeError(void)
{
    const TInt error = gAudioAsyncError;
    gAudioAsyncError = KErrNone;
    return error;
}

extern "C" void EasyRpgAudioShutdown(void)
{
    StopBgmNow();
    StopNativeSe();
    for (TInt index = 0; index < KNativeSeVoiceCount; ++index)
    {
        delete gNativeSeVoices[index];
        gNativeSeVoices[index] = NULL;
    }
    delete gNativeBgm;
    gNativeBgm = NULL;
    delete gMixer;
    gMixer = NULL;
}

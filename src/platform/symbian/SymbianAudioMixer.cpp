#include "easyrpgsymbian/SymbianAudioMixer.h"

CSymbianAudioMixer* CSymbianAudioMixer::NewL()
{
    CSymbianAudioMixer* self = new (ELeave) CSymbianAudioMixer;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CSymbianAudioMixer::CSymbianAudioMixer()
    : iStream(NULL),
      iQueuedOutputBufferCount(0),
      iClipCount(0),
      iCachedBytes(0),
      iCacheByteLimit(KMaxCachedAudioBytes),
      iNextSerial(0),
      iClipUseSerial(0),
      iReady(EFalse),
      iShuttingDown(EFalse),
      iLastError(KErrNone)
{
    TInt i;
    for (i = 0; i < KMaxCachedClips; ++i)
    {
        iClips[i].iKey.Zero();
        iClips[i].iPcm16 = NULL;
        iClips[i].iFrameCount = 0;
        iClips[i].iUseSerial = 0;
        iClips[i].iKeyHash = 0;
        iClips[i].iHashNext = -1;
        iClips[i].iPinned = EFalse;
    }
    for (i = 0; i < KClipHashBucketCount; ++i)
        iClipHashBuckets[i] = -1;
    Mem::FillZ(iVoices, sizeof(iVoices));
    Mem::FillZ(iStreams, sizeof(iStreams));
    for (i = 0; i < EStreamBusCount; ++i)
        iStreams[i].iVolume = 100;
    for (i = 0; i < KOutputBufferCount; ++i)
        iOutputBufferQueued[i] = EFalse;
}

void CSymbianAudioMixer::ConstructL()
{
    iStream = CMdaAudioOutputStream::NewL(*this);
    TMdaAudioDataSettings settings;
    Mem::FillZ(&settings, sizeof(settings));
    settings.iSampleRate = TMdaAudioDataSettings::ESampleRate22050Hz;
    settings.iChannels = TMdaAudioDataSettings::EChannelsStereo;
    iStream->Open(&settings);
}

CSymbianAudioMixer::~CSymbianAudioMixer()
{
    iShuttingDown = ETrue;
    if (iStream)
        iStream->Stop();
    delete iStream;
    iStream = NULL;

    TInt i;
    for (i = 0; i < KMaxCachedClips; ++i)
    {
        delete iClips[i].iPcm16;
        iClips[i].iPcm16 = NULL;
    }
}

TUint32 CSymbianAudioMixer::HashKey(const TDesC& aKey)
{
    TUint32 hash = 2166136261u;
    TInt i;
    for (i = 0; i < aKey.Length(); ++i)
    {
        TUint32 character = static_cast<TUint16>(aKey[i]);
        if (character >= 'A' && character <= 'Z')
            character += 'a' - 'A';
        hash ^= character;
        hash *= 16777619u;
    }
    return hash;
}

TInt CSymbianAudioMixer::ClampVolume(TInt aVolume)
{
    if (aVolume < 0)
        return 0;
    if (aVolume > 100)
        return 100;
    return aVolume;
}

TInt16 CSymbianAudioMixer::ReadPcm16(const TUint8* aData)
{
    const TUint16 value = static_cast<TUint16>(aData[0]) |
                          static_cast<TUint16>(static_cast<TUint16>(aData[1]) << 8);
    return static_cast<TInt16>(value);
}

void CSymbianAudioMixer::WritePcm16(TUint8* aData, TInt aValue)
{
    if (aValue > 32767)
        aValue = 32767;
    else if (aValue < -32768)
        aValue = -32768;
    const TUint16 value = static_cast<TUint16>(static_cast<TInt16>(aValue));
    aData[0] = static_cast<TUint8>(value & 0xffu);
    aData[1] = static_cast<TUint8>(value >> 8);
}

CSymbianAudioMixer::TCachedClip* CSymbianAudioMixer::FindCachedClip(const TDesC& aKey)
{
    const TUint32 hash = HashKey(aKey);
    TInt index = iClipHashBuckets[hash & (KClipHashBucketCount - 1)];
    while (index >= 0)
    {
        TCachedClip& clip = iClips[index];
        if (clip.iPcm16 && clip.iKeyHash == hash && clip.iKey.CompareF(aKey) == 0)
            return &clip;
        index = clip.iHashNext;
    }
    return NULL;
}

const CSymbianAudioMixer::TCachedClip* CSymbianAudioMixer::FindCachedClip(const TDesC& aKey) const
{
    const TUint32 hash = HashKey(aKey);
    TInt index = iClipHashBuckets[hash & (KClipHashBucketCount - 1)];
    while (index >= 0)
    {
        const TCachedClip& clip = iClips[index];
        if (clip.iPcm16 && clip.iKeyHash == hash && clip.iKey.CompareF(aKey) == 0)
            return &clip;
        index = clip.iHashNext;
    }
    return NULL;
}

void CSymbianAudioMixer::LinkCachedClip(TInt aClipIndex)
{
    TCachedClip& clip = iClips[aClipIndex];
    const TInt bucket = static_cast<TInt>(clip.iKeyHash & (KClipHashBucketCount - 1));
    clip.iHashNext = iClipHashBuckets[bucket];
    iClipHashBuckets[bucket] = aClipIndex;
}

void CSymbianAudioMixer::UnlinkCachedClip(TInt aClipIndex)
{
    TCachedClip& clip = iClips[aClipIndex];
    const TInt bucket = static_cast<TInt>(clip.iKeyHash & (KClipHashBucketCount - 1));
    TInt previous = -1;
    TInt current = iClipHashBuckets[bucket];
    while (current >= 0)
    {
        if (current == aClipIndex)
        {
            if (previous >= 0)
                iClips[previous].iHashNext = clip.iHashNext;
            else
                iClipHashBuckets[bucket] = clip.iHashNext;
            clip.iHashNext = -1;
            return;
        }
        previous = current;
        current = iClips[current].iHashNext;
    }
    clip.iHashNext = -1;
}

TBool CSymbianAudioMixer::VoiceIsActive(const TVoice& aVoice)
{
    return aVoice.iActive && aVoice.iClip && aVoice.iClip->iPcm16 &&
           aVoice.iPosition < aVoice.iClip->iFrameCount;
}

void CSymbianAudioMixer::ClearVoice(TVoice& aVoice)
{
    aVoice.iClip = NULL;
    aVoice.iPosition = 0;
    aVoice.iVolume = 0;
    aVoice.iPriority = 0;
    aVoice.iSerial = 0;
    aVoice.iActive = EFalse;
}

TBool CSymbianAudioMixer::ClipIsActive(TInt aClipIndex) const
{
    const TCachedClip* clip = &iClips[aClipIndex];
    TInt i;
    for (i = 0; i < KMaxVoices; ++i)
    {
        if (VoiceIsActive(iVoices[i]) && iVoices[i].iClip == clip)
            return ETrue;
    }
    return EFalse;
}

TInt CSymbianAudioMixer::LeastRecentlyUsedClip() const
{
    TInt selected = -1;
    TUint32 oldest = 0xffffffffu;
    TInt i;
    for (i = 0; i < KMaxCachedClips; ++i)
    {
        const TCachedClip& clip = iClips[i];
        if (!clip.iPcm16 || clip.iPinned || ClipIsActive(i))
            continue;
        if (clip.iUseSerial < oldest)
        {
            oldest = clip.iUseSerial;
            selected = i;
        }
    }
    return selected;
}

void CSymbianAudioMixer::ReleaseClip(TInt aClipIndex)
{
    if (aClipIndex < 0 || aClipIndex >= KMaxCachedClips)
        return;
    TCachedClip& clip = iClips[aClipIndex];
    if (!clip.iPcm16 || ClipIsActive(aClipIndex))
        return;
    UnlinkCachedClip(aClipIndex);
    iCachedBytes -= clip.iPcm16->Length();
    if (iCachedBytes < 0)
        iCachedBytes = 0;
    delete clip.iPcm16;
    clip.iPcm16 = NULL;
    clip.iKey.Zero();
    clip.iFrameCount = 0;
    clip.iUseSerial = 0;
    clip.iKeyHash = 0;
    clip.iHashNext = -1;
    clip.iPinned = EFalse;
    if (iClipCount > 0)
        --iClipCount;
}

void CSymbianAudioMixer::CachePcm16L(const TDesC& aKey,
                                     const TInt16* aSamples,
                                     TInt aFrameCount,
                                     TInt aSampleRate,
                                     TInt aChannels,
                                     TBool aPinned)
{
    if (!aSamples || aFrameCount <= 0 || aSampleRate <= 0 || (aChannels != 1 && aChannels != 2))
        User::Leave(KErrArgument);
    if (aKey.Length() <= 0 || aKey.Length() > KMaxCachedKeyLength)
        User::Leave(KErrTooBig);

    TCachedClip* existing = FindCachedClip(aKey);
    if (existing)
    {
        existing->iPinned = aPinned;
        existing->iUseSerial = ++iClipUseSerial;
        return;
    }

    const TInt64 outputFrames64 =
        (static_cast<TInt64>(aFrameCount) * KOutputSampleRate + aSampleRate - 1) / aSampleRate;
    if (outputFrames64 <= 0 || outputFrames64 > KMaxTInt / (KOutputChannels * 2))
        User::Leave(KErrTooBig);
    const TInt outputFrames = static_cast<TInt>(outputFrames64);
    const TInt outputBytes = outputFrames * KOutputChannels * 2;
    if (outputBytes > KMaxCachedClipBytes || outputBytes > iCacheByteLimit)
        User::Leave(KErrTooBig);

    while (iClipCount >= KMaxCachedClips || iCachedBytes + outputBytes > iCacheByteLimit)
    {
        const TInt releaseIndex = LeastRecentlyUsedClip();
        if (releaseIndex < 0)
            User::Leave(KErrInUse);
        ReleaseClip(releaseIndex);
    }

    TInt slot = -1;
    TInt i;
    for (i = 0; i < KMaxCachedClips; ++i)
    {
        if (!iClips[i].iPcm16)
        {
            slot = i;
            break;
        }
    }
    if (slot < 0)
        User::Leave(KErrNoMemory);

    HBufC8* pcm = HBufC8::NewLC(outputBytes);
    TPtr8 output = pcm->Des();
    output.SetLength(outputBytes);
    TUint8* destination = const_cast<TUint8*>(output.Ptr());

    const TInt64 step = (static_cast<TInt64>(aSampleRate) << 16) / KOutputSampleRate;
    TInt64 phase = 0;
    for (i = 0; i < outputFrames; ++i)
    {
        TInt sourceFrame = static_cast<TInt>(phase >> 16);
        if (sourceFrame >= aFrameCount)
            sourceFrame = aFrameCount - 1;
        TInt nextFrame = sourceFrame + 1;
        if (nextFrame >= aFrameCount)
            nextFrame = sourceFrame;
        const TInt fraction = static_cast<TInt>(phase & 0xffff);

        TInt channel;
        for (channel = 0; channel < KOutputChannels; ++channel)
        {
            const TInt sourceChannel = aChannels == 1 ? 0 : channel;
            const TInt first = aSamples[sourceFrame * aChannels + sourceChannel];
            const TInt second = aSamples[nextFrame * aChannels + sourceChannel];
            const TInt sample = first + static_cast<TInt>(
                (static_cast<TInt64>(second - first) * fraction) >> 16);
            WritePcm16(destination + (i * KOutputChannels + channel) * 2, sample);
        }
        phase += step;
    }

    TCachedClip& clip = iClips[slot];
    clip.iKey.Copy(aKey);
    clip.iPcm16 = pcm;
    clip.iFrameCount = outputFrames;
    clip.iUseSerial = ++iClipUseSerial;
    clip.iKeyHash = HashKey(aKey);
    clip.iHashNext = -1;
    clip.iPinned = aPinned;
    LinkCachedClip(slot);
    ++iClipCount;
    iCachedBytes += outputBytes;
    CleanupStack::Pop(pcm);
}

void CSymbianAudioMixer::SetCachedPinned(const TDesC& aKey, TBool aPinned)
{
    TCachedClip* clip = FindCachedClip(aKey);
    if (clip)
        clip->iPinned = aPinned;
}

TInt CSymbianAudioMixer::SelectPlaybackVoice(TInt aPriority) const
{
    TInt selected = -1;
    TInt selectedPriority = KMaxTInt;
    TUint32 oldestSerial = 0xffffffffu;
    TInt i;
    for (i = 0; i < KMaxVoices; ++i)
    {
        const TVoice& voice = iVoices[i];
        if (!VoiceIsActive(voice))
            return i;
        if (voice.iPriority > aPriority)
            continue;
        if (voice.iPriority < selectedPriority ||
            (voice.iPriority == selectedPriority && voice.iSerial < oldestSerial))
        {
            selected = i;
            selectedPriority = voice.iPriority;
            oldestSerial = voice.iSerial;
        }
    }
    return selected;
}

TBool CSymbianAudioMixer::PlaySe(const TDesC& aKey, TInt aVolume, TInt aPriority)
{
    TCachedClip* clip = FindCachedClip(aKey);
    if (!clip)
        return EFalse;
    const TInt selected = SelectPlaybackVoice(aPriority);
    if (selected < 0)
        return EFalse;

    clip->iUseSerial = ++iClipUseSerial;
    TVoice& voice = iVoices[selected];
    voice.iClip = clip;
    voice.iPosition = 0;
    voice.iVolume = ClampVolume(aVolume);
    voice.iPriority = aPriority;
    voice.iSerial = ++iNextSerial;
    voice.iActive = ETrue;
    QueueOutputBuffers();
    return ETrue;
}

void CSymbianAudioMixer::StopSe()
{
    TInt i;
    for (i = 0; i < KMaxVoices; ++i)
        ClearVoice(iVoices[i]);
}

TInt CSymbianAudioMixer::WriteStreamPcm16(TStreamBus aBus,
                                          const TInt16* aSamples,
                                          TInt aFrameCount)
{
    if (aBus < 0 || aBus >= EStreamBusCount || !aSamples || aFrameCount <= 0)
        return 0;
    TStreamState& stream = iStreams[aBus];
    const TInt writable = Min(aFrameCount, KStreamBufferFrames - stream.iQueuedFrames);
    TInt frame;
    for (frame = 0; frame < writable; ++frame)
    {
        const TInt destinationFrame = stream.iWriteFrame;
        stream.iSamples[destinationFrame * KOutputChannels] =
            aSamples[frame * KOutputChannels];
        stream.iSamples[destinationFrame * KOutputChannels + 1] =
            aSamples[frame * KOutputChannels + 1];
        ++stream.iWriteFrame;
        if (stream.iWriteFrame >= KStreamBufferFrames)
            stream.iWriteFrame = 0;
    }
    stream.iQueuedFrames += writable;
    QueueOutputBuffers();
    return writable;
}

void CSymbianAudioMixer::ClearStream(TStreamBus aBus)
{
    if (aBus < 0 || aBus >= EStreamBusCount)
        return;
    TStreamState& stream = iStreams[aBus];
    stream.iReadFrame = 0;
    stream.iWriteFrame = 0;
    stream.iQueuedFrames = 0;
}

void CSymbianAudioMixer::SetStreamVolume(TStreamBus aBus, TInt aVolume)
{
    if (aBus < 0 || aBus >= EStreamBusCount)
        return;
    iStreams[aBus].iVolume = ClampVolume(aVolume);
}

TInt CSymbianAudioMixer::StreamFreeFrames(TStreamBus aBus) const
{
    if (aBus < 0 || aBus >= EStreamBusCount)
        return 0;
    return KStreamBufferFrames - iStreams[aBus].iQueuedFrames;
}

TInt CSymbianAudioMixer::StreamQueuedFrames(TStreamBus aBus) const
{
    if (aBus < 0 || aBus >= EStreamBusCount)
        return 0;
    return iStreams[aBus].iQueuedFrames;
}

TBool CSymbianAudioMixer::MixStream(TStreamState& aStream,
                                    TInt* aMixedSamples,
                                    TInt aFrameCount)
{
    const TInt frames = Min(aFrameCount, aStream.iQueuedFrames);
    const TInt volume = aStream.iVolume;
    TInt frame;
    for (frame = 0; frame < frames; ++frame)
    {
        const TInt sourceFrame = aStream.iReadFrame;
        const TInt outputOffset = frame * KOutputChannels;
        aMixedSamples[outputOffset] +=
            (static_cast<TInt>(aStream.iSamples[sourceFrame * KOutputChannels]) * volume) / 100;
        aMixedSamples[outputOffset + 1] +=
            (static_cast<TInt>(aStream.iSamples[sourceFrame * KOutputChannels + 1]) * volume) / 100;
        ++aStream.iReadFrame;
        if (aStream.iReadFrame >= KStreamBufferFrames)
            aStream.iReadFrame = 0;
    }
    aStream.iQueuedFrames -= frames;
    return frames > 0;
}

TBool CSymbianAudioMixer::MixVoice(TVoice& aVoice,
                                   TInt* aMixedSamples,
                                   TInt aFrameCount)
{
    if (!VoiceIsActive(aVoice))
    {
        ClearVoice(aVoice);
        return EFalse;
    }

    const TInt remaining = aVoice.iClip->iFrameCount - aVoice.iPosition;
    const TInt frames = Min(aFrameCount, remaining);
    const TUint8* source = aVoice.iClip->iPcm16->Ptr() +
                           aVoice.iPosition * KOutputChannels * 2;
    const TInt volume = aVoice.iVolume;
    TInt frame;
    for (frame = 0; frame < frames; ++frame)
    {
        const TInt sourceOffset = frame * KOutputChannels * 2;
        const TInt outputOffset = frame * KOutputChannels;
        const TInt left = ReadPcm16(source + sourceOffset);
        const TInt right = ReadPcm16(source + sourceOffset + 2);
        aMixedSamples[outputOffset] += (left * volume) / 100;
        aMixedSamples[outputOffset + 1] += (right * volume) / 100;
    }
    aVoice.iPosition += frames;
    if (aVoice.iPosition >= aVoice.iClip->iFrameCount)
        ClearVoice(aVoice);
    return frames > 0;
}

void CSymbianAudioMixer::FillOutputBuffer(TInt aBufferIndex)
{
    TBuf8<KOutputBufferBytes>& output = iOutputBuffers[aBufferIndex];
    output.SetLength(KOutputBufferBytes);
    TInt mixed[KOutputBufferBytes / 2];
    Mem::FillZ(mixed, sizeof(mixed));
    const TInt frameCount = KOutputBufferBytes / (KOutputChannels * 2);

    TInt bus;
    for (bus = 0; bus < EStreamBusCount; ++bus)
        MixStream(iStreams[bus], mixed, frameCount);

    TInt voice;
    for (voice = 0; voice < KMaxVoices; ++voice)
        MixVoice(iVoices[voice], mixed, frameCount);

    TUint8* destination = const_cast<TUint8*>(output.Ptr());
    TInt sample;
    for (sample = 0; sample < KOutputBufferBytes / 2; ++sample)
        WritePcm16(destination + sample * 2, mixed[sample]);
}

TInt CSymbianAudioMixer::FreeOutputBufferIndex() const
{
    TInt i;
    for (i = 0; i < KOutputBufferCount; ++i)
    {
        if (!iOutputBufferQueued[i])
            return i;
    }
    return -1;
}

TInt CSymbianAudioMixer::OutputBufferIndex(const TDesC8& aBuffer) const
{
    TInt i;
    for (i = 0; i < KOutputBufferCount; ++i)
    {
        if (aBuffer.Ptr() == iOutputBuffers[i].Ptr())
            return i;
    }
    return -1;
}

void CSymbianAudioMixer::QueueOutputBuffers()
{
    if (!iStream || !iReady || iShuttingDown)
        return;
    while (iQueuedOutputBufferCount < KOutputQueueDepth)
    {
        const TInt bufferIndex = FreeOutputBufferIndex();
        if (bufferIndex < 0)
            return;
        FillOutputBuffer(bufferIndex);
        TRAPD(error, iStream->WriteL(iOutputBuffers[bufferIndex]));
        if (error != KErrNone)
        {
            iReady = EFalse;
            iLastError = error;
            return;
        }
        iOutputBufferQueued[bufferIndex] = ETrue;
        ++iQueuedOutputBufferCount;
    }
}

void CSymbianAudioMixer::ResetOutputQueue()
{
    TInt i;
    for (i = 0; i < KOutputBufferCount; ++i)
        iOutputBufferQueued[i] = EFalse;
    iQueuedOutputBufferCount = 0;
}

void CSymbianAudioMixer::ClearAll()
{
    StopSe();
    TInt bus;
    for (bus = 0; bus < EStreamBusCount; ++bus)
        ClearStream(static_cast<TStreamBus>(bus));
}

TInt CSymbianAudioMixer::CachedBytes() const
{
    return iCachedBytes;
}

void CSymbianAudioMixer::TrimCache(TInt aByteLimit)
{
    if (aByteLimit < 0)
        aByteLimit = 0;
    if (aByteLimit > KMaxCachedAudioBytes)
        aByteLimit = KMaxCachedAudioBytes;
    iCacheByteLimit = aByteLimit;
    while (iCachedBytes > iCacheByteLimit)
    {
        const TInt releaseIndex = LeastRecentlyUsedClip();
        if (releaseIndex < 0)
            return;
        ReleaseClip(releaseIndex);
    }
}

TBool CSymbianAudioMixer::IsReady() const
{
    return iReady;
}

TInt CSymbianAudioMixer::ConsumeError()
{
    const TInt error = iLastError;
    iLastError = KErrNone;
    return error;
}

TInt CSymbianAudioMixer::OutputSampleRate()
{
    return KOutputSampleRate;
}

TInt CSymbianAudioMixer::OutputChannels()
{
    return KOutputChannels;
}

void CSymbianAudioMixer::MaoscOpenComplete(TInt aError)
{
    iReady = (aError == KErrNone);
    if (iReady)
        QueueOutputBuffers();
    else
        iLastError = aError;
}

void CSymbianAudioMixer::MaoscBufferCopied(TInt aError, const TDesC8& aBuffer)
{
    const TInt bufferIndex = OutputBufferIndex(aBuffer);
    if (bufferIndex >= 0 && iOutputBufferQueued[bufferIndex])
    {
        iOutputBufferQueued[bufferIndex] = EFalse;
        if (iQueuedOutputBufferCount > 0)
            --iQueuedOutputBufferCount;
    }
    if (aError == KErrNone)
        QueueOutputBuffers();
    else if (aError != KErrAbort && !iShuttingDown)
    {
        iReady = EFalse;
        iLastError = aError;
    }
}

void CSymbianAudioMixer::MaoscPlayComplete(TInt aError)
{
    if (iShuttingDown)
        return;
    if (aError == KErrNone || aError == KErrUnderflow)
    {
        ResetOutputQueue();
        QueueOutputBuffers();
    }
    else if (aError != KErrAbort)
    {
        iReady = EFalse;
        iLastError = aError;
    }
}

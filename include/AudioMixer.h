#ifndef EASYRPG_AUDIO_MIXER_H
#define EASYRPG_AUDIO_MIXER_H

#include <e32base.h>
#include <mda/common/audio.h>
#include <mdaaudiooutputstream.h>

class CAudioMixer : public CBase, public MMdaAudioOutputStreamCallback
{
public:
    enum TStreamBus
    {
        EStreamBgm = 0,
        EStreamBgs,
        EStreamBusCount
    };

    static CAudioMixer* NewL();
    ~CAudioMixer();

    void CachePcm16L(const TDesC& aKey, const TInt16* aSamples, TInt aFrameCount, TInt aSampleRate, TInt aChannels, TBool aPinned = EFalse);
    void SetCachedPinned(const TDesC& aKey, TBool aPinned);
    TBool IsCached(const TDesC& aKey) const;
    TBool PlaySe(const TDesC& aKey, TInt aVolume, TInt aPriority = 0);
    void StopSe();
    TInt WriteStreamPcm16(TStreamBus aBus, const TInt16* aSamples, TInt aFrameCount);
    void ClearStream(TStreamBus aBus);
    void SetStreamVolume(TStreamBus aBus, TInt aVolume);
    void SetMasterVolume(TInt aVolume);
    TInt StreamFreeFrames(TStreamBus aBus) const;
    TInt StreamQueuedFrames(TStreamBus aBus) const;
    void CommitStreams();
    void TrimCache(TInt aByteLimit);
    TInt ConsumeError();
    static TInt OutputSampleRate();

private:
    enum
    {
        KOutputSampleRate = 22050,
        KOutputChannels = 2,
        KOutputBufferBytes = 4096,
        KOutputBufferCount = 7,
        KOutputQueueDepth = 6,
        KOutputFillBurst = 1,
        KStreamBufferFrames = 4096,
        KMaxCachedClips = 128,
        KMaxCachedKeyLength = 160,
        KMaxCachedClipBytes = 1536 * 1024,
        KMaxCachedAudioBytes = 4 * 1024 * 1024,
        KMaxVoices = 16,
        KClipHashBucketCount = 256
    };

    struct TCachedClip
    {
        TBuf<KMaxCachedKeyLength> iKey;
        HBufC8* iPcm16;
        TInt iFrameCount;
        TUint32 iUseSerial;
        TUint32 iKeyHash;
        TInt iHashNext;
        TBool iPinned;
    };

    struct TVoice
    {
        const TCachedClip* iClip;
        TInt iPosition;
        TInt iVolume;
        TInt iPriority;
        TUint32 iSerial;
        TBool iActive;
    };

    struct TStreamState
    {
        TInt16 iSamples[KStreamBufferFrames * KOutputChannels];
        TInt iReadFrame;
        TInt iWriteFrame;
        TInt iQueuedFrames;
        TInt iVolume;
        TBool iActive;
    };

    CAudioMixer();
    void ConstructL();
    TCachedClip* FindCachedClip(const TDesC& aKey);
    const TCachedClip* FindCachedClip(const TDesC& aKey) const;
    void LinkCachedClip(TInt aClipIndex);
    void UnlinkCachedClip(TInt aClipIndex);
    TBool ClipIsActive(TInt aClipIndex) const;
    TInt LeastRecentlyUsedClip() const;
    void ReleaseClip(TInt aClipIndex);
    TInt SelectPlaybackVoice(TInt aPriority) const;
    TBool MixVoice(TVoice& aVoice, TInt* aMixedSamples, TInt aFrameCount);
    TBool MixStream(TStreamState& aStream, TInt* aMixedSamples, TInt aFrameCount);
    static TInt VolumeGain(TInt aVolume, TInt aFullScale);
    static void AccumulateFrames(TInt* aMixedSamples, const TInt16* aSource, TInt aFrameCount, TInt aVolume);
    static void WriteFrames(TInt16* aDestination, const TInt16* aSource, TInt aFrameCount, TInt aVolume);
    TBool FillSingleStreamOutput(TInt aBufferIndex, TInt aFrameCount);
    void FillOutputBuffer(TInt aBufferIndex);
    TInt FreeOutputBufferIndex() const;
    TInt OutputBufferIndex(const TDesC8& aBuffer) const;
    TBool HasAudioToMix() const;
    TBool CanFillOutputBuffer() const;
    void QueueOutputBuffers();
    void ResetOutputQueue();
    static void ClearVoice(TVoice& aVoice);
    static TBool VoiceIsActive(const TVoice& aVoice);
    static TUint32 HashKey(const TDesC& aKey);
    static void WritePcm16(TUint8* aData, TInt aValue);

    CMdaAudioOutputStream* iStream;
    TCachedClip iClips[KMaxCachedClips];
    TInt iClipHashBuckets[KClipHashBucketCount];
    TVoice iVoices[KMaxVoices];
    TStreamState iStreams[EStreamBusCount];
    TBuf8<KOutputBufferBytes> iOutputBuffers[KOutputBufferCount];
    TInt iMixBuffer[KOutputBufferBytes / 2];
    TBool iOutputBufferQueued[KOutputBufferCount];
    TInt iQueuedOutputBufferCount;
    TInt iClipCount;
    TInt iCachedBytes;
    TInt iCacheByteLimit;
    TUint32 iNextSerial;
    TUint32 iClipUseSerial;
    TBool iReady;
    TBool iShuttingDown;
    TInt iLastError;
    TInt iMasterVolume;

    void MaoscOpenComplete(TInt aError);
    void MaoscBufferCopied(TInt aError, const TDesC8& aBuffer);
    void MaoscPlayComplete(TInt aError);
};

#endif

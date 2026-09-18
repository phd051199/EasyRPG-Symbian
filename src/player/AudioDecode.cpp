#include "AudioDecode.h"

namespace
{
TUint16 ReadLittleEndian16(const TUint8 *aData)
{
    return static_cast<TUint16>(aData[0]) |
           static_cast<TUint16>(static_cast<TUint16>(aData[1]) << 8);
}

TUint32 ReadLittleEndian32(const TUint8 *aData)
{
    return static_cast<TUint32>(aData[0]) |
           (static_cast<TUint32>(aData[1]) << 8) |
           (static_cast<TUint32>(aData[2]) << 16) |
           (static_cast<TUint32>(aData[3]) << 24);
}

TInt ClampPcm16(TInt64 aValue)
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

TInt DecodeAdpcmNibble(TAdpcmState &aState, TUint8 aNibble)
{
    static const TInt KAdaptation[16] =
        {
            230,
            230,
            230,
            230,
            307,
            409,
            512,
            614,
            768,
            614,
            512,
            409,
            307,
            230,
            230,
            230};
    const TInt signedNibble = (aNibble & 8u)
                                  ? static_cast<TInt>(aNibble) - 16
                                  : aNibble;
    const TInt64 predictor =
        static_cast<TInt64>(aState.iSample1) * aState.iCoef1 +
        static_cast<TInt64>(aState.iSample2) * aState.iCoef2;
    const TInt prediction = ClampPcm16(
        predictor / 256 + static_cast<TInt64>(signedNibble) * aState.iDelta);
    aState.iSample2 = aState.iSample1;
    aState.iSample1 = prediction;
    TInt64 nextDelta =
        static_cast<TInt64>(KAdaptation[aNibble & 15u]) * aState.iDelta / 256;
    if (nextDelta < 16)
        nextDelta = 16;
    if (nextDelta > KMaxTInt)
        nextDelta = KMaxTInt;
    aState.iDelta = static_cast<TInt>(nextDelta);
    return prediction;
}

void WritePcm16Sample(TUint8 *aDestination, TInt &aSampleIndex, TInt aValue)
{
    const TUint16 encoded = static_cast<TUint16>(static_cast<TInt16>(aValue));
    aDestination[aSampleIndex * 2] = static_cast<TUint8>(encoded & 0xffu);
    aDestination[aSampleIndex * 2 + 1] = static_cast<TUint8>(encoded >> 8);
    ++aSampleIndex;
}

HBufC8 *DecodeMsAdpcmLC(const TUint8 *aFormat,
                        TUint32 aFormatBytes,
                        const TUint8 *aData,
                        TUint32 aDataBytes,
                        TInt &aFrameCount,
                        TInt &aSampleRate,
                        TInt &aChannels)
{
    if (!aFormat || aFormatBytes < 22 || !aData || aDataBytes == 0)
        User::Leave(KErrCorrupt);

    const TInt channels = ReadLittleEndian16(aFormat + 2);
    const TUint32 sampleRate = ReadLittleEndian32(aFormat + 4);
    const TInt blockAlign = ReadLittleEndian16(aFormat + 12);
    const TInt bits = ReadLittleEndian16(aFormat + 14);
    const TInt samplesPerBlock = ReadLittleEndian16(aFormat + 18);
    const TInt coefficientCount = ReadLittleEndian16(aFormat + 20);
    if ((channels != 1 && channels != 2) || sampleRate == 0 ||
        sampleRate > static_cast<TUint32>(KMaxTInt) ||
        blockAlign <= channels * 7 || bits != 4 || samplesPerBlock < 2 ||
        coefficientCount <= 0 ||
        22u + static_cast<TUint32>(coefficientCount) * 4u > aFormatBytes)
        User::Leave(KErrNotSupported);

    const TUint32 blockCount =
        (aDataBytes + static_cast<TUint32>(blockAlign) - 1u) /
        static_cast<TUint32>(blockAlign);
    const TInt64 maxFrames64 = static_cast<TInt64>(blockCount) * samplesPerBlock;
    if (maxFrames64 <= 0 || maxFrames64 > KMaxTInt / (channels * 2))
        User::Leave(KErrTooBig);

    HBufC8 *pcm = HBufC8::NewLC(static_cast<TInt>(maxFrames64) * channels * 2);
    TPtr8 output = pcm->Des();
    output.SetLength(output.MaxLength());
    TUint8 *destination = const_cast<TUint8 *>(output.Ptr());
    TInt writeSample = 0;
    TUint32 dataOffset = 0;

    while (dataOffset < aDataBytes)
    {
        TUint32 blockBytes = aDataBytes - dataOffset;
        if (blockBytes > static_cast<TUint32>(blockAlign))
            blockBytes = blockAlign;
        const TUint8 *block = aData + dataOffset;
        const TInt headerBytes = channels * 7;
        if (blockBytes < static_cast<TUint32>(headerBytes))
            User::Leave(KErrCorrupt);

        TAdpcmState state[2];
        TInt channel = 0;
        for (channel = 0; channel < channels; ++channel)
        {
            const TInt predictor = block[channel];
            if (predictor >= coefficientCount)
                User::Leave(KErrCorrupt);
            const TUint8 *coefficient = aFormat + 22 + predictor * 4;
            state[channel].iCoef1 = static_cast<TInt16>(ReadLittleEndian16(coefficient));
            state[channel].iCoef2 = static_cast<TInt16>(ReadLittleEndian16(coefficient + 2));
        }

        TInt cursor = channels;
        for (channel = 0; channel < channels; ++channel)
        {
            state[channel].iDelta = static_cast<TInt16>(ReadLittleEndian16(block + cursor));
            if (state[channel].iDelta < 16)
                state[channel].iDelta = 16;
            cursor += 2;
        }
        for (channel = 0; channel < channels; ++channel)
        {
            state[channel].iSample1 = static_cast<TInt16>(ReadLittleEndian16(block + cursor));
            cursor += 2;
        }
        for (channel = 0; channel < channels; ++channel)
        {
            state[channel].iSample2 = static_cast<TInt16>(ReadLittleEndian16(block + cursor));
            cursor += 2;
        }

        for (channel = 0; channel < channels; ++channel)
            WritePcm16Sample(destination, writeSample, state[channel].iSample2);
        for (channel = 0; channel < channels; ++channel)
            WritePcm16Sample(destination, writeSample, state[channel].iSample1);

        TInt framesWritten = 2;
        if (channels == 2)
        {
            while (framesWritten < samplesPerBlock && cursor < static_cast<TInt>(blockBytes))
            {
                const TUint8 value = block[cursor++];
                WritePcm16Sample(destination,
                                 writeSample,
                                 DecodeAdpcmNibble(state[0], value >> 4));
                WritePcm16Sample(destination,
                                 writeSample,
                                 DecodeAdpcmNibble(state[1], value & 15u));
                ++framesWritten;
            }
        }
        else
        {
            while (framesWritten < samplesPerBlock && cursor < static_cast<TInt>(blockBytes))
            {
                const TUint8 value = block[cursor++];
                WritePcm16Sample(destination,
                                 writeSample,
                                 DecodeAdpcmNibble(state[0], value >> 4));
                ++framesWritten;
                if (framesWritten < samplesPerBlock)
                {
                    WritePcm16Sample(destination,
                                     writeSample,
                                     DecodeAdpcmNibble(state[0], value & 15u));
                    ++framesWritten;
                }
            }
        }

        dataOffset += blockBytes;
    }

    output.SetLength(writeSample * 2);
    aFrameCount = writeSample / channels;
    aSampleRate = static_cast<TInt>(sampleRate);
    aChannels = channels;
    return pcm;
}
} // namespace

HBufC8 *TAudioDecode::DecodeWavePcm16LC(const TDesC8 &aFile,
                                        TInt &aFrameCount,
                                        TInt &aSampleRate,
                                        TInt &aChannels)
{
    aFrameCount = 0;
    aSampleRate = 0;
    aChannels = 0;

    if (aFile.Length() < 12 ||
        Mem::Compare(aFile.Ptr(), 4, reinterpret_cast<const TUint8 *>("RIFF"), 4) != 0 ||
        Mem::Compare(aFile.Ptr() + 8, 4, reinterpret_cast<const TUint8 *>("WAVE"), 4) != 0)
        User::Leave(KErrCorrupt);

    TUint16 format = 0;
    const TUint8 *formatData = NULL;
    TUint32 formatBytes = 0;
    TUint16 channels = 0;
    TUint32 sampleRate = 0;
    TUint16 blockAlign = 0;
    TUint16 bits = 0;
    const TUint8 *sampleData = NULL;
    TUint32 sampleBytes = 0;

    TInt position = 12;
    while (position <= aFile.Length() - 8)
    {
        const TUint8 *chunk = aFile.Ptr() + position;
        const TUint32 chunkSize = ReadLittleEndian32(chunk + 4);
        position += 8;
        if (chunkSize > static_cast<TUint32>(aFile.Length() - position))
            User::Leave(KErrCorrupt);

        if (Mem::Compare(chunk, 4, reinterpret_cast<const TUint8 *>("fmt "), 4) == 0)
        {
            if (chunkSize < 16)
                User::Leave(KErrCorrupt);
            const TUint8 *fmt = aFile.Ptr() + position;
            formatData = fmt;
            formatBytes = chunkSize;
            format = ReadLittleEndian16(fmt);
            channels = ReadLittleEndian16(fmt + 2);
            sampleRate = ReadLittleEndian32(fmt + 4);
            blockAlign = ReadLittleEndian16(fmt + 12);
            bits = ReadLittleEndian16(fmt + 14);
        }
        else if (Mem::Compare(chunk, 4, reinterpret_cast<const TUint8 *>("data"), 4) == 0)
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
        return DecodeMsAdpcmLC(formatData,
                               formatBytes,
                               sampleData,
                               sampleBytes,
                               aFrameCount,
                               aSampleRate,
                               aChannels);

    if (format != 1 || !sampleData ||
        (channels != 1 && channels != 2) ||
        bits != 16 || sampleRate == 0 ||
        sampleRate > static_cast<TUint32>(KMaxTInt))
        User::Leave(KErrNotSupported);

    const TInt expectedAlign = channels * 2;
    if (blockAlign != expectedAlign || blockAlign == 0 || sampleBytes % blockAlign != 0)
        User::Leave(KErrCorrupt);

    const TUint32 frames = sampleBytes / blockAlign;
    if (frames == 0 || frames > static_cast<TUint32>(KMaxTInt / (channels * 2)))
        User::Leave(KErrTooBig);

    const TInt pcmBytes = static_cast<TInt>(frames) * channels * 2;
    HBufC8 *pcm = HBufC8::NewLC(pcmBytes);
    TPtr8 output = pcm->Des();
    output.SetLength(pcmBytes);
    TUint8 *destination = const_cast<TUint8 *>(output.Ptr());
    Mem::Copy(destination, sampleData, pcmBytes);

    aFrameCount = static_cast<TInt>(frames);
    aSampleRate = static_cast<TInt>(sampleRate);
    aChannels = channels;
    return pcm;
}

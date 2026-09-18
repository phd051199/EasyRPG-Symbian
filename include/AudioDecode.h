#ifndef EASYRPG_AUDIO_DECODE_H
#define EASYRPG_AUDIO_DECODE_H

#include <e32base.h>

class TAudioDecode
{
  public:
    static HBufC8 *DecodeWavePcm16LC(const TDesC8 &aFile,
                                     TInt &aFrameCount,
                                     TInt &aSampleRate,
                                     TInt &aChannels);
};

#endif

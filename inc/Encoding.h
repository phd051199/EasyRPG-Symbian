#ifndef EASYRPG_ENCODING_H
#define EASYRPG_ENCODING_H

#include <e32base.h>

class TEncoding
{
public:
    static void ShiftJisToUnicodeL(const TDesC8& aSource,
                                   TDes& aDestination);
    static void Utf8ToUnicodeL(const TDesC8& aSource,
                               TDes& aDestination);
    static void UnicodeToUtf8L(const TDesC& aSource,
                               TDes8& aDestination);
    static void NormalizeJapaneseNfcInPlace(TDes& aText);
};

#endif

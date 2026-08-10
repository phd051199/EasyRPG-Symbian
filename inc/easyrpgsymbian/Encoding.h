#ifndef EASYRPG_SYMBIAN_ENCODING_H
#define EASYRPG_SYMBIAN_ENCODING_H

#include <e32base.h>
#include <f32file.h>

class TEncoding
{
public:
    static void ShiftJisToUnicodeL(RFs& aFs,
                                   const TDesC8& aSource,
                                   TDes& aDestination);
};

#endif

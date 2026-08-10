#include <charconv.h>

#include "easyrpgsymbian/Encoding.h"

void TEncoding::ShiftJisToUnicodeL(RFs& aFs,
                                   const TDesC8& aSource,
                                   TDes& aDestination)
{
    aDestination.Zero();
    if (aSource.Length() == 0)
        return;

    CCnvCharacterSetConverter* converter = CCnvCharacterSetConverter::NewLC();
    const CCnvCharacterSetConverter::TAvailability availability =
        converter->PrepareToConvertToOrFromL(KCharacterSetIdentifierShiftJis, aFs);
    if (availability != CCnvCharacterSetConverter::EAvailable)
        User::Leave(KErrNotSupported);

    TInt state = CCnvCharacterSetConverter::KStateDefault;
    TInt sourceOffset = 0;
    while (sourceOffset < aSource.Length())
    {
        const TPtrC8 remainingSource = aSource.Mid(sourceOffset);
        TBuf<128> convertedText;
        const TInt unconverted = converter->ConvertToUnicode(convertedText,
                                                              remainingSource,
                                                              state);
        if (unconverted < 0)
            User::Leave(unconverted);

        if (convertedText.Length() > aDestination.MaxLength() - aDestination.Length())
            User::Leave(KErrOverflow);
        aDestination.Append(convertedText);

        const TInt convertedBytes = remainingSource.Length() - unconverted;
        if (convertedBytes <= 0)
            User::Leave(KErrCorrupt);
        sourceOffset += convertedBytes;
    }

    CleanupStack::PopAndDestroy(converter);
}

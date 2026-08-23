#include "Encoding.h"
#include "CoreEncoding.h"

#include <charconv.h>
#include <utf.h>
#include <f32file.h>

namespace
{
RFs gLegacyEncodingFs;
TBool gLegacyEncodingFsConnected = EFalse;
CCnvCharacterSetConverter* gLegacyEncodingConverter = NULL;
CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* gLegacyEncodingCharsets = NULL;
TUint gLegacyEncodingPrepared = 0;

TUint CharacterSetForCodepage(TUint aCodepage)
{
    switch (aCodepage)
    {
    case 932: return KCharacterSetIdentifierShiftJis;
    case 936: return KCharacterSetIdentifierGbk;
    case 950: return KCharacterSetIdentifierBig5;
    default: return 0;
    }
}

void EnsureLegacyEncodingL()
{
    if (!gLegacyEncodingFsConnected)
    {
        User::LeaveIfError(gLegacyEncodingFs.Connect());
        gLegacyEncodingFsConnected = ETrue;
    }
    if (!gLegacyEncodingConverter)
        gLegacyEncodingConverter = CCnvCharacterSetConverter::NewL();
    if (!gLegacyEncodingCharsets)
        gLegacyEncodingCharsets = CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableL(gLegacyEncodingFs);
}

TBool HasLegacyEncoding(TUint aIdentifier)
{
    if (!gLegacyEncodingCharsets)
        return EFalse;
    for (TInt index = 0; index < gLegacyEncodingCharsets->Count(); ++index)
    {
        if (gLegacyEncodingCharsets->At(index).Identifier() == aIdentifier)
            return ETrue;
    }
    return EFalse;
}

void PrepareLegacyEncodingL(TUint aCodepage)
{
    EnsureLegacyEncodingL();
    const TUint identifier = CharacterSetForCodepage(aCodepage);
    if (!identifier || !HasLegacyEncoding(identifier))
        User::Leave(KErrNotSupported);
    if (gLegacyEncodingPrepared == identifier)
        return;
    gLegacyEncodingConverter->PrepareToConvertToOrFromL(identifier, *gLegacyEncodingCharsets, gLegacyEncodingFs);
    gLegacyEncodingPrepared = identifier;
}

#ifndef EASYRPG_LAUNCHER_PROCESS
TInt CoreEncodingStatus(TInt aError)
{
    if (aError == KErrNone)
        return 0;
    if (aError == KErrOverflow)
        return -2;
    return -1;
}

TUint CodepageForCharacterSet(TUint aIdentifier)
{
    switch (aIdentifier)
    {
    case KCharacterSetIdentifierShiftJis: return 932;
    case KCharacterSetIdentifierGbk: return 936;
    case KCharacterSetIdentifierBig5: return 950;
    case KCharacterSetIdentifierUtf8: return 65001;
    default: return 0;
    }
}

void DetectLegacyEncodingL(const TDesC8& aSample,
                           unsigned int& aCodepage,
                           int& aConfidence)
{
    EnsureLegacyEncodingL();
    TUint identifier = 0;
    TInt confidence = 0;
    CCnvCharacterSetConverter::AutoDetectCharacterSetL(
        confidence, identifier, *gLegacyEncodingCharsets, aSample);
    aCodepage = CodepageForCharacterSet(identifier);
    aConfidence = confidence;
}

void LegacyToUtf16L(TUint aCodepage,
                    const unsigned char* aSource,
                    TUint aSourceLength,
                    unsigned short* aDestination,
                    TUint aDestinationCapacity,
                    unsigned int& aDestinationLength)
{
    PrepareLegacyEncodingL(aCodepage);
    TPtrC8 source(reinterpret_cast<const TUint8*>(aSource), static_cast<TInt>(aSourceLength));
    TPtr16 destination(reinterpret_cast<TUint16*>(aDestination), 0, static_cast<TInt>(aDestinationCapacity));
    TInt state = CCnvCharacterSetConverter::KStateDefault;
    TInt unconvertible = 0;
    const TInt remaining = gLegacyEncodingConverter->ConvertToUnicode(destination, source, state, unconvertible);
    if (remaining < 0)
        User::Leave(remaining);
    if (remaining != 0)
        User::Leave(KErrOverflow);
    if (unconvertible != 0)
        User::Leave(KErrCorrupt);
    aDestinationLength = static_cast<unsigned int>(destination.Length());
}

void Utf16ToLegacyL(TUint aCodepage,
                    const unsigned short* aSource,
                    TUint aSourceLength,
                    unsigned char* aDestination,
                    TUint aDestinationCapacity,
                    unsigned int& aDestinationLength)
{
    PrepareLegacyEncodingL(aCodepage);
    TPtrC16 source(reinterpret_cast<const TUint16*>(aSource), static_cast<TInt>(aSourceLength));
    TPtr8 destination(reinterpret_cast<TUint8*>(aDestination), 0, static_cast<TInt>(aDestinationCapacity));
    TInt unconvertible = 0;
    const TInt remaining = gLegacyEncodingConverter->ConvertFromUnicode(destination, source, unconvertible);
    if (remaining < 0)
        User::Leave(remaining);
    if (remaining != 0)
        User::Leave(KErrOverflow);
    if (unconvertible != 0)
        User::Leave(KErrCorrupt);
    aDestinationLength = static_cast<unsigned int>(destination.Length());
}
#endif
TUint16 ComposeJapaneseKana(TUint16 aBase, TUint16 aMark)
{
    const TBool dakuten = (aMark == 0x3099);
    const TBool handakuten = (aMark == 0x309a);
    if (!dakuten && !handakuten)
        return 0;

    if (dakuten)
    {
        if (aBase == 0x3046) return 0x3094;
        if (aBase == 0x309d) return 0x309e;
        if (aBase == 0x30a6) return 0x30f4;
        if (aBase == 0x30fd) return 0x30fe;
        if (aBase >= 0x30ef && aBase <= 0x30f2)
            return static_cast<TUint16>(0x30f7 + (aBase - 0x30ef));
    }

    const TUint16 hiraganaDakuten[] =
    {
        0x304b,0x304d,0x304f,0x3051,0x3053,
        0x3055,0x3057,0x3059,0x305b,0x305d,
        0x305f,0x3061,0x3064,0x3066,0x3068
    };
    const TUint16 katakanaDakuten[] =
    {
        0x30ab,0x30ad,0x30af,0x30b1,0x30b3,
        0x30b5,0x30b7,0x30b9,0x30bb,0x30bd,
        0x30bf,0x30c1,0x30c4,0x30c6,0x30c8
    };

    if (dakuten)
    {
        for (TInt index = 0; index < 15; ++index)
        {
            if (aBase == hiraganaDakuten[index] || aBase == katakanaDakuten[index])
                return static_cast<TUint16>(aBase + 1);
        }
    }

    const TUint16 hiraganaH[] = {0x306f,0x3072,0x3075,0x3078,0x307b};
    const TUint16 katakanaH[] = {0x30cf,0x30d2,0x30d5,0x30d8,0x30db};
    for (TInt index = 0; index < 5; ++index)
    {
        if (aBase == hiraganaH[index] || aBase == katakanaH[index])
            return static_cast<TUint16>(aBase + (dakuten ? 1 : 2));
    }

    return 0;
}
}

#ifndef EASYRPG_LAUNCHER_PROCESS
extern "C" int EasyRpgPlatformDetectLegacyEncoding(const unsigned char* aSample,
                                                     unsigned int aSampleLength,
                                                     unsigned int* aCodepage,
                                                     int* aConfidence)
{
    if (!aCodepage || !aConfidence || (!aSample && aSampleLength != 0))
        return -1;
    *aCodepage = 0;
    *aConfidence = 0;
    if (aSampleLength == 0)
        return 0;

    const TPtrC8 sample(reinterpret_cast<const TUint8*>(aSample), static_cast<TInt>(aSampleLength));
    unsigned int codepage = 0;
    int confidence = 0;
    TInt error = KErrNone;
    TRAP(error, DetectLegacyEncodingL(sample, codepage, confidence));
    if (error == KErrNone)
    {
        *aCodepage = codepage;
        *aConfidence = confidence;
    }
    return CoreEncodingStatus(error);
}

extern "C" int EasyRpgPlatformLegacyToUtf16(unsigned int aCodepage,
                                             const unsigned char* aSource,
                                             unsigned int aSourceLength,
                                             unsigned short* aDestination,
                                             unsigned int aDestinationCapacity,
                                             unsigned int* aDestinationLength)
{
    if (!aDestinationLength || (!aSource && aSourceLength != 0) ||
        (!aDestination && aDestinationCapacity != 0))
        return -1;
    *aDestinationLength = 0;
    unsigned int written = 0;
    TInt error = KErrNone;
    TRAP(error, LegacyToUtf16L(aCodepage, aSource, aSourceLength,
                               aDestination, aDestinationCapacity, written));
    if (error == KErrNone)
        *aDestinationLength = written;
    return CoreEncodingStatus(error);
}

extern "C" int EasyRpgPlatformUtf16ToLegacy(unsigned int aCodepage,
                                             const unsigned short* aSource,
                                             unsigned int aSourceLength,
                                             unsigned char* aDestination,
                                             unsigned int aDestinationCapacity,
                                             unsigned int* aDestinationLength)
{
    if (!aDestinationLength || (!aSource && aSourceLength != 0) ||
        (!aDestination && aDestinationCapacity != 0))
        return -1;
    *aDestinationLength = 0;
    unsigned int written = 0;
    TInt error = KErrNone;
    TRAP(error, Utf16ToLegacyL(aCodepage, aSource, aSourceLength,
                               aDestination, aDestinationCapacity, written));
    if (error == KErrNone)
        *aDestinationLength = written;
    return CoreEncodingStatus(error);
}
#endif

void TEncoding::ShiftJisToUnicodeL(const TDesC8& aSource,
                                   TDes& aDestination)
{
    aDestination.Zero();
    if (aSource.Length() == 0)
        return;

#ifdef EASYRPG_LAUNCHER_PROCESS
    PrepareLegacyEncodingL(932);
    TInt state = CCnvCharacterSetConverter::KStateDefault;
    TInt unconvertible = 0;
    const TInt remaining = gLegacyEncodingConverter->ConvertToUnicode(
        aDestination, aSource, state, unconvertible);
    if (remaining < 0)
        User::Leave(remaining);
    if (remaining != 0)
        User::Leave(KErrOverflow);
    if (unconvertible != 0)
        User::Leave(KErrCorrupt);
#else
    unsigned int written = 0;
    TUint16* destination = const_cast<TUint16*>(aDestination.Ptr());
    const TInt result = EasyRpgCoreCp932ToUtf16(
        reinterpret_cast<const unsigned char*>(aSource.Ptr()),
        static_cast<unsigned int>(aSource.Length()),
        reinterpret_cast<unsigned short*>(destination),
        static_cast<unsigned int>(aDestination.MaxLength()),
        &written);

    if (result == -2)
        User::Leave(KErrOverflow);
    if (result != 0)
        User::Leave(KErrCorrupt);
    if (written > static_cast<unsigned int>(aDestination.MaxLength()))
        User::Leave(KErrOverflow);

    aDestination.SetLength(static_cast<TInt>(written));
#endif
    NormalizeJapaneseNfcInPlace(aDestination);
}

void TEncoding::Utf8ToUnicodeL(const TDesC8& aSource,
                               TDes& aDestination)
{
    aDestination.Zero();
    if (aSource.Length() == 0)
        return;

    const TInt remaining = CnvUtfConverter::ConvertToUnicodeFromUtf8(aDestination, aSource);
    if (remaining < 0)
        User::Leave(KErrCorrupt);
    if (remaining != 0)
        User::Leave(KErrOverflow);

    NormalizeJapaneseNfcInPlace(aDestination);
}

void TEncoding::Utf8CStringToUnicodeL(const char* aSource,
                                      TDes& aDestination)
{
    if (!aSource)
        User::Leave(KErrArgument);

    const TInt maxSourceBytes = aDestination.MaxLength() > KMaxTInt / 4
        ? KMaxTInt
        : aDestination.MaxLength() * 4;
    TInt length = 0;
    while (length < maxSourceBytes && aSource[length] != 0)
        ++length;
    if (length == maxSourceBytes)
        User::Leave(KErrOverflow);
    const TPtrC8 source(reinterpret_cast<const TUint8*>(aSource), length);
    Utf8ToUnicodeL(source, aDestination);
}

void TEncoding::UnicodeToUtf8L(const TDesC& aSource,
                               TDes8& aDestination)
{
    aDestination.Zero();
    const TInt remaining = CnvUtfConverter::ConvertFromUnicodeToUtf8(aDestination, aSource);
    if (remaining < 0)
        User::Leave(KErrCorrupt);
    if (remaining != 0)
        User::Leave(KErrOverflow);
}

void TEncoding::UnicodeToUtf8CStringL(const TDesC& aSource,
                                      TDes8& aDestination)
{
    UnicodeToUtf8L(aSource, aDestination);
    if (aDestination.Length() >= aDestination.MaxLength())
        User::Leave(KErrOverflow);
    aDestination.Append(static_cast<TUint8>(0));
}

void TEncoding::NormalizeJapaneseNfcInPlace(TDes& aText)
{
    TInt writePosition = 0;
    for (TInt readPosition = 0; readPosition < aText.Length(); ++readPosition)
    {
        const TUint16 value = aText[readPosition];
        if ((value == 0x3099 || value == 0x309a) && writePosition > 0)
        {
            const TUint16 composed = ComposeJapaneseKana(aText[writePosition - 1], value);
            if (composed != 0)
            {
                aText[writePosition - 1] = composed;
                continue;
            }
        }
        aText[writePosition++] = value;
    }
    aText.SetLength(writePosition);
}

void TEncoding::Shutdown()
{
    delete gLegacyEncodingConverter;
    gLegacyEncodingConverter = NULL;
    delete gLegacyEncodingCharsets;
    gLegacyEncodingCharsets = NULL;
    gLegacyEncodingPrepared = 0;
    if (gLegacyEncodingFsConnected)
    {
        gLegacyEncodingFs.Close();
        gLegacyEncodingFsConnected = EFalse;
    }
}

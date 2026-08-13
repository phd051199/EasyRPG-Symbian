#include "GameMetadata.h"
#include "ZipVfs.h"

namespace
{
const TInt KMaxMetadataLdbSize = 4 * 1024 * 1024;
_LIT8(KLcfDatabaseHeader, "LcfDataBase");

void CopyListText(const TDesC& aSource, TDes& aTarget)
{
    aTarget.Copy(aSource.Left(aTarget.MaxLength()));
    for (TInt index = 0; index < aTarget.Length(); ++index)
    {
        if (aTarget[index] == '\t' || aTarget[index] == '\r' ||
            aTarget[index] == '\n')
            aTarget[index] = ' ';
    }
}

TBool ReadLcfInt(const TDesC8& aBytes,
                 TInt& aPosition,
                 TInt aEnd,
                 TInt& aValue)
{
    aValue = 0;
    TInt count = 0;
    while (aPosition < aEnd && count < 5)
    {
        const TUint8 value = aBytes[aPosition++];
        if (aValue > (KMaxTInt >> 7))
            return EFalse;
        aValue = (aValue << 7) | (value & 0x7f);
        ++count;
        if ((value & 0x80) == 0)
            return ETrue;
    }
    return EFalse;
}

TInt DetectSystemEngine(const TDesC8& aBytes, TInt aStart, TInt aEnd)
{
    TInt position = aStart;
    while (position < aEnd)
    {
        TInt fieldId = 0;
        if (!ReadLcfInt(aBytes, position, aEnd, fieldId))
            return 0;
        if (fieldId == 0)
            return 2000;

        TInt fieldLength = 0;
        if (!ReadLcfInt(aBytes, position, aEnd, fieldLength) ||
            fieldLength < 0 || position > aEnd - fieldLength)
            return 0;

        const TInt fieldEnd = position + fieldLength;
        if (fieldId == 0x0a)
        {
            TInt ldbId = 0;
            if (!ReadLcfInt(aBytes, position, fieldEnd, ldbId))
                return 0;
            return ldbId == 2003 ? 2003 : 2000;
        }
        position = fieldEnd;
    }
    return 2000;
}

TInt DetectEngineSignature(const CZipVfs& aResources)
{
    const TBool hasLdb = aResources.Exists(_L("RPG_RT.ldb"));
    const TBool hasLmt = aResources.Exists(_L("RPG_RT.lmt"));
    const TBool hasXp = aResources.Exists(_L("Data/Scripts.rxdata"));
    const TBool hasVx = aResources.Exists(_L("Data/Scripts.rvdata"));
    const TBool hasVxa = aResources.Exists(_L("Data/Scripts.rvdata2"));
    const TInt rgssMatches = static_cast<TInt>(hasXp) +
                             static_cast<TInt>(hasVx) +
                             static_cast<TInt>(hasVxa);
    if (rgssMatches > 1 || ((hasLdb && hasLmt) && rgssMatches != 0))
        return EGameEngineUnknown;
    if (hasXp)
        return EGameEngineRpgXp;
    if (hasVx)
        return EGameEngineRpgVx;
    if (hasVxa)
        return EGameEngineRpgVxAce;
    if (hasLdb && hasLmt)
        return EGameEngineRpg2000;
    return EGameEngineUnknown;
}

TInt DetectRpgMakerVersionL(CZipVfs& aResources)
{
    HBufC8* data = aResources.ReadFileLC(_L("RPG_RT.ldb"), KMaxMetadataLdbSize);
    const TPtrC8 bytes(*data);

    TInt position = 0;
    TInt headerLength = 0;
    TInt version = 0;
    if (ReadLcfInt(bytes, position, bytes.Length(), headerLength) &&
        headerLength == KLcfDatabaseHeader().Length() &&
        position <= bytes.Length() - headerLength &&
        bytes.Mid(position, headerLength).Compare(KLcfDatabaseHeader) == 0)
    {
        position += headerLength;
        while (position < bytes.Length())
        {
            TInt fieldId = 0;
            if (!ReadLcfInt(bytes, position, bytes.Length(), fieldId) || fieldId == 0)
                break;

            TInt fieldLength = 0;
            if (!ReadLcfInt(bytes, position, bytes.Length(), fieldLength) ||
                fieldLength < 0 || position > bytes.Length() - fieldLength)
                break;

            const TInt fieldEnd = position + fieldLength;
            if (fieldId == 0x16)
            {
                version = DetectSystemEngine(bytes, position, fieldEnd);
                break;
            }
            position = fieldEnd;
        }
    }

    CleanupStack::PopAndDestroy(data);
    return version == EGameEngineRpg2003 ? EGameEngineRpg2003 : EGameEngineRpg2000;
}

void ReadIniTitleL(CZipVfs& aResources,
                   const TDesC& aFileName,
                   const TDesC8& aKey,
                   TDes& aTitle)
{
    HBufC8* data = aResources.ReadFileLC(aFileName, 4096);
    const TPtrC8 bytes(*data);
    const TInt keyPosition = bytes.Find(aKey);
    if (keyPosition != KErrNotFound)
    {
        TBuf<KMaxFileName> title;
        TInt position = keyPosition + aKey.Length();
        while (position < bytes.Length() && title.Length() < title.MaxLength())
        {
            const TUint8 value = bytes[position++];
            if (value == '\r' || value == '\n')
                break;
            if (value < 0x20 || value > 0x7e)
            {
                title.Zero();
                break;
            }
            title.Append(static_cast<TUint16>(value));
        }
        if (title.Length() > 0)
            CopyListText(title, aTitle);
    }
    CleanupStack::PopAndDestroy(data);
}

void ReadGameTitleL(CZipVfs& aResources, TInt aVersion, TDes& aTitle)
{
    if (aVersion == EGameEngineRpgXp ||
        aVersion == EGameEngineRpgVx ||
        aVersion == EGameEngineRpgVxAce)
    {
        _LIT8(KTitleKey, "Title=");
        ReadIniTitleL(aResources, _L("Game.ini"), KTitleKey, aTitle);
        return;
    }

    _LIT8(KGameTitleKey, "GameTitle=");
    ReadIniTitleL(aResources, _L("RPG_RT.ini"), KGameTitleKey, aTitle);
}
}

void ReadGameMetadataL(RFs& aFs,
                       const TDesC& aArchivePath,
                       TDes& aTitle,
                       TInt& aVersion)
{
    CZipVfs* resources = CZipVfs::OpenL(aFs, aArchivePath);
    CleanupStack::PushL(resources);
    aVersion = DetectEngineSignature(*resources);
    if (aVersion == EGameEngineRpg2000)
        TRAP_IGNORE(aVersion = DetectRpgMakerVersionL(*resources));
    TRAP_IGNORE(ReadGameTitleL(*resources, aVersion, aTitle));
    CleanupStack::PopAndDestroy(resources);
}

void FormatGameSubtitle(RFs& aFs,
                        const TDesC& aArchivePath,
                        TInt aVersion,
                        TDes& aSubtitle)
{
    TEntry entry;
    TInt64 bytes = 0;
    if (aFs.Entry(aArchivePath, entry) == KErrNone && !entry.IsDir())
        bytes = entry.iSize;

    const TInt64 tenths = (bytes * 10 + 524288) / 1048576;
    const TInt whole = static_cast<TInt>(tenths / 10);
    const TInt fraction = static_cast<TInt>(tenths % 10);
    if (aVersion == EGameEngineRpg2000 || aVersion == EGameEngineRpg2003)
        aSubtitle.Format(_L("%d.%d MB (RPG Maker %d)"), whole, fraction, aVersion);
    else if (aVersion == EGameEngineRpgXp)
        aSubtitle.Format(_L("%d.%d MB (RPG Maker XP)"), whole, fraction);
    else if (aVersion == EGameEngineRpgVx)
        aSubtitle.Format(_L("%d.%d MB (RPG Maker VX)"), whole, fraction);
    else if (aVersion == EGameEngineRpgVxAce)
        aSubtitle.Format(_L("%d.%d MB (RPG Maker VX Ace)"), whole, fraction);
    else
        aSubtitle.Format(_L("%d.%d MB (Unknown RPG Maker)"), whole, fraction);
}

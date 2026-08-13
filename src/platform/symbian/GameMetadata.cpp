#include "GameMetadata.h"
#include "ZipVfs.h"

namespace
{
const TInt KMaxMetadataLdbSize = 4 * 1024 * 1024;
const TUint32 KZipCentralHeaderSignature = 0x02014b50u;
const TUint32 KZipEndSignature = 0x06054b50u;
const TInt KZipEndHeaderBytes = 22;
const TInt KZipMaxCommentBytes = 65535;
_LIT8(KLcfDatabaseHeader, "LcfDataBase");

TUint16 ReadZip16(const TUint8* aBytes)
{
    return static_cast<TUint16>(aBytes[0]) |
           static_cast<TUint16>(static_cast<TUint16>(aBytes[1]) << 8);
}

TUint32 ReadZip32(const TUint8* aBytes)
{
    return static_cast<TUint32>(aBytes[0]) |
           (static_cast<TUint32>(aBytes[1]) << 8) |
           (static_cast<TUint32>(aBytes[2]) << 16) |
           (static_cast<TUint32>(aBytes[3]) << 24);
}

TUint8 FoldZipPathChar(TUint8 aValue)
{
    if (aValue == '\\')
        return '/';
    if (aValue >= 'A' && aValue <= 'Z')
        return static_cast<TUint8>(aValue + ('a' - 'A'));
    return aValue;
}

TBool RawPathEndsWith(const TDesC8& aPath, const TDesC8& aSuffix)
{
    if (aPath.Length() < aSuffix.Length())
        return EFalse;
    const TInt offset = aPath.Length() - aSuffix.Length();
    for (TInt index = 0; index < aSuffix.Length(); ++index)
    {
        if (FoldZipPathChar(aPath[offset + index]) !=
            FoldZipPathChar(aSuffix[index]))
            return EFalse;
    }
    if (offset == 0)
        return ETrue;
    return FoldZipPathChar(aPath[offset - 1]) == '/';
}

TInt DetectEngineFromZipDirectoryL(RFs& aFs, const TDesC& aArchivePath)
{
    RFile archive;
    User::LeaveIfError(archive.Open(aFs,
                                    aArchivePath,
                                    EFileRead | EFileShareReadersOnly));
    CleanupClosePushL(archive);

    TInt fileSize = 0;
    User::LeaveIfError(archive.Size(fileSize));
    if (fileSize < KZipEndHeaderBytes)
        User::Leave(KErrCorrupt);

    TInt tailSize = KZipEndHeaderBytes + KZipMaxCommentBytes;
    if (tailSize > fileSize)
        tailSize = fileSize;
    HBufC8* tail = HBufC8::NewLC(tailSize);
    TPtr8 tailBytes = tail->Des();
    User::LeaveIfError(archive.Read(fileSize - tailSize, tailBytes, tailSize));
    if (tailBytes.Length() != tailSize)
        User::Leave(KErrEof);

    TInt eocd = tailSize - KZipEndHeaderBytes;
    for (; eocd >= 0; --eocd)
    {
        if (ReadZip32(tailBytes.Ptr() + eocd) != KZipEndSignature)
            continue;
        const TUint16 commentLength = ReadZip16(tailBytes.Ptr() + eocd + 20);
        if (eocd + KZipEndHeaderBytes + commentLength == tailSize)
            break;
    }
    if (eocd < 0)
        User::Leave(KErrCorrupt);

    const TUint8* end = tailBytes.Ptr() + eocd;
    const TUint16 diskNumber = ReadZip16(end + 4);
    const TUint16 centralDisk = ReadZip16(end + 6);
    const TUint16 diskEntries = ReadZip16(end + 8);
    const TUint16 totalEntries = ReadZip16(end + 10);
    const TUint32 centralSize = ReadZip32(end + 12);
    const TUint32 centralOffset = ReadZip32(end + 16);
    if (diskNumber != 0 || centralDisk != 0 || diskEntries != totalEntries ||
        totalEntries == 0xffffu || centralSize == 0xffffffffu ||
        centralOffset == 0xffffffffu ||
        centralOffset > static_cast<TUint32>(fileSize) ||
        centralSize > static_cast<TUint32>(fileSize) - centralOffset)
        User::Leave(KErrNotSupported);

    _LIT8(KLdb, "RPG_RT.ldb");
    _LIT8(KLmt, "RPG_RT.lmt");
    _LIT8(KXp, "Data/Scripts.rxdata");
    _LIT8(KVx, "Data/Scripts.rvdata");
    _LIT8(KVxa, "Data/Scripts.rvdata2");

    TBool hasLdb = EFalse;
    TBool hasLmt = EFalse;
    TBool hasXp = EFalse;
    TBool hasVx = EFalse;
    TBool hasVxa = EFalse;
    TInt cursor = static_cast<TInt>(centralOffset);
    TBuf8<46> header;

    for (TInt entry = 0; entry < totalEntries; ++entry)
    {
        header.Zero();
        User::LeaveIfError(archive.Read(cursor, header, 46));
        if (header.Length() != 46 ||
            ReadZip32(header.Ptr()) != KZipCentralHeaderSignature)
            User::Leave(KErrCorrupt);

        const TUint16 nameLength = ReadZip16(header.Ptr() + 28);
        const TUint16 extraLength = ReadZip16(header.Ptr() + 30);
        const TUint16 commentLength = ReadZip16(header.Ptr() + 32);
        const TInt recordBytes = 46 + nameLength + extraLength + commentLength;
        if (recordBytes < 46 || cursor > fileSize - recordBytes)
            User::Leave(KErrCorrupt);

        if (nameLength > 0)
        {
            HBufC8* name = HBufC8::NewLC(nameLength);
            TPtr8 nameBytes = name->Des();
            User::LeaveIfError(archive.Read(cursor + 46, nameBytes, nameLength));
            if (nameBytes.Length() != nameLength)
                User::Leave(KErrEof);

            if (RawPathEndsWith(nameBytes, KLdb))
                hasLdb = ETrue;
            else if (RawPathEndsWith(nameBytes, KLmt))
                hasLmt = ETrue;
            else if (RawPathEndsWith(nameBytes, KXp))
                hasXp = ETrue;
            else if (RawPathEndsWith(nameBytes, KVx))
                hasVx = ETrue;
            else if (RawPathEndsWith(nameBytes, KVxa))
                hasVxa = ETrue;
            CleanupStack::PopAndDestroy(name);
        }

        cursor += recordBytes;
    }

    CleanupStack::PopAndDestroy(tail);
    CleanupStack::PopAndDestroy(&archive);

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
    const TInt signature = DetectEngineSignature(aResources);
    if (signature != EGameEngineRpg2000)
        return signature;

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

TInt DetectGameEngineL(RFs& aFs,
                       const TDesC& aArchivePath)
{
    return DetectEngineFromZipDirectoryL(aFs, aArchivePath);
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

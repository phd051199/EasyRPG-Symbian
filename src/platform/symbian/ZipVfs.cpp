#include "Encoding.h"
#include "ZipVfs.h"

namespace
{
const TUint32 KLocalHeaderSignature = 0x04034b50u;
const TUint32 KCentralHeaderSignature = 0x02014b50u;
const TUint32 KEndOfCentralDirectorySignature = 0x06054b50u;
const TUint16 KFlagEncrypted = 0x0001u;
const TUint16 KFlagUtf8 = 0x0800u;
const TUint16 KMethodStored = 0u;
const TUint16 KExtraUnicodePath = 0x7075u;
const TInt KEndOfCentralDirectoryBytes = 22;
const TInt KMaxZipCommentBytes = 65535;
const TInt KRgss3HeaderBytes = 12;
const TInt KRgss3RecordBytes = 16;

TUint16 Read16(const TUint8* aBytes)
{
    return static_cast<TUint16>(aBytes[0]) |
           static_cast<TUint16>(static_cast<TUint16>(aBytes[1]) << 8);
}

TUint32 AdvanceRgssDataKey(TUint32 aKey, TUint32 aBlocks)
{
    TUint32 accumulatedMultiplier = 1u;
    TUint32 accumulatedIncrement = 0u;
    TUint32 multiplier = 7u;
    TUint32 increment = 3u;
    while (aBlocks != 0)
    {
        if ((aBlocks & 1u) != 0)
        {
            accumulatedMultiplier *= multiplier;
            accumulatedIncrement = accumulatedIncrement * multiplier + increment;
        }
        increment = (multiplier + 1u) * increment;
        multiplier *= multiplier;
        aBlocks >>= 1;
    }
    return accumulatedMultiplier * aKey + accumulatedIncrement;
}

TUint32 Read32(const TUint8* aBytes)
{
    return static_cast<TUint32>(aBytes[0]) |
           (static_cast<TUint32>(aBytes[1]) << 8) |
           (static_cast<TUint32>(aBytes[2]) << 16) |
           (static_cast<TUint32>(aBytes[3]) << 24);
}

TUint16 FoldPathChar(TUint16 aValue)
{
    if (aValue == '\\')
        return '/';
    if (aValue >= 'A' && aValue <= 'Z')
        return static_cast<TUint16>(aValue + ('a' - 'A'));
    return aValue;
}

TBool IsDirectoryName(const TDesC8& aRawName)
{
    if (aRawName.Length() == 0)
        return EFalse;
    const TUint8 last = aRawName[aRawName.Length() - 1];
    return last == '/' || last == '\\';
}

TBool FindExtraField(const TDesC8& aExtra,
                     TUint16 aWantedId,
                     TPtrC8& aPayload)
{
    TInt cursor = 0;
    while (cursor <= aExtra.Length() - 4)
    {
        const TUint8* bytes = aExtra.Ptr() + cursor;
        const TUint16 id = Read16(bytes);
        const TUint16 size = Read16(bytes + 2);
        cursor += 4;
        if (size > aExtra.Length() - cursor)
            return EFalse;
        if (id == aWantedId)
        {
            aPayload.Set(aExtra.Mid(cursor, size));
            return ETrue;
        }
        cursor += size;
    }
    return EFalse;
}

void DecodeArchiveNameL(const TDesC8& aRawName,
                        const TDesC8& aExtra,
                        TUint16 aFlags,
                        TDes& aDestination)
{
    TPtrC8 unicodePath;
    TBool usedUnicodeExtra = EFalse;
    if (FindExtraField(aExtra, KExtraUnicodePath, unicodePath) &&
        unicodePath.Length() >= 5 && unicodePath[0] == 1)
    {
        const TPtrC8 utf8Name = unicodePath.Mid(5);
        TEncoding::Utf8ToUnicodeL(utf8Name, aDestination);
        usedUnicodeExtra = ETrue;
    }

    if (!usedUnicodeExtra)
    {
        if (aFlags & KFlagUtf8)
        {
            TEncoding::Utf8ToUnicodeL(aRawName, aDestination);
        }
        else
        {
            TBool hasNonAscii = EFalse;
            for (TInt index = 0; index < aRawName.Length(); ++index)
            {
                if (aRawName[index] >= 0x80)
                {
                    hasNonAscii = ETrue;
                    break;
                }
            }

            if (!hasNonAscii)
            {
                if (aRawName.Length() > aDestination.MaxLength())
                    User::Leave(KErrOverflow);
                aDestination.Zero();
                for (TInt index = 0; index < aRawName.Length(); ++index)
                    aDestination.Append(static_cast<TUint16>(aRawName[index]));
            }
            else
            {
                TInt utf8Error = KErrCorrupt;
                TRAP(utf8Error, TEncoding::Utf8ToUnicodeL(aRawName, aDestination));
                if (utf8Error != KErrNone)
                    TEncoding::ShiftJisToUnicodeL(aRawName, aDestination);
            }
        }
    }
}

}

CZipVfs* CZipVfs::OpenL(RFs& aFs,
                        const TDesC& aArchivePath)
{
    CZipVfs* self = new (ELeave) CZipVfs(aFs);
    CleanupStack::PushL(self);
    self->ConstructL(aArchivePath);
    CleanupStack::Pop(self);
    return self;
}

CZipVfs::CZipVfs(RFs& aFs)
    : iFs(aFs),
      iProjectPrefix(NULL),
      iCentralDirectory(NULL),
      iRgssArchiveDataOffset(0),
      iRgssArchiveFileSize(0)
{
}

void CZipVfs::ConstructL(const TDesC& aArchivePath)
{
    if (aArchivePath.Length() == 0 || aArchivePath.Length() > iArchivePath.MaxLength())
        User::Leave(KErrArgument);

    iArchivePath.Copy(aArchivePath);
    User::LeaveIfError(iArchive.Open(iFs,
                                     iArchivePath,
                                     EFileRead | EFileShareReadersOnly));

    BuildIndexL();
    ResolveProjectRootL();
    BuildRgss3ArchiveIndexL();
}

CZipVfs::~CZipVfs()
{
    TInt bucket = 0;
    for (bucket = 0; bucket < KBucketCount; ++bucket)
    {
        iBuckets[bucket].Close();
        iRgssBuckets[bucket].Close();
    }

    for (TInt index = 0; index < iRgssEntries.Count(); ++index)
    {
        delete iRgssEntries[index].iPath;
        iRgssEntries[index].iPath = NULL;
    }
    iRgssEntries.Close();
    iEntries.Close();

    delete iCentralDirectory;
    iCentralDirectory = NULL;
    delete iProjectPrefix;
    iProjectPrefix = NULL;
    iArchive.Close();
}

void CZipVfs::BuildIndexL()
{
    TInt fileSize = 0;
    User::LeaveIfError(iArchive.Size(fileSize));
    if (fileSize < KEndOfCentralDirectoryBytes)
        User::Leave(KErrCorrupt);

    TInt tailSize = KEndOfCentralDirectoryBytes + KMaxZipCommentBytes;
    if (tailSize > fileSize)
        tailSize = fileSize;

    HBufC8* tail = HBufC8::NewLC(tailSize);
    TPtr8 tailBytes = tail->Des();
    User::LeaveIfError(iArchive.Read(fileSize - tailSize, tailBytes, tailSize));
    if (tailBytes.Length() != tailSize)
        User::Leave(KErrEof);

    TInt eocd = tailSize - KEndOfCentralDirectoryBytes;
    for (; eocd >= 0; --eocd)
    {
        if (Read32(tailBytes.Ptr() + eocd) != KEndOfCentralDirectorySignature)
            continue;
        const TUint16 commentLength = Read16(tailBytes.Ptr() + eocd + 20);
        if (eocd + KEndOfCentralDirectoryBytes + commentLength == tailSize)
            break;
    }
    if (eocd < 0)
        User::Leave(KErrCorrupt);

    const TUint8* eocdBytes = tailBytes.Ptr() + eocd;
    const TUint16 diskNumber = Read16(eocdBytes + 4);
    const TUint16 centralDisk = Read16(eocdBytes + 6);
    const TUint16 diskEntries = Read16(eocdBytes + 8);
    const TUint16 totalEntries = Read16(eocdBytes + 10);
    const TUint32 centralSize32 = Read32(eocdBytes + 12);
    const TUint32 centralOffset32 = Read32(eocdBytes + 16);

    if (diskNumber != 0 || centralDisk != 0 || diskEntries != totalEntries)
        User::Leave(KErrNotSupported);
    if (totalEntries == 0xffffu || centralSize32 == 0xffffffffu ||
        centralOffset32 == 0xffffffffu)
        User::Leave(KErrNotSupported);
    if (centralSize32 > static_cast<TUint32>(KMaxCentralDirectoryBytes))
        User::Leave(KErrTooBig);
    if (centralOffset32 > static_cast<TUint32>(fileSize) ||
        centralSize32 > static_cast<TUint32>(fileSize) - centralOffset32)
        User::Leave(KErrCorrupt);

    iEntries.ReserveL(totalEntries);
    const TInt bucketReserve = (totalEntries + KBucketCount - 1) / KBucketCount + 8;
    for (TInt bucket = 0; bucket < KBucketCount; ++bucket)
        iBuckets[bucket].ReserveL(bucketReserve);

    const TInt centralSize = static_cast<TInt>(centralSize32);
    const TInt centralOffset = static_cast<TInt>(centralOffset32);
    iCentralDirectory = HBufC8::NewL(centralSize);
    TPtr8 centralBytes = iCentralDirectory->Des();
    User::LeaveIfError(iArchive.Read(centralOffset, centralBytes, centralSize));
    if (centralBytes.Length() != centralSize)
        User::Leave(KErrEof);

    TInt cursor = 0;
    TInt entryNumber = 0;
    for (entryNumber = 0; entryNumber < totalEntries; ++entryNumber)
    {
        if (cursor > centralSize - 46)
            User::Leave(KErrCorrupt);
        const TUint8* header = centralBytes.Ptr() + cursor;
        if (Read32(header) != KCentralHeaderSignature)
            User::Leave(KErrCorrupt);

        const TUint16 flags = Read16(header + 8);
        const TUint16 method = Read16(header + 10);
        const TUint32 compressedSize = Read32(header + 20);
        const TUint32 uncompressedSize = Read32(header + 24);
        const TUint16 nameLength = Read16(header + 28);
        const TUint16 extraLength = Read16(header + 30);
        const TUint16 commentLength = Read16(header + 32);
        const TUint16 startDisk = Read16(header + 34);
        const TUint32 localHeaderOffset = Read32(header + 42);

        const TInt recordBytes = 46 + nameLength + extraLength + commentLength;
        if (recordBytes < 46 || recordBytes > centralSize - cursor)
            User::Leave(KErrCorrupt);
        if (startDisk != 0 || localHeaderOffset == 0xffffffffu ||
            compressedSize == 0xffffffffu || uncompressedSize == 0xffffffffu)
            User::Leave(KErrNotSupported);

        const TPtrC8 rawName(header + 46, nameLength);
        const TPtrC8 extra(header + 46 + nameLength, extraLength);

        if (!IsDirectoryName(rawName))
        {
            if (nameLength == 0 || nameLength > KMaxArchivePathChars * 4)
                User::Leave(KErrTooBig);

            TPtrC8 unicodePath;
            TInt unicodeCapacity = nameLength;
            if (FindExtraField(extra, KExtraUnicodePath, unicodePath) &&
                unicodePath.Length() > 5 && unicodePath.Length() - 5 > unicodeCapacity)
                unicodeCapacity = unicodePath.Length() - 5;
            if (unicodeCapacity > KMaxArchivePathChars * 4)
                User::Leave(KErrTooBig);

            TFileName decodedPath;
            DecodeArchiveNameL(rawName, extra, flags, decodedPath);
            if (decodedPath.Length() > KMaxArchivePathChars)
                User::Leave(KErrTooBig);
            NormalizePathInPlaceL(decodedPath);

            if ((flags & KFlagEncrypted) != 0 || method != KMethodStored)
                User::Leave(KErrNotSupported);
            if (compressedSize != uncompressedSize)
                User::Leave(KErrCorrupt);

            AddEntryL(decodedPath,
                      uncompressedSize,
                      localHeaderOffset,
                      cursor);
        }

        cursor += recordBytes;
    }

    CleanupStack::PopAndDestroy(tail);
}

void CZipVfs::ResolveProjectRootL()
{
    _LIT(KDatabaseName, "RPG_RT.ldb");
    _LIT(KTreeName, "RPG_RT.lmt");
    _LIT(KGameIniName, "Game.ini");
    _LIT(KXpScriptsName, "Data/Scripts.rxdata");
    _LIT(KVxScriptsName, "Data/Scripts.rvdata");
    _LIT(KVxaScriptsName, "Data/Scripts.rvdata2");
    _LIT(KXpArchiveName, "Game.rgssad");
    _LIT(KVxArchiveName, "Game.rgss2a");
    _LIT(KVxaArchiveName, "Game.rgss3a");

    TInt index = 0;
    for (index = 0; index < iEntries.Count(); ++index)
    {
        TFileName path;
        DecodeEntryPathL(iEntries[index], path);
        if (!PathEndsWith(path, KDatabaseName))
            continue;

        const TInt prefixLength = path.Length() - KDatabaseName().Length();
        if (prefixLength > 0 && path[prefixLength - 1] != '/')
            continue;

        HBufC* prefix = path.Left(prefixLength).AllocLC();
        TFileName treePath;
        if (prefix->Length() + KTreeName().Length() > treePath.MaxLength())
        {
            CleanupStack::PopAndDestroy(prefix);
            continue;
        }
        treePath.Copy(*prefix);
        treePath.Append(KTreeName);

        if (FindFullPath(treePath))
        {
            iProjectPrefix = prefix;
            CleanupStack::Pop(prefix);
            return;
        }
        CleanupStack::PopAndDestroy(prefix);
    }

    for (index = 0; index < iEntries.Count(); ++index)
    {
        TFileName path;
        DecodeEntryPathL(iEntries[index], path);
        if (!PathEndsWith(path, KGameIniName))
            continue;

        const TInt prefixLength = path.Length() - KGameIniName().Length();
        if (prefixLength > 0 && path[prefixLength - 1] != '/')
            continue;

        HBufC* prefix = path.Left(prefixLength).AllocLC();
        TFileName scriptsPath;
        if (prefix->Length() + KVxaScriptsName().Length() > scriptsPath.MaxLength())
        {
            CleanupStack::PopAndDestroy(prefix);
            continue;
        }

        scriptsPath.Copy(*prefix);
        scriptsPath.Append(KXpScriptsName);
        TBool found = FindFullPath(scriptsPath) != NULL;
        if (!found)
        {
            scriptsPath.Copy(*prefix);
            scriptsPath.Append(KVxScriptsName);
            found = FindFullPath(scriptsPath) != NULL;
        }
        if (!found)
        {
            scriptsPath.Copy(*prefix);
            scriptsPath.Append(KVxaScriptsName);
            found = FindFullPath(scriptsPath) != NULL;
        }
        if (!found)
        {
            scriptsPath.Copy(*prefix);
            scriptsPath.Append(KXpArchiveName);
            found = FindFullPath(scriptsPath) != NULL;
        }
        if (!found)
        {
            scriptsPath.Copy(*prefix);
            scriptsPath.Append(KVxArchiveName);
            found = FindFullPath(scriptsPath) != NULL;
        }
        if (!found)
        {
            scriptsPath.Copy(*prefix);
            scriptsPath.Append(KVxaArchiveName);
            found = FindFullPath(scriptsPath) != NULL;
        }

        if (found)
        {
            iProjectPrefix = prefix;
            CleanupStack::Pop(prefix);
            return;
        }
        CleanupStack::PopAndDestroy(prefix);
    }

    User::Leave(KErrNotFound);
}

void CZipVfs::BuildRgss3ArchiveIndexL()
{
    _LIT(KArchiveName, "Game.rgss3a");
    const TZipVfsEntry* archiveEntry = FindEntry(KArchiveName);
    if (!archiveEntry)
        return;
    if (archiveEntry->iFileSize > static_cast<TUint32>(KMaxTInt) ||
        archiveEntry->iFileSize < KRgss3HeaderBytes)
        User::Leave(KErrCorrupt);

    iRgssArchiveDataOffset = EntryDataOffsetL(*archiveEntry);
    iRgssArchiveFileSize = static_cast<TInt>(archiveEntry->iFileSize);

    TBuf8<KRgss3HeaderBytes> header;
    User::LeaveIfError(iArchive.Read(iRgssArchiveDataOffset,
                                     header,
                                     KRgss3HeaderBytes));
    if (header.Length() != KRgss3HeaderBytes ||
        header[0] != 'R' || header[1] != 'G' || header[2] != 'S' ||
        header[3] != 'S' || header[4] != 'A' || header[5] != 'D' ||
        header[6] != 0 || header[7] != 3)
        User::Leave(KErrNotSupported);

    const TUint32 rawKey = Read32(header.Ptr() + 8);
    const TUint32 archiveKey = rawKey * 9u + 3u;
    const TUint8 keyBytes[4] = {
        static_cast<TUint8>(archiveKey & 0xffu),
        static_cast<TUint8>((archiveKey >> 8) & 0xffu),
        static_cast<TUint8>((archiveKey >> 16) & 0xffu),
        static_cast<TUint8>((archiveKey >> 24) & 0xffu)
    };

    for (TInt bucket = 0; bucket < KBucketCount; ++bucket)
        iRgssBuckets[bucket].ReserveL(8);

    TInt cursor = KRgss3HeaderBytes;
    TBool terminated = EFalse;
    while (cursor <= iRgssArchiveFileSize - KRgss3RecordBytes)
    {
        TBuf8<KRgss3RecordBytes> record;
        User::LeaveIfError(iArchive.Read(iRgssArchiveDataOffset + cursor,
                                         record,
                                         KRgss3RecordBytes));
        if (record.Length() != KRgss3RecordBytes)
            User::Leave(KErrEof);
        cursor += KRgss3RecordBytes;

        const TUint32 dataOffset = Read32(record.Ptr()) ^ archiveKey;
        const TUint32 fileSize = Read32(record.Ptr() + 4) ^ archiveKey;
        const TUint32 fileKey = Read32(record.Ptr() + 8) ^ archiveKey;
        const TUint32 nameLength32 = Read32(record.Ptr() + 12) ^ archiveKey;
        if (dataOffset == 0)
        {
            terminated = ETrue;
            break;
        }
        if (nameLength32 == 0 ||
            nameLength32 > static_cast<TUint32>(KMaxArchivePathChars * 4) ||
            nameLength32 > static_cast<TUint32>(iRgssArchiveFileSize - cursor) ||
            dataOffset > static_cast<TUint32>(iRgssArchiveFileSize) ||
            fileSize > static_cast<TUint32>(iRgssArchiveFileSize) - dataOffset)
            User::Leave(KErrCorrupt);

        const TInt nameLength = static_cast<TInt>(nameLength32);
        HBufC8* rawName = HBufC8::NewLC(nameLength);
        TPtr8 rawNameBytes = rawName->Des();
        User::LeaveIfError(iArchive.Read(iRgssArchiveDataOffset + cursor,
                                         rawNameBytes,
                                         nameLength));
        if (rawNameBytes.Length() != nameLength)
            User::Leave(KErrEof);
        cursor += nameLength;

        for (TInt index = 0; index < nameLength; ++index)
            rawNameBytes[index] ^= keyBytes[index & 3];

        TFileName path;
        TInt utf8Error = KErrNone;
        TRAP(utf8Error, TEncoding::Utf8ToUnicodeL(rawNameBytes, path));
        if (utf8Error != KErrNone)
            TEncoding::ShiftJisToUnicodeL(rawNameBytes, path);
        NormalizePathInPlaceL(path);
        CleanupStack::PopAndDestroy(rawName);

        // Loose files in the outer ZIP override files packed in Game.rgss3a.
        if (FindEntry(path) || FindRgssEntry(path))
            continue;

        TRgssVfsEntry entry;
        entry.iPath = path.AllocLC();
        entry.iHash = HashPath(path);
        entry.iFileSize = fileSize;
        entry.iDataOffset = dataOffset;
        entry.iKey = fileKey;
        User::LeaveIfError(iRgssEntries.Append(entry));
        CleanupStack::Pop(entry.iPath);

        const TInt entryIndex = iRgssEntries.Count() - 1;
        const TInt bucket = static_cast<TInt>(entry.iHash & (KBucketCount - 1));
        const TInt bucketError = iRgssBuckets[bucket].Append(entryIndex);
        if (bucketError != KErrNone)
        {
            delete iRgssEntries[entryIndex].iPath;
            iRgssEntries.Remove(entryIndex);
            User::Leave(bucketError);
        }
    }

    if (!terminated)
        User::Leave(KErrCorrupt);
}

void CZipVfs::AddEntryL(const TDesC& aPath,
                        TUint32 aFileSize,
                        TUint32 aLocalHeaderOffset,
                        TInt aCentralOffset)
{
    if (aPath.Length() == 0)
        return;

    if (FindFullPath(aPath))
        return;

    TZipVfsEntry entry;
    entry.iHash = HashPath(aPath);
    entry.iFileSize = aFileSize;
    entry.iLocalHeaderOffset = aLocalHeaderOffset;
    entry.iCentralOffset = aCentralOffset;
    entry.iDataOffset = -1;
    User::LeaveIfError(iEntries.Append(entry));
    const TInt entryIndex = iEntries.Count() - 1;
    const TInt bucket = static_cast<TInt>(entry.iHash & (KBucketCount - 1));
    const TInt bucketError = iBuckets[bucket].Append(entryIndex);
    if (bucketError != KErrNone)
    {
        iEntries.Remove(entryIndex);
        User::Leave(bucketError);
    }
}

void CZipVfs::DecodeEntryPathL(const TZipVfsEntry& aEntry, TDes& aPath) const
{
    if (!iCentralDirectory || aEntry.iCentralOffset < 0)
        User::Leave(KErrNotReady);

    const TDesC8& central = *iCentralDirectory;
    if (aEntry.iCentralOffset > central.Length() - 46)
        User::Leave(KErrCorrupt);

    const TUint8* header = central.Ptr() + aEntry.iCentralOffset;
    if (Read32(header) != KCentralHeaderSignature)
        User::Leave(KErrCorrupt);

    const TUint16 flags = Read16(header + 8);
    const TUint16 nameLength = Read16(header + 28);
    const TUint16 extraLength = Read16(header + 30);
    const TUint16 commentLength = Read16(header + 32);
    const TInt recordBytes = 46 + nameLength + extraLength + commentLength;
    if (nameLength == 0 || nameLength > KMaxArchivePathChars * 4 ||
        recordBytes < 46 || recordBytes > central.Length() - aEntry.iCentralOffset)
        User::Leave(KErrCorrupt);

    const TPtrC8 rawName(header + 46, nameLength);
    const TPtrC8 extra(header + 46 + nameLength, extraLength);
    aPath.Zero();
    DecodeArchiveNameL(rawName, extra, flags, aPath);
    if (aPath.Length() > KMaxArchivePathChars)
        User::Leave(KErrTooBig);
    NormalizePathInPlaceL(aPath);
}

TBool CZipVfs::Exists(const TDesC& aRelativePath) const
{
    return FindEntry(aRelativePath) != NULL || FindRgssEntry(aRelativePath) != NULL;
}

const TZipVfsEntry* CZipVfs::FindEntry(const TDesC& aRelativePath) const
{
    TFileName fullPath;
    TRAPD(error, BuildFullPathL(aRelativePath, fullPath));
    if (error != KErrNone)
        return NULL;
    return FindFullPath(fullPath);
}

const TZipVfsEntry* CZipVfs::FindFullPath(const TDesC& aFullPath) const
{
    const TUint32 hash = HashPath(aFullPath);
    const TInt bucket = static_cast<TInt>(hash & (KBucketCount - 1));
    TInt position = 0;
    for (position = 0; position < iBuckets[bucket].Count(); ++position)
    {
        const TInt entryIndex = iBuckets[bucket][position];
        if (entryIndex < 0 || entryIndex >= iEntries.Count())
            continue;
        const TZipVfsEntry& entry = iEntries[entryIndex];
        if (entry.iHash != hash)
            continue;
        TFileName path;
        TRAPD(error, DecodeEntryPathL(entry, path));
        if (error == KErrNone && PathEquals(path, aFullPath))
            return &entry;
    }
    return NULL;
}

const TRgssVfsEntry* CZipVfs::FindRgssEntry(const TDesC& aRelativePath,
                                             TInt* aIndex) const
{
    TFileName relative;
    if (aRelativePath.Length() == 0 || aRelativePath.Length() > relative.MaxLength())
        return NULL;
    relative.Copy(aRelativePath);
    TRAPD(error, NormalizePathInPlaceL(relative));
    if (error != KErrNone)
        return NULL;

    const TUint32 hash = HashPath(relative);
    const TInt bucket = static_cast<TInt>(hash & (KBucketCount - 1));
    for (TInt position = 0; position < iRgssBuckets[bucket].Count(); ++position)
    {
        const TInt entryIndex = iRgssBuckets[bucket][position];
        if (entryIndex < 0 || entryIndex >= iRgssEntries.Count())
            continue;
        const TRgssVfsEntry& entry = iRgssEntries[entryIndex];
        if (entry.iHash != hash || !entry.iPath)
            continue;
        if (PathEquals(*entry.iPath, relative))
        {
            if (aIndex)
                *aIndex = entryIndex;
            return &entry;
        }
    }
    return NULL;
}

void CZipVfs::BuildFullPathL(const TDesC& aRelativePath, TDes& aFullPath) const
{
    if (!iProjectPrefix || aRelativePath.Length() == 0)
        User::Leave(KErrArgument);

    TFileName relative;
    if (aRelativePath.Length() > relative.MaxLength())
        User::Leave(KErrOverflow);
    relative.Copy(aRelativePath);
    NormalizePathInPlaceL(relative);

    if (iProjectPrefix->Length() + relative.Length() > aFullPath.MaxLength())
        User::Leave(KErrOverflow);
    aFullPath.Copy(*iProjectPrefix);
    aFullPath.Append(relative);
}

HBufC8* CZipVfs::ReadFileLC(const TDesC& aRelativePath, TInt aMaxBytes) const
{
    if (aMaxBytes < 0)
        User::Leave(KErrArgument);

    TInt dataOffset = 0;
    TInt fileSize = 0;
    OpenStoredFileL(aRelativePath, dataOffset, fileSize);
    if (fileSize > aMaxBytes)
        User::Leave(KErrTooBig);

    HBufC8* output = HBufC8::NewLC(fileSize);
    TPtr8 bytes = output->Des();
    if (fileSize > 0)
    {
        ReadStoredFileL(dataOffset, fileSize, 0, bytes);
        if (bytes.Length() != fileSize)
            User::Leave(KErrEof);
    }
    return output;
}

TInt CZipVfs::EntryDataOffsetL(const TZipVfsEntry& aEntry) const
{
    TBuf8<30> localHeader;
    User::LeaveIfError(iArchive.Read(static_cast<TInt>(aEntry.iLocalHeaderOffset),
                                    localHeader,
                                    localHeader.MaxLength()));
    if (localHeader.Length() != localHeader.MaxLength() ||
        Read32(localHeader.Ptr()) != KLocalHeaderSignature)
        User::Leave(KErrCorrupt);
    if ((Read16(localHeader.Ptr() + 6) & KFlagEncrypted) != 0 ||
        Read16(localHeader.Ptr() + 8) != KMethodStored)
        User::Leave(KErrNotSupported);

    const TUint16 localNameLength = Read16(localHeader.Ptr() + 26);
    const TUint16 localExtraLength = Read16(localHeader.Ptr() + 28);
    const TUint32 dataOffset32 = aEntry.iLocalHeaderOffset + 30u +
                                 localNameLength + localExtraLength;

    TInt archiveSize = 0;
    User::LeaveIfError(iArchive.Size(archiveSize));
    if (dataOffset32 > static_cast<TUint32>(archiveSize) ||
        aEntry.iFileSize > static_cast<TUint32>(archiveSize) - dataOffset32)
        User::Leave(KErrCorrupt);
    return static_cast<TInt>(dataOffset32);
}

void CZipVfs::OpenStoredFileL(const TDesC& aRelativePath,
                              TInt& aDataOffset,
                              TInt& aFileSize) const
{
    const TZipVfsEntry* entry = FindEntry(aRelativePath);
    if (entry)
    {
        if (entry->iFileSize > static_cast<TUint32>(KMaxTInt))
            User::Leave(KErrTooBig);
        aDataOffset = EntryDataOffsetL(*entry);
        aFileSize = static_cast<TInt>(entry->iFileSize);
        return;
    }

    TInt rgssIndex = -1;
    const TRgssVfsEntry* rgssEntry = FindRgssEntry(aRelativePath, &rgssIndex);
    if (!rgssEntry)
        User::Leave(KErrNotFound);
    if (rgssEntry->iFileSize > static_cast<TUint32>(KMaxTInt) || rgssIndex < 0)
        User::Leave(KErrTooBig);

    // Negative offsets are private virtual handles for files stored inside
    // Game.rgss3a. Direct ZIP offsets are always non-negative.
    aDataOffset = -(rgssIndex + 1);
    aFileSize = static_cast<TInt>(rgssEntry->iFileSize);
}

void CZipVfs::ReadStoredFileL(TInt aDataOffset,
                              TInt aFileSize,
                              TInt aPosition,
                              TDes8& aBuffer) const
{
    if (aFileSize < 0 || aPosition < 0)
        User::Leave(KErrArgument);

    aBuffer.Zero();
    if (aPosition >= aFileSize || aBuffer.MaxLength() == 0)
        return;

    const TInt count = Min(aBuffer.MaxLength(), aFileSize - aPosition);
    if (aDataOffset >= 0)
    {
        User::LeaveIfError(iArchive.Read(aDataOffset + aPosition, aBuffer, count));
        if (aBuffer.Length() != count)
            User::Leave(KErrEof);
        return;
    }

    const TInt rgssIndex = -aDataOffset - 1;
    if (rgssIndex < 0 || rgssIndex >= iRgssEntries.Count())
        User::Leave(KErrArgument);
    const TRgssVfsEntry& entry = iRgssEntries[rgssIndex];
    if (entry.iFileSize != static_cast<TUint32>(aFileSize) ||
        entry.iDataOffset > static_cast<TUint32>(iRgssArchiveFileSize) ||
        entry.iFileSize > static_cast<TUint32>(iRgssArchiveFileSize) - entry.iDataOffset)
        User::Leave(KErrCorrupt);

    const TUint32 absoluteOffset32 = static_cast<TUint32>(iRgssArchiveDataOffset) +
                                     entry.iDataOffset +
                                     static_cast<TUint32>(aPosition);
    if (absoluteOffset32 > static_cast<TUint32>(KMaxTInt))
        User::Leave(KErrTooBig);
    User::LeaveIfError(iArchive.Read(static_cast<TInt>(absoluteOffset32),
                                     aBuffer,
                                     count));
    if (aBuffer.Length() != count)
        User::Leave(KErrEof);

    TUint32 dataKey = AdvanceRgssDataKey(entry.iKey,
                                          static_cast<TUint32>(aPosition) >> 2);
    TInt keyBytePosition = aPosition & 3;
    for (TInt index = 0; index < aBuffer.Length(); ++index)
    {
        aBuffer[index] ^= static_cast<TUint8>((dataKey >> (keyBytePosition * 8)) & 0xffu);
        ++keyBytePosition;
        if (keyBytePosition == 4)
        {
            keyBytePosition = 0;
            dataKey = dataKey * 7u + 3u;
        }
    }
}

const TDesC& CZipVfs::ArchivePath() const
{
    return iArchivePath;
}

const TDesC& CZipVfs::ProjectPrefix() const
{
    return iProjectPrefix ? static_cast<const TDesC&>(*iProjectPrefix) : KNullDesC();
}

TInt CZipVfs::EntryCount() const
{
    return iEntries.Count() + iRgssEntries.Count();
}

void CZipVfs::EntryRelativePathL(TInt aIndex, TDes& aPath) const
{
    if (aIndex < 0 || !iProjectPrefix)
        User::Leave(KErrArgument);

    if (aIndex >= iEntries.Count())
    {
        const TInt rgssIndex = aIndex - iEntries.Count();
        if (rgssIndex < 0 || rgssIndex >= iRgssEntries.Count() ||
            !iRgssEntries[rgssIndex].iPath)
            User::Leave(KErrArgument);
        if (iRgssEntries[rgssIndex].iPath->Length() > aPath.MaxLength())
            User::Leave(KErrOverflow);
        aPath.Copy(*iRgssEntries[rgssIndex].iPath);
        return;
    }

    TFileName fullPath;
    DecodeEntryPathL(iEntries[aIndex], fullPath);
    const TInt prefixLength = iProjectPrefix->Length();
    if (prefixLength > fullPath.Length() ||
        !PathEquals(fullPath.Left(prefixLength), *iProjectPrefix))
        User::Leave(KErrNotFound);

    const TPtrC relative = fullPath.Mid(prefixLength);
    if (relative.Length() == 0 || relative.Length() > aPath.MaxLength())
        User::Leave(KErrOverflow);
    aPath.Copy(relative);
}

TUint32 CZipVfs::HashPath(const TDesC& aPath)
{
    TUint32 hash = 2166136261u;
    TInt index = 0;
    for (index = 0; index < aPath.Length(); ++index)
    {
        const TUint16 value = FoldPathChar(aPath[index]);
        hash ^= static_cast<TUint8>(value & 0xffu);
        hash *= 16777619u;
        hash ^= static_cast<TUint8>((value >> 8) & 0xffu);
        hash *= 16777619u;
    }
    return hash;
}

TBool CZipVfs::PathEquals(const TDesC& aLeft, const TDesC& aRight)
{
    if (aLeft.Length() != aRight.Length())
        return EFalse;
    TInt index = 0;
    for (index = 0; index < aLeft.Length(); ++index)
    {
        if (FoldPathChar(aLeft[index]) != FoldPathChar(aRight[index]))
            return EFalse;
    }
    return ETrue;
}

TBool CZipVfs::PathEndsWith(const TDesC& aPath, const TDesC& aSuffix)
{
    if (aSuffix.Length() > aPath.Length())
        return EFalse;
    const TInt offset = aPath.Length() - aSuffix.Length();
    TInt index = 0;
    for (index = 0; index < aSuffix.Length(); ++index)
    {
        if (FoldPathChar(aPath[offset + index]) != FoldPathChar(aSuffix[index]))
            return EFalse;
    }
    return ETrue;
}

void CZipVfs::NormalizePathInPlaceL(TDes& aPath)
{
    if (aPath.Length() == 0)
        User::Leave(KErrArgument);

    if (aPath.Length() > KMaxArchivePathChars)
        User::Leave(KErrTooBig);

    TFileName normalized;
    TInt source = 0;
    while (source < aPath.Length())
    {
        TUint16 value = aPath[source++];
        if (value == '\\')
            value = '/';

        if (value == '/')
        {
            if (normalized.Length() == 0 ||
                normalized[normalized.Length() - 1] == '/')
                continue;
        }

        if (normalized.Length() >= normalized.MaxLength())
            User::Leave(KErrOverflow);
        normalized.Append(value);
    }

    while (normalized.Length() >= 2 && normalized[0] == '.' && normalized[1] == '/')
        normalized.Delete(0, 2);

    if (normalized.Length() == 0 || normalized[0] == '/' || normalized.Locate(':') != KErrNotFound)
        User::Leave(KErrArgument);

    TInt componentStart = 0;
    TInt index = 0;
    for (index = 0; index <= normalized.Length(); ++index)
    {
        if (index != normalized.Length() && normalized[index] != '/')
            continue;
        const TInt componentLength = index - componentStart;
        if ((componentLength == 1 && normalized[componentStart] == '.') ||
            (componentLength == 2 && normalized[componentStart] == '.' &&
             normalized[componentStart + 1] == '.'))
            User::Leave(KErrArgument);
        componentStart = index + 1;
    }

    TEncoding::NormalizeJapaneseNfcInPlace(normalized);
    if (normalized.Length() > aPath.MaxLength())
        User::Leave(KErrOverflow);
    aPath.Copy(normalized);
}

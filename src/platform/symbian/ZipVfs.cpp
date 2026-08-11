#include <zlib.h>

#include "CoreZipCrypto.h"
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
const TUint16 KMethodDeflate = 8u;
const TUint16 KMethodWinZipAes = 99u;
const TUint16 KExtraUnicodePath = 0x7075u;
const TUint16 KExtraWinZipAes = 0x9901u;
const TInt KEndOfCentralDirectoryBytes = 22;
const TInt KMaxZipCommentBytes = 65535;
_LIT(KLcfCacheDirectory, "C:\\Data\\EasyRPG\\Cache\\");

TUint16 Read16(const TUint8* aBytes)
{
    return static_cast<TUint16>(aBytes[0]) |
           static_cast<TUint16>(static_cast<TUint16>(aBytes[1]) << 8);
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

            TInt utf8Error = KErrCorrupt;
            if (hasNonAscii)
                TRAP(utf8Error, TEncoding::Utf8ToUnicodeL(aRawName, aDestination));
            if (!hasNonAscii || utf8Error != KErrNone)
                TEncoding::ShiftJisToUnicodeL(aRawName, aDestination);
        }
    }
}

TBool ParseAesExtra(const TDesC8& aExtra,
                    TUint8& aStrength,
                    TUint16& aActualMethod)
{
    TPtrC8 payload;
    if (!FindExtraField(aExtra, KExtraWinZipAes, payload) || payload.Length() != 7)
        return EFalse;

    if (payload[2] != 'A' || payload[3] != 'E')
        return EFalse;
    if (payload[4] < 1 || payload[4] > 3)
        return EFalse;

    aStrength = payload[4];
    aActualMethod = Read16(payload.Ptr() + 5);
    return ETrue;
}

void InflateRawL(const TDesC8& aSource, TDes8& aDestination)
{
    z_stream stream;
    Mem::FillZ(&stream, sizeof(stream));

    stream.next_in = const_cast<Bytef*>(
        reinterpret_cast<const Bytef*>(aSource.Ptr()));
    stream.avail_in = static_cast<uInt>(aSource.Length());

    TUint8 dummy = 0;
    if (aDestination.MaxLength() > 0)
    {
        stream.next_out = reinterpret_cast<Bytef*>(
            const_cast<TUint8*>(aDestination.Ptr()));
        stream.avail_out = static_cast<uInt>(aDestination.MaxLength());
    }
    else
    {
        stream.next_out = reinterpret_cast<Bytef*>(&dummy);
        stream.avail_out = 1;
    }

    const TInt initResult = inflateInit2(&stream, -MAX_WBITS);
    if (initResult != Z_OK)
        User::Leave(initResult == Z_MEM_ERROR ? KErrNoMemory : KErrCorrupt);

    const TInt inflateResult = inflate(&stream, Z_FINISH);
    const uLong totalOut = stream.total_out;
    const TInt endResult = inflateEnd(&stream);

    if (inflateResult != Z_STREAM_END || endResult != Z_OK)
        User::Leave(KErrCorrupt);
    if (totalOut != static_cast<uLong>(aDestination.MaxLength()))
        User::Leave(KErrCorrupt);

    aDestination.SetLength(aDestination.MaxLength());
}

void CleanupInflate(TAny* aPointer)
{
    if (aPointer)
        inflateEnd(static_cast<z_stream*>(aPointer));
}
}

class CZipVfsEntry : public CBase
{
public:
    static CZipVfsEntry* NewLC(const TDesC& aPath,
                               TUint16 aFlags,
                               TUint16 aMethod,
                               TUint32 aCrc32,
                               TUint32 aCompressedSize,
                               TUint32 aUncompressedSize,
                               TUint32 aLocalHeaderOffset,
                               TUint8 aAesStrength,
                               TUint16 aAesMethod)
    {
        CZipVfsEntry* self = new (ELeave) CZipVfsEntry;
        CleanupStack::PushL(self);
        self->iPath = aPath.AllocL();
        self->iFlags = aFlags;
        self->iMethod = aMethod;
        self->iCrc32 = aCrc32;
        self->iCompressedSize = aCompressedSize;
        self->iUncompressedSize = aUncompressedSize;
        self->iLocalHeaderOffset = aLocalHeaderOffset;
        self->iAesStrength = aAesStrength;
        self->iAesMethod = aAesMethod;
        self->iHash = 0;
        return self;
    }

    ~CZipVfsEntry()
    {
        delete iPath;
        iPath = NULL;
    }

public:
    HBufC* iPath;
    TUint32 iHash;
    TUint16 iFlags;
    TUint16 iMethod;
    TUint32 iCrc32;
    TUint32 iCompressedSize;
    TUint32 iUncompressedSize;
    TUint32 iLocalHeaderOffset;
    TUint8 iAesStrength;
    TUint16 iAesMethod;
};

CZipVfs* CZipVfs::OpenL(RFs& aFs,
                        const TDesC& aArchivePath,
                        const TDesC8& aPassword)
{
    CZipVfs* self = new (ELeave) CZipVfs(aFs);
    CleanupStack::PushL(self);
    self->ConstructL(aArchivePath, aPassword);
    CleanupStack::Pop(self);
    return self;
}

CZipVfs::CZipVfs(RFs& aFs)
    : iFs(aFs),
      iPassword(NULL),
      iProjectPrefix(NULL)
{
}

void CZipVfs::ConstructL(const TDesC& aArchivePath, const TDesC8& aPassword)
{
    if (aArchivePath.Length() == 0 || aArchivePath.Length() > iArchivePath.MaxLength())
        User::Leave(KErrArgument);

    iArchivePath.Copy(aArchivePath);
    iPassword = aPassword.AllocL();
    User::LeaveIfError(iArchive.Open(iFs,
                                     iArchivePath,
                                     EFileRead | EFileShareReadersOnly));

    BuildIndexL();
    ResolveProjectRootL();
}

CZipVfs::~CZipVfs()
{
    TInt bucket = 0;
    for (bucket = 0; bucket < KBucketCount; ++bucket)
        iBuckets[bucket].Close();

    iEntries.ResetAndDestroy();
    iEntries.Close();

    delete iProjectPrefix;
    iProjectPrefix = NULL;
    delete iPassword;
    iPassword = NULL;
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

    const TInt centralSize = static_cast<TInt>(centralSize32);
    const TInt centralOffset = static_cast<TInt>(centralOffset32);
    HBufC8* central = HBufC8::NewLC(centralSize);
    TPtr8 centralBytes = central->Des();
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
        const TUint32 crc32Value = Read32(header + 16);
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

            HBufC* decoded = HBufC::NewLC(unicodeCapacity);
            TPtr decodedPath = decoded->Des();
            DecodeArchiveNameL(rawName, extra, flags, decodedPath);
            if (decodedPath.Length() > KMaxArchivePathChars)
                User::Leave(KErrTooBig);
            NormalizePathInPlaceL(decodedPath);

            TUint8 aesStrength = 0;
            TUint16 aesMethod = 0;
            if (flags & KFlagEncrypted)
            {
                if (method != KMethodWinZipAes ||
                    !ParseAesExtra(extra, aesStrength, aesMethod))
                    User::Leave(KErrNotSupported);
                if (aesMethod != KMethodStored && aesMethod != KMethodDeflate)
                    User::Leave(KErrNotSupported);
            }
            else if (method != KMethodStored && method != KMethodDeflate)
            {
                User::Leave(KErrNotSupported);
            }

            AddEntryL(decodedPath,
                      flags,
                      method,
                      crc32Value,
                      compressedSize,
                      uncompressedSize,
                      localHeaderOffset,
                      aesStrength,
                      aesMethod);
            CleanupStack::PopAndDestroy(decoded);
        }

        cursor += recordBytes;
    }

    CleanupStack::PopAndDestroy(central);
    CleanupStack::PopAndDestroy(tail);
}

void CZipVfs::ResolveProjectRootL()
{
    _LIT(KDatabaseName, "RPG_RT.ldb");
    _LIT(KTreeName, "RPG_RT.lmt");

    TInt index = 0;
    for (index = 0; index < iEntries.Count(); ++index)
    {
        const TDesC& path = *iEntries[index]->iPath;
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

    User::Leave(KErrNotFound);
}

void CZipVfs::AddEntryL(const TDesC& aPath,
                        TUint16 aFlags,
                        TUint16 aMethod,
                        TUint32 aCrc32,
                        TUint32 aCompressedSize,
                        TUint32 aUncompressedSize,
                        TUint32 aLocalHeaderOffset,
                        TUint8 aAesStrength,
                        TUint16 aAesMethod)
{
    if (aPath.Length() == 0)
        return;

    if (FindFullPath(aPath))
        return;

    CZipVfsEntry* entry = CZipVfsEntry::NewLC(aPath,
                                               aFlags,
                                               aMethod,
                                               aCrc32,
                                               aCompressedSize,
                                               aUncompressedSize,
                                               aLocalHeaderOffset,
                                               aAesStrength,
                                               aAesMethod);
    entry->iHash = HashPath(aPath);
    User::LeaveIfError(iEntries.Append(entry));
    const TInt entryIndex = iEntries.Count() - 1;
    const TInt bucket = static_cast<TInt>(entry->iHash & (KBucketCount - 1));
    const TInt bucketError = iBuckets[bucket].Append(entryIndex);
    if (bucketError != KErrNone)
    {
        iEntries.Remove(entryIndex);
        User::Leave(bucketError);
    }
    CleanupStack::Pop(entry);
}

TBool CZipVfs::Exists(const TDesC& aRelativePath) const
{
    return FindEntry(aRelativePath) != NULL;
}

const CZipVfsEntry* CZipVfs::FindEntry(const TDesC& aRelativePath) const
{
    TFileName fullPath;
    TRAPD(error, BuildFullPathL(aRelativePath, fullPath));
    if (error != KErrNone)
        return NULL;
    return FindFullPath(fullPath);
}

const CZipVfsEntry* CZipVfs::FindFullPath(const TDesC& aFullPath) const
{
    const TUint32 hash = HashPath(aFullPath);
    const TInt bucket = static_cast<TInt>(hash & (KBucketCount - 1));
    TInt position = 0;
    for (position = 0; position < iBuckets[bucket].Count(); ++position)
    {
        const TInt entryIndex = iBuckets[bucket][position];
        if (entryIndex < 0 || entryIndex >= iEntries.Count())
            continue;
        const CZipVfsEntry* entry = iEntries[entryIndex];
        if (entry->iHash == hash && PathEquals(*entry->iPath, aFullPath))
            return entry;
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
    const CZipVfsEntry* entry = FindEntry(aRelativePath);
    if (!entry)
        User::Leave(KErrNotFound);
    return DecodeEntryLC(*entry, aMaxBytes);
}

TInt CZipVfs::EntryDataOffsetL(const CZipVfsEntry& aEntry) const
{
    TBuf8<30> localHeader;
    User::LeaveIfError(iArchive.Read(static_cast<TInt>(aEntry.iLocalHeaderOffset),
                                    localHeader,
                                    localHeader.MaxLength()));
    if (localHeader.Length() != localHeader.MaxLength() ||
        Read32(localHeader.Ptr()) != KLocalHeaderSignature)
        User::Leave(KErrCorrupt);

    const TUint16 localNameLength = Read16(localHeader.Ptr() + 26);
    const TUint16 localExtraLength = Read16(localHeader.Ptr() + 28);
    const TUint32 dataOffset32 = aEntry.iLocalHeaderOffset + 30u +
                                 localNameLength + localExtraLength;

    TInt archiveSize = 0;
    User::LeaveIfError(iArchive.Size(archiveSize));
    if (dataOffset32 > static_cast<TUint32>(archiveSize) ||
        aEntry.iCompressedSize > static_cast<TUint32>(archiveSize) - dataOffset32)
        User::Leave(KErrCorrupt);
    return static_cast<TInt>(dataOffset32);
}

void CZipVfs::MaterializeEntryL(const CZipVfsEntry& aEntry,
                                RFile& aOutput,
                                TInt aMaxBytes) const
{
    if (aEntry.iUncompressedSize > static_cast<TUint32>(aMaxBytes) ||
        aEntry.iUncompressedSize > static_cast<TUint32>(KMaxTInt) ||
        aEntry.iCompressedSize > static_cast<TUint32>(KMaxTInt))
        User::Leave(KErrTooBig);

    if (aEntry.iFlags & KFlagEncrypted)
    {
        HBufC8* data = DecodeEntryLC(aEntry, aMaxBytes);
        User::LeaveIfError(aOutput.Write(*data));
        CleanupStack::PopAndDestroy(data);
        return;
    }

    const TInt dataOffset = EntryDataOffsetL(aEntry);
    const TInt inputCapacity = 16 * 1024;
    const TInt outputCapacity = 16 * 1024;
    HBufC8* input = HBufC8::NewLC(inputCapacity);
    HBufC8* output = HBufC8::NewLC(outputCapacity);
    TPtr8 inputBytes = input->Des();
    TPtr8 outputBytes = output->Des();

    if (aEntry.iMethod == KMethodStored)
    {
        if (aEntry.iCompressedSize != aEntry.iUncompressedSize)
            User::Leave(KErrCorrupt);
        TInt remaining = static_cast<TInt>(aEntry.iCompressedSize);
        TInt position = dataOffset;
        while (remaining > 0)
        {
            const TInt count = Min(remaining, inputCapacity);
            inputBytes.Zero();
            User::LeaveIfError(iArchive.Read(position, inputBytes, count));
            if (inputBytes.Length() != count)
                User::Leave(KErrEof);
            User::LeaveIfError(aOutput.Write(inputBytes));
            position += count;
            remaining -= count;
        }
    }
    else if (aEntry.iMethod == KMethodDeflate)
    {
        z_stream stream;
        Mem::FillZ(&stream, sizeof(stream));
        const TInt initResult = inflateInit2(&stream, -MAX_WBITS);
        if (initResult != Z_OK)
            User::Leave(initResult == Z_MEM_ERROR ? KErrNoMemory : KErrCorrupt);
        CleanupStack::PushL(TCleanupItem(&CleanupInflate, &stream));

        TInt remaining = static_cast<TInt>(aEntry.iCompressedSize);
        TInt position = dataOffset;
        TBool finished = EFalse;
        while (!finished)
        {
            if (stream.avail_in == 0 && remaining > 0)
            {
                const TInt count = Min(remaining, inputCapacity);
                inputBytes.Zero();
                User::LeaveIfError(iArchive.Read(position, inputBytes, count));
                if (inputBytes.Length() != count)
                    User::Leave(KErrEof);
                stream.next_in = const_cast<Bytef*>(
                    reinterpret_cast<const Bytef*>(inputBytes.Ptr()));
                stream.avail_in = static_cast<uInt>(inputBytes.Length());
                position += count;
                remaining -= count;
            }

            outputBytes.SetLength(outputCapacity);
            stream.next_out = reinterpret_cast<Bytef*>(
                const_cast<TUint8*>(outputBytes.Ptr()));
            stream.avail_out = static_cast<uInt>(outputCapacity);
            const TInt inflateResult = inflate(&stream, Z_NO_FLUSH);
            const TInt produced = outputCapacity - static_cast<TInt>(stream.avail_out);
            if (produced > 0)
            {
                outputBytes.SetLength(produced);
                User::LeaveIfError(aOutput.Write(outputBytes));
            }

            if (inflateResult == Z_STREAM_END)
            {
                finished = ETrue;
            }
            else if (inflateResult != Z_OK)
            {
                User::Leave(KErrCorrupt);
            }
            else if (remaining == 0 && stream.avail_in == 0 && produced == 0)
            {
                User::Leave(KErrCorrupt);
            }
        }

        if (stream.total_out != static_cast<uLong>(aEntry.iUncompressedSize) ||
            remaining != 0 || stream.avail_in != 0)
            User::Leave(KErrCorrupt);
        CleanupStack::PopAndDestroy();
    }
    else
    {
        User::Leave(KErrNotSupported);
    }

    CleanupStack::PopAndDestroy(output);
    CleanupStack::PopAndDestroy(input);
}

void CZipVfs::MaterializeFileL(const TDesC& aRelativePath,
                               const TDesC& aCacheFileName,
                               TDes& aMaterializedPath,
                               TInt aMaxBytes) const
{
    if (aCacheFileName.Length() == 0 || aMaxBytes < 0)
        User::Leave(KErrArgument);

    const CZipVfsEntry* entry = FindEntry(aRelativePath);
    if (!entry)
        User::Leave(KErrNotFound);
    if (entry->iUncompressedSize > static_cast<TUint32>(aMaxBytes) ||
        entry->iUncompressedSize > static_cast<TUint32>(KMaxTInt))
        User::Leave(KErrTooBig);

    TInt index = 0;
    for (index = 0; index < aCacheFileName.Length(); ++index)
    {
        const TUint16 value = aCacheFileName[index];
        if (value == '/' || value == '\\' || value == ':')
            User::Leave(KErrArgument);
    }

    TFileName destination;
    if (KLcfCacheDirectory().Length() + aCacheFileName.Length() > destination.MaxLength() ||
        KLcfCacheDirectory().Length() + aCacheFileName.Length() > aMaterializedPath.MaxLength())
        User::Leave(KErrOverflow);
    destination.Copy(KLcfCacheDirectory);
    destination.Append(aCacheFileName);

    const TInt mkdirError = iFs.MkDirAll(KLcfCacheDirectory);
    if (mkdirError != KErrNone && mkdirError != KErrAlreadyExists)
        User::Leave(mkdirError);

    TEntry existing;
    TEntry archiveEntry;
    if (iFs.Entry(destination, existing) == KErrNone &&
        iFs.Entry(iArchivePath, archiveEntry) == KErrNone &&
        !existing.IsDir() &&
        existing.iSize == static_cast<TInt>(entry->iUncompressedSize) &&
        existing.iModified.Int64() >= archiveEntry.iModified.Int64())
    {
        aMaterializedPath.Copy(destination);
        return;
    }

    RFile output;
    User::LeaveIfError(output.Replace(iFs,
                                      destination,
                                      EFileWrite | EFileShareExclusive));
    CleanupClosePushL(output);
    TRAPD(writeError,
    {
        MaterializeEntryL(*entry, output, aMaxBytes);
        User::LeaveIfError(output.Flush());
    });
    CleanupStack::PopAndDestroy(&output);
    if (writeError != KErrNone)
    {
        iFs.Delete(destination);
        User::Leave(writeError);
    }

    aMaterializedPath.Copy(destination);
}

HBufC8* CZipVfs::DecodeEntryLC(const CZipVfsEntry& aEntry, TInt aMaxBytes) const
{
    if (aEntry.iUncompressedSize > static_cast<TUint32>(aMaxBytes) ||
        aEntry.iUncompressedSize > static_cast<TUint32>(KMaxTInt) ||
        aEntry.iCompressedSize > static_cast<TUint32>(KMaxTInt))
        User::Leave(KErrTooBig);

    const TInt dataOffset = EntryDataOffsetL(aEntry);
    const TInt compressedSize = static_cast<TInt>(aEntry.iCompressedSize);
    HBufC8* compressed = HBufC8::NewLC(compressedSize);
    TPtr8 compressedBytes = compressed->Des();
    if (compressedSize > 0)
    {
        User::LeaveIfError(iArchive.Read(dataOffset,
                                        compressedBytes,
                                        compressedSize));
        if (compressedBytes.Length() != compressedSize)
            User::Leave(KErrEof);
    }

    HBufC8* decrypted = NULL;
    TPtrC8 payload(compressedBytes);
    TUint16 payloadMethod = aEntry.iMethod;

    if (aEntry.iFlags & KFlagEncrypted)
    {
        if (!iPassword || iPassword->Length() == 0)
            User::Leave(KErrAccessDenied);

        decrypted = HBufC8::NewLC(compressedSize);
        TPtr8 decryptedBytes = decrypted->Des();
        unsigned int plainSize = 0;
        const TInt decryptResult = EasyRpgCoreWinZipAesDecrypt(
            compressedBytes.Ptr(),
            static_cast<unsigned int>(compressedBytes.Length()),
            iPassword->Ptr(),
            static_cast<unsigned int>(iPassword->Length()),
            aEntry.iAesStrength,
            const_cast<TUint8*>(decryptedBytes.Ptr()),
            static_cast<unsigned int>(decryptedBytes.MaxLength()),
            &plainSize);
        if (decryptResult != 0)
        {
            if (decryptResult == -5)
                User::Leave(KErrAccessDenied);
            User::Leave(KErrCorrupt);
        }
        if (plainSize > static_cast<unsigned int>(decryptedBytes.MaxLength()))
            User::Leave(KErrCorrupt);
        decryptedBytes.SetLength(static_cast<TInt>(plainSize));
        payload.Set(decryptedBytes);
        payloadMethod = aEntry.iAesMethod;
    }

    const TInt outputSize = static_cast<TInt>(aEntry.iUncompressedSize);
    HBufC8* output = HBufC8::NewLC(outputSize);
    TPtr8 outputBytes = output->Des();

    if (payloadMethod == KMethodStored)
    {
        if (payload.Length() != outputSize)
            User::Leave(KErrCorrupt);
        outputBytes.Copy(payload);
    }
    else if (payloadMethod == KMethodDeflate)
    {
        InflateRawL(payload, outputBytes);
    }
    else
    {
        User::Leave(KErrNotSupported);
    }

    CleanupStack::Pop(output);
    if (decrypted)
        CleanupStack::PopAndDestroy(decrypted);
    CleanupStack::PopAndDestroy(compressed);
    CleanupStack::PushL(output);
    return output;
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
    return iEntries.Count();
}

void CZipVfs::EntryRelativePathL(TInt aIndex, TDes& aPath) const
{
    if (aIndex < 0 || aIndex >= iEntries.Count() || !iProjectPrefix)
        User::Leave(KErrArgument);

    const TDesC& fullPath = *iEntries[aIndex]->iPath;
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

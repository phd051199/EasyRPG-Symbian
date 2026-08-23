#include <e32base.h>
#include <badesca.h>
#include <f32file.h>
#include <stddef.h>
#include "Encoding.h"
#include "DirentBridge.h"
#include "ZipVfs.h"

struct __sFILE;
typedef struct __sFILE FILE;
extern "C" FILE* wfopen(const wchar_t* aName, const wchar_t* aMode);
extern "C" int fclose(FILE* aStream);
extern "C" size_t fread(void* aBuffer, size_t aSize, size_t aCount, FILE* aStream);
extern "C" int fseek(FILE* aStream, long aOffset, int aOrigin);
extern "C" long ftell(FILE* aStream);
extern "C" int feof(FILE* aStream);
extern "C" int ferror(FILE* aStream);
extern "C" int ungetc(int aCharacter, FILE* aStream);
extern "C" char* fgets(char* aBuffer, int aCount, FILE* aStream);

struct EasyRpgDir
{
    EasyRpgDir()
        : iEntries(NULL),
          iVirtualNames(NULL),
          iIndex(0)
    {
    }

    ~EasyRpgDir()
    {
        delete iEntries;
        iEntries = NULL;
        delete iVirtualNames;
        iVirtualNames = NULL;
        iVirtualTypes.Close();
        iFs.Close();
    }

    RFs iFs;
    CDir* iEntries;
    CDesCArrayFlat* iVirtualNames;
    RArray<TUint8> iVirtualTypes;
    TInt iIndex;
    EasyRpgDirent iEntry;
    char iName[1024];
};

class CVirtualFile : public CBase
{
public:
    CVirtualFile(CZipVfs* aVfs,
                 TInt aDataOffset,
                 TInt aFileSize,
                 HBufC8* aBuffer)
        : iVfs(aVfs),
          iBuffer(aBuffer),
          iReadAhead(NULL),
          iDataOffset(aDataOffset),
          iFileSize(aFileSize),
          iPosition(0),
          iReadAheadStart(-1),
          iReadAheadLength(0),
          iError(KErrNone),
          iEof(EFalse),
          iHasPushback(EFalse),
          iPushback(0)
    {
    }

    ~CVirtualFile()
    {
        delete iBuffer;
        iBuffer = NULL;
        delete iReadAhead;
        iReadAhead = NULL;
    }

    CZipVfs* iVfs;
    HBufC8* iBuffer;
    HBufC8* iReadAhead;
    TInt iDataOffset;
    TInt iFileSize;
    TInt iPosition;
    TInt iReadAheadStart;
    TInt iReadAheadLength;
    TInt iError;
    TBool iEof;
    TBool iHasPushback;
    TUint8 iPushback;
};

namespace
{
CZipVfs* gVirtualVfs = NULL;
TFileName gVirtualRoot;
const TInt KMaxVirtualFiles = 32;
const TInt KVirtualBufferMaxBytes = 128 * 1024;
const TInt KVirtualBufferBudgetBytes = 1024 * 1024;
const TInt KVirtualLowMemoryBufferMaxBytes = 32 * 1024;
const TInt KVirtualLowMemoryBufferBudgetBytes = 128 * 1024;
const TInt KVirtualReadAheadBytes = 16 * 1024;
const TInt KVirtualLowMemoryReadAheadBytes = 8 * 1024;
const TInt KVirtualDirCacheSlots = 8;
const TInt KVirtualDirCacheMaxEntries = 512;
const TInt KVirtualDirCacheEntryBudget = 1024;
const TInt KVirtualLowMemoryDirCacheEntryBudget = 256;
CVirtualFile* gVirtualFiles[KMaxVirtualFiles] = {NULL};
TInt gVirtualBufferedBytes = 0;
TInt gVirtualReadAheadBytes = 0;
TInt gVirtualMemoryPressure = 0;

class CVirtualDirCache : public CBase
{
public:
    CVirtualDirCache()
        : iDirectory(NULL),
          iNames(NULL),
          iExists(EFalse),
          iStamp(0)
    {
    }

    ~CVirtualDirCache()
    {
        delete iDirectory;
        iDirectory = NULL;
        delete iNames;
        iNames = NULL;
        iTypes.Close();
    }

    HBufC* iDirectory;
    CDesCArrayFlat* iNames;
    RArray<TUint8> iTypes;
    TBool iExists;
    TUint32 iStamp;
};

CVirtualDirCache* gVirtualDirCache[KVirtualDirCacheSlots] = {NULL};
TUint32 gVirtualDirCacheStamp = 0;
TInt gVirtualDirCachedEntries = 0;

TInt VirtualReadAheadBytes()
{
    return gVirtualMemoryPressure > 0
        ? KVirtualLowMemoryReadAheadBytes
        : KVirtualReadAheadBytes;
}

TInt VirtualDirCacheEntryBudget()
{
    return gVirtualMemoryPressure > 0
        ? KVirtualLowMemoryDirCacheEntryBudget
        : KVirtualDirCacheEntryBudget;
}

TInt VirtualDirCacheEntryCount(const CVirtualDirCache* aCache)
{
    return aCache && aCache->iNames ? aCache->iNames->Count() : 0;
}

void ReleaseVirtualDirCacheSlot(TInt aSlot)
{
    if (aSlot < 0 || aSlot >= KVirtualDirCacheSlots || !gVirtualDirCache[aSlot])
        return;
    gVirtualDirCachedEntries -= VirtualDirCacheEntryCount(gVirtualDirCache[aSlot]);
    if (gVirtualDirCachedEntries < 0)
        gVirtualDirCachedEntries = 0;
    delete gVirtualDirCache[aSlot];
    gVirtualDirCache[aSlot] = NULL;
}

TInt OldestVirtualDirCacheSlot()
{
    TInt slot = -1;
    TUint32 oldestStamp = KMaxTUint32;
    for (TInt index = 0; index < KVirtualDirCacheSlots; ++index)
    {
        if (!gVirtualDirCache[index])
            continue;
        if (gVirtualDirCache[index]->iStamp < oldestStamp)
        {
            oldestStamp = gVirtualDirCache[index]->iStamp;
            slot = index;
        }
    }
    return slot;
}

void TrimVirtualDirCacheToBudget(TInt aEntryBudget)
{
    if (aEntryBudget < 0)
        aEntryBudget = 0;
    while (gVirtualDirCachedEntries > aEntryBudget)
    {
        const TInt slot = OldestVirtualDirCacheSlot();
        if (slot < 0)
            break;
        ReleaseVirtualDirCacheSlot(slot);
    }
}

TBool ShouldBufferVirtualFile(const TDesC& aRelative, TInt aFileSize)
{
    const TInt maxFileBytes = gVirtualMemoryPressure > 0
        ? KVirtualLowMemoryBufferMaxBytes
        : KVirtualBufferMaxBytes;
    const TInt budgetBytes = gVirtualMemoryPressure > 0
        ? KVirtualLowMemoryBufferBudgetBytes
        : KVirtualBufferBudgetBytes;
    if (aFileSize <= 0 || aFileSize > maxFileBytes)
        return EFalse;
    if (gVirtualBufferedBytes > budgetBytes - aFileSize)
        return EFalse;
    if (aRelative.Length() >= 6 && aRelative.Left(6).CompareF(_L("Music/")) == 0)
        return EFalse;
    if (aRelative.Length() >= 6 && aRelative.Left(6).CompareF(_L("Movie/")) == 0)
        return EFalse;
    return ETrue;
}

void ClearVirtualDirCache()
{
    for (TInt index = 0; index < KVirtualDirCacheSlots; ++index)
        ReleaseVirtualDirCacheSlot(index);
    gVirtualDirCachedEntries = 0;
    gVirtualDirCacheStamp = 0;
}

CVirtualDirCache* FindVirtualDirCache(const TDesC& aDirectory)
{
    for (TInt index = 0; index < KVirtualDirCacheSlots; ++index)
    {
        CVirtualDirCache* cache = gVirtualDirCache[index];
        if (!cache || !cache->iDirectory)
            continue;
        if (cache->iDirectory->CompareF(aDirectory) != 0)
            continue;
        cache->iStamp = ++gVirtualDirCacheStamp;
        return cache;
    }
    return NULL;
}

void AppendVirtualEntryDirectL(EasyRpgDir& aDir, const TDesC& aName, TUint8 aType)
{
    User::LeaveIfError(aDir.iVirtualTypes.Append(aType));
    TRAPD(error, aDir.iVirtualNames->AppendL(aName));
    if (error != KErrNone)
    {
        aDir.iVirtualTypes.Remove(aDir.iVirtualTypes.Count() - 1);
        User::Leave(error);
    }
}

void PopulateVirtualDirFromCacheL(EasyRpgDir& aDir, const CVirtualDirCache& aCache)
{
    if (!aCache.iNames || aCache.iNames->Count() != aCache.iTypes.Count())
        User::Leave(KErrCorrupt);
    for (TInt index = 0; index < aCache.iNames->Count(); ++index)
        AppendVirtualEntryDirectL(aDir, (*aCache.iNames)[index], aCache.iTypes[index]);
}

void StoreVirtualDirCacheL(const TDesC& aDirectory,
                           const EasyRpgDir& aDir,
                           TBool aExists)
{
    const TInt entryCount = aDir.iVirtualNames ? aDir.iVirtualNames->Count() : 0;
    const TInt entryBudget = VirtualDirCacheEntryBudget();
    if (entryBudget <= 0 || entryCount > KVirtualDirCacheMaxEntries ||
        entryCount > entryBudget)
        return;

    while (gVirtualDirCachedEntries > entryBudget - entryCount)
    {
        const TInt oldest = OldestVirtualDirCacheSlot();
        if (oldest < 0)
            break;
        ReleaseVirtualDirCacheSlot(oldest);
    }

    TInt slot = -1;
    TUint32 oldestStamp = KMaxTUint32;
    for (TInt index = 0; index < KVirtualDirCacheSlots; ++index)
    {
        if (!gVirtualDirCache[index])
        {
            slot = index;
            break;
        }
        if (gVirtualDirCache[index]->iStamp < oldestStamp)
        {
            oldestStamp = gVirtualDirCache[index]->iStamp;
            slot = index;
        }
    }
    if (slot < 0)
        return;

    CVirtualDirCache* cache = new (ELeave) CVirtualDirCache;
    CleanupStack::PushL(cache);
    cache->iDirectory = aDirectory.AllocL();
    cache->iNames = new (ELeave) CDesCArrayFlat(16);
    cache->iExists = aExists;
    cache->iStamp = ++gVirtualDirCacheStamp;
    for (TInt index = 0; index < entryCount; ++index)
    {
        User::LeaveIfError(cache->iTypes.Append(aDir.iVirtualTypes[index]));
        cache->iNames->AppendL((*aDir.iVirtualNames)[index]);
    }
    CleanupStack::Pop(cache);
    ReleaseVirtualDirCacheSlot(slot);
    gVirtualDirCache[slot] = cache;
    gVirtualDirCachedEntries += entryCount;
}

TInt CStringLength(const char* aText)
{
    TInt length = 0;
    if (!aText)
        return 0;
    while (aText[length] != 0)
        ++length;
    return length;
}

void DecodeFsPathL(const char* aPath, TDes& aUnicode)
{
    TEncoding::Utf8CStringToUnicodeL(aPath, aUnicode);
    for (TInt i = 0; i < aUnicode.Length(); ++i)
    {
        if (aUnicode[i] == '/')
            aUnicode[i] = '\\';
    }
}

TBool VirtualDirectoryRelativePathL(const char* aPath, TDes& aRelative)
{
    if (!gVirtualVfs || gVirtualRoot.Length() == 0)
        return EFalse;

    TFileName path;
    DecodeFsPathL(aPath, path);
    while (path.Length() > 0 && path[path.Length() - 1] == '\\')
        path.SetLength(path.Length() - 1);

    TFileName root;
    root.Copy(gVirtualRoot);
    while (root.Length() > 0 && root[root.Length() - 1] == '\\')
        root.SetLength(root.Length() - 1);

    if (path.CompareF(root) == 0)
    {
        aRelative.Zero();
        return ETrue;
    }

    if (path.Length() <= root.Length() ||
        path.Left(root.Length()).CompareF(root) != 0 ||
        path[root.Length()] != '\\')
        return EFalse;

    aRelative.Copy(path.Mid(root.Length() + 1));
    for (TInt index = 0; index < aRelative.Length(); ++index)
    {
        if (aRelative[index] == '\\')
            aRelative[index] = '/';
    }
    if (aRelative.Length() > 0 && aRelative[aRelative.Length() - 1] != '/')
        aRelative.Append('/');
    return ETrue;
}

void AppendVirtualEntryL(EasyRpgDir& aDir, const TDesC& aName, TUint8 aType)
{
    for (TInt index = 0; index < aDir.iVirtualNames->Count(); ++index)
    {
        if ((*aDir.iVirtualNames)[index].CompareF(aName) == 0)
        {
            if (aType == 4)
                aDir.iVirtualTypes[index] = 4;
            return;
        }
    }

    User::LeaveIfError(aDir.iVirtualTypes.Append(aType));
    TRAPD(error, aDir.iVirtualNames->AppendL(aName));
    if (error != KErrNone)
    {
        aDir.iVirtualTypes.Remove(aDir.iVirtualTypes.Count() - 1);
        User::Leave(error);
    }
}

EasyRpgDir* OpenVirtualDirL(const char* aPath)
{
    TFileName relativeDirectory;
    if (!VirtualDirectoryRelativePathL(aPath, relativeDirectory))
        return NULL;

    EasyRpgDir* dir = new (ELeave) EasyRpgDir;
    CleanupStack::PushL(dir);
    dir->iVirtualNames = new (ELeave) CDesCArrayFlat(16);

    CVirtualDirCache* cached = FindVirtualDirCache(relativeDirectory);
    if (cached)
    {
        if (!cached->iExists)
        {
            CleanupStack::PopAndDestroy(dir);
            return NULL;
        }
        PopulateVirtualDirFromCacheL(*dir, *cached);
        CleanupStack::Pop(dir);
        return dir;
    }

    for (TInt index = 0; index < gVirtualVfs->EntryCount(); ++index)
    {
        TFileName relativePath;
        TRAPD(pathError, gVirtualVfs->EntryRelativePathL(index, relativePath));
        if (pathError == KErrNotFound)
            continue;
        User::LeaveIfError(pathError);

        for (TInt charIndex = 0; charIndex < relativePath.Length(); ++charIndex)
        {
            if (relativePath[charIndex] == '\\')
                relativePath[charIndex] = '/';
        }

        if (relativePath.Length() <= relativeDirectory.Length() ||
            relativePath.Left(relativeDirectory.Length()).CompareF(relativeDirectory) != 0)
            continue;

        const TPtrC remainder = relativePath.Mid(relativeDirectory.Length());
        const TInt slash = remainder.Locate('/');
        const TPtrC name = slash == KErrNotFound ? remainder : remainder.Left(slash);
        if (name.Length() == 0)
            continue;
        if (slash == KErrNotFound)
            AppendVirtualEntryDirectL(*dir, name, 8);
        else
            AppendVirtualEntryL(*dir, name, 4);
    }

    if (relativeDirectory.Length() > 0 && dir->iVirtualNames->Count() == 0)
    {
        StoreVirtualDirCacheL(relativeDirectory, *dir, EFalse);
        CleanupStack::PopAndDestroy(dir);
        return NULL;
    }

    StoreVirtualDirCacheL(relativeDirectory, *dir, ETrue);

    CleanupStack::Pop(dir);
    return dir;
}

EasyRpgDir* OpenDirL(const char* aPath)
{
    EasyRpgDir* virtualDir = OpenVirtualDirL(aPath);
    if (virtualDir)
        return virtualDir;

    EasyRpgDir* dir = new (ELeave) EasyRpgDir;
    CleanupStack::PushL(dir);
    User::LeaveIfError(dir->iFs.Connect());

    TFileName path;
    DecodeFsPathL(aPath, path);
    if (path.Length() == 0 || path[path.Length() - 1] != '\\')
        path.Append('\\');
    path.Append('*');

    User::LeaveIfError(dir->iFs.GetDir(path,
                                      KEntryAttNormal | KEntryAttDir | KEntryAttHidden | KEntryAttSystem,
                                      ESortNone,
                                      dir->iEntries));
    CleanupStack::Pop(dir);
    return dir;
}

TBool VirtualRelativePathL(const TDesC& aPath, TDes& aRelative)
{
    if (!gVirtualVfs || gVirtualRoot.Length() == 0)
        return EFalse;
    if (aPath.Length() < gVirtualRoot.Length() ||
        aPath.Left(gVirtualRoot.Length()).CompareF(gVirtualRoot) != 0)
        return EFalse;

    aRelative.Copy(aPath.Mid(gVirtualRoot.Length()));
    while (aRelative.Length() > 0 &&
           (aRelative[0] == '\\' || aRelative[0] == '/'))
        aRelative.Delete(0, 1);
    for (TInt index = 0; index < aRelative.Length(); ++index)
    {
        if (aRelative[index] == '\\')
            aRelative[index] = '/';
    }
    return aRelative.Length() > 0;
}

TBool VirtualRelativePathL(const char* aPath, TDes& aRelative)
{
    TFileName path;
    DecodeFsPathL(aPath, path);
    return VirtualRelativePathL(path, aRelative);
}

// Validates a FILE* handle by pointer comparison only — never dereferences
// the candidate, so a stale handle after fclose yields NULL instead of
// touching freed memory. Deliberately a registry scan, not a stored slot
// index (which would require dereferencing the object to read).
CVirtualFile* FindVirtualFile(FILE* aStream, TInt* aIndex)
{
    for (TInt index = 0; index < KMaxVirtualFiles; ++index)
    {
        if (reinterpret_cast<FILE*>(gVirtualFiles[index]) != aStream)
            continue;
        if (aIndex)
            *aIndex = index;
        return gVirtualFiles[index];
    }
    return NULL;
}

void RegisterVirtualFileL(CVirtualFile* aFile)
{
    for (TInt index = 0; index < KMaxVirtualFiles; ++index)
    {
        if (gVirtualFiles[index])
            continue;
        gVirtualFiles[index] = aFile;
        return;
    }
    User::Leave(KErrTooBig);
}

FILE* OpenFileL(const char* aPath, const char* aMode)
{
    TFileName path;
    DecodeFsPathL(aPath, path);

    if (aMode && aMode[0] == 'r')
    {
        TFileName relative;
        if (VirtualRelativePathL(path, relative))
        {
            TInt dataOffset = 0;
            TInt fileSize = 0;
            gVirtualVfs->OpenStoredFileL(relative, dataOffset, fileSize);
            HBufC8* buffer = NULL;
            if (ShouldBufferVirtualFile(relative, fileSize))
            {
                buffer = HBufC8::NewLC(fileSize);
                TPtr8 bytes = buffer->Des();
                gVirtualVfs->ReadStoredFileL(dataOffset, fileSize, 0, bytes);
                if (bytes.Length() != fileSize)
                    User::Leave(KErrEof);
            }
            CVirtualFile* file = new (ELeave) CVirtualFile(gVirtualVfs,
                                                           dataOffset,
                                                           fileSize,
                                                           buffer);
            if (buffer)
                CleanupStack::Pop(buffer);
            CleanupStack::PushL(file);
            RegisterVirtualFileL(file);
            if (buffer)
                gVirtualBufferedBytes += buffer->Length();
            CleanupStack::Pop(file);
            return reinterpret_cast<FILE*>(file);
        }
    }

    if (path.Length() >= path.MaxLength())
        User::Leave(KErrOverflow);
    path.Append(0);

    TBuf<16> mode;
    const TInt modeLength = CStringLength(aMode);
    if (modeLength >= mode.MaxLength())
        User::Leave(KErrArgument);
    for (TInt i = 0; i < modeLength; ++i)
        mode.Append(static_cast<TUint16>(static_cast<unsigned char>(aMode[i])));
    mode.Append(0);

    FILE* result = wfopen(reinterpret_cast<const wchar_t*>(path.Ptr()),
                          reinterpret_cast<const wchar_t*>(mode.Ptr()));
    return result;
}
}

void EasyRpgSetVirtualProject(CZipVfs* aVfs, const TDesC& aRoot)
{
    ClearVirtualDirCache();
    gVirtualVfs = aVfs;
    gVirtualRoot.Copy(aRoot.Left(gVirtualRoot.MaxLength()));
    for (TInt index = 0; index < gVirtualRoot.Length(); ++index)
    {
        if (gVirtualRoot[index] == '/')
            gVirtualRoot[index] = '\\';
    }
    if (gVirtualRoot.Length() > 0 && gVirtualRoot[gVirtualRoot.Length() - 1] != '\\')
        gVirtualRoot.Append('\\');
}

void EasyRpgClearVirtualProject()
{
    ClearVirtualDirCache();
    for (TInt index = 0; index < KMaxVirtualFiles; ++index)
    {
        if (gVirtualFiles[index] && gVirtualFiles[index]->iBuffer)
            gVirtualBufferedBytes -= gVirtualFiles[index]->iBuffer->Length();
        if (gVirtualFiles[index] && gVirtualFiles[index]->iReadAhead)
            gVirtualReadAheadBytes -= gVirtualFiles[index]->iReadAhead->Des().MaxLength();
        delete gVirtualFiles[index];
        gVirtualFiles[index] = NULL;
    }
    gVirtualBufferedBytes = 0;
    gVirtualReadAheadBytes = 0;
    gVirtualMemoryPressure = 0;
    gVirtualVfs = NULL;
    gVirtualRoot.Zero();
}

TInt EasyRpgSetVirtualFileMemoryPressure(TInt aLevel)
{
    if (aLevel < 0)
        aLevel = 0;
    else if (aLevel > 1)
        aLevel = 1;
    gVirtualMemoryPressure = aLevel;
    TrimVirtualDirCacheToBudget(VirtualDirCacheEntryBudget());

    const TInt maxFileBytes = aLevel > 0
        ? KVirtualLowMemoryBufferMaxBytes
        : KVirtualBufferMaxBytes;
    const TInt targetBytes = aLevel > 0
        ? KVirtualLowMemoryBufferBudgetBytes
        : KVirtualBufferBudgetBytes;
    TBool oversizedBuffer = EFalse;
    if (aLevel > 0)
    {
        for (TInt index = 0; index < KMaxVirtualFiles; ++index)
        {
            const CVirtualFile* file = gVirtualFiles[index];
            if (file && file->iBuffer && file->iBuffer->Length() > maxFileBytes)
            {
                oversizedBuffer = ETrue;
                break;
            }
        }
    }
    TInt freedBytes = 0;
    if (gVirtualBufferedBytes > targetBytes || oversizedBuffer)
    {
        for (TInt index = 0; index < KMaxVirtualFiles; ++index)
        {
            CVirtualFile* file = gVirtualFiles[index];
            if (!file || !file->iBuffer)
                continue;
            if (gVirtualBufferedBytes <= targetBytes && file->iBuffer->Length() <= maxFileBytes)
                continue;
            const TInt bufferBytes = file->iBuffer->Length();
            delete file->iBuffer;
            file->iBuffer = NULL;
            file->iReadAheadStart = -1;
            file->iReadAheadLength = 0;
            gVirtualBufferedBytes -= bufferBytes;
            freedBytes += bufferBytes;
        }
    }

    const TInt readAheadTarget = VirtualReadAheadBytes();
    for (TInt index = 0; index < KMaxVirtualFiles; ++index)
    {
        CVirtualFile* file = gVirtualFiles[index];
        if (!file || !file->iReadAhead)
            continue;
        const TInt capacity = file->iReadAhead->Des().MaxLength();
        if (capacity <= readAheadTarget)
            continue;
        delete file->iReadAhead;
        file->iReadAhead = NULL;
        file->iReadAheadStart = -1;
        file->iReadAheadLength = 0;
        gVirtualReadAheadBytes -= capacity;
        freedBytes += capacity;
    }
    if (gVirtualBufferedBytes < 0)
        gVirtualBufferedBytes = 0;
    if (gVirtualReadAheadBytes < 0)
        gVirtualReadAheadBytes = 0;
    return freedBytes;
}

void EasyRpgBuildVirtualPathL(const TDesC& aRelativePath, TDes& aPath)
{
    if (!gVirtualVfs || gVirtualRoot.Length() == 0 || aRelativePath.Length() == 0)
        User::Leave(KErrNotReady);
    if (gVirtualRoot.Length() + aRelativePath.Length() > aPath.MaxLength())
        User::Leave(KErrOverflow);

    aPath.Copy(gVirtualRoot);
    aPath.Append(aRelativePath);
    for (TInt index = gVirtualRoot.Length(); index < aPath.Length(); ++index)
    {
        if (aPath[index] == '/')
            aPath[index] = '\\';
    }
}

TBool EasyRpgIsVirtualPath(const TDesC& aPath)
{
    TFileName relative;
    return VirtualRelativePathL(aPath, relative);
}

HBufC8* EasyRpgReadVirtualFileLC(const TDesC& aPath, TInt aMaxBytes)
{
    TFileName relative;
    if (!VirtualRelativePathL(aPath, relative))
        User::Leave(KErrNotFound);
    return gVirtualVfs->ReadFileLC(relative, aMaxBytes);
}

extern "C" FILE* EasyRpgFopen(const char* aPath, const char* aMode)
{
    FILE* result = NULL;
    TRAPD(error, result = OpenFileL(aPath, aMode));
    return error == KErrNone ? result : NULL;
}

extern "C" int EasyRpgFclose(FILE* aStream)
{
    TInt index = -1;
    CVirtualFile* file = FindVirtualFile(aStream, &index);
    if (!file)
        return fclose(aStream);
    gVirtualFiles[index] = NULL;
    if (file->iBuffer)
        gVirtualBufferedBytes -= file->iBuffer->Length();
    if (file->iReadAhead)
        gVirtualReadAheadBytes -= file->iReadAhead->Des().MaxLength();
    if (gVirtualBufferedBytes < 0)
        gVirtualBufferedBytes = 0;
    if (gVirtualReadAheadBytes < 0)
        gVirtualReadAheadBytes = 0;
    delete file;
    return 0;
}

extern "C" size_t EasyRpgFread(void* aBuffer,
                                size_t aSize,
                                size_t aCount,
                                FILE* aStream)
{
    CVirtualFile* file = FindVirtualFile(aStream, NULL);
    if (!file)
        return fread(aBuffer, aSize, aCount, aStream);
    if (!aBuffer || aSize == 0 || aCount == 0)
        return 0;
    if (aCount > static_cast<size_t>(KMaxTInt) / aSize)
    {
        file->iError = KErrTooBig;
        return 0;
    }

    const TInt requested = static_cast<TInt>(aSize * aCount);
    TInt written = 0;
    TUint8* output = static_cast<TUint8*>(aBuffer);
    if (file->iHasPushback && requested > 0)
    {
        output[0] = file->iPushback;
        file->iHasPushback = EFalse;
        ++file->iPosition;
        ++written;
    }

    if (written < requested && file->iPosition < file->iFileSize)
    {
        const TInt remaining = file->iFileSize - file->iPosition;
        const TInt wanted = Min(requested - written, remaining);
        if (file->iBuffer)
        {
            Mem::Copy(output + written,
                      file->iBuffer->Ptr() + file->iPosition,
                      wanted);
            file->iPosition += wanted;
            written += wanted;
        }
        else
        {
            while (written < requested && file->iPosition < file->iFileSize)
            {
                const TInt cacheEnd = file->iReadAheadStart >= 0
                    ? file->iReadAheadStart + file->iReadAheadLength
                    : -1;
                if (file->iReadAheadStart >= 0 &&
                    file->iPosition >= file->iReadAheadStart &&
                    file->iPosition < cacheEnd)
                {
                    const TInt cacheOffset = file->iPosition - file->iReadAheadStart;
                    const TInt available = file->iReadAheadLength - cacheOffset;
                    const TInt count = Min(requested - written, available);
                    Mem::Copy(output + written,
                              file->iReadAhead->Ptr() + cacheOffset,
                              count);
                    file->iPosition += count;
                    written += count;
                    continue;
                }

                const TInt remaining = file->iFileSize - file->iPosition;
                const TInt needed = Min(requested - written, remaining);
                const TInt targetReadAhead = VirtualReadAheadBytes();
                if (needed >= targetReadAhead)
                {
                    TPtr8 bytes(output + written, 0, needed);
                    TRAPD(error,
                          file->iVfs->ReadStoredFileL(file->iDataOffset,
                                                      file->iFileSize,
                                                      file->iPosition,
                                                      bytes));
                    if (error != KErrNone)
                    {
                        file->iError = error;
                        return static_cast<size_t>(written) / aSize;
                    }
                    file->iPosition += bytes.Length();
                    written += bytes.Length();
                    if (bytes.Length() == 0)
                        break;
                    continue;
                }

                if (!file->iReadAhead)
                {
                    HBufC8* readAheadBuffer = NULL;
                    TRAPD(allocError, readAheadBuffer = HBufC8::NewL(targetReadAhead));
                    if (allocError != KErrNone)
                    {
                        file->iError = allocError;
                        return static_cast<size_t>(written) / aSize;
                    }
                    file->iReadAhead = readAheadBuffer;
                    gVirtualReadAheadBytes += targetReadAhead;
                }
                TPtr8 readAhead(file->iReadAhead->Des());
                readAhead.Zero();
                TRAPD(error,
                      file->iVfs->ReadStoredFileL(file->iDataOffset,
                                                  file->iFileSize,
                                                  file->iPosition,
                                                  readAhead));
                if (error != KErrNone)
                {
                    file->iError = error;
                    return static_cast<size_t>(written) / aSize;
                }
                file->iReadAheadStart = file->iPosition;
                file->iReadAheadLength = readAhead.Length();
                if (file->iReadAheadLength == 0)
                    break;
            }
        }
    }

    file->iEof = written < requested;
    return static_cast<size_t>(written) / aSize;
}

extern "C" int EasyRpgFseek(FILE* aStream, long aOffset, int aOrigin)
{
    CVirtualFile* file = FindVirtualFile(aStream, NULL);
    if (!file)
        return fseek(aStream, aOffset, aOrigin);

    TInt64 base = 0;
    if (aOrigin == 0)
        base = 0;
    else if (aOrigin == 1)
        base = file->iPosition;
    else if (aOrigin == 2)
        base = file->iFileSize;
    else
    {
        file->iError = KErrArgument;
        return -1;
    }

    const TInt64 position = base + static_cast<TInt64>(aOffset);
    if (position < 0 || position > KMaxTInt)
    {
        file->iError = KErrArgument;
        return -1;
    }
    file->iPosition = static_cast<TInt>(position);
    file->iEof = EFalse;
    file->iHasPushback = EFalse;
    return 0;
}

extern "C" long EasyRpgFtell(FILE* aStream)
{
    CVirtualFile* file = FindVirtualFile(aStream, NULL);
    if (!file)
        return ftell(aStream);
    return static_cast<long>(file->iPosition);
}

extern "C" int EasyRpgFeof(FILE* aStream)
{
    CVirtualFile* file = FindVirtualFile(aStream, NULL);
    if (!file)
        return feof(aStream);
    return file->iEof ? 1 : 0;
}

extern "C" int EasyRpgFerror(FILE* aStream)
{
    CVirtualFile* file = FindVirtualFile(aStream, NULL);
    if (!file)
        return ferror(aStream);
    return file->iError == KErrNone ? 0 : 1;
}

extern "C" int EasyRpgUngetc(int aCharacter, FILE* aStream)
{
    CVirtualFile* file = FindVirtualFile(aStream, NULL);
    if (!file)
        return ungetc(aCharacter, aStream);
    if (aCharacter < 0 || aCharacter > 255 || file->iHasPushback || file->iPosition <= 0)
        return -1;
    --file->iPosition;
    file->iPushback = static_cast<TUint8>(aCharacter);
    file->iHasPushback = ETrue;
    file->iEof = EFalse;
    return aCharacter;
}

extern "C" char* EasyRpgFgets(char* aBuffer, int aCount, FILE* aStream)
{
    CVirtualFile* file = FindVirtualFile(aStream, NULL);
    if (!file)
        return fgets(aBuffer, aCount, aStream);
    if (!aBuffer || aCount <= 1)
        return NULL;

    TInt length = 0;
    while (length < aCount - 1)
    {
        unsigned char value = 0;
        if (EasyRpgFread(&value, 1, 1, aStream) != 1)
            break;
        aBuffer[length++] = static_cast<char>(value);
        if (value == '\n')
            break;
    }
    if (length == 0)
        return NULL;
    aBuffer[length] = 0;
    return aBuffer;
}

extern "C" EasyRpgDir* EasyRpgOpendir(const char* aPath)
{
    EasyRpgDir* result = NULL;
    TRAPD(error, result = OpenDirL(aPath));
    return error == KErrNone ? result : NULL;
}

extern "C" EasyRpgDirent* EasyRpgReaddir(EasyRpgDir* aDir)
{
    if (!aDir)
        return NULL;

    TPtrC unicodeName;
    TUint8 type = 8;
    if (aDir->iVirtualNames)
    {
        if (aDir->iIndex >= aDir->iVirtualNames->Count())
            return NULL;
        unicodeName.Set((*aDir->iVirtualNames)[aDir->iIndex]);
        type = aDir->iVirtualTypes[aDir->iIndex];
        ++aDir->iIndex;
    }
    else
    {
        if (!aDir->iEntries || aDir->iIndex >= aDir->iEntries->Count())
            return NULL;
        const TEntry& entry = (*aDir->iEntries)[aDir->iIndex++];
        unicodeName.Set(entry.iName);
        type = entry.IsDir() ? 4 : 8;
    }

    TPtr8 name(reinterpret_cast<TUint8*>(aDir->iName), 0, sizeof(aDir->iName) - 1);
    TRAPD(error, TEncoding::UnicodeToUtf8L(unicodeName, name));
    if (error != KErrNone)
        return NULL;
    aDir->iName[name.Length()] = 0;

    aDir->iEntry.d_fileno = 0;
    aDir->iEntry.d_namlen = static_cast<unsigned short>(name.Length());
    aDir->iEntry.d_name = aDir->iName;
    aDir->iEntry.d_type = type;
    return &aDir->iEntry;
}

extern "C" int EasyRpgClosedir(EasyRpgDir* aDir)
{
    if (!aDir)
        return -1;
    delete aDir;
    return 0;
}

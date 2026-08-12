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
          iDataOffset(aDataOffset),
          iFileSize(aFileSize),
          iPosition(0),
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
    }

    CZipVfs* iVfs;
    HBufC8* iBuffer;
    TInt iDataOffset;
    TInt iFileSize;
    TInt iPosition;
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
const TInt KVirtualBufferMaxBytes = 1024 * 1024;
CVirtualFile* gVirtualFiles[KMaxVirtualFiles] = {NULL};

TBool ShouldBufferVirtualFile(const TDesC& aRelative, TInt aFileSize)
{
    if (aFileSize <= 0 || aFileSize > KVirtualBufferMaxBytes)
        return EFalse;
    if (aRelative.Length() >= 6 && aRelative.Left(6).CompareF(_L("Music/")) == 0)
        return EFalse;
    if (aRelative.Length() >= 6 && aRelative.Left(6).CompareF(_L("Movie/")) == 0)
        return EFalse;
    return ETrue;
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

void Utf8PathL(const char* aPath, TDes& aUnicode)
{
    if (!aPath)
        User::Leave(KErrArgument);
    const TInt length = CStringLength(aPath);
    TPtrC8 source(reinterpret_cast<const TUint8*>(aPath), length);
    TEncoding::Utf8ToUnicodeL(source, aUnicode);
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
    Utf8PathL(aPath, path);
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
        AppendVirtualEntryL(*dir, name, slash == KErrNotFound ? 8 : 4);
    }

    if (relativeDirectory.Length() > 0 && dir->iVirtualNames->Count() == 0)
    {
        CleanupStack::PopAndDestroy(dir);
        return NULL;
    }

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
    Utf8PathL(aPath, path);
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
    Utf8PathL(aPath, path);
    return VirtualRelativePathL(path, aRelative);
}

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
    Utf8PathL(aPath, path);

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
                buffer = gVirtualVfs->ReadFileLC(relative, KVirtualBufferMaxBytes);
            CVirtualFile* file = new (ELeave) CVirtualFile(gVirtualVfs,
                                                           dataOffset,
                                                           fileSize,
                                                           buffer);
            if (buffer)
                CleanupStack::Pop(buffer);
            CleanupStack::PushL(file);
            RegisterVirtualFileL(file);
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

    return wfopen(reinterpret_cast<const wchar_t*>(path.Ptr()),
                  reinterpret_cast<const wchar_t*>(mode.Ptr()));
}
}

void EasyRpgSetVirtualProject(CZipVfs* aVfs, const TDesC& aRoot)
{
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
    for (TInt index = 0; index < KMaxVirtualFiles; ++index)
    {
        delete gVirtualFiles[index];
        gVirtualFiles[index] = NULL;
    }
    gVirtualVfs = NULL;
    gVirtualRoot.Zero();
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
            TPtr8 bytes(output + written, 0, wanted);
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

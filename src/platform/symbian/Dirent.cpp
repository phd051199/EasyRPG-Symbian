#include <e32base.h>
#include <badesca.h>
#include <f32file.h>
#include "Encoding.h"
#include "DirentBridge.h"
#include "ZipVfs.h"

struct __sFILE;
typedef struct __sFILE FILE;
extern "C" FILE* wfopen(const wchar_t* aName, const wchar_t* aMode);

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

namespace
{
CZipVfs* gVirtualVfs = NULL;
TFileName gVirtualRoot;

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

TBool VirtualRelativePathL(const char* aPath, TDes& aRelative)
{
    if (!gVirtualVfs || gVirtualRoot.Length() == 0)
        return EFalse;

    TFileName path;
    Utf8PathL(aPath, path);
    if (path.Length() < gVirtualRoot.Length() ||
        path.Left(gVirtualRoot.Length()).CompareF(gVirtualRoot) != 0)
        return EFalse;

    aRelative.Copy(path.Mid(gVirtualRoot.Length()));
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

TUint32 HashVirtualPath(const TDesC& aPath, TUint32 hash)
{
    for (TInt index = 0; index < aPath.Length(); ++index)
    {
        TUint16 value = aPath[index];
        if (value == '\\')
            value = '/';
        if (value >= 'A' && value <= 'Z')
            value = static_cast<TUint16>(value + ('a' - 'A'));
        hash ^= static_cast<TUint8>(value & 0xffu);
        hash *= 16777619u;
        hash ^= static_cast<TUint8>((value >> 8) & 0xffu);
        hash *= 16777619u;
    }
    return hash;
}

void CacheNameL(const TDesC& aRelative, TDes& aCacheName)
{
    TUint32 hash = HashVirtualPath(gVirtualVfs->ArchivePath(), 2166136261u);
    hash = HashVirtualPath(aRelative, hash);
    aCacheName.Format(_L("vfs_%08x"), hash);
    const TInt slash = aRelative.LocateReverse('/');
    const TInt dot = aRelative.LocateReverse('.');
    if (dot != KErrNotFound && dot > slash && aRelative.Length() - dot <= 12)
        aCacheName.Append(aRelative.Mid(dot));
}

FILE* OpenFileL(const char* aPath, const char* aMode)
{
    TFileName path;
    Utf8PathL(aPath, path);

    if (aMode && aMode[0] == 'r')
    {
        TFileName relative;
        if (VirtualRelativePathL(aPath, relative) && gVirtualVfs->Exists(relative))
        {
            TFileName cacheName;
            CacheNameL(relative, cacheName);
            gVirtualVfs->MaterializeFileL(relative,
                                          cacheName,
                                          path,
                                          KMaxTInt);
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
    gVirtualVfs = NULL;
    gVirtualRoot.Zero();
}

extern "C" FILE* EasyRpgFopen(const char* aPath, const char* aMode)
{
    FILE* result = NULL;
    TRAPD(error, result = OpenFileL(aPath, aMode));
    return error == KErrNone ? result : NULL;
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

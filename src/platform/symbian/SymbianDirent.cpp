#include <e32base.h>
#include <f32file.h>
#include "easyrpgsymbian/Encoding.h"
#include "easyrpgsymbian/SymbianDirentBridge.h"

struct __sFILE;
typedef struct __sFILE FILE;
extern "C" FILE* wfopen(const wchar_t* aName, const wchar_t* aMode);

struct EasyRpgDir
{
    RFs iFs;
    CDir* iEntries;
    TInt iIndex;
    EasyRpgDirent iEntry;
    char iName[1024];
};

namespace
{
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

EasyRpgDir* OpenDirL(const char* aPath)
{
    EasyRpgDir* dir = new (ELeave) EasyRpgDir;
    dir->iEntries = NULL;
    dir->iIndex = 0;
    User::LeaveIfError(dir->iFs.Connect());

    TFileName path;
    Utf8PathL(aPath, path);
    if (path.Length() == 0 || path[path.Length() - 1] != '\\')
        path.Append('\\');
    path.Append('*');

    const TInt error = dir->iFs.GetDir(path,
                                       KEntryAttNormal | KEntryAttDir | KEntryAttHidden | KEntryAttSystem,
                                       ESortNone,
                                       dir->iEntries);
    if (error != KErrNone)
    {
        dir->iFs.Close();
        delete dir;
        User::Leave(error);
    }
    return dir;
}

FILE* OpenFileL(const char* aPath, const char* aMode)
{
    TFileName path;
    Utf8PathL(aPath, path);
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
    if (!aDir || !aDir->iEntries || aDir->iIndex >= aDir->iEntries->Count())
        return NULL;

    const TEntry& entry = (*aDir->iEntries)[aDir->iIndex++];
    TPtr8 name(reinterpret_cast<TUint8*>(aDir->iName), 0, sizeof(aDir->iName) - 1);
    TRAPD(error, TEncoding::UnicodeToUtf8L(entry.iName, name));
    if (error != KErrNone)
        return NULL;
    aDir->iName[name.Length()] = 0;

    aDir->iEntry.d_fileno = 0;
    aDir->iEntry.d_namlen = static_cast<unsigned short>(name.Length());
    aDir->iEntry.d_name = aDir->iName;
    aDir->iEntry.d_type = entry.IsDir() ? 4 : 8;
    return &aDir->iEntry;
}

extern "C" int EasyRpgClosedir(EasyRpgDir* aDir)
{
    if (!aDir)
        return -1;
    delete aDir->iEntries;
    aDir->iEntries = NULL;
    aDir->iFs.Close();
    delete aDir;
    return 0;
}

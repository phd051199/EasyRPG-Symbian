#ifndef EASYRPG_APP_PATHS_H
#define EASYRPG_APP_PATHS_H

#include <e32std.h>

namespace EasyRpgPaths
{
inline TInt Build(TDes& aPath, const TDesC& aSuffix)
{
    const TFileName executable = RProcess().FileName();
    if (executable.Length() < 2 || executable[1] != ':')
        return KErrNotReady;
    if (aSuffix.Length() == 0 || aSuffix[0] != '\\')
        return KErrArgument;

    TPtrC suffix(aSuffix);
    if (aSuffix.Length() >= 5 &&
        aSuffix.Left(5).CompareF(_L("\\Data")) == 0 &&
        (aSuffix.Length() == 5 || aSuffix[5] == '\\'))
    {
        suffix.Set(aSuffix.Mid(5));
    }

    if (2 + suffix.Length() > aPath.MaxLength())
        return KErrOverflow;

    aPath.Zero();
    aPath.Append(executable.Left(2));
    aPath.Append(suffix);
    return KErrNone;
}

inline void BuildL(TDes& aPath, const TDesC& aSuffix)
{
    User::LeaveIfError(Build(aPath, aSuffix));
}

// Spool a virtual (archive) file into a private temp file below the given
// directory suffix so platform media players can open it by path.
// Implemented in NativeAudio.cpp.
void SpoolVirtualFileL(const TDesC& aVirtualSource,
                       const TDesC& aDirectorySuffix,
                       const TDesC& aBaseName,
                       TDes& aSpoolPath);

}

#endif

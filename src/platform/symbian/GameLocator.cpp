#include "easyrpgsymbian/GameLocator.h"

namespace
{
void NormalizeRoot(TFileName& aPath)
{
    if (aPath.Length() == 0)
        return;

    const TChar last = aPath[aPath.Length() - 1];
    if (last != '\\' && last != '/')
        aPath.Append(_L("\\"));
}

TBool BuildChildPath(const TDesC& aRoot, const TDesC& aName, TFileName& aOut)
{
    if (aRoot.Length() + aName.Length() + 1 > aOut.MaxLength())
        return EFalse;

    aOut.Copy(aRoot);
    NormalizeRoot(aOut);
    if (aOut.Length() + aName.Length() > aOut.MaxLength())
        return EFalse;
    aOut.Append(aName);
    return ETrue;
}
}

TBool TGameLocator::FileExists(RFs& aFs, const TDesC& aPath)
{
    TEntry entry;
    const TInt error = aFs.Entry(aPath, entry);
    return error == KErrNone && !entry.IsDir();
}

TBool TGameLocator::IsRpg2kProject(RFs& aFs, const TDesC& aRoot)
{
    TFileName databasePath;
    TFileName treePath;

    if (!BuildChildPath(aRoot, _L("RPG_RT.ldb"), databasePath))
        return EFalse;
    if (!BuildChildPath(aRoot, _L("RPG_RT.lmt"), treePath))
        return EFalse;

    return FileExists(aFs, databasePath) && FileExists(aFs, treePath);
}

TBool TGameLocator::TestRoot(RFs& aFs, const TDesC& aCandidate, TFileName& aRoot)
{
    if (!IsRpg2kProject(aFs, aCandidate))
        return EFalse;

    aRoot.Copy(aCandidate);
    NormalizeRoot(aRoot);
    return ETrue;
}

TInt TGameLocator::FindNobihazaRoot(RFs& aFs, TFileName& aRoot)
{
    aRoot.Zero();

    // Prefer the memory card. The first port intentionally consumes an
    // already-extracted RPG Maker project instead of decoding AES ZIP data on
    // the E72. tools/prepare_nobihaza.py stages to this canonical directory.
    _LIT(KMemoryCardCanonical, "E:\\EasyRPG\\Games\\NobihazaVH\\");
    _LIT(KMemoryCardDisplayName, "E:\\EasyRPG\\Games\\Nobihaza VH\\");
    _LIT(KMemoryCardRoot, "E:\\Nobihaza VH\\");
    _LIT(KPhoneCanonical, "C:\\Data\\EasyRPG\\Games\\NobihazaVH\\");
    _LIT(KPhoneDisplayName, "C:\\Data\\EasyRPG\\Games\\Nobihaza VH\\");

    if (TestRoot(aFs, KMemoryCardCanonical, aRoot))
        return KErrNone;
    if (TestRoot(aFs, KMemoryCardDisplayName, aRoot))
        return KErrNone;
    if (TestRoot(aFs, KMemoryCardRoot, aRoot))
        return KErrNone;
    if (TestRoot(aFs, KPhoneCanonical, aRoot))
        return KErrNone;
    if (TestRoot(aFs, KPhoneDisplayName, aRoot))
        return KErrNone;

    return KErrNotFound;
}

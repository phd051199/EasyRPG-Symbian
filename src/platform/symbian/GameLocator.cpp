#include "easyrpgsymbian/GameLocator.h"

TBool TGameLocator::FileExists(RFs& aFs, const TDesC& aPath)
{
    TEntry entry;
    const TInt error = aFs.Entry(aPath, entry);
    return error == KErrNone && !entry.IsDir();
}

TInt TGameLocator::FindNobihazaArchive(RFs& aFs, TFileName& aArchivePath)
{
    aArchivePath.Zero();

    // Prefer the E72 internal user-data drive. No extraction/renaming step is
    // required; CZipVfs reads archive metadata directly so Japanese resource
    // names are preserved. Memory-card paths remain optional fallbacks.
    _LIT(KPhoneCanonical, "C:\\Data\\EasyRPG\\Games\\NobihazaVH.zip");
    _LIT(KPhoneDisplayName, "C:\\Data\\EasyRPG\\Games\\Nobihaza VH.zip");
    _LIT(KPhoneDataCanonical, "C:\\Data\\NobihazaVH.zip");
    _LIT(KPhoneDataDisplayName, "C:\\Data\\Nobihaza VH.zip");
    _LIT(KMemoryCardCanonical, "E:\\EasyRPG\\Games\\NobihazaVH.zip");
    _LIT(KMemoryCardDisplayName, "E:\\EasyRPG\\Games\\Nobihaza VH.zip");
    _LIT(KMemoryCardRootCanonical, "E:\\NobihazaVH.zip");
    _LIT(KMemoryCardRootDisplayName, "E:\\Nobihaza VH.zip");

    if (FileExists(aFs, KPhoneCanonical))
    {
        aArchivePath.Copy(KPhoneCanonical);
        return KErrNone;
    }
    if (FileExists(aFs, KPhoneDisplayName))
    {
        aArchivePath.Copy(KPhoneDisplayName);
        return KErrNone;
    }
    if (FileExists(aFs, KPhoneDataCanonical))
    {
        aArchivePath.Copy(KPhoneDataCanonical);
        return KErrNone;
    }
    if (FileExists(aFs, KPhoneDataDisplayName))
    {
        aArchivePath.Copy(KPhoneDataDisplayName);
        return KErrNone;
    }
    if (FileExists(aFs, KMemoryCardCanonical))
    {
        aArchivePath.Copy(KMemoryCardCanonical);
        return KErrNone;
    }
    if (FileExists(aFs, KMemoryCardDisplayName))
    {
        aArchivePath.Copy(KMemoryCardDisplayName);
        return KErrNone;
    }
    if (FileExists(aFs, KMemoryCardRootCanonical))
    {
        aArchivePath.Copy(KMemoryCardRootCanonical);
        return KErrNone;
    }
    if (FileExists(aFs, KMemoryCardRootDisplayName))
    {
        aArchivePath.Copy(KMemoryCardRootDisplayName);
        return KErrNone;
    }

    return KErrNotFound;
}

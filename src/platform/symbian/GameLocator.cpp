#include "easyrpgsymbian/GameLocator.h"

namespace
{
_LIT(KEasyRpgGamesPhone, "C:\\Data\\EasyRPG\\Games\\");
_LIT(KEasyRpgGamesMemoryCard, "E:\\EasyRPG\\Games\\");
_LIT(KZipPattern, "*.zip");
_LIT(KZipExtension, ".zip");
}

void TGameLocator::ListGameArchivesL(RFs& aFs,
                                     CDesCArrayFlat& aDisplayNames,
                                     CDesCArrayFlat& aArchivePaths)
{
    aDisplayNames.Reset();
    aArchivePaths.Reset();

    AppendGameArchivesL(aFs,
                        KEasyRpgGamesPhone,
                        aDisplayNames,
                        aArchivePaths);
    AppendGameArchivesL(aFs,
                        KEasyRpgGamesMemoryCard,
                        aDisplayNames,
                        aArchivePaths);
}

void TGameLocator::AppendGameArchivesL(RFs& aFs,
                                       const TDesC& aDirectory,
                                       CDesCArrayFlat& aDisplayNames,
                                       CDesCArrayFlat& aArchivePaths)
{
    TFileName pattern;
    if (aDirectory.Length() + KZipPattern().Length() > pattern.MaxLength())
        User::Leave(KErrOverflow);

    pattern.Copy(aDirectory);
    pattern.Append(KZipPattern);

    CDir* directory = NULL;
    const TInt error = aFs.GetDir(pattern,
                                  KEntryAttNormal,
                                  ESortByName,
                                  directory);
    if (error == KErrNotFound || error == KErrPathNotFound ||
        error == KErrNotReady)
        return;
    User::LeaveIfError(error);
    CleanupStack::PushL(directory);

    for (TInt index = 0; index < directory->Count(); ++index)
    {
        const TEntry& entry = (*directory)[index];
        if (entry.IsDir())
            continue;

        TFileName archivePath;
        if (aDirectory.Length() + entry.iName.Length() > archivePath.MaxLength())
            continue;
        archivePath.Copy(aDirectory);
        archivePath.Append(entry.iName);

        TFileName title;
        title.Copy(entry.iName.Left(title.MaxLength()));
        if (title.Length() >= KZipExtension().Length() &&
            title.Right(KZipExtension().Length()).CompareF(KZipExtension) == 0)
        {
            title.SetLength(title.Length() - KZipExtension().Length());
        }

        TBuf<KMaxFileName> displayName;
        if (title.Length() > displayName.MaxLength() - 6)
            title.SetLength(displayName.MaxLength() - 6);
        displayName.Format(_L("%S [%c:]"), &title, aDirectory[0]);

        aDisplayNames.AppendL(displayName);
        aArchivePaths.AppendL(archivePath);
    }

    CleanupStack::PopAndDestroy(directory);
}

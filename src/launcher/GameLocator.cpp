#include "GameLocator.h"
#include "AppConfig.h"
#include "AppPaths.h"

namespace
{
_LIT(KZipPattern, "*.zip");
_LIT(KZipExtension, ".zip");
} // namespace

void TGameLocator::ListGameArchivesL(RFs &aFs,
                                     CDesCArrayFlat &aDisplayNames,
                                     CDesCArrayFlat &aArchivePaths,
                                     RArray<TInt> &aArchiveSizes)
{
    aDisplayNames.Reset();
    aArchivePaths.Reset();
    aArchiveSizes.Reset();

    TFileName installedGamesDirectory;
    if (EasyRpgPaths::Build(installedGamesDirectory,
                            EasyRpgApp::KGamesDirectorySuffix) == KErrNone)
    {
        AppendGameArchivesL(aFs,
                            installedGamesDirectory,
                            aDisplayNames,
                            aArchivePaths,
                            aArchiveSizes);
    }
    if (installedGamesDirectory.CompareF(EasyRpgApp::KMemoryCardGamesDirectory) != 0)
    {
        AppendGameArchivesL(aFs,
                            EasyRpgApp::KMemoryCardGamesDirectory,
                            aDisplayNames,
                            aArchivePaths,
                            aArchiveSizes);
    }
}

void TGameLocator::AppendGameArchivesL(RFs &aFs,
                                       const TDesC &aDirectory,
                                       CDesCArrayFlat &aDisplayNames,
                                       CDesCArrayFlat &aArchivePaths,
                                       RArray<TInt> &aArchiveSizes)
{
    TFileName pattern;
    if (aDirectory.Length() + KZipPattern().Length() > pattern.MaxLength())
        User::Leave(KErrOverflow);

    pattern.Copy(aDirectory);
    pattern.Append(KZipPattern);

    CDir *directory = NULL;
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
        const TEntry &entry = (*directory)[index];
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
        displayName.Copy(title.Left(displayName.MaxLength()));
        aArchivePaths.AppendL(archivePath);
        aDisplayNames.AppendL(displayName);
        aArchiveSizes.AppendL(entry.iSize);
    }

    CleanupStack::PopAndDestroy(directory);
}

#ifndef EASYRPG_SYMBIAN_GAME_LOCATOR_H
#define EASYRPG_SYMBIAN_GAME_LOCATOR_H

#include <badesca.h>
#include <e32base.h>
#include <f32file.h>

class TGameLocator
{
public:
    // Enumerates supported game archives from exactly these two locations:
    // E:\EasyRPG\Games\ and C:\EasyRPG\Games\. No fallback directories are
    // scanned. Display names and archive paths are appended in matching order.
    static void ListGameArchivesL(RFs& aFs,
                                  CDesCArrayFlat& aDisplayNames,
                                  CDesCArrayFlat& aArchivePaths);

private:
    static void AppendGameArchivesL(RFs& aFs,
                                    const TDesC& aDirectory,
                                    CDesCArrayFlat& aDisplayNames,
                                    CDesCArrayFlat& aArchivePaths);
};

#endif

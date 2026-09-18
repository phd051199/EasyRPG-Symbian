#ifndef EASYRPG_GAME_LOCATOR_H
#define EASYRPG_GAME_LOCATOR_H

#include <badesca.h>
#include <e32base.h>
#include <f32file.h>

class TGameLocator
{
  public:
    static void ListGameArchivesL(RFs &aFs,
                                  CDesCArrayFlat &aDisplayNames,
                                  CDesCArrayFlat &aArchivePaths,
                                  RArray<TInt> &aArchiveSizes);

  private:
    static void AppendGameArchivesL(RFs &aFs,
                                    const TDesC &aDirectory,
                                    CDesCArrayFlat &aDisplayNames,
                                    CDesCArrayFlat &aArchivePaths,
                                    RArray<TInt> &aArchiveSizes);
};

#endif

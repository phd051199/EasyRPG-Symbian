#ifndef EASYRPG_SYMBIAN_GAME_LOCATOR_H
#define EASYRPG_SYMBIAN_GAME_LOCATOR_H

#include <e32base.h>
#include <f32file.h>

class TGameLocator
{
public:
    static TInt FindNobihazaRoot(RFs& aFs, TFileName& aRoot);
    static TBool IsRpg2kProject(RFs& aFs, const TDesC& aRoot);

private:
    static TBool FileExists(RFs& aFs, const TDesC& aPath);
    static TBool TestRoot(RFs& aFs, const TDesC& aCandidate, TFileName& aRoot);
};

#endif

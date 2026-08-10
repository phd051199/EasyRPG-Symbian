#ifndef EASYRPG_SYMBIAN_GAME_LOCATOR_H
#define EASYRPG_SYMBIAN_GAME_LOCATOR_H

#include <e32base.h>
#include <f32file.h>

class TGameLocator
{
public:
    static TInt FindNobihazaArchive(RFs& aFs, TFileName& aArchivePath);

private:
    static TBool FileExists(RFs& aFs, const TDesC& aPath);
};

#endif

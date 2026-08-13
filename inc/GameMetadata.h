#ifndef EASYRPG_GAME_METADATA_H
#define EASYRPG_GAME_METADATA_H

#include <e32base.h>
#include <f32file.h>

void ReadGameMetadataL(RFs& aFs,
                       const TDesC& aArchivePath,
                       TDes& aTitle,
                       TInt& aVersion);
void FormatGameSubtitle(RFs& aFs,
                        const TDesC& aArchivePath,
                        TInt aVersion,
                        TDes& aSubtitle);

#endif

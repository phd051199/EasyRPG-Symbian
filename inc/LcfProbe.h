#ifndef EASYRPG_LCF_PROBE_H
#define EASYRPG_LCF_PROBE_H

#include <e32base.h>

class CZipVfs;

struct TLcfProjectSummary
{
    TUint32 iDatabaseRecords;
    TUint32 iMapTreeEntries;
    TUint32 iMapUnitRecords;
    TUint32 iDatabaseHash;
    TUint32 iMapTreeHash;
    TUint32 iMapUnitHash;

    TUint32 iActiveNode;
    TUint32 iStartMapId;
    TUint32 iStartX;
    TUint32 iStartY;

    TUint32 iMapChipsetId;
    TUint32 iMapWidth;
    TUint32 iMapHeight;
    TUint32 iMapLowerLayerTiles;
    TUint32 iMapUpperLayerTiles;
    TUint32 iMapEventCount;
    TUint32 iMapEventPageCount;
    TUint32 iMapEventCommandCount;
    TUint32 iMapSaveCount;
    TUint32 iInitialActorId;
    TUint32 iInitialActorCharacterIndex;
    TBool iInitialActorTransparent;

    TBuf8<96> iInitialActorCharacterNameSjis;
    TBuf8<96> iTitleNameSjis;
    TBuf8<96> iSystemNameSjis;
    TBuf8<96> iChipsetNameSjis;
};

class TLcfProbe
{
public:
    static TInt ProbeProject(CZipVfs& aResources,
                             TDes& aGameTitle,
                             TLcfProjectSummary& aSummary);

private:
    static TInt InspectFile(CZipVfs& aResources,
                            const TDesC& aFileName,
                            TInt aFileType,
                            TLcfProjectSummary& aSummary);
    static TInt ReadGameTitle(CZipVfs& aResources, TDes& aGameTitle);
};

#endif

#include "easyrpgsymbian/LcfProbe.h"
#include "easyrpgsymbian/UpstreamLmuBridge.h"
#include "easyrpgsymbian/ZipVfs.h"

namespace
{
const TInt KMaxProbeFileSize = 4 * 1024 * 1024;

enum TLcfProbeFileType
{
    ELcfDatabase = 1,
    ELcfMapTree = 2,
    ELcfMapUnit = 3
};

TInt NarrowAsciiPath(const TDesC& aSource, TDes8& aDestination)
{
    aDestination.Zero();
    if (aSource.Length() + 1 > aDestination.MaxLength())
        return KErrOverflow;

    TInt index = 0;
    for (index = 0; index < aSource.Length(); ++index)
    {
        const TUint16 value = aSource[index];
        if (value > 0x7fu)
            return KErrNotSupported;
        aDestination.Append(static_cast<TUint8>(value));
    }
    aDestination.Append(static_cast<TUint8>(0));
    return KErrNone;
}

const char* NarrowPathPtr(const TDesC8& aPath)
{
    return reinterpret_cast<const char*>(aPath.Ptr());
}

void CopyCoreName(const unsigned char* aSource,
                  TUint32 aSourceLength,
                  TDes8& aDestination)
{
    aDestination.Zero();
    if (!aSource || aSourceLength == 0)
        return;

    TInt copyLength = static_cast<TInt>(aSourceLength);
    if (copyLength > aDestination.MaxLength())
        copyLength = aDestination.MaxLength();
    aDestination.Append(aSource, copyLength);
}
}

TInt TLcfProbe::ProbeProject(CZipVfs& aResources,
                             TDes& aGameTitle,
                             TLcfProjectSummary& aSummary)
{
    aGameTitle.Zero();
    aSummary.iDatabaseRecords = 0;
    aSummary.iMapTreeEntries = 0;
    aSummary.iMapUnitRecords = 0;
    aSummary.iDatabaseHash = 0;
    aSummary.iMapTreeHash = 0;
    aSummary.iMapUnitHash = 0;
    aSummary.iActiveNode = 0;
    aSummary.iStartMapId = 0;
    aSummary.iStartX = 0;
    aSummary.iStartY = 0;
    aSummary.iMapChipsetId = 0;
    aSummary.iMapWidth = 0;
    aSummary.iMapHeight = 0;
    aSummary.iMapLowerLayerTiles = 0;
    aSummary.iMapUpperLayerTiles = 0;
    aSummary.iMapEventCount = 0;
    aSummary.iMapEventPageCount = 0;
    aSummary.iMapEventCommandCount = 0;
    aSummary.iMapSaveCount = 0;
    aSummary.iInitialActorId = 0;
    aSummary.iInitialActorCharacterIndex = 0;
    aSummary.iInitialActorTransparent = EFalse;
    aSummary.iInitialActorCharacterNameSjis.Zero();
    aSummary.iTitleNameSjis.Zero();
    aSummary.iSystemNameSjis.Zero();
    aSummary.iChipsetNameSjis.Zero();

    TInt error = InspectFile(aResources,
                             _L("RPG_RT.lmt"),
                             ELcfMapTree,
                             aSummary);
    if (error == KErrNone)
    {
        TFileName startMapName;
        startMapName.Format(_L("Map%04u.lmu"), aSummary.iStartMapId);
        error = InspectFile(aResources,
                            startMapName,
                            ELcfMapUnit,
                            aSummary);
    }
    if (error == KErrNone)
    {
        error = InspectFile(aResources,
                            _L("RPG_RT.ldb"),
                            ELcfDatabase,
                            aSummary);
    }

    if (error == KErrNone)
    {
        const TInt titleError = ReadGameTitle(aResources, aGameTitle);
        if (titleError != KErrNone)
            aGameTitle.Copy(_L("RPG Maker 2000 project"));
    }

    return error;
}

TInt TLcfProbe::InspectFile(CZipVfs& aResources,
                            const TDesC& aFileName,
                            TInt aFileType,
                            TLcfProjectSummary& aSummary)
{
    TFileName materializedPath;
    TPtrC cacheName;
    switch (aFileType)
    {
    case ELcfDatabase:
        cacheName.Set(_L("RPG_RT.ldb"));
        break;
    case ELcfMapTree:
        cacheName.Set(_L("RPG_RT.lmt"));
        break;
    case ELcfMapUnit:
        cacheName.Set(_L("active.lmu"));
        break;
    default:
        return KErrArgument;
    }

    TRAPD(materializeError,
          aResources.MaterializeFileL(aFileName,
                                      cacheName,
                                      materializedPath,
                                      KMaxProbeFileSize));
    if (materializeError != KErrNone)
        return materializeError;

    TBuf8<KMaxFileName + 1> narrowPath;
    const TInt narrowError = NarrowAsciiPath(materializedPath, narrowPath);
    if (narrowError != KErrNone)
        return narrowError;

    const char* path = NarrowPathPtr(narrowPath);

    switch (aFileType)
    {
    case ELcfMapTree:
    {
        EasyRpgUpstreamLmtSummary upstream;
        const TInt coreError = EasyRpgUpstreamLoadLmtSummary(path, "", &upstream);
        if (coreError != 0 || upstream.start_map_id <= 0 || upstream.map_count == 0)
            return KErrCorrupt;

        aSummary.iMapTreeEntries = upstream.map_count;
        aSummary.iActiveNode = static_cast<TUint32>(upstream.active_node);
        aSummary.iStartMapId = static_cast<TUint32>(upstream.start_map_id);
        aSummary.iStartX = static_cast<TUint32>(upstream.start_x);
        aSummary.iStartY = static_cast<TUint32>(upstream.start_y);
        return KErrNone;
    }

    case ELcfMapUnit:
    {
        EasyRpgUpstreamLmuSummary upstream;
        const TInt coreError = EasyRpgUpstreamReadLmuSummary(path, "", &upstream);
        if (coreError != 0 || upstream.width <= 0 || upstream.height <= 0)
            return KErrCorrupt;

        const TUint32 expectedTiles =
            static_cast<TUint32>(upstream.width) * static_cast<TUint32>(upstream.height);
        if (upstream.lower_layer_count != expectedTiles ||
            upstream.upper_layer_count != expectedTiles)
            return KErrCorrupt;

        aSummary.iMapUnitRecords = 1;
        aSummary.iMapChipsetId = static_cast<TUint32>(upstream.chipset_id);
        aSummary.iMapWidth = static_cast<TUint32>(upstream.width);
        aSummary.iMapHeight = static_cast<TUint32>(upstream.height);
        aSummary.iMapLowerLayerTiles = upstream.lower_layer_count;
        aSummary.iMapUpperLayerTiles = upstream.upper_layer_count;
        aSummary.iMapEventCount = upstream.event_count;
        aSummary.iMapEventPageCount = upstream.event_page_count;
        aSummary.iMapEventCommandCount = upstream.event_command_count;
        aSummary.iMapSaveCount = upstream.save_count;
        return KErrNone;
    }

    case ELcfDatabase:
    {
        unsigned char titleName[96];
        unsigned char systemName[96];
        unsigned char chipsetName[96];
        unsigned int titleNameLength = 0;
        unsigned int systemNameLength = 0;
        unsigned int chipsetNameLength = 0;
        EasyRpgUpstreamLdbSummary upstream;

        const TInt coreError = EasyRpgUpstreamLoadLdbSummary(
            path,
            "",
            static_cast<int>(aSummary.iMapChipsetId),
            &upstream,
            titleName,
            sizeof(titleName),
            &titleNameLength,
            systemName,
            sizeof(systemName),
            &systemNameLength,
            chipsetName,
            sizeof(chipsetName),
            &chipsetNameLength);
        if (coreError != 0 || upstream.chipset_count == 0)
            return coreError == -3 ? KErrNotFound : KErrCorrupt;

        aSummary.iDatabaseRecords = upstream.actor_count +
                                    upstream.chipset_count +
                                    upstream.common_event_count;
        aSummary.iInitialActorId = static_cast<TUint32>(upstream.initial_actor_id > 0 ? upstream.initial_actor_id : 0);
        aSummary.iInitialActorCharacterIndex = static_cast<TUint32>(upstream.initial_actor_character_index > 0 ? upstream.initial_actor_character_index : 0);
        aSummary.iInitialActorTransparent = upstream.initial_actor_transparent != 0;
        CopyCoreName(upstream.initial_actor_character_name,
                     upstream.initial_actor_character_name_length,
                     aSummary.iInitialActorCharacterNameSjis);
        CopyCoreName(titleName, titleNameLength, aSummary.iTitleNameSjis);
        CopyCoreName(systemName, systemNameLength, aSummary.iSystemNameSjis);
        CopyCoreName(chipsetName, chipsetNameLength, aSummary.iChipsetNameSjis);
        if (aSummary.iTitleNameSjis.Length() == 0 ||
            aSummary.iSystemNameSjis.Length() == 0 ||
            aSummary.iChipsetNameSjis.Length() == 0)
            return KErrCorrupt;
        return KErrNone;
    }
    }

    return KErrArgument;
}

TInt TLcfProbe::ReadGameTitle(CZipVfs& aResources, TDes& aGameTitle)
{
    HBufC8* fileData = NULL;
    TRAPD(error, fileData = aResources.ReadFileLC(_L("RPG_RT.ini"), 64 * 1024);
                 CleanupStack::Pop(fileData));
    if (error != KErrNone || !fileData)
        return error != KErrNone ? error : KErrNoMemory;

    const TPtrC8 allBytes(*fileData);
    const TPtrC8 bytes = allBytes.Left(allBytes.Length() > 512 ? 512 : allBytes.Length());

    _LIT8(KGameTitleKey, "GameTitle=");
    const TInt keyPosition = bytes.Find(KGameTitleKey);
    if (keyPosition == KErrNotFound)
    {
        delete fileData;
        return KErrNotFound;
    }

    TInt position = keyPosition + KGameTitleKey().Length();
    aGameTitle.Zero();
    while (position < bytes.Length() && aGameTitle.Length() < aGameTitle.MaxLength())
    {
        const TUint8 value = bytes[position++];
        if (value == '\r' || value == '\n')
            break;
        aGameTitle.Append(static_cast<TUint16>(value));
    }

    const TInt result = aGameTitle.Length() > 0 ? KErrNone : KErrNotFound;
    delete fileData;
    return result;
}

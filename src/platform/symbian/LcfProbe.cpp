#include "easyrpgsymbian/CoreLcf.h"
#include "easyrpgsymbian/LcfProbe.h"
#include "easyrpgsymbian/ZipVfs.h"

namespace
{
const TInt KMaxProbeFileSize = 4 * 1024 * 1024;

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
    aSummary.iTitleNameSjis.Zero();
    aSummary.iSystemNameSjis.Zero();
    aSummary.iChipsetNameSjis.Zero();

    // Resolve the active map first so the following LDB pass can select the
    // exact chipset resource referenced by that map without keeping the full
    // database resident in memory.
    TInt error = InspectFile(aResources,
                             _L("RPG_RT.lmt"),
                             EASYRPG_LCF_MAP_TREE,
                             aSummary);
    if (error == KErrNone)
    {
        TFileName startMapName;
        startMapName.Format(_L("Map%04u.lmu"), aSummary.iStartMapId);
        error = InspectFile(aResources,
                            startMapName,
                            EASYRPG_LCF_MAP_UNIT,
                            aSummary);
    }
    if (error == KErrNone)
    {
        error = InspectFile(aResources,
                            _L("RPG_RT.ldb"),
                            EASYRPG_LCF_DATABASE,
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
    HBufC8* buffer = NULL;
    TRAPD(error, buffer = aResources.ReadFileLC(aFileName, KMaxProbeFileSize);
                 CleanupStack::Pop(buffer));
    if (error != KErrNone || !buffer)
        return error != KErrNone ? error : KErrNoMemory;

    const TPtrC8 bytes(*buffer);

    EasyRpgCoreLcfSummary coreSummary;
    const unsigned char* rawBytes =
        reinterpret_cast<const unsigned char*>(bytes.Ptr());
    const unsigned int rawSize = static_cast<unsigned int>(bytes.Length());
    const TInt coreError = EasyRpgCoreInspectLcf(aFileType,
                                                 rawBytes,
                                                 rawSize,
                                                 &coreSummary);

    unsigned char chipsetName[EASYRPG_LCF_RESOURCE_NAME_CAPACITY];
    unsigned int chipsetNameLength = 0;
    TInt chipsetError = KErrNone;
    if (coreError == 0 && aFileType == EASYRPG_LCF_DATABASE)
    {
        if (aSummary.iMapChipsetId == 0)
        {
            chipsetError = KErrNotReady;
        }
        else
        {
            const TInt coreChipsetError = EasyRpgCoreFindChipsetName(
                rawBytes,
                rawSize,
                aSummary.iMapChipsetId,
                chipsetName,
                sizeof(chipsetName),
                &chipsetNameLength);
            if (coreChipsetError != 0)
                chipsetError = KErrNotFound;
        }
    }

    delete buffer;
    buffer = NULL;

    if (coreError != 0 || coreSummary.record_count == 0)
        return KErrCorrupt;
    if (chipsetError != KErrNone)
        return chipsetError;

    switch (aFileType)
    {
    case EASYRPG_LCF_DATABASE:
        aSummary.iDatabaseRecords = coreSummary.record_count;
        aSummary.iDatabaseHash = coreSummary.fnv1a;
        CopyCoreName(coreSummary.title_name,
                     coreSummary.title_name_length,
                     aSummary.iTitleNameSjis);
        CopyCoreName(coreSummary.system_name,
                     coreSummary.system_name_length,
                     aSummary.iSystemNameSjis);
        CopyCoreName(chipsetName,
                     chipsetNameLength,
                     aSummary.iChipsetNameSjis);
        if (aSummary.iTitleNameSjis.Length() == 0 ||
            aSummary.iSystemNameSjis.Length() == 0 ||
            aSummary.iChipsetNameSjis.Length() == 0)
            return KErrCorrupt;
        break;

    case EASYRPG_LCF_MAP_TREE:
        if (!coreSummary.terminator_seen || coreSummary.start_map_id == 0)
            return KErrCorrupt;
        aSummary.iMapTreeEntries = coreSummary.record_count;
        aSummary.iMapTreeHash = coreSummary.fnv1a;
        aSummary.iActiveNode = coreSummary.active_node;
        aSummary.iStartMapId = coreSummary.start_map_id;
        aSummary.iStartX = coreSummary.start_x;
        aSummary.iStartY = coreSummary.start_y;
        break;

    case EASYRPG_LCF_MAP_UNIT:
        if (!coreSummary.terminator_seen ||
            coreSummary.map_width == 0 ||
            coreSummary.map_height == 0)
            return KErrCorrupt;
        aSummary.iMapUnitRecords = coreSummary.record_count;
        aSummary.iMapUnitHash = coreSummary.fnv1a;
        aSummary.iMapChipsetId = coreSummary.map_chipset_id;
        aSummary.iMapWidth = coreSummary.map_width;
        aSummary.iMapHeight = coreSummary.map_height;
        aSummary.iMapLowerLayerTiles = coreSummary.map_lower_layer_tiles;
        aSummary.iMapUpperLayerTiles = coreSummary.map_upper_layer_tiles;
        aSummary.iMapEventCount = coreSummary.map_event_count;
        aSummary.iMapEventPageCount = coreSummary.map_event_page_count;
        aSummary.iMapEventCommandCount = coreSummary.map_event_command_count;
        aSummary.iMapSaveCount = coreSummary.map_save_count;
        break;

    default:
        return KErrArgument;
    }

    return KErrNone;
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

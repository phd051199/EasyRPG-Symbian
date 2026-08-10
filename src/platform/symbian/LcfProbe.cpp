#include "easyrpgsymbian/CoreLcf.h"
#include "easyrpgsymbian/LcfProbe.h"

namespace
{
const TInt KMaxProbeFileSize = 4 * 1024 * 1024;

TInt BuildPath(const TDesC& aRoot, const TDesC& aName, TFileName& aPath)
{
    if (aRoot.Length() + aName.Length() + 1 > aPath.MaxLength())
        return KErrOverflow;

    aPath.Copy(aRoot);
    if (aPath.Length() > 0)
    {
        const TChar last = aPath[aPath.Length() - 1];
        if (last != '\\' && last != '/')
            aPath.Append(_L("\\"));
    }

    if (aPath.Length() + aName.Length() > aPath.MaxLength())
        return KErrOverflow;
    aPath.Append(aName);
    return KErrNone;
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

TInt TLcfProbe::ProbeProject(const TDesC& aGameRoot,
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
    aSummary.iMapSaveCount = 0;
    aSummary.iTitleNameSjis.Zero();
    aSummary.iSystemNameSjis.Zero();
    aSummary.iChipsetNameSjis.Zero();

    if (aGameRoot.Length() == 0)
        return KErrNotFound;

    RFs fs;
    TInt error = fs.Connect();
    if (error != KErrNone)
        return error;

    // Resolve the active map first so the following LDB pass can select the
    // exact chipset resource referenced by that map without keeping the full
    // database resident in memory.
    error = InspectFile(fs,
                        aGameRoot,
                        _L("RPG_RT.lmt"),
                        EASYRPG_LCF_MAP_TREE,
                        aSummary);
    if (error == KErrNone)
    {
        TFileName startMapName;
        startMapName.Format(_L("Map%04u.lmu"), aSummary.iStartMapId);
        error = InspectFile(fs,
                            aGameRoot,
                            startMapName,
                            EASYRPG_LCF_MAP_UNIT,
                            aSummary);
    }
    if (error == KErrNone)
    {
        error = InspectFile(fs,
                            aGameRoot,
                            _L("RPG_RT.ldb"),
                            EASYRPG_LCF_DATABASE,
                            aSummary);
    }

    if (error == KErrNone)
    {
        const TInt titleError = ReadGameTitle(fs, aGameRoot, aGameTitle);
        if (titleError != KErrNone)
            aGameTitle.Copy(_L("RPG Maker 2000 project"));
    }

    fs.Close();
    return error;
}

TInt TLcfProbe::InspectFile(RFs& aFs,
                            const TDesC& aGameRoot,
                            const TDesC& aFileName,
                            TInt aFileType,
                            TLcfProjectSummary& aSummary)
{
    TFileName path;
    TInt error = BuildPath(aGameRoot, aFileName, path);
    if (error != KErrNone)
        return error;

    RFile file;
    error = file.Open(aFs, path, EFileRead | EFileShareReadersOnly);
    if (error != KErrNone)
        return error;

    TInt fileSize = 0;
    error = file.Size(fileSize);
    if (error != KErrNone)
    {
        file.Close();
        return error;
    }
    if (fileSize <= 0 || fileSize > KMaxProbeFileSize)
    {
        file.Close();
        return KErrTooBig;
    }

    HBufC8* buffer = NULL;
    TRAP(error, buffer = HBufC8::NewL(fileSize));
    if (error != KErrNone || !buffer)
    {
        file.Close();
        return error != KErrNone ? error : KErrNoMemory;
    }

    TPtr8 bytes = buffer->Des();
    error = file.Read(bytes, fileSize);
    file.Close();
    if (error != KErrNone)
    {
        delete buffer;
        return error;
    }
    if (bytes.Length() != fileSize)
    {
        delete buffer;
        return KErrEof;
    }

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
        aSummary.iMapSaveCount = coreSummary.map_save_count;
        break;

    default:
        return KErrArgument;
    }

    return KErrNone;
}

TInt TLcfProbe::ReadGameTitle(RFs& aFs, const TDesC& aGameRoot, TDes& aGameTitle)
{
    TFileName path;
    TInt error = BuildPath(aGameRoot, _L("RPG_RT.ini"), path);
    if (error != KErrNone)
        return error;

    RFile file;
    error = file.Open(aFs, path, EFileRead | EFileShareReadersOnly);
    if (error != KErrNone)
        return error;

    TBuf8<512> bytes;
    error = file.Read(bytes);
    file.Close();
    if (error != KErrNone)
        return error;

    _LIT8(KGameTitleKey, "GameTitle=");
    const TInt keyPosition = bytes.Find(KGameTitleKey);
    if (keyPosition == KErrNotFound)
        return KErrNotFound;

    TInt position = keyPosition + KGameTitleKey().Length();
    aGameTitle.Zero();
    while (position < bytes.Length() && aGameTitle.Length() < aGameTitle.MaxLength())
    {
        const TUint8 value = bytes[position++];
        if (value == '\r' || value == '\n')
            break;
        aGameTitle.Append(static_cast<TUint16>(value));
    }

    return aGameTitle.Length() > 0 ? KErrNone : KErrNotFound;
}

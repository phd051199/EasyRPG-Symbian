#include "easyrpgsymbian/PlayerRuntime.h"
#include "easyrpgsymbian/DisplaySurface.h"
#include "easyrpgsymbian/Encoding.h"
#include "easyrpgsymbian/LcfProbe.h"
#include "easyrpgsymbian/ModernCoreProbe.h"
#include "easyrpgsymbian/UpstreamLcfLinkAnchor.h"
#include "easyrpgsymbian/UpstreamGraphicsBridge.h"
#include "easyrpgsymbian/ZipVfs.h"

namespace
{
const TInt KMaxMapFileBytes = 4 * 1024 * 1024;
const TInt KMaxGraphicFileBytes = 2 * 1024 * 1024;

void NarrowAsciiPathL(const TDesC& aSource, TDes8& aDestination)
{
    aDestination.Zero();
    if (aSource.Length() + 1 > aDestination.MaxLength())
        User::Leave(KErrOverflow);
    for (TInt index = 0; index < aSource.Length(); ++index)
    {
        const TUint16 value = aSource[index];
        if (value > 0x7fu)
            User::Leave(KErrNotSupported);
        aDestination.Append(static_cast<TUint8>(value));
    }
    aDestination.Append(static_cast<TUint8>(0));
}

void FindGraphicPathL(CZipVfs& aResources,
                      const TDesC& aDirectory,
                      const TDesC8& aGraphicNameSjis,
                      TDes& aPath)
{
    TBuf<128> graphicName;
    TEncoding::ShiftJisToUnicodeL(aGraphicNameSjis, graphicName);
    if (graphicName.Length() == 0)
        User::Leave(KErrCorrupt);

    TFileName candidate;
    candidate.Copy(aDirectory);
    candidate.Append(graphicName);
    const TInt baseLength = candidate.Length();

    candidate.Append(_L(".xyz"));
    if (aResources.Exists(candidate))
    {
        aPath.Copy(candidate);
        return;
    }

    candidate.SetLength(baseLength);
    candidate.Append(_L(".bmp"));
    if (aResources.Exists(candidate))
    {
        aPath.Copy(candidate);
        return;
    }

    candidate.SetLength(baseLength);
    candidate.Append(_L(".png"));
    if (aResources.Exists(candidate))
    {
        aPath.Copy(candidate);
        return;
    }

    User::Leave(KErrNotFound);
}
}

CPlayerRuntime* CPlayerRuntime::NewL(CZipVfs* aResources,
                                    const TDesC& aGameSource,
                                    CSymbianDisplaySurface& aDisplaySurface)
{
    CPlayerRuntime* self = new (ELeave) CPlayerRuntime(aResources, aDisplaySurface);
    CleanupStack::PushL(self);
    self->ConstructL(aGameSource);
    CleanupStack::Pop(self);
    return self;
}

CPlayerRuntime::CPlayerRuntime(CZipVfs* aResources,
                               CSymbianDisplaySurface& aDisplaySurface)
    : iResources(aResources),
      iDisplaySurface(aDisplaySurface),
      iPressedKeys(0),
      iBootFrame(0),
      iBootPhase(-1)
{
    Mem::FillZ(&iBootSequence, sizeof(iBootSequence));
}

void CPlayerRuntime::ConstructL(const TDesC&)
{
    if (!iResources)
        User::Leave(KErrNotFound);

    TLcfProjectSummary projectSummary;
    TBuf<96> gameTitle;
    User::LeaveIfError(TLcfProbe::ProbeProject(*iResources, gameTitle, projectSummary));

    if (EasyRpgModernCoreProbe(5) != 33 || EasyRpgUpstreamLmuLinkAnchor() != 0)
        User::Leave(KErrNotSupported);

    const TInt graphicsError = EasyRpgUpstreamGraphicsInit(iDisplaySurface.Size().iWidth,
                                                          iDisplaySurface.Size().iHeight);
    if (graphicsError != 0)
        User::Leave(KErrNotSupported);

    LoadMapL(static_cast<TInt>(projectSummary.iStartMapId),
             static_cast<TInt>(projectSummary.iStartX),
             static_cast<TInt>(projectSummary.iStartY));

    TFileName startMapName;
    startMapName.Format(_L("Map%04u.lmu"), projectSummary.iStartMapId);
    TFileName materializedMapPath;
    iResources->MaterializeFileL(startMapName,
                                 _L("active.lmu"),
                                 materializedMapPath,
                                 KMaxMapFileBytes);
    PrepareBootSequenceL(materializedMapPath);

    if (!projectSummary.iInitialActorTransparent &&
        projectSummary.iInitialActorCharacterNameSjis.Length() > 0)
    {
        TFileName characterPath;
        FindGraphicPathL(*iResources,
                         _L("CharSet\\"),
                         projectSummary.iInitialActorCharacterNameSjis,
                         characterPath);
        HBufC8* characterData = iResources->ReadFileLC(characterPath, KMaxGraphicFileBytes);
        const TInt playerError = EasyRpgUpstreamGraphicsLoadPlayer(
            reinterpret_cast<const unsigned char*>(characterData->Ptr()),
            static_cast<unsigned int>(characterData->Length()),
            static_cast<int>(projectSummary.iInitialActorCharacterIndex),
            projectSummary.iInitialActorTransparent ? 1 : 0,
            static_cast<int>(projectSummary.iStartX),
            static_cast<int>(projectSummary.iStartY));
        CleanupStack::PopAndDestroy(characterData);
        if (playerError != 0)
            User::Leave(KErrCorrupt);
    }

    if (EasyRpgUpstreamGraphicsRenderFrame() != 0)
        User::Leave(KErrGeneral);
    iDisplaySurface.PresentL();
}

void CPlayerRuntime::LoadMapL(TInt aMapId, TInt aX, TInt aY)
{
    if (aMapId <= 0)
        User::Leave(KErrArgument);

    TFileName mapName;
    mapName.Format(_L("Map%04d.lmu"), aMapId);
    TFileName materializedMapPath;
    iResources->MaterializeFileL(mapName,
                                 _L("active.lmu"),
                                 materializedMapPath,
                                 KMaxMapFileBytes);

    TBuf8<KMaxFileName + 1> narrowMapPath;
    NarrowAsciiPathL(materializedMapPath, narrowMapPath);

    EasyRpgUpstreamLmuSummary mapSummary;
    const TInt summaryError = EasyRpgUpstreamReadLmuSummary(
        reinterpret_cast<const char*>(narrowMapPath.Ptr()),
        "",
        &mapSummary);
    if (summaryError != 0 || mapSummary.chipset_id <= 0)
        User::Leave(KErrCorrupt);

    unsigned char chipsetNameBytes[96];
    unsigned int chipsetNameLength = 0;
    const TInt chipsetNameError = EasyRpgUpstreamGetChipsetName(
        mapSummary.chipset_id,
        chipsetNameBytes,
        sizeof(chipsetNameBytes),
        &chipsetNameLength);
    if (chipsetNameError != 0 || chipsetNameLength == 0 || chipsetNameLength > 96)
        User::Leave(KErrNotFound);

    TBuf8<96> chipsetNameSjis;
    chipsetNameSjis.Append(chipsetNameBytes, static_cast<TInt>(chipsetNameLength));
    TFileName chipsetPath;
    FindGraphicPathL(*iResources,
                     _L("ChipSet\\"),
                     chipsetNameSjis,
                     chipsetPath);
    HBufC8* chipsetData = iResources->ReadFileLC(chipsetPath, KMaxGraphicFileBytes);
    const TInt mapError = EasyRpgUpstreamGraphicsLoadMap(
        reinterpret_cast<const char*>(narrowMapPath.Ptr()),
        reinterpret_cast<const unsigned char*>(chipsetData->Ptr()),
        static_cast<unsigned int>(chipsetData->Length()),
        mapSummary.chipset_id,
        aX,
        aY);
    CleanupStack::PopAndDestroy(chipsetData);
    if (mapError != 0)
        User::Leave(KErrCorrupt);
}

void CPlayerRuntime::PrepareBootSequenceL(const TDesC& aMaterializedMapPath)
{
    TBuf8<KMaxFileName + 1> narrowMapPath;
    NarrowAsciiPathL(aMaterializedMapPath, narrowMapPath);
    Mem::FillZ(&iBootSequence, sizeof(iBootSequence));
    iBootPictureNameSjis.Zero();
    iBootFrame = 0;
    iBootPhase = -1;

    const TInt bootError = EasyRpgUpstreamReadBootSequence(
        reinterpret_cast<const char*>(narrowMapPath.Ptr()),
        "",
        &iBootSequence);
    if (bootError != 0 ||
        iBootSequence.picture_name_length == 0 ||
        iBootSequence.picture_name_length > static_cast<unsigned int>(iBootPictureNameSjis.MaxLength()))
        return;

    iBootPictureNameSjis.Append(iBootSequence.picture_name,
                                static_cast<TInt>(iBootSequence.picture_name_length));
    iBootPhase = 0;
}

void CPlayerRuntime::UpdateBootSequenceL()
{
    if (iBootPhase < 0)
        return;

    ++iBootFrame;
    if (iBootPhase == 0)
    {
        const TUint32 waitFrames = static_cast<TUint32>(iBootSequence.wait_before_picture * 6);
        if (iBootFrame < waitFrames)
            return;

        TFileName picturePath;
        FindGraphicPathL(*iResources,
                         _L("Picture\\"),
                         iBootPictureNameSjis,
                         picturePath);
        HBufC8* pictureData = iResources->ReadFileLC(picturePath, KMaxGraphicFileBytes);
        const TInt showError = EasyRpgUpstreamGraphicsShowPicture(
            reinterpret_cast<const unsigned char*>(pictureData->Ptr()),
            static_cast<unsigned int>(pictureData->Length()),
            iBootSequence.picture_x,
            iBootSequence.picture_y,
            iBootSequence.picture_magnify,
            iBootSequence.picture_opacity);
        CleanupStack::PopAndDestroy(pictureData);
        if (showError != 0)
            User::Leave(KErrCorrupt);

        const TInt moveError = EasyRpgUpstreamGraphicsMovePicture(
            iBootSequence.first_move_x,
            iBootSequence.first_move_y,
            iBootSequence.first_move_magnify,
            iBootSequence.first_move_opacity,
            iBootSequence.first_move_duration * 3);
        if (moveError != 0)
            User::Leave(KErrCorrupt);

        iBootFrame = 0;
        iBootPhase = 1;
        return;
    }

    if (iBootPhase == 1)
    {
        const TUint32 phaseFrames = static_cast<TUint32>(
            (iBootSequence.first_move_duration + iBootSequence.wait_after_first_move) * 6);
        if (iBootFrame < phaseFrames)
            return;

        const TInt moveError = EasyRpgUpstreamGraphicsMovePicture(
            iBootSequence.second_move_x,
            iBootSequence.second_move_y,
            iBootSequence.second_move_magnify,
            iBootSequence.second_move_opacity,
            iBootSequence.second_move_duration * 3);
        if (moveError != 0)
            User::Leave(KErrCorrupt);

        iBootFrame = 0;
        iBootPhase = 2;
        return;
    }

    const TUint32 phaseFrames = static_cast<TUint32>(
        (iBootSequence.second_move_duration + iBootSequence.wait_after_second_move) * 6);
    if (iBootFrame < phaseFrames)
        return;

    EasyRpgUpstreamGraphicsErasePicture();
    LoadMapL(iBootSequence.target_map_id,
             iBootSequence.target_x,
             iBootSequence.target_y);
    iBootFrame = 0;
    iBootPhase = -1;
}

CPlayerRuntime::~CPlayerRuntime()
{
    EasyRpgUpstreamGraphicsShutdown();
}

void CPlayerRuntime::SetKeyState(TUint32 aKey, TBool aPressed)
{
    if (aPressed)
        iPressedKeys |= aKey;
    else
        iPressedKeys &= ~aKey;
}

void CPlayerRuntime::ReleaseAllKeys()
{
    iPressedKeys = 0;
}

void CPlayerRuntime::TickL(TUint32 aLogicFrame, TBool aRenderFrame)
{
    TUint32 upstreamKeys = 0;
    if ((iPressedKeys & EKeyUp) != 0) upstreamKeys |= 1u << 13;
    if ((iPressedKeys & EKeyDown) != 0) upstreamKeys |= 1u << 15;
    if ((iPressedKeys & EKeyLeft) != 0) upstreamKeys |= 1u << 12;
    if ((iPressedKeys & EKeyRight) != 0) upstreamKeys |= 1u << 14;
    if ((iPressedKeys & EKeyDecision) != 0) upstreamKeys |= 1u << 4;
    if ((iPressedKeys & EKeyCancel) != 0) upstreamKeys |= 1u << 6;
    if ((iPressedKeys & EKeyShift) != 0) upstreamKeys |= 1u << 19;
    EasyRpgUpstreamGraphicsSetKeys(upstreamKeys, 0);
    EasyRpgUpstreamGraphicsSetTicks(static_cast<unsigned int>((aLogicFrame * 1000u) / 60u));
    UpdateBootSequenceL();
    if (aRenderFrame)
    {
        if (EasyRpgUpstreamGraphicsRenderFrame() != 0)
            User::Leave(KErrGeneral);
        iDisplaySurface.PresentL();
    }
}

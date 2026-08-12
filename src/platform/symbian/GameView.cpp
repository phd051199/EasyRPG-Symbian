#include <aknappui.h>
#include <aknlists.h>
#include <aknnotewrappers.h>
#include <coemain.h>
#include <e32keys.h>
#include <eikbtgpc.h>
#include <eikclbd.h>
#include <eikenv.h>
#include <eikscrlb.h>
#include <gdi.h>
#include <w32std.h>

#include "AudioBridge.h"
#include "DisplaySurface.h"
#include "GameLocator.h"
#include "GameView.h"
#include "MovieBridge.h"
#include "PerfOverlay.h"
#include "PlayerRuntime.h"
#include "AppUi.h"
#include "ZipVfs.h"

namespace
{
const TInt KGameWidth = 320;
const TInt KGameHeight = 240;
const TInt KMaxMetadataLdbSize = 4 * 1024 * 1024;
const TInt KDefaultMasterVolume = 50;
const TInt KVolumeStep = 10;
const TInt KVolumeOverlayTicks = 90;
_LIT(KSettingsDirectory, "C:\\Data\\EasyRPG\\");
_LIT(KSettingsPath, "C:\\Data\\EasyRPG\\settings.ini");
_LIT8(KMasterVolumeSetting, "master_volume=");
_LIT(KDoubleListItemFormat, "\t%S\t%S");
_LIT(KNoGamesFound, "No games found");
_LIT(KEmptyListSubtitle, "");
_LIT8(KLcfDatabaseHeader, "LcfDataBase");

enum TGameMenuItem
{
    EGameMenuSound = 0,
    EGameMenuVolume,
    EGameMenuFrameSkip,
    EGameMenuKeys,
    EGameMenuReset,
    EGameMenuFps,
    EGameMenuExit,
    EGameMenuItemCount
};

TBool RectsOverlap(const TRect& aFirst, const TRect& aSecond)
{
    return aFirst.iTl.iX < aSecond.iBr.iX &&
        aSecond.iTl.iX < aFirst.iBr.iX &&
        aFirst.iTl.iY < aSecond.iBr.iY &&
        aSecond.iTl.iY < aFirst.iBr.iY;
}

TInt ClampMasterVolume(TInt aVolume)
{
    if (aVolume < 0)
        return 0;
    if (aVolume > 100)
        return 100;
    return aVolume;
}

TInt LoadMasterVolume(RFs& aFs)
{
    RFile file;
    if (file.Open(aFs, KSettingsPath, EFileRead | EFileShareReadersOnly) != KErrNone)
        return KDefaultMasterVolume;

    TBuf8<128> data;
    const TInt readError = file.Read(data);
    file.Close();
    if (readError != KErrNone)
        return KDefaultMasterVolume;

    const TInt keyPosition = data.Find(KMasterVolumeSetting);
    if (keyPosition == KErrNotFound)
        return KDefaultMasterVolume;

    TInt position = keyPosition + KMasterVolumeSetting().Length();
    TInt volume = 0;
    TBool hasDigit = EFalse;
    while (position < data.Length())
    {
        const TUint8 value = data[position++];
        if (value < '0' || value > '9')
            break;
        hasDigit = ETrue;
        volume = volume * 10 + value - '0';
        if (volume > 100)
            return 100;
    }
    return hasDigit ? ClampMasterVolume(volume) : KDefaultMasterVolume;
}

TInt SaveMasterVolume(RFs& aFs, TInt aVolume)
{
    TInt error = aFs.MkDirAll(KSettingsDirectory);
    if (error != KErrNone && error != KErrAlreadyExists)
        return error;

    RFile file;
    error = file.Replace(aFs, KSettingsPath, EFileWrite | EFileShareExclusive);
    if (error != KErrNone)
        return error;

    TBuf8<32> data;
    data.Format(_L8("master_volume=%d\n"), ClampMasterVolume(aVolume));
    error = file.Write(data);
    file.Close();
    return error;
}

const TUint8 KSmallDigits[10][7] =
{
    {14, 17, 19, 21, 25, 17, 14},
    {4, 12, 4, 4, 4, 4, 14},
    {14, 17, 1, 2, 4, 8, 31},
    {30, 1, 1, 14, 1, 1, 30},
    {2, 6, 10, 18, 31, 2, 2},
    {31, 16, 16, 30, 1, 1, 30},
    {14, 16, 16, 30, 17, 17, 14},
    {31, 1, 2, 4, 8, 8, 8},
    {14, 17, 17, 14, 17, 17, 14},
    {14, 17, 17, 15, 1, 1, 14}
};

const TUint8 KSmallLetters[26][7] =
{
    {14, 17, 17, 31, 17, 17, 17},
    {30, 17, 17, 30, 17, 17, 30},
    {14, 17, 16, 16, 16, 17, 14},
    {30, 17, 17, 17, 17, 17, 30},
    {31, 16, 16, 30, 16, 16, 31},
    {31, 16, 16, 30, 16, 16, 16},
    {14, 17, 16, 23, 17, 17, 15},
    {17, 17, 17, 31, 17, 17, 17},
    {14, 4, 4, 4, 4, 4, 14},
    {7, 2, 2, 2, 18, 18, 12},
    {17, 18, 20, 24, 20, 18, 17},
    {16, 16, 16, 16, 16, 16, 31},
    {17, 27, 21, 21, 17, 17, 17},
    {17, 25, 21, 19, 17, 17, 17},
    {14, 17, 17, 17, 17, 17, 14},
    {30, 17, 17, 30, 16, 16, 16},
    {14, 17, 17, 17, 21, 18, 13},
    {30, 17, 17, 30, 20, 18, 17},
    {15, 16, 16, 14, 1, 1, 30},
    {31, 4, 4, 4, 4, 4, 4},
    {17, 17, 17, 17, 17, 17, 14},
    {17, 17, 17, 17, 17, 10, 4},
    {17, 17, 17, 21, 21, 21, 10},
    {17, 17, 10, 4, 10, 17, 17},
    {17, 17, 10, 4, 4, 4, 4},
    {31, 1, 2, 4, 8, 16, 31}
};

TUint8 SmallGlyphRow(TUint8 aCharacter, TInt aRow)
{
    if (aRow < 0 || aRow >= 7)
        return 0;
    if (aCharacter >= '0' && aCharacter <= '9')
        return KSmallDigits[aCharacter - '0'][aRow];
    if (aCharacter >= 'A' && aCharacter <= 'Z')
        return KSmallLetters[aCharacter - 'A'][aRow];

    static const TUint8 up[7] = {4, 14, 21, 4, 4, 4, 4};
    static const TUint8 down[7] = {4, 4, 4, 4, 21, 14, 4};
    static const TUint8 left[7] = {0, 4, 8, 31, 8, 4, 0};
    static const TUint8 right[7] = {0, 4, 2, 31, 2, 4, 0};
    static const TUint8 slash[7] = {1, 2, 2, 4, 8, 8, 16};
    static const TUint8 plus[7] = {0, 4, 4, 31, 4, 4, 0};
    static const TUint8 minus[7] = {0, 0, 0, 31, 0, 0, 0};
    static const TUint8 multiply[7] = {0, 17, 10, 4, 10, 17, 0};
    static const TUint8 period[7] = {0, 0, 0, 0, 0, 12, 12};
    static const TUint8 colon[7] = {0, 12, 12, 0, 12, 12, 0};
    static const TUint8 percent[7] = {17, 2, 4, 4, 8, 16, 17};

    switch (aCharacter)
    {
    case 1: return up[aRow];
    case 2: return down[aRow];
    case 3: return left[aRow];
    case 4: return right[aRow];
    case '/': return slash[aRow];
    case '+': return plus[aRow];
    case '-': return minus[aRow];
    case '*': return multiply[aRow];
    case '.': return period[aRow];
    case ':': return colon[aRow];
    case '%': return percent[aRow];
    default: return 0;
    }
}

void DrawSmallBitmapText(CWindowGc& aGc,
                         TInt aX,
                         TInt aY,
                         const TDesC8& aText)
{
    for (TInt index = 0; index < aText.Length(); ++index)
    {
        for (TInt row = 0; row < 7; ++row)
        {
            const TUint8 bits = SmallGlyphRow(aText[index], row);
            for (TInt column = 0; column < 5; ++column)
            {
                if ((bits & (1u << (4 - column))) != 0)
                    aGc.DrawRect(TRect(TPoint(aX + index * 6 + column, aY + row), TSize(1, 1)));
            }
        }
    }
}

void CopyListText(const TDesC& aSource, TDes& aTarget)
{
    aTarget.Copy(aSource.Left(aTarget.MaxLength()));
    for (TInt index = 0; index < aTarget.Length(); ++index)
    {
        if (aTarget[index] == '\t' || aTarget[index] == '\r' ||
            aTarget[index] == '\n')
        {
            aTarget[index] = ' ';
        }
    }
}

TBool ReadLcfInt(const TDesC8& aBytes,
                 TInt& aPosition,
                 TInt aEnd,
                 TInt& aValue)
{
    aValue = 0;
    TInt count = 0;
    while (aPosition < aEnd && count < 5)
    {
        const TUint8 value = aBytes[aPosition++];
        if (aValue > (KMaxTInt >> 7))
            return EFalse;
        aValue = (aValue << 7) | (value & 0x7f);
        ++count;
        if ((value & 0x80) == 0)
            return ETrue;
    }
    return EFalse;
}

TInt DetectSystemEngine(const TDesC8& aBytes, TInt aStart, TInt aEnd)
{
    TInt position = aStart;
    while (position < aEnd)
    {
        TInt fieldId = 0;
        if (!ReadLcfInt(aBytes, position, aEnd, fieldId))
            return 0;
        if (fieldId == 0)
            return 2000;

        TInt fieldLength = 0;
        if (!ReadLcfInt(aBytes, position, aEnd, fieldLength) ||
            fieldLength < 0 || position > aEnd - fieldLength)
            return 0;

        const TInt fieldEnd = position + fieldLength;
        if (fieldId == 0x0a)
        {
            TInt ldbId = 0;
            if (!ReadLcfInt(aBytes, position, fieldEnd, ldbId))
                return 0;
            return ldbId == 2003 ? 2003 : 2000;
        }
        position = fieldEnd;
    }
    return 2000;
}

TInt DetectRpgMakerVersionL(CZipVfs& aResources)
{
    HBufC8* data = aResources.ReadFileLC(_L("RPG_RT.ldb"),
                                          KMaxMetadataLdbSize);
    const TPtrC8 bytes(*data);

    TInt position = 0;
    TInt headerLength = 0;
    TInt version = 0;
    if (ReadLcfInt(bytes, position, bytes.Length(), headerLength) &&
        headerLength == KLcfDatabaseHeader().Length() &&
        position <= bytes.Length() - headerLength &&
        bytes.Mid(position, headerLength).Compare(KLcfDatabaseHeader) == 0)
    {
        position += headerLength;
        while (position < bytes.Length())
        {
            TInt fieldId = 0;
            if (!ReadLcfInt(bytes, position, bytes.Length(), fieldId))
                break;
            if (fieldId == 0)
                break;

            TInt fieldLength = 0;
            if (!ReadLcfInt(bytes, position, bytes.Length(), fieldLength) ||
                fieldLength < 0 || position > bytes.Length() - fieldLength)
                break;

            const TInt fieldEnd = position + fieldLength;
            if (fieldId == 0x16)
            {
                version = DetectSystemEngine(bytes, position, fieldEnd);
                break;
            }
            position = fieldEnd;
        }
    }

    CleanupStack::PopAndDestroy(data);
    return version;
}

void ReadGameTitleL(CZipVfs& aResources, TDes& aTitle)
{
    HBufC8* data = aResources.ReadFileLC(_L("RPG_RT.ini"), 4096);
    const TPtrC8 bytes(*data);
    _LIT8(KGameTitleKey, "GameTitle=");
    const TInt keyPosition = bytes.Find(KGameTitleKey);
    if (keyPosition != KErrNotFound)
    {
        TBuf<KMaxFileName> title;
        TInt position = keyPosition + KGameTitleKey().Length();
        while (position < bytes.Length() && title.Length() < title.MaxLength())
        {
            const TUint8 value = bytes[position++];
            if (value == '\r' || value == '\n')
                break;
            if (value < 0x20 || value > 0x7e)
            {
                title.Zero();
                break;
            }
            title.Append(static_cast<TUint16>(value));
        }
        if (title.Length() > 0)
            CopyListText(title, aTitle);
    }
    CleanupStack::PopAndDestroy(data);
}

void ReadGameMetadataL(RFs& aFs,
                       const TDesC& aArchivePath,
                       TDes& aTitle,
                       TInt& aVersion)
{
    CZipVfs* resources = CZipVfs::OpenL(aFs,
                                        aArchivePath);
    CleanupStack::PushL(resources);
    TRAP_IGNORE(ReadGameTitleL(*resources, aTitle));
    TRAP_IGNORE(aVersion = DetectRpgMakerVersionL(*resources));
    CleanupStack::PopAndDestroy(resources);
}

void FormatGameSubtitle(RFs& aFs,
                        const TDesC& aArchivePath,
                        TInt aVersion,
                        TDes& aSubtitle)
{
    TEntry entry;
    TInt64 bytes = 0;
    if (aFs.Entry(aArchivePath, entry) == KErrNone && !entry.IsDir())
        bytes = entry.iSize;

    const TInt64 tenths = (bytes * 10 + 524288) / 1048576;
    const TInt whole = static_cast<TInt>(tenths / 10);
    const TInt fraction = static_cast<TInt>(tenths % 10);
    if (aVersion == 2000 || aVersion == 2003)
        aSubtitle.Format(_L("%d.%d MB (RPG Maker %d)"),
                         whole,
                         fraction,
                         aVersion);
    else
        aSubtitle.Format(_L("%d.%d MB (RPG Maker 2000/2003)"),
                         whole,
                         fraction);
}

void SetPlayerKeyMask(CPlayerRuntime& aRuntime, TUint32 aMask, TBool aPressed)
{
    const TUint32 keys[] =
    {
        CPlayerRuntime::EKeyUp,
        CPlayerRuntime::EKeyDown,
        CPlayerRuntime::EKeyLeft,
        CPlayerRuntime::EKeyRight,
        CPlayerRuntime::EKeyDecision,
        CPlayerRuntime::EKeyCancel,
        CPlayerRuntime::EKeyShift,
        CPlayerRuntime::EKeyN0,
        CPlayerRuntime::EKeyN1,
        CPlayerRuntime::EKeyN2,
        CPlayerRuntime::EKeyN3,
        CPlayerRuntime::EKeyN4,
        CPlayerRuntime::EKeyN5,
        CPlayerRuntime::EKeyN6,
        CPlayerRuntime::EKeyN7,
        CPlayerRuntime::EKeyN8,
        CPlayerRuntime::EKeyN9,
        CPlayerRuntime::EKeyPlus,
        CPlayerRuntime::EKeyMinus,
        CPlayerRuntime::EKeyMultiply,
        CPlayerRuntime::EKeyDivide,
        CPlayerRuntime::EKeyPeriod,
        CPlayerRuntime::EKeyDebug,
        CPlayerRuntime::EKeyScreenshot
    };
    for (TUint index = 0; index < sizeof(keys) / sizeof(keys[0]); ++index)
    {
        if ((aMask & keys[index]) != 0)
            aRuntime.SetKeyState(keys[index], aPressed);
    }
}
}

CGameView* CGameView::NewL(const TRect& aRect)
{
    CGameView* self = new (ELeave) CGameView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop(self);
    return self;
}

CGameView::CGameView()
    : iListBox(NULL),
      iListItems(NULL),
      iArchivePaths(NULL),
      iDisplaySurface(NULL),
      iPerfOverlay(NULL),
      iResources(NULL),
      iRuntime(NULL),
      iFrameTimer(NULL),
      iStartGameCallback(NULL),
      iForeground(ETrue),
      iKeyMapVisible(EFalse),
      iGameMenuActive(EFalse),
      iGameMenuSelection(EGameMenuSound),
      iLeftSoftkeyHeld(EFalse),
      iRightSoftkeyHeld(EFalse),
      iPendingCancelPulse(EFalse),
      iHeldPlayerKeys(0),
      iPendingPlayerKeyPulses(0),
      iDynamicPlayerScanCount(0),
      iVolumeUpHeld(EFalse),
      iVolumeDownHeld(EFalse),
      iFrameDrawPending(EFalse),
      iVolumeOverlayErasePending(EFalse),
      iMasterVolume(KDefaultMasterVolume),
      iAudibleVolume(KDefaultMasterVolume),
      iVolumeOverlayTicks(0)
{
}

void CGameView::ConstructL(const TRect& aRect)
{
    CreateWindowL();
    EasyRpgMovieInitializeL(CCoeEnv::Static()->WsSession(),
                            *CCoeEnv::Static()->ScreenDevice(),
                            Window());
    User::LeaveIfError(iFs.Connect());
    iMasterVolume = LoadMasterVolume(iFs);
    if (iMasterVolume > 0)
        iAudibleVolume = iMasterVolume;
    EasyRpgAudioSetMasterVolume(iMasterVolume);
    iFrameTimer = CFrameTimer::NewL(*this);
    iStartGameCallback = new (ELeave) CAsyncCallBack(
        TCallBack(&CGameView::StartGameCallback, this),
        CActive::EPriorityHigh);

    iListBox = new (ELeave) CAknDoubleStyleListBox;
    iListBox->SetContainerWindowL(*this);
    iListBox->ConstructL(this, EAknListBoxSelectionList);
    iListBox->SetListBoxObserver(this);
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

    iListItems = new (ELeave) CDesCArrayFlat(16);
    iArchivePaths = new (ELeave) CDesCArrayFlat(16);
    iListBox->Model()->SetItemTextArray(iListItems);
    iListBox->Model()->SetOwnershipType(ELbmDoesNotOwnItemArray);

    BuildGameListL();

    SetRect(aRect);
    SetFocus(ETrue);
    ActivateL();
    iListBox->ActivateL();
    iListBox->SetFocus(ETrue);
}

CGameView::~CGameView()
{
    EasyRpgMovieShutdown();
    if (iStartGameCallback)
        iStartGameCallback->Cancel();
    delete iStartGameCallback;
    iStartGameCallback = NULL;

    if (iFrameTimer)
        iFrameTimer->Stop();

    delete iFrameTimer;
    iFrameTimer = NULL;

    delete iRuntime;
    iRuntime = NULL;

    delete iResources;
    iResources = NULL;

    delete iDisplaySurface;
    iDisplaySurface = NULL;

    delete iPerfOverlay;
    iPerfOverlay = NULL;

    delete iListBox;
    iListBox = NULL;

    delete iListItems;
    iListItems = NULL;

    delete iArchivePaths;
    iArchivePaths = NULL;

    iFs.Close();
}

void CGameView::BuildGameListL()
{
    iListItems->Reset();
    iArchivePaths->Reset();

    CDesCArrayFlat* displayNames = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(displayNames);
    TGameLocator::ListGameArchivesL(iFs, *displayNames, *iArchivePaths);

    for (TInt index = 0; index < displayNames->Count(); ++index)
    {
        TBuf<KMaxFileName> title;
        CopyListText((*displayNames)[index], title);

        TInt version = 0;
        TRAP_IGNORE(ReadGameMetadataL(iFs,
                                      (*iArchivePaths)[index],
                                      title,
                                      version));

        TBuf<64> subtitle;
        FormatGameSubtitle(iFs,
                           (*iArchivePaths)[index],
                           version,
                           subtitle);

        TBuf<KMaxFileName + 80> item;
        item.Format(KDoubleListItemFormat, &title, &subtitle);
        iListItems->AppendL(item);
    }

    if (iListItems->Count() == 0)
    {
        TBuf<64> item;
        item.Format(KDoubleListItemFormat, &KNoGamesFound, &KEmptyListSubtitle);
        iListItems->AppendL(item);
    }

    CleanupStack::PopAndDestroy(displayNames);

    iListBox->HandleItemAdditionL();
    iListBox->SetCurrentItemIndex(0);
}

void CGameView::StartSelectedGameL()
{
    if (iRuntime || !iListBox || !iArchivePaths)
        return;

    const TInt index = iListBox->CurrentItemIndex();
    if (index < 0 || index >= iArchivePaths->Count())
        return;

    TFileName selectedArchive;
    selectedArchive.Copy((*iArchivePaths)[index]);

    CDisplaySurface* displaySurface =
        CDisplaySurface::NewL(TSize(KGameWidth, KGameHeight));
    CleanupStack::PushL(displaySurface);

    CZipVfs* resources = CZipVfs::OpenL(iFs,
                                        selectedArchive);
    CleanupStack::PushL(resources);

    CPlayerRuntime* runtime = CPlayerRuntime::NewL(resources,
                                                   selectedArchive,
                                                   *displaySurface);
    CleanupStack::PushL(runtime);

    if (iListBox->ScrollBarFrame())
    {
        iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
            CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOff);
    }

    iListBox->SetFocus(EFalse);
    iListBox->MakeVisible(EFalse);
    SetFocus(ETrue);

    CEasyRpgAppUi* appUi = static_cast<CEasyRpgAppUi*>(CEikonEnv::Static()->EikAppUi());
    appUi->EnterGameFullscreen();
    const TSize screenSize = CCoeEnv::Static()->ScreenDevice()->SizeInPixels();
    SetRect(TRect(TPoint(0, 0), screenSize));

    CleanupStack::Pop(runtime);
    CleanupStack::Pop(resources);
    CleanupStack::Pop(displaySurface);

    iDisplaySurface = displaySurface;
    iResources = resources;
    iRuntime = runtime;
    iGameRoot.Copy(selectedArchive);
    iKeyMapVisible = EFalse;
    iGameMenuActive = EFalse;
    iGameMenuSelection = EGameMenuSound;
    iLeftSoftkeyHeld = EFalse;
    iRightSoftkeyHeld = EFalse;
    iPendingCancelPulse = EFalse;
    iHeldPlayerKeys = 0;
    iPendingPlayerKeyPulses = 0;
    iDynamicPlayerScanCount = 0;
    iVolumeUpHeld = EFalse;
    iVolumeDownHeld = EFalse;
    iVolumeOverlayTicks = 0;
    iVolumeOverlayErasePending = EFalse;
    iFrameDrawPending = EFalse;
    if (iPerfOverlay)
        iPerfOverlay->Reset();
    DrawNow();

    if (iForeground)
        iFrameTimer->Start();
}

void CGameView::StopGame()
{
    EasyRpgMovieStop();
    EasyRpgMovieAcknowledge();
    iKeyMapVisible = EFalse;
    iGameMenuActive = EFalse;
    iGameMenuSelection = EGameMenuSound;
    iLeftSoftkeyHeld = EFalse;
    iRightSoftkeyHeld = EFalse;
    iPendingCancelPulse = EFalse;
    iHeldPlayerKeys = 0;
    iPendingPlayerKeyPulses = 0;
    iDynamicPlayerScanCount = 0;
    iVolumeUpHeld = EFalse;
    iVolumeDownHeld = EFalse;
    iVolumeOverlayTicks = 0;
    iVolumeOverlayErasePending = EFalse;
    iFrameDrawPending = EFalse;
    if (iFrameTimer)
        iFrameTimer->Stop();
    delete iRuntime;
    iRuntime = NULL;
    delete iResources;
    iResources = NULL;
    delete iDisplaySurface;
    iDisplaySurface = NULL;
    iGameRoot.Zero();

    CEasyRpgAppUi* appUi = static_cast<CEasyRpgAppUi*>(CEikonEnv::Static()->EikAppUi());
    appUi->ExitGameFullscreen();
    SetRect(appUi->ClientRect());

    if (iListBox)
    {
        if (iListBox->ScrollBarFrame())
        {
            TRAP_IGNORE(iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
                CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto));
        }
        iListBox->SetRect(Rect());
        iListBox->MakeVisible(ETrue);
        iListBox->SetFocus(ETrue);
        SetFocus(ETrue);
        iListBox->DrawNow();
    }
}

void CGameView::ShowLoadErrorL(TInt aError)
{
    TBuf<64> message;
    message.Format(_L("Cannot load game (%d)"), aError);
    CAknErrorNote* note = new (ELeave) CAknErrorNote;
    note->ExecuteLD(message);
}

void CGameView::SetForeground(TBool aForeground)
{
    iForeground = aForeground;
    EasyRpgMovieSetForeground(aForeground);

    if (!iRuntime || !iFrameTimer)
    {
        if (aForeground && iListBox)
        {
            SetFocus(ETrue);
            iListBox->SetFocus(ETrue);
            iListBox->DrawNow();
        }
        return;
    }

    if (!aForeground)
    {
        iPendingCancelPulse = EFalse;
        iHeldPlayerKeys = 0;
        iPendingPlayerKeyPulses = 0;
        iDynamicPlayerScanCount = 0;
        iVolumeUpHeld = EFalse;
        iVolumeDownHeld = EFalse;
        iLeftSoftkeyHeld = EFalse;
        iRightSoftkeyHeld = EFalse;
        iFrameDrawPending = EFalse;
        iRuntime->Suspend();
        iFrameTimer->Stop();
    }
    else
    {
        iFrameDrawPending = EFalse;
        if (!iKeyMapVisible && !iGameMenuActive)
        {
            iRuntime->Resume();
            if (iPerfOverlay)
                iPerfOverlay->Reset();
            iFrameTimer->Start();
        }
        DrawNow();
    }
}

void CGameView::UpdateLayout()
{
    CAknAppUi* appUi = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi());
    if (!iRuntime)
    {
        SetRect(appUi->ClientRect());
        return;
    }

    const TSize screenSize = CCoeEnv::Static()->ScreenDevice()->SizeInPixels();
    SetRect(TRect(TPoint(0, 0), screenSize));
}

TBool CGameView::IsGameRunning() const
{
    return iRuntime != NULL;
}

void CGameView::HandleLeftSoftkeyL()
{
    if (!iRuntime)
        return;
    if (!iGameMenuActive)
    {
        OpenGameMenu();
        return;
    }
    if (!iKeyMapVisible)
        ActivateGameMenuItemL();
}

void CGameView::HandleRightSoftkeyL()
{
    if (!iRuntime)
        return;
    if (iGameMenuActive)
    {
        if (iKeyMapVisible)
        {
            iKeyMapVisible = EFalse;
            iFrameDrawPending = EFalse;
            DrawNow();
        }
        else
        {
            CloseGameMenu();
        }
        return;
    }
    iPendingCancelPulse = ETrue;
}

void CGameView::PlaySelectedGameL()
{
    StartSelectedGameL();
}

void CGameView::OpenGameMenu()
{
    if (!iRuntime || iGameMenuActive)
        return;

    iRuntime->ReleaseAllKeys();
    iPendingCancelPulse = EFalse;
    iHeldPlayerKeys = 0;
    iPendingPlayerKeyPulses = 0;
    iDynamicPlayerScanCount = 0;
    iFrameDrawPending = EFalse;
    iKeyMapVisible = EFalse;
    iGameMenuActive = ETrue;
    iRuntime->Suspend();
    if (iFrameTimer)
        iFrameTimer->Stop();
    DrawNow();
}

void CGameView::CloseGameMenu()
{
    if (!iRuntime || !iGameMenuActive)
        return;

    iKeyMapVisible = EFalse;
    iGameMenuActive = EFalse;
    iPendingCancelPulse = EFalse;
    iHeldPlayerKeys = 0;
    iPendingPlayerKeyPulses = 0;
    iDynamicPlayerScanCount = 0;
    iFrameDrawPending = EFalse;
    if (iForeground)
    {
        iRuntime->Resume();
        if (iPerfOverlay)
            iPerfOverlay->Reset();
        if (iFrameTimer)
            iFrameTimer->Start();
    }
    DrawNow();
}

void CGameView::MoveGameMenuSelection(TInt aDelta)
{
    if (!iGameMenuActive || iKeyMapVisible)
        return;
    iGameMenuSelection += aDelta;
    while (iGameMenuSelection < 0)
        iGameMenuSelection += EGameMenuItemCount;
    while (iGameMenuSelection >= EGameMenuItemCount)
        iGameMenuSelection -= EGameMenuItemCount;
    iFrameDrawPending = EFalse;
    DrawNow();
}

void CGameView::ActivateGameMenuItemL()
{
    if (!iGameMenuActive || iKeyMapVisible)
        return;

    switch (iGameMenuSelection)
    {
    case EGameMenuSound:
        ToggleSound();
        break;
    case EGameMenuVolume:
        AdjustMasterVolume(KVolumeStep);
        break;
    case EGameMenuFrameSkip:
        ToggleFrameSkip();
        DrawNow();
        break;
    case EGameMenuKeys:
        iKeyMapVisible = ETrue;
        iFrameDrawPending = EFalse;
        DrawNow();
        break;
    case EGameMenuReset:
        RestartGameL();
        break;
    case EGameMenuFps:
        ToggleFpsOverlayL();
        break;
    case EGameMenuExit:
        StopGame();
        break;
    default:
        break;
    }
}

void CGameView::ToggleSound()
{
    if (iMasterVolume > 0)
    {
        iAudibleVolume = iMasterVolume;
        iMasterVolume = 0;
    }
    else
    {
        if (iAudibleVolume <= 0)
            iAudibleVolume = KDefaultMasterVolume;
        iMasterVolume = iAudibleVolume;
        SaveMasterVolume(iFs, iMasterVolume);
    }
    EasyRpgAudioSetMasterVolume(iMasterVolume);
    iVolumeOverlayTicks = KVolumeOverlayTicks;
    iVolumeOverlayErasePending = EFalse;
    iFrameDrawPending = EFalse;
    DrawNow();
}

void CGameView::ToggleFrameSkip()
{
    if (iFrameTimer)
        iFrameTimer->SetFrameSkipEnabled(!iFrameTimer->FrameSkipEnabled());
}

void CGameView::RestartGameL()
{
    if (!iRuntime)
        return;
    StopGame();
    StartSelectedGameL();
}

void CGameView::ToggleFpsOverlayL()
{
    if (iPerfOverlay)
    {
        delete iPerfOverlay;
        iPerfOverlay = NULL;
    }
    else
    {
        iPerfOverlay = CPerfOverlay::NewL();
    }
    iFrameDrawPending = EFalse;
    DrawNow();
}

void CGameView::AdjustMasterVolume(TInt aDelta)
{
    const TInt volume = ClampMasterVolume(iMasterVolume + aDelta);
    if (volume != iMasterVolume)
    {
        iMasterVolume = volume;
        if (iMasterVolume > 0)
            iAudibleVolume = iMasterVolume;
        EasyRpgAudioSetMasterVolume(iMasterVolume);
        SaveMasterVolume(iFs, iMasterVolume);
    }
    iVolumeOverlayTicks = KVolumeOverlayTicks;
    iVolumeOverlayErasePending = EFalse;
    iFrameDrawPending = EFalse;
    DrawNow();
}

void CGameView::DrawVolumeOverlay(CWindowGc& aGc, const TPoint& aDestination) const
{
    if (iVolumeOverlayTicks <= 0 || iGameMenuActive)
        return;

    TBuf8<16> text;
    text.Format(_L8("VOLUME %d%%"), iMasterVolume);
    const TInt width = text.Length() * 6 - 1;
    const TInt x = aDestination.iX + (KGameWidth - width) / 2;
    const TInt y = aDestination.iY + KGameHeight - 12;

    aGc.SetPenStyle(CGraphicsContext::ENullPen);
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(TRgb(0, 0, 0));
    DrawSmallBitmapText(aGc, x + 1, y + 1, text);
    aGc.SetBrushColor(TRgb(255, 255, 255));
    DrawSmallBitmapText(aGc, x, y, text);
}

void CGameView::DrawGameMenuOverlay(CWindowGc& aGc) const
{
    if (!iGameMenuActive || iKeyMapVisible)
        return;

    const TRect view = Rect();
    const TInt panelWidth = Min(156, Max(1, view.Width() - 12));
    const TInt panelHeight = Min(116, Max(1, view.Height() - 12));
    const TPoint panelOrigin(view.iTl.iX + 6,
                             view.iTl.iY + (view.Height() - panelHeight) / 2);
    const TRect panel(panelOrigin, TSize(panelWidth, panelHeight));

    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenColor(TRgb(255, 255, 255));
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(TRgb(0, 0, 0));
    aGc.DrawRect(panel);

    aGc.SetPenStyle(CGraphicsContext::ENullPen);
    aGc.SetBrushColor(TRgb(255, 255, 255));
    DrawSmallBitmapText(aGc, panel.iTl.iX + 7, panel.iTl.iY + 6, _L8("MENU"));

    const TInt rowX = panel.iTl.iX + 5;
    const TInt rowWidth = panelWidth - 10;
    const TInt firstY = panel.iTl.iY + 19;
    const TInt rowStep = 11;
    for (TInt item = 0; item < EGameMenuItemCount; ++item)
    {
        TBuf8<28> text;
        switch (item)
        {
        case EGameMenuSound:
            text.Copy(iMasterVolume > 0 ? _L8("SOUND ON") : _L8("SOUND OFF"));
            break;
        case EGameMenuVolume:
            text.Format(_L8("VOLUME %d%%"), iMasterVolume);
            break;
        case EGameMenuFrameSkip:
            text.Copy(iFrameTimer && iFrameTimer->FrameSkipEnabled()
                ? _L8("FRAMESKIP AUTO")
                : _L8("FRAMESKIP OFF"));
            break;
        case EGameMenuKeys:
            text.Copy(_L8("KEYS"));
            break;
        case EGameMenuReset:
            text.Copy(_L8("RESET"));
            break;
        case EGameMenuFps:
            text.Copy(iPerfOverlay ? _L8("FPS ON") : _L8("FPS OFF"));
            break;
        case EGameMenuExit:
            text.Copy(_L8("EXIT"));
            break;
        default:
            text.Zero();
            break;
        }

        const TInt y = firstY + item * rowStep;
        if (item == iGameMenuSelection)
        {
            aGc.SetBrushColor(TRgb(255, 255, 255));
            aGc.DrawRect(TRect(TPoint(rowX, y - 2), TSize(rowWidth, 10)));
            aGc.SetBrushColor(TRgb(0, 0, 0));
            DrawSmallBitmapText(aGc, rowX + 4, y, text);
            aGc.SetBrushColor(TRgb(255, 255, 255));
        }
        else
        {
            DrawSmallBitmapText(aGc, rowX + 4, y, text);
        }
    }

    DrawSmallBitmapText(aGc,
                        panel.iTl.iX + 7,
                        panel.iBr.iY - 10,
                        _L8("SELECT"));
    DrawSmallBitmapText(aGc,
                        panel.iBr.iX - 31,
                        panel.iBr.iY - 10,
                        _L8("BACK"));
}

void CGameView::DrawKeyMapOverlay(CWindowGc& aGc) const
{
    const TRect view = Rect();
    const TInt panelWidth = Min(304, Max(1, view.Width() - 16));
    const TInt panelHeight = Min(87, Max(1, view.Height() - 16));
    const TPoint panelOrigin(view.iTl.iX + (view.Width() - panelWidth) / 2,
                             view.iTl.iY + (view.Height() - panelHeight) / 2);
    const TRect panel(panelOrigin, TSize(panelWidth, panelHeight));

    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenColor(TRgb(255, 255, 255));
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(TRgb(0, 0, 0));
    aGc.DrawRect(panel);

    aGc.SetPenStyle(CGraphicsContext::ENullPen);
    aGc.SetBrushColor(TRgb(255, 255, 255));

    const TInt leftX = panel.iTl.iX + 8;
    const TInt rightX = panel.iTl.iX + panelWidth / 2 + 4;
    TInt leftY = panel.iTl.iY + 7;
    TInt rightY = panel.iTl.iY + 16;
    const TInt step = 9;
    DrawSmallBitmapText(aGc, leftX, leftY, _L8("KEY  \x04  ACTION"));
    leftY += step;
    DrawSmallBitmapText(aGc, leftX, leftY, _L8("\x01 / W  \x04  UP"));
    leftY += step;
    DrawSmallBitmapText(aGc, leftX, leftY, _L8("\x02 / S  \x04  DOWN"));
    leftY += step;
    DrawSmallBitmapText(aGc, leftX, leftY, _L8("\x03 / A  \x04  LEFT"));
    leftY += step;
    DrawSmallBitmapText(aGc, leftX, leftY, _L8("\x04 / D  \x04  RIGHT"));
    leftY += step;
    DrawSmallBitmapText(aGc, leftX, leftY, _L8("CENTER/Z  \x04  ATTACK/OK"));
    leftY += step;
    DrawSmallBitmapText(aGc, leftX, leftY, _L8("ENTER/SPACE  \x04  ATTACK/OK"));

    DrawSmallBitmapText(aGc, rightX, rightY, _L8("R SOFT  \x04  BACK"));
    rightY += step;
    DrawSmallBitmapText(aGc, rightX, rightY, _L8("SHIFT  \x04  SHIFT"));
    rightY += step;
    DrawSmallBitmapText(aGc, rightX, rightY, _L8("0-9  \x04  NUMBER"));
    rightY += step;
    DrawSmallBitmapText(aGc, rightX, rightY, _L8("+ - * / .  \x04  OPERATOR"));
    rightY += step;
    DrawSmallBitmapText(aGc, rightX, rightY, _L8("CENTER  \x04  BACK"));
    rightY += step;
    if (iPerfOverlay)
        DrawSmallBitmapText(aGc, rightX, rightY, _L8("F  \x04  FPS OFF"));
    else
        DrawSmallBitmapText(aGc, rightX, rightY, _L8("F  \x04  FPS ON"));
    rightY += step;
    DrawSmallBitmapText(aGc, rightX, rightY, _L8("P  \x04  SCREENSHOT"));
    rightY += step;
    DrawSmallBitmapText(aGc, rightX, rightY, _L8("SIDE +/-  \x04  VOLUME"));
}

void CGameView::Draw(const TRect& aRect) const
{
    if (!iRuntime || !iDisplaySurface)
        return;

    const TBool frameDraw = iFrameDrawPending;
    iFrameDrawPending = EFalse;
    const TSize bitmapSize = iDisplaySurface->Size();
    const TSize viewSize = Rect().Size();
    if (!iDisplaySurface->HasFrame() || (!frameDraw && viewSize != bitmapSize))
    {
        CWindowGc& gc = SystemGc();
        gc.SetPenStyle(CGraphicsContext::ENullPen);
        gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
        gc.SetBrushColor(TRgb(0, 0, 0));
        gc.DrawRect(Rect());
    }

    if (!iDisplaySurface->HasFrame())
    {
        CWindowGc& gc = SystemGc();
        if (iKeyMapVisible)
            DrawKeyMapOverlay(gc);
        else if (iGameMenuActive)
            DrawGameMenuOverlay(gc);
        DrawVolumeOverlay(gc, TPoint(0, 0));
        return;
    }

    TPoint destination(0, 0);
    if (viewSize.iWidth > bitmapSize.iWidth)
        destination.iX = (viewSize.iWidth - bitmapSize.iWidth) / 2;
    if (viewSize.iHeight > bitmapSize.iHeight)
        destination.iY = (viewSize.iHeight - bitmapSize.iHeight) / 2;

    CWindowGc& gc = SystemGc();
    TBool overlayNeedsDraw = iPerfOverlay && iPerfOverlay->IsDirty();
    TRect overlayRect;
    TPoint overlayPosition;
    if (iPerfOverlay)
    {
        const TSize overlaySize = iPerfOverlay->Bitmap().SizeInPixels();
        overlayPosition = TPoint(2, KGameHeight - overlaySize.iHeight - 2);
        overlayRect = TRect(overlayPosition, overlaySize);
    }

    if (frameDraw)
    {
        const TInt dirtyCount = iDisplaySurface->DirtyRegionCount();
        for (TInt index = 0; index < dirtyCount; ++index)
        {
            const TRect& dirty = iDisplaySurface->DirtyRegion(index);
            gc.BitBlt(destination + dirty.iTl,
                      &iDisplaySurface->Bitmap(),
                      dirty);
            if (iPerfOverlay && RectsOverlap(dirty, overlayRect))
                overlayNeedsDraw = ETrue;
        }
        iDisplaySurface->ClearDirtyRegions();
    }
    else
    {
        const TInt left = Max(0, aRect.iTl.iX - destination.iX);
        const TInt top = Max(0, aRect.iTl.iY - destination.iY);
        const TInt right = Min(bitmapSize.iWidth, aRect.iBr.iX - destination.iX);
        const TInt bottom = Min(bitmapSize.iHeight, aRect.iBr.iY - destination.iY);
        if (right > left && bottom > top)
        {
            const TRect source(left, top, right, bottom);
            gc.BitBlt(destination + source.iTl,
                      &iDisplaySurface->Bitmap(),
                      source);
            if (iPerfOverlay && RectsOverlap(source, overlayRect))
                overlayNeedsDraw = ETrue;
        }
        iDisplaySurface->ClearDirtyRegions();
    }

    if (iVolumeOverlayErasePending)
    {
        const TInt overlayWidth = 65;
        const TRect source(TPoint((KGameWidth - overlayWidth) / 2,
                                  KGameHeight - 12),
                           TSize(overlayWidth + 1, 9));
        gc.BitBlt(destination + source.iTl,
                  &iDisplaySurface->Bitmap(),
                  source);
        iVolumeOverlayErasePending = EFalse;
    }

    if (iPerfOverlay && overlayNeedsDraw)
    {
        const TSize overlaySize = iPerfOverlay->Bitmap().SizeInPixels();
        if (iPerfOverlay->IsDirty())
        {
            gc.BitBlt(destination + overlayPosition,
                      &iDisplaySurface->Bitmap(),
                      overlayRect);
        }
        gc.BitBltMasked(destination + overlayPosition,
                        &iPerfOverlay->Bitmap(),
                        TRect(TPoint(0, 0), overlaySize),
                        &iPerfOverlay->MaskBitmap(),
                        EFalse);
        iPerfOverlay->ClearDirty();
    }

    if (iKeyMapVisible)
        DrawKeyMapOverlay(gc);
    else if (iGameMenuActive)
        DrawGameMenuOverlay(gc);
    DrawVolumeOverlay(gc, destination);
}

void CGameView::SizeChanged()
{
    if (iListBox && !iRuntime)
        iListBox->SetRect(Rect());
}

void CGameView::HandleResourceChange(TInt aType)
{
    CCoeControl::HandleResourceChange(aType);
    if (aType == KEikDynamicLayoutVariantSwitch)
    {
        UpdateLayout();
        if (iRuntime)
        {
            iFrameDrawPending = EFalse;
            DrawNow();
        }
    }
}

void CGameView::FocusChanged(TDrawNow aDrawNow)
{
    CCoeControl::FocusChanged(aDrawNow);

    if (iRuntime)
    {
        if (!IsFocused())
        {
            iVolumeUpHeld = EFalse;
            iVolumeDownHeld = EFalse;
            iLeftSoftkeyHeld = EFalse;
            iRightSoftkeyHeld = EFalse;
            iRuntime->ReleaseAllKeys();
        }
    }
    else if (iListBox)
    {
        iListBox->SetFocus(IsFocused());
    }
}

TInt CGameView::CountComponentControls() const
{
    return iListBox != NULL && !iRuntime ? 1 : 0;
}

CCoeControl* CGameView::ComponentControl(TInt aIndex) const
{
    return aIndex == 0 && !iRuntime ? iListBox : NULL;
}

TKeyResponse CGameView::HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
    return OfferKeyEventL(aKeyEvent, aType);
}

TKeyResponse CGameView::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
    if (!iRuntime)
    {
        if (aType != EEventKey)
            return EKeyWasNotConsumed;
        return iListBox != NULL
            ? iListBox->OfferKeyEventL(aKeyEvent, aType)
            : EKeyWasNotConsumed;
    }

    const TBool leftSoftkey =
        aKeyEvent.iScanCode == EStdKeyDevice0 || aKeyEvent.iCode == EKeyDevice0;
    const TBool rightSoftkey =
        aKeyEvent.iScanCode == EStdKeyDevice1 || aKeyEvent.iCode == EKeyDevice1;
    const TBool volumeUp =
        aKeyEvent.iScanCode == EStdKeyIncVolume || aKeyEvent.iCode == EKeyIncVolume;
    const TBool volumeDown =
        aKeyEvent.iScanCode == EStdKeyDecVolume || aKeyEvent.iCode == EKeyDecVolume;

    if (volumeUp || volumeDown)
    {
        TBool& held = volumeUp ? iVolumeUpHeld : iVolumeDownHeld;
        if (aType == EEventKeyDown)
        {
            if (!held)
                AdjustMasterVolume(volumeUp ? KVolumeStep : -KVolumeStep);
            held = ETrue;
        }
        else if (aType == EEventKey)
        {
            if (!held || aKeyEvent.iRepeats > 0)
                AdjustMasterVolume(volumeUp ? KVolumeStep : -KVolumeStep);
            held = EFalse;
        }
        else if (aType == EEventKeyUp)
        {
            held = EFalse;
        }
        return EKeyWasConsumed;
    }

    if (leftSoftkey)
    {
        if (aType == EEventKeyDown)
        {
            if (!iLeftSoftkeyHeld)
                HandleLeftSoftkeyL();
            iLeftSoftkeyHeld = ETrue;
        }
        else if (aType == EEventKey)
        {
            if (!iLeftSoftkeyHeld && aKeyEvent.iRepeats == 0)
                HandleLeftSoftkeyL();
            iLeftSoftkeyHeld = EFalse;
        }
        else if (aType == EEventKeyUp)
        {
            iLeftSoftkeyHeld = EFalse;
        }
        return EKeyWasConsumed;
    }

    if (rightSoftkey)
    {
        if (iGameMenuActive)
        {
            if (aType == EEventKeyDown)
            {
                if (!iRightSoftkeyHeld)
                    HandleRightSoftkeyL();
                iRightSoftkeyHeld = ETrue;
            }
            else if (aType == EEventKey)
            {
                if (!iRightSoftkeyHeld && aKeyEvent.iRepeats == 0)
                    HandleRightSoftkeyL();
                iRightSoftkeyHeld = EFalse;
            }
            else if (aType == EEventKeyUp)
            {
                iRightSoftkeyHeld = EFalse;
            }
            return EKeyWasConsumed;
        }

        if (aType == EEventKeyDown)
        {
            iHeldPlayerKeys |= CPlayerRuntime::EKeyCancel;
            iRuntime->SetKeyState(CPlayerRuntime::EKeyCancel, ETrue);
        }
        else if (aType == EEventKeyUp)
        {
            iHeldPlayerKeys &= ~CPlayerRuntime::EKeyCancel;
            iRuntime->SetKeyState(CPlayerRuntime::EKeyCancel, EFalse);
        }
        else if (aType == EEventKey &&
                 (iHeldPlayerKeys & CPlayerRuntime::EKeyCancel) == 0)
        {
            iPendingPlayerKeyPulses |= CPlayerRuntime::EKeyCancel;
        }
        return EKeyWasConsumed;
    }

    if (iGameMenuActive)
    {
        if (aType != EEventKey)
            return EKeyWasConsumed;

        TUint32 menuKey = MapPlayerScanKey(aKeyEvent);
        if (menuKey == 0)
            menuKey = MapPlayerCharacterKey(aKeyEvent.iCode);

        if (iKeyMapVisible)
        {
            if (menuKey == CPlayerRuntime::EKeyDecision)
            {
                iKeyMapVisible = EFalse;
                iFrameDrawPending = EFalse;
                DrawNow();
            }
            else if (aKeyEvent.iRepeats == 0 &&
                     (aKeyEvent.iCode == 'F' || aKeyEvent.iCode == 'f'))
            {
                ToggleFpsOverlayL();
            }
            return EKeyWasConsumed;
        }

        if (menuKey == CPlayerRuntime::EKeyUp)
            MoveGameMenuSelection(-1);
        else if (menuKey == CPlayerRuntime::EKeyDown)
            MoveGameMenuSelection(1);
        else if (menuKey == CPlayerRuntime::EKeyLeft &&
                 iGameMenuSelection == EGameMenuVolume)
            AdjustMasterVolume(-KVolumeStep);
        else if (menuKey == CPlayerRuntime::EKeyRight &&
                 iGameMenuSelection == EGameMenuVolume)
            AdjustMasterVolume(KVolumeStep);
        else if (menuKey == CPlayerRuntime::EKeyDecision)
            ActivateGameMenuItemL();
        else if (menuKey == CPlayerRuntime::EKeyCancel)
            CloseGameMenu();
        return EKeyWasConsumed;
    }

    TUint32 scanKey = MapPlayerScanKey(aKeyEvent);
    if (scanKey == 0)
        scanKey = FindDynamicPlayerScanKey(aKeyEvent.iScanCode);

    if (aType == EEventKeyDown && scanKey != 0)
    {
        iHeldPlayerKeys |= scanKey;
        iRuntime->SetKeyState(scanKey, ETrue);
        return EKeyWasConsumed;
    }

    if (aType == EEventKeyUp && scanKey != 0)
    {
        iHeldPlayerKeys &= ~scanKey;
        iRuntime->SetKeyState(scanKey, EFalse);
        ForgetDynamicPlayerScanKey(aKeyEvent.iScanCode);
        return EKeyWasConsumed;
    }

    if (aType == EEventKey)
    {
        if (scanKey != 0)
        {
            if ((iHeldPlayerKeys & scanKey) == 0)
                iPendingPlayerKeyPulses |= scanKey;
            return EKeyWasConsumed;
        }

        const TUint32 playerKey = MapPlayerCharacterKey(aKeyEvent.iCode);
        if (playerKey != 0)
        {
            if (aKeyEvent.iScanCode != 0)
            {
                RememberDynamicPlayerScanKey(aKeyEvent.iScanCode, playerKey);
                iHeldPlayerKeys |= playerKey;
                iRuntime->SetKeyState(playerKey, ETrue);
            }
            iPendingPlayerKeyPulses |= playerKey;
            return EKeyWasConsumed;
        }
    }

    return EKeyWasNotConsumed;
}

void CGameView::HandleListBoxEventL(CEikListBox*, TListBoxEvent aEventType)
{
    if (iRuntime || !iStartGameCallback)
        return;

    if ((aEventType == EEventEnterKeyPressed ||
         aEventType == EEventItemClicked) &&
        !iStartGameCallback->IsActive())
    {
        iStartGameCallback->CallBack();
    }
}

TInt CGameView::StartGameCallback(TAny* aPtr)
{
    CGameView* self = static_cast<CGameView*>(aPtr);
    if (!self || self->iRuntime)
        return KErrNone;

    TRAPD(error, self->StartSelectedGameL());
    if (error != KErrNone)
    {
        self->iGameRoot.Zero();
        TRAP_IGNORE(self->ShowLoadErrorL(error));
    }
    return KErrNone;
}

void CGameView::FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame)
{
    if (!iForeground || !iRuntime)
        return;

    if (iVolumeOverlayTicks > 0)
    {
        --iVolumeOverlayTicks;
        if (iVolumeOverlayTicks == 0)
        {
            iVolumeOverlayErasePending = ETrue;
            DrawDeferred();
        }
    }

    TUint32 pulseKeys = iPendingPlayerKeyPulses;
    if (iPendingCancelPulse)
        pulseKeys |= CPlayerRuntime::EKeyCancel;
    if (pulseKeys != 0)
        SetPlayerKeyMask(*iRuntime, pulseKeys, ETrue);
    const TBool movieActive = EasyRpgMovieIsActive();
    iRuntime->TickL(aLogicFrame, aRenderFrame && !movieActive);
    if (pulseKeys != 0)
        SetPlayerKeyMask(*iRuntime, pulseKeys & ~iHeldPlayerKeys, EFalse);
    iPendingPlayerKeyPulses = 0;
    iPendingCancelPulse = EFalse;
    if (EasyRpgMovieConsumeRedraw())
    {
        iFrameDrawPending = EFalse;
        DrawNow();
    }
    if (aRenderFrame && !EasyRpgMovieIsActive() && iDisplaySurface && iDisplaySurface->HasFrame())
    {
        if (iPerfOverlay)
            iPerfOverlay->FrameRendered();
        const TBool overlayDirty = iPerfOverlay && iPerfOverlay->IsDirty();
        if (iDisplaySurface->HasDirtyRegions() || overlayDirty)
        {
            iFrameDrawPending = ETrue;
            DrawDeferred();
        }
    }
    if (!iRuntime->IsRunning())
    {
        StopGame();
        return;
    }
}

void CGameView::FrameError(TInt aError)
{
    StopGame();
    TRAP_IGNORE(ShowLoadErrorL(aError));
}

TUint32 CGameView::MapPlayerScanKey(const TKeyEvent& aKeyEvent) const
{
    switch (aKeyEvent.iScanCode)
    {
    case EStdKeyUpArrow:
    case 'W':
        return CPlayerRuntime::EKeyUp;
    case EStdKeyDownArrow:
    case 'S':
        return CPlayerRuntime::EKeyDown;
    case EStdKeyLeftArrow:
    case 'A':
        return CPlayerRuntime::EKeyLeft;
    case EStdKeyRightArrow:
    case 'D':
        return CPlayerRuntime::EKeyRight;
    case EStdKeyEnter:
    case EStdKeyDevice3:
    case EStdKeySpace:
    case 'Z':
        return CPlayerRuntime::EKeyDecision;
    case EStdKeyEscape:
        return CPlayerRuntime::EKeyCancel;
    case EStdKeyLeftShift:
    case EStdKeyRightShift:
        return CPlayerRuntime::EKeyShift;
    case '0':
    case EStdKeyNkp0:
        return CPlayerRuntime::EKeyN0;
    case '1':
    case EStdKeyNkp1:
        return CPlayerRuntime::EKeyN1;
    case '2':
    case EStdKeyNkp2:
        return CPlayerRuntime::EKeyN2;
    case '3':
    case EStdKeyNkp3:
        return CPlayerRuntime::EKeyN3;
    case '4':
    case EStdKeyNkp4:
        return CPlayerRuntime::EKeyN4;
    case '5':
    case EStdKeyNkp5:
        return CPlayerRuntime::EKeyN5;
    case '6':
    case EStdKeyNkp6:
        return CPlayerRuntime::EKeyN6;
    case '7':
    case EStdKeyNkp7:
        return CPlayerRuntime::EKeyN7;
    case '8':
    case EStdKeyNkp8:
        return CPlayerRuntime::EKeyN8;
    case '9':
    case EStdKeyNkp9:
        return CPlayerRuntime::EKeyN9;
    case EStdKeyNkpPlus:
        return CPlayerRuntime::EKeyPlus;
    case EStdKeyMinus:
    case EStdKeyNkpMinus:
        return CPlayerRuntime::EKeyMinus;
    case EStdKeyNkpAsterisk:
        return CPlayerRuntime::EKeyMultiply;
    case EStdKeyForwardSlash:
    case EStdKeyNkpForwardSlash:
        return CPlayerRuntime::EKeyDivide;
    case EStdKeyFullStop:
    case EStdKeyNkpFullStop:
        return CPlayerRuntime::EKeyPeriod;
    case 'P':
        return CPlayerRuntime::EKeyScreenshot;
    default:
        return 0;
    }
}

TUint32 CGameView::MapPlayerCharacterKey(TInt aCode) const
{
    switch (aCode)
    {
    case EKeyUpArrow:
    case 'W':
    case 'w':
        return CPlayerRuntime::EKeyUp;
    case EKeyDownArrow:
    case 'S':
    case 's':
        return CPlayerRuntime::EKeyDown;
    case EKeyLeftArrow:
    case 'A':
    case 'a':
        return CPlayerRuntime::EKeyLeft;
    case EKeyRightArrow:
    case 'D':
    case 'd':
        return CPlayerRuntime::EKeyRight;
    case EKeyEnter:
    case EKeyDevice3:
    case 'Z':
    case 'z':
    case ' ':
        return CPlayerRuntime::EKeyDecision;
    case EKeyEscape:
        return CPlayerRuntime::EKeyCancel;
    case EKeyLeftShift:
    case EKeyRightShift:
        return CPlayerRuntime::EKeyShift;
    case '0':
        return CPlayerRuntime::EKeyN0;
    case '1':
        return CPlayerRuntime::EKeyN1;
    case '2':
        return CPlayerRuntime::EKeyN2;
    case '3':
        return CPlayerRuntime::EKeyN3;
    case '4':
        return CPlayerRuntime::EKeyN4;
    case '5':
        return CPlayerRuntime::EKeyN5;
    case '6':
        return CPlayerRuntime::EKeyN6;
    case '7':
        return CPlayerRuntime::EKeyN7;
    case '8':
        return CPlayerRuntime::EKeyN8;
    case '9':
        return CPlayerRuntime::EKeyN9;
    case '+':
        return CPlayerRuntime::EKeyPlus;
    case '-':
        return CPlayerRuntime::EKeyMinus;
    case '*':
        return CPlayerRuntime::EKeyMultiply;
    case '/':
        return CPlayerRuntime::EKeyDivide;
    case '.':
        return CPlayerRuntime::EKeyPeriod;
    case 'P':
    case 'p':
        return CPlayerRuntime::EKeyScreenshot;
    default:
        return 0;
    }
}

TUint32 CGameView::FindDynamicPlayerScanKey(TInt aScanCode) const
{
    for (TInt index = 0; index < iDynamicPlayerScanCount; ++index)
    {
        if (iDynamicPlayerScanCodes[index] == aScanCode)
            return iDynamicPlayerScanKeys[index];
    }
    return 0;
}

void CGameView::RememberDynamicPlayerScanKey(TInt aScanCode, TUint32 aPlayerKey)
{
    for (TInt index = 0; index < iDynamicPlayerScanCount; ++index)
    {
        if (iDynamicPlayerScanCodes[index] == aScanCode)
        {
            iDynamicPlayerScanKeys[index] = aPlayerKey;
            return;
        }
    }

    if (iDynamicPlayerScanCount >= 16)
        return;
    iDynamicPlayerScanCodes[iDynamicPlayerScanCount] = aScanCode;
    iDynamicPlayerScanKeys[iDynamicPlayerScanCount] = aPlayerKey;
    ++iDynamicPlayerScanCount;
}

void CGameView::ForgetDynamicPlayerScanKey(TInt aScanCode)
{
    for (TInt index = 0; index < iDynamicPlayerScanCount; ++index)
    {
        if (iDynamicPlayerScanCodes[index] != aScanCode)
            continue;
        --iDynamicPlayerScanCount;
        iDynamicPlayerScanCodes[index] = iDynamicPlayerScanCodes[iDynamicPlayerScanCount];
        iDynamicPlayerScanKeys[index] = iDynamicPlayerScanKeys[iDynamicPlayerScanCount];
        return;
    }
}


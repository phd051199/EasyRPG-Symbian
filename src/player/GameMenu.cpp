#include <bitdev.h>
#include <e32keys.h>
#include <fbs.h>
#include <gdi.h>

#include "GameMenu.h"
#include "AppConfig.h"
#include "AppPaths.h"

namespace
{
const TInt KDefaultVolume = 50;
const TInt KVolumeStep = 10;
_LIT8(KMasterVolumeSetting, "master_volume=");
_LIT8(KSoundSetting, "sound_enabled=");
_LIT8(KFrameSkipSetting, "frameskip_enabled=");
_LIT8(KFpsSetting, "fps_enabled=");

enum TMenuKey
{
    EMenuKeyNone = 0,
    EMenuKeyUp = 1,
    EMenuKeyDown,
    EMenuKeyLeft,
    EMenuKeyRight,
    EMenuKeyDecision,
    EMenuKeyCancel
};

TUint32 MenuKeyBit(TInt aKey)
{
    return aKey > EMenuKeyNone && aKey <= EMenuKeyCancel
        ? (1u << static_cast<TUint32>(aKey))
        : 0;
}

TInt ClampVolume(TInt aVolume)
{
    if (aVolume < 0)
        return 0;
    if (aVolume > 100)
        return 100;
    return aVolume;
}

TBool ReadSettingInt(const TDesC8& aData, const TDesC8& aKey, TInt& aValue)
{
    const TInt keyPosition = aData.Find(aKey);
    if (keyPosition < 0)
        return EFalse;
    TInt position = keyPosition + aKey.Length();
    TInt value = 0;
    TBool hasDigit = EFalse;
    while (position < aData.Length())
    {
        const TUint8 c = aData[position++];
        if (c < '0' || c > '9')
            break;
        hasDigit = ETrue;
        value = value * 10 + (c - '0');
    }
    if (!hasDigit)
        return EFalse;
    aValue = value;
    return ETrue;
}

void CopyAscii(const TDesC8& aSource, TDes& aDestination)
{
    aDestination.Zero();
    const TInt count = Min(aSource.Length(), aDestination.MaxLength());
    for (TInt index = 0; index < count; ++index)
    {
        const TUint8 c = aSource[index];
        aDestination.Append(static_cast<TChar>(c >= 32 && c < 127 ? c : '?'));
    }
}
}

CGameMenu* CGameMenu::NewL(MGameMenuHost& aHost, RFs& aFs)
{
    CGameMenu* self = new (ELeave) CGameMenu(aHost, aFs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CGameMenu::CGameMenu(MGameMenuHost& aHost, RFs& aFs)
    : iHost(aHost),
      iFs(aFs),
      iOpen(EFalse),
      iKeysVisible(EFalse),
      iSelection(0),
      iLeftSoftHeld(EFalse),
      iRightSoftHeld(EFalse),
      iHeldMenuKeys(0),
      iVolume(KDefaultVolume),
      iSoundEnabled(ETrue),
      iFrameSkipEnabled(EFalse),
      iFpsEnabled(ETrue)
{
}

void CGameMenu::ConstructL()
{
    LoadSettings();
    ApplySettingsL();
}

CGameMenu::~CGameMenu()
{
}

void CGameMenu::LoadSettings()
{
    TFileName settingsPath;
    if (EasyRpgPaths::Build(settingsPath, EasyRpgApp::KSettingsFileSuffix) != KErrNone)
        return;
    RFile file;
    if (file.Open(iFs, settingsPath, EFileRead | EFileShareReadersOnly) != KErrNone)
        return;
    TBuf8<256> data;
    const TInt error = file.Read(data);
    file.Close();
    if (error != KErrNone)
        return;

    TInt value = 0;
    const TBool hasVolume = ReadSettingInt(data, KMasterVolumeSetting, value);
    if (hasVolume)
        iVolume = ClampVolume(value);
    if (ReadSettingInt(data, KSoundSetting, value))
        iSoundEnabled = value != 0;
    else if (hasVolume)
        iSoundEnabled = iVolume > 0;
    if (ReadSettingInt(data, KFrameSkipSetting, value))
        iFrameSkipEnabled = value != 0;
    if (ReadSettingInt(data, KFpsSetting, value))
        iFpsEnabled = value != 0;
}

void CGameMenu::SaveSettings()
{
    TFileName settingsDirectory;
    TFileName settingsPath;
    if (EasyRpgPaths::Build(settingsDirectory, EasyRpgApp::KSettingsDirectorySuffix) != KErrNone ||
        EasyRpgPaths::Build(settingsPath, EasyRpgApp::KSettingsFileSuffix) != KErrNone)
        return;
    const TInt directoryError = iFs.MkDirAll(settingsDirectory);
    if (directoryError != KErrNone && directoryError != KErrAlreadyExists)
        return;
    RFile file;
    if (file.Replace(iFs, settingsPath, EFileWrite | EFileShareExclusive) != KErrNone)
        return;
    TBuf8<128> data;
    data.Format(_L8("master_volume=%d\nsound_enabled=%d\nframeskip_enabled=%d\nfps_enabled=%d\n"),
                iVolume,
                iSoundEnabled ? 1 : 0,
                iFrameSkipEnabled ? 1 : 0,
                iFpsEnabled ? 1 : 0);
    file.Write(data);
    file.Close();
}

void CGameMenu::ApplySettingsL()
{
    const TUint32 features = iHost.GameMenuFeatures();
    if (features & EGameMenuFeatureVolume)
        iHost.GameMenuSetVolumeL(iVolume);
    if (features & EGameMenuFeatureSound)
        iHost.GameMenuSetSoundEnabledL(iSoundEnabled);
    if (features & EGameMenuFeatureFrameSkip)
        iHost.GameMenuSetFrameSkipEnabledL(iFrameSkipEnabled);
    if (features & EGameMenuFeatureFps)
        iHost.GameMenuSetFpsEnabledL(iFpsEnabled);
}

TBool CGameMenu::IsOpen() const
{
    return iOpen;
}

TInt CGameMenu::Selection() const
{
    return iSelection;
}

TInt CGameMenu::Volume() const
{
    return iVolume;
}

TBool CGameMenu::SoundEnabled() const
{
    return iSoundEnabled;
}

TInt CGameMenu::ItemCount() const
{
    const TUint32 features = iHost.GameMenuFeatures();
    TInt count = 2;
    if (features & EGameMenuFeatureSound) ++count;
    if (features & EGameMenuFeatureVolume) ++count;
    if (features & EGameMenuFeatureFrameSkip) ++count;
    if (features & EGameMenuFeatureFps) ++count;
    if (features & EGameMenuFeatureKeys) ++count;
    count += Max(0, iHost.GameMenuCustomItemCount());
    if (features & EGameMenuFeatureReset) ++count;
    return count;
}

CGameMenu::TItemKind CGameMenu::ItemKindAt(TInt aIndex, TInt& aCustomIndex) const
{
    aCustomIndex = -1;
    if (aIndex < 0)
        return EItemInvalid;
    const TUint32 features = iHost.GameMenuFeatures();
    TInt cursor = 0;
#define MATCH_FEATURE(flag, kind) \
    if (features & (flag)) { if (aIndex == cursor) return (kind); ++cursor; }
    MATCH_FEATURE(EGameMenuFeatureSound, EItemSound)
    MATCH_FEATURE(EGameMenuFeatureVolume, EItemVolume)
    MATCH_FEATURE(EGameMenuFeatureFrameSkip, EItemFrameSkip)
    MATCH_FEATURE(EGameMenuFeatureFps, EItemFps)
    MATCH_FEATURE(EGameMenuFeatureKeys, EItemKeys)
#undef MATCH_FEATURE

    const TInt customCount = Max(0, iHost.GameMenuCustomItemCount());
    if (aIndex >= cursor && aIndex < cursor + customCount)
    {
        aCustomIndex = aIndex - cursor;
        return EItemCustom;
    }
    cursor += customCount;
    if (features & EGameMenuFeatureReset)
    {
        if (aIndex == cursor)
            return EItemReset;
        ++cursor;
    }
    if (aIndex == cursor++)
        return EItemResume;
    if (aIndex == cursor)
        return EItemExit;
    return EItemInvalid;
}

void CGameMenu::ItemText(TInt aIndex, TDes8& aText) const
{
    aText.Zero();
    TInt customIndex = -1;
    switch (ItemKindAt(aIndex, customIndex))
    {
    case EItemSound:
        aText.Copy(iSoundEnabled ? _L8("SOUND ON") : _L8("SOUND OFF"));
        break;
    case EItemVolume:
        aText.Format(_L8("VOLUME %d%%"), iVolume);
        break;
    case EItemFrameSkip:
        aText.Copy(iFrameSkipEnabled ? _L8("FRAMESKIP AUTO") : _L8("FRAMESKIP OFF"));
        break;
    case EItemFps:
        aText.Copy(iFpsEnabled ? _L8("FPS ON") : _L8("FPS OFF"));
        break;
    case EItemKeys:
        aText.Copy(_L8("KEYS"));
        break;
    case EItemCustom:
        iHost.GameMenuCustomItemText(customIndex, aText);
        break;
    case EItemReset:
        aText.Copy(_L8("RESET"));
        break;
    case EItemResume:
        aText.Copy(_L8("RESUME"));
        break;
    case EItemExit:
        aText.Copy(_L8("EXIT TO LAUNCHER"));
        break;
    default:
        break;
    }
}

TBool CGameMenu::ItemAdjustable(TInt aIndex) const
{
    TInt customIndex = -1;
    return ItemKindAt(aIndex, customIndex) == EItemVolume;
}

void CGameMenu::OpenL()
{
    if (iOpen || ItemCount() <= 0)
        return;
    NormalizeSelection();
    iHeldMenuKeys = 0;
    iKeysVisible = EFalse;
    iOpen = ETrue;
    TRAPD(error, iHost.GameMenuPauseL(ETrue));
    if (error != KErrNone)
    {
        iOpen = EFalse;
        User::Leave(error);
    }
    iHost.GameMenuRequestRedraw();
}

void CGameMenu::CloseL()
{
    if (!iOpen)
        return;
    iHeldMenuKeys = 0;
    iKeysVisible = EFalse;
    iOpen = EFalse;
    TRAPD(error, iHost.GameMenuPauseL(EFalse));
    if (error != KErrNone)
    {
        iOpen = ETrue;
        User::Leave(error);
    }
    iHost.GameMenuRequestRedraw();
}

void CGameMenu::ResetInputState()
{
    iLeftSoftHeld = EFalse;
    iRightSoftHeld = EFalse;
    iHeldMenuKeys = 0;
}

void CGameMenu::NormalizeSelection()
{
    const TInt count = ItemCount();
    if (count <= 0)
    {
        iSelection = 0;
        return;
    }
    while (iSelection < 0)
        iSelection += count;
    while (iSelection >= count)
        iSelection -= count;
}

void CGameMenu::MoveSelection(TInt aDelta)
{
    if (!iOpen || iKeysVisible || ItemCount() <= 0)
        return;
    iSelection += aDelta;
    NormalizeSelection();
    iHost.GameMenuRequestRedraw();
}

void CGameMenu::ActivateSelectionL()
{
    NormalizeSelection();
    TInt customIndex = -1;
    switch (ItemKindAt(iSelection, customIndex))
    {
    case EItemSound:
        iSoundEnabled = !iSoundEnabled;
        iHost.GameMenuSetSoundEnabledL(iSoundEnabled);
        SaveSettings();
        break;
    case EItemVolume:
        AdjustSelectionL(1);
        return;
    case EItemFrameSkip:
        iFrameSkipEnabled = !iFrameSkipEnabled;
        iHost.GameMenuSetFrameSkipEnabledL(iFrameSkipEnabled);
        SaveSettings();
        break;
    case EItemFps:
        iFpsEnabled = !iFpsEnabled;
        iHost.GameMenuSetFpsEnabledL(iFpsEnabled);
        SaveSettings();
        break;
    case EItemKeys:
        iKeysVisible = ETrue;
        break;
    case EItemCustom:
        if (!iHost.GameMenuCustomActivateL(customIndex))
        {
            CloseL();
            return;
        }
        break;
    case EItemReset:
        iHost.GameMenuResetL();
        CloseL();
        return;
    case EItemResume:
        CloseL();
        return;
    case EItemExit:
        iHost.GameMenuExitL();
        return;
    default:
        return;
    }
    iHost.GameMenuRequestRedraw();
}

void CGameMenu::AdjustSelectionL(TInt aDelta)
{
    if (!ItemAdjustable(iSelection) || aDelta == 0)
        return;
    iVolume = ClampVolume(iVolume + aDelta * KVolumeStep);
    iHost.GameMenuSetVolumeL(iVolume);
    SaveSettings();
    iHost.GameMenuRequestRedraw();
}

TInt CGameMenu::MapMenuKey(const TKeyEvent& aKey) const
{
    switch (aKey.iScanCode)
    {
    case EStdKeyUpArrow:
    case 'W': return EMenuKeyUp;
    case EStdKeyDownArrow:
    case 'S': return EMenuKeyDown;
    case EStdKeyLeftArrow:
    case 'A': return EMenuKeyLeft;
    case EStdKeyRightArrow:
    case 'D': return EMenuKeyRight;
    case EStdKeyEnter:
    case EStdKeyDevice3:
    case EStdKeySpace:
    case 'Z': return EMenuKeyDecision;
    case EStdKeyEscape: return EMenuKeyCancel;
    default:
        break;
    }
    switch (aKey.iCode)
    {
    case EKeyUpArrow:
    case 'W':
    case 'w': return EMenuKeyUp;
    case EKeyDownArrow:
    case 'S':
    case 's': return EMenuKeyDown;
    case EKeyLeftArrow:
    case 'A':
    case 'a': return EMenuKeyLeft;
    case EKeyRightArrow:
    case 'D':
    case 'd': return EMenuKeyRight;
    case EKeyEnter:
    case EKeyDevice3:
    case 'Z':
    case 'z':
    case ' ': return EMenuKeyDecision;
    case EKeyEscape: return EMenuKeyCancel;
    default: return EMenuKeyNone;
    }
}

TBool CGameMenu::HandleSoftKeyL(TBool aLeft, TInt aType, const TKeyEvent& aKey)
{
    TBool& held = aLeft ? iLeftSoftHeld : iRightSoftHeld;
    TBool trigger = EFalse;
    if (aType == EEventKeyDown)
    {
        trigger = !held;
        held = ETrue;
    }
    else if (aType == EEventKey)
    {
        trigger = !held && aKey.iRepeats == 0;
        held = EFalse;
    }
    else if (aType == EEventKeyUp)
    {
        held = EFalse;
    }
    if (!trigger)
        return ETrue;

    if (!iOpen)
    {
        if (!aLeft)
            return EFalse;
        OpenL();
        return ETrue;
    }
    if (iKeysVisible)
    {
        iKeysVisible = EFalse;
        iHost.GameMenuRequestRedraw();
        return ETrue;
    }
    if (aLeft)
        ActivateSelectionL();
    else
        CloseL();
    return ETrue;
}

TBool CGameMenu::TriggerMenuKey(TInt aMenuKey, TInt aType, const TKeyEvent& aKey)
{
    const TUint32 bit = MenuKeyBit(aMenuKey);
    if (bit == 0)
        return EFalse;
    if (aType == EEventKeyDown)
    {
        const TBool trigger = (iHeldMenuKeys & bit) == 0;
        iHeldMenuKeys |= bit;
        return trigger;
    }
    if (aType == EEventKeyUp)
    {
        iHeldMenuKeys &= ~bit;
        return EFalse;
    }
    if (aType == EEventKey)
        return (iHeldMenuKeys & bit) == 0 || aKey.iRepeats > 0;
    return EFalse;
}

TBool CGameMenu::HandleKeyEventL(TInt aType, const TKeyEvent& aKey)
{
    const TBool leftSoft = aKey.iScanCode == EStdKeyDevice0 || aKey.iCode == EKeyDevice0;
    const TBool rightSoft = aKey.iScanCode == EStdKeyDevice1 || aKey.iCode == EKeyDevice1;
    if (leftSoft)
        return HandleSoftKeyL(ETrue, aType, aKey);
    if (rightSoft)
    {
        if (!iOpen)
            return EFalse;
        return HandleSoftKeyL(EFalse, aType, aKey);
    }
    if (!iOpen)
        return EFalse;

    const TInt menuKey = MapMenuKey(aKey);
    if (menuKey == EMenuKeyNone)
        return ETrue;
    const TBool trigger = TriggerMenuKey(menuKey, aType, aKey);
    if (!trigger)
        return ETrue;

    if (iKeysVisible)
    {
        if (menuKey == EMenuKeyDecision || menuKey == EMenuKeyCancel)
        {
            iKeysVisible = EFalse;
            iHost.GameMenuRequestRedraw();
        }
        return ETrue;
    }

    switch (menuKey)
    {
    case EMenuKeyUp:
        MoveSelection(-1);
        break;
    case EMenuKeyDown:
        MoveSelection(1);
        break;
    case EMenuKeyLeft:
        AdjustSelectionL(-1);
        break;
    case EMenuKeyRight:
        AdjustSelectionL(1);
        break;
    case EMenuKeyDecision:
        ActivateSelectionL();
        break;
    case EMenuKeyCancel:
        CloseL();
        break;
    default:
        break;
    }
    return ETrue;
}

void CGameMenu::DrawL(CGraphicsContext& aGc, CBitmapDevice& aDevice)
{
    if (!iOpen)
        return;
    if (iKeysVisible)
        DrawKeysL(aGc, aDevice);
    else
        DrawMenuL(aGc, aDevice);
}

void CGameMenu::DrawMenuL(CGraphicsContext& aGc, CBitmapDevice& aDevice)
{
    const TInt itemCount = ItemCount();
    if (itemCount <= 0)
        return;
    NormalizeSelection();

    const TSize screenSize = aDevice.SizeInPixels();
    const TInt rowHeight = 16;
    const TInt panelWidth = Min(190, Max(1, screenSize.iWidth - 12));
    const TInt wantedHeight = 38 + itemCount * rowHeight;
    const TInt panelHeight = Min(wantedHeight, Max(1, screenSize.iHeight - 12));
    const TPoint origin(0, screenSize.iHeight - panelHeight);
    const TRect panel(origin, TSize(panelWidth, panelHeight));

    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenColor(TRgb(255, 255, 255));
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(TRgb(0, 0, 0));
    aGc.DrawRect(panel);

    CFont* font = NULL;
    User::LeaveIfError(aDevice.GetNearestFontInPixels(font, TFontSpec(_L("Series 60 Sans"), 11)));
    if (!font)
        User::Leave(KErrNotFound);
    aGc.UseFont(font);
    aGc.SetPenColor(TRgb(255, 255, 255));
    aGc.SetBrushStyle(CGraphicsContext::ENullBrush);
    aGc.DrawText(_L("MENU"), TPoint(panel.iTl.iX + 7, panel.iTl.iY + 13));

    const TInt rowX = panel.iTl.iX + 5;
    const TInt rowWidth = panelWidth - 10;
    const TInt firstBaseline = panel.iTl.iY + 29;
    const TInt visibleRows = Max(1, (panelHeight - 42) / rowHeight);
    TInt firstItem = 0;
    if (iSelection >= visibleRows)
        firstItem = iSelection - visibleRows + 1;
    if (firstItem + visibleRows > itemCount)
        firstItem = Max(0, itemCount - visibleRows);

    for (TInt row = 0; row < visibleRows && firstItem + row < itemCount; ++row)
    {
        const TInt item = firstItem + row;
        const TInt baseline = firstBaseline + row * rowHeight;
        TBuf8<64> text8;
        ItemText(item, text8);
        TBuf<64> text;
        CopyAscii(text8, text);
        if (item == iSelection)
        {
            aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
            aGc.SetBrushColor(TRgb(255, 255, 255));
            aGc.SetPenColor(TRgb(255, 255, 255));
            aGc.DrawRect(TRect(TPoint(rowX, baseline - 11), TSize(rowWidth, 14)));
            aGc.SetBrushStyle(CGraphicsContext::ENullBrush);
            aGc.SetPenColor(TRgb(0, 0, 0));
            aGc.DrawText(text, TPoint(rowX + 4, baseline));
            aGc.SetPenColor(TRgb(255, 255, 255));
        }
        else
        {
            aGc.DrawText(text, TPoint(rowX + 4, baseline));
        }
    }

    aGc.DrawText(_L("SELECT"), TPoint(panel.iTl.iX + 7, panel.iBr.iY - 4));
    const TInt backWidth = font->TextWidthInPixels(_L("BACK"));
    aGc.DrawText(_L("BACK"), TPoint(panel.iBr.iX - backWidth - 7, panel.iBr.iY - 4));
    aGc.DiscardFont();
    aDevice.ReleaseFont(font);
}

void CGameMenu::DrawKeysL(CGraphicsContext& aGc, CBitmapDevice& aDevice)
{
    const TSize screenSize = aDevice.SizeInPixels();
    const TInt panelWidth = Min(292, Max(1, screenSize.iWidth - 12));
    const TInt panelHeight = Min(150, Max(1, screenSize.iHeight - 12));
    const TRect panel(TPoint((screenSize.iWidth - panelWidth) / 2,
                             (screenSize.iHeight - panelHeight) / 2),
                      TSize(panelWidth, panelHeight));

    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenColor(TRgb(255, 255, 255));
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(TRgb(0, 0, 0));
    aGc.DrawRect(panel);

    CFont* font = NULL;
    User::LeaveIfError(aDevice.GetNearestFontInPixels(font, TFontSpec(_L("Series 60 Sans"), 11)));
    if (!font)
        User::Leave(KErrNotFound);
    aGc.UseFont(font);
    aGc.SetBrushStyle(CGraphicsContext::ENullBrush);
    aGc.SetPenColor(TRgb(255, 255, 255));

    TInt x = panel.iTl.iX + 10;
    TInt y = panel.iTl.iY + 16;
    const TInt step = 18;
    aGc.DrawText(_L("KEYS"), TPoint(x, y));
    y += step;
    aGc.DrawText(_L("UP / W        UP"), TPoint(x, y));
    y += step;
    aGc.DrawText(_L("DOWN / S      DOWN"), TPoint(x, y));
    y += step;
    aGc.DrawText(_L("LEFT / A      LEFT"), TPoint(x, y));
    y += step;
    aGc.DrawText(_L("RIGHT / D     RIGHT"), TPoint(x, y));
    y += step;
    aGc.DrawText(_L("CENTER / Z    PRIMARY"), TPoint(x, y));
    y += step;
    aGc.DrawText(_L("R SOFT / ESC  BACK"), TPoint(x, y));
    aGc.DrawText(_L("CENTER/BACK TO MENU"), TPoint(x, panel.iBr.iY - 7));

    aGc.DiscardFont();
    aDevice.ReleaseFont(font);
}

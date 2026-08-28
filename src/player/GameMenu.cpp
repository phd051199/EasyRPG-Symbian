#include <bitdev.h>
#include <e32keys.h>
#include <fbs.h>
#include <gdi.h>

#include "GameMenu.h"
#include "PlayerRuntime.h"
#include "AppConfig.h"
#include "AppPaths.h"
#include "GohuFont.h"
#include "KeyMapping.h"
#include "NativeAudio.h"

namespace
{
const TInt KDefaultVolume = 50;
const TInt KVolumeStep = 10;
const TInt KGohuGlyphWidth = 6;
const TInt KGohuGlyphHeight = 11;
const TInt KMenuFontScale = 1;
const TInt KMenuTextHeight = 12;
const TInt KMenuTextAdvance = KGohuGlyphWidth * KMenuFontScale;
_LIT8(KMasterVolumeSetting, "master_volume=");
_LIT8(KSoundSetting, "sound_enabled=");
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

void AppendSettingInt(TDes8& aData, const TDesC8& aKey, TInt aValue)
{
    TBuf8<24> value;
    value.Num(aValue);
    if (aData.Length() + aKey.Length() + value.Length() + 1 > aData.MaxLength())
        return;
    aData.Append(aKey);
    aData.Append(value);
    aData.Append('\n');
}

TInt GohuTextWidth(const TDesC8& aText)
{
    return aText.Length() * KMenuTextAdvance;
}

void DrawGohuText(CGraphicsContext& aGc,
                  const TDesC8& aText,
                  const TPoint& aOrigin,
                  const TRgb& aColor)
{
    aGc.SetPenStyle(CGraphicsContext::ENullPen);
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(aColor);

    TInt cursorX = aOrigin.iX;
    for (TInt index = 0; index < aText.Length(); ++index)
    {
        const TUint8 c = aText[index];
        const unsigned char* rows = EasyRpgGohuGlyphRows(c >= 32 && c < 127 ? c : '?');
        if (!rows)
        {
            cursorX += KMenuTextAdvance;
            continue;
        }

        for (TInt y = 0; y < KGohuGlyphHeight; ++y)
        {
            TInt x = 0;
            while (x < KGohuGlyphWidth)
            {
                while (x < KGohuGlyphWidth && (rows[y] & (1u << x)) == 0)
                    ++x;
                const TInt runStart = x;
                while (x < KGohuGlyphWidth && (rows[y] & (1u << x)) != 0)
                    ++x;
                if (runStart < x)
                {
                    aGc.DrawRect(TRect(TPoint(cursorX + runStart * KMenuFontScale,
                                             aOrigin.iY + y * KMenuFontScale),
                                       TSize((x - runStart) * KMenuFontScale,
                                             KMenuFontScale)));
                }
            }
        }
        cursorX += KMenuTextAdvance;
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
      iHelpVisible(EFalse),
      iControlsVisible(EFalse),
      iSelection(0),
      iControlSelection(0),
      iCaptureControlIndex(-1),
      iCapturedScanCode(0),
      iLeftSoftHeld(EFalse),
      iRightSoftHeld(EFalse),
      iRightSoftCaptured(EFalse),
      iHeldMenuKeys(0),
      iVolume(KDefaultVolume),
      iSoundEnabled(ETrue),
      iFpsEnabled(EFalse)
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
    EasyRpgResetCustomPlayerKeyBindings();
    TFileName settingsPath;
    if (EasyRpgPaths::Build(settingsPath, EasyRpgApp::KSettingsFileSuffix) != KErrNone)
        return;
    RFile file;
    if (file.Open(iFs, settingsPath, EFileRead | EFileShareReadersOnly) != KErrNone)
        return;
    TBuf8<2048> data;
    const TInt error = file.Read(data);
    file.Close();
    if (error != KErrNone)
        return;

    TInt value = 0;
    const TBool hasVolume = ReadSettingInt(data, KMasterVolumeSetting, value);
    if (hasVolume)
        iVolume = TNativeAudio::ClampVolume(value);
    if (ReadSettingInt(data, KSoundSetting, value))
        iSoundEnabled = value != 0;
    else if (hasVolume)
        iSoundEnabled = iVolume > 0;
    if (ReadSettingInt(data, KFpsSetting, value))
        iFpsEnabled = value != 0;

    const TInt keyCount = EasyRpgConfigurablePlayerKeyCount();
    for (TInt index = 0; index < keyCount; ++index)
    {
        const TUint32 playerKey = EasyRpgConfigurablePlayerKeyAt(index);
        TBuf8<32> settingName;
        EasyRpgPlayerKeySettingName(playerKey, settingName);
        if (settingName.Length() <= 0)
            continue;

        TBuf8<40> scanSetting(settingName);
        scanSetting.Append(_L8("_scan="));
        TInt scanCode = 0;
        if (!ReadSettingInt(data, scanSetting, scanCode))
            continue;

        TBuf8<40> charSetting(settingName);
        charSetting.Append(_L8("_char="));
        TInt charCode = 0;
        ReadSettingInt(data, charSetting, charCode);
        EasyRpgSetCustomPlayerKeyBinding(playerKey, scanCode, charCode);
    }
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
    TBuf8<2048> data;
    data.Format(_L8("master_volume=%d\nsound_enabled=%d\nfps_enabled=%d\n"),
                iVolume,
                iSoundEnabled ? 1 : 0,
                iFpsEnabled ? 1 : 0);

    const TInt keyCount = EasyRpgConfigurablePlayerKeyCount();
    for (TInt index = 0; index < keyCount; ++index)
    {
        const TUint32 playerKey = EasyRpgConfigurablePlayerKeyAt(index);
        TInt scanCode = 0;
        TInt charCode = 0;
        if (!EasyRpgGetCustomPlayerKeyBinding(playerKey, scanCode, charCode))
            continue;

        TBuf8<32> settingName;
        EasyRpgPlayerKeySettingName(playerKey, settingName);
        if (settingName.Length() <= 0)
            continue;

        TBuf8<40> scanSetting(settingName);
        scanSetting.Append(_L8("_scan="));
        AppendSettingInt(data, scanSetting, scanCode);
        TBuf8<40> charSetting(settingName);
        charSetting.Append(_L8("_char="));
        AppendSettingInt(data, charSetting, charCode);
    }
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
    if (features & EGameMenuFeatureFps)
        iHost.GameMenuSetFpsEnabledL(iFpsEnabled);
}

TBool CGameMenu::IsOpen() const
{
    return iOpen;
}

TInt CGameMenu::ItemCount() const
{
    const TUint32 features = iHost.GameMenuFeatures();
    TInt count = 1;
    if (features & EGameMenuFeatureSound) ++count;
    if (features & EGameMenuFeatureVolume) ++count;
    if (features & EGameMenuFeatureFps) ++count;
    if (features & EGameMenuFeatureHelp) ++count;
    if (features & EGameMenuFeatureControls) ++count;
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
    MATCH_FEATURE(EGameMenuFeatureFps, EItemFps)
    MATCH_FEATURE(EGameMenuFeatureHelp, EItemHelp)
    MATCH_FEATURE(EGameMenuFeatureControls, EItemControls)
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
        aText.Copy(iSoundEnabled ? _L8("SOUND: ON") : _L8("SOUND: OFF"));
        break;
    case EItemVolume:
        aText.Format(_L8("VOLUME %d%%"), iVolume);
        break;
    case EItemFps:
        aText.Copy(iFpsEnabled ? _L8("FPS: ON") : _L8("FPS: OFF"));
        break;
    case EItemHelp:
        aText.Copy(_L8("HELP"));
        break;
    case EItemControls:
        aText.Copy(_L8("CONTROLS"));
        break;
    case EItemCustom:
        iHost.GameMenuCustomItemText(customIndex, aText);
        break;
    case EItemReset:
        aText.Copy(_L8("RESET"));
        break;
    case EItemExit:
        aText.Copy(_L8("EXIT"));
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
    iHelpVisible = EFalse;
    iControlsVisible = EFalse;
    iCaptureControlIndex = -1;
    iCapturedScanCode = 0;
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
    iHelpVisible = EFalse;
    iControlsVisible = EFalse;
    iCaptureControlIndex = -1;
    iCapturedScanCode = 0;
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
    iRightSoftCaptured = EFalse;
    iHeldMenuKeys = 0;
    iCaptureControlIndex = -1;
    iCapturedScanCode = 0;
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
    if (!iOpen || iHelpVisible || iControlsVisible || ItemCount() <= 0)
        return;
    iSelection += aDelta;
    NormalizeSelection();
    iHost.GameMenuRequestRedraw();
}

void CGameMenu::NormalizeControlSelection()
{
    const TInt count = EasyRpgConfigurablePlayerKeyCount() + 1;
    if (count <= 0)
    {
        iControlSelection = 0;
        return;
    }
    while (iControlSelection < 0)
        iControlSelection += count;
    while (iControlSelection >= count)
        iControlSelection -= count;
}

void CGameMenu::MoveControlSelection(TInt aDelta)
{
    if (!iControlsVisible || iCaptureControlIndex >= 0 || aDelta == 0)
        return;
    iControlSelection += aDelta;
    NormalizeControlSelection();
    iHost.GameMenuRequestRedraw();
}

void CGameMenu::ActivateControlSelectionL()
{
    if (!iControlsVisible || iCaptureControlIndex >= 0)
        return;
    NormalizeControlSelection();
    const TInt keyCount = EasyRpgConfigurablePlayerKeyCount();
    if (iControlSelection == keyCount)
    {
        EasyRpgResetCustomPlayerKeyBindings();
        SaveSettings();
        iHost.GameMenuRequestRedraw();
        return;
    }
    if (iControlSelection < 0 || iControlSelection >= keyCount)
        return;
    iCaptureControlIndex = iControlSelection;
    iCapturedScanCode = 0;
    iHeldMenuKeys = 0;
    iHost.GameMenuRequestRedraw();
}

TBool CGameMenu::HandleControlCaptureL(TInt aType, const TKeyEvent& aKey)
{
    if (!iControlsVisible)
        return EFalse;

    if (iCapturedScanCode != 0)
    {
        if (aKey.iScanCode == iCapturedScanCode)
        {
            if (aType == EEventKeyUp || aType == EEventKey)
                iCapturedScanCode = 0;
            return ETrue;
        }
        // Some keyboard drivers omit KeyUp. A different scan code proves the
        // captured physical press has ended, so do not keep the latch forever.
        iCapturedScanCode = 0;
    }

    if (iCaptureControlIndex < 0)
        return EFalse;
    if (aType == EEventKeyUp)
        return ETrue;
    if (aType == EEventKey && aKey.iRepeats > 0)
        return ETrue;
    if (aType != EEventKeyDown && aType != EEventKey)
        return ETrue;
    if (aKey.iScanCode == 0 && aKey.iCode == 0)
        return ETrue;

    const TUint32 playerKey =
        EasyRpgConfigurablePlayerKeyAt(iCaptureControlIndex);
    if (playerKey == 0)
    {
        iCaptureControlIndex = -1;
        return ETrue;
    }

    EasyRpgSetCustomPlayerKeyBinding(playerKey, aKey.iScanCode, aKey.iCode);
    SaveSettings();
    iCaptureControlIndex = -1;
    iCapturedScanCode = aType == EEventKeyDown ? aKey.iScanCode : 0;
    iHeldMenuKeys = 0;
    iHost.GameMenuRequestRedraw();
    return ETrue;
}

void CGameMenu::ControlItemText(TInt aIndex, TDes8& aText) const
{
    aText.Zero();
    const TInt keyCount = EasyRpgConfigurablePlayerKeyCount();
    if (aIndex == keyCount)
    {
        aText.Copy(_L8("RESET DEFAULTS"));
        return;
    }
    if (aIndex < 0 || aIndex >= keyCount)
        return;

    const TUint32 playerKey = EasyRpgConfigurablePlayerKeyAt(aIndex);
    TBuf8<24> actionName;
    EasyRpgPlayerKeyName(playerKey, actionName);
    aText.Copy(actionName.Left(aText.MaxLength()));
    if (aText.Length() + 2 <= aText.MaxLength())
        aText.Append(_L8(": "));

    if (aIndex == iCaptureControlIndex)
    {
        _LIT8(KPressKey, "PRESS KEY");
        if (aText.Length() + KPressKey().Length() <= aText.MaxLength())
            aText.Append(KPressKey);
        return;
    }

    TBuf8<24> bindingName;
    EasyRpgPlayerKeyBindingName(playerKey, bindingName);
    const TInt remaining = aText.MaxLength() - aText.Length();
    if (remaining > 0)
        aText.Append(bindingName.Left(remaining));
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
    case EItemFps:
        iFpsEnabled = !iFpsEnabled;
        iHost.GameMenuSetFpsEnabledL(iFpsEnabled);
        SaveSettings();
        break;
    case EItemHelp:
        iHelpVisible = ETrue;
        break;
    case EItemControls:
        iControlsVisible = ETrue;
        iCaptureControlIndex = -1;
        iCapturedScanCode = 0;
        NormalizeControlSelection();
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
    iVolume = TNativeAudio::ClampVolume(iVolume + aDelta * KVolumeStep);
    iHost.GameMenuSetVolumeL(iVolume);
    SaveSettings();
    iHost.GameMenuRequestRedraw();
}

TInt CGameMenu::MapMenuKey(const TKeyEvent& aKey) const
{
    // Menu navigation is deliberately independent from gameplay remapping.
    // This guarantees a way to navigate/undo mappings even after a bad bind.
    const TInt scanCode = aKey.iScanCode >= 'a' && aKey.iScanCode <= 'z'
        ? aKey.iScanCode - 'a' + 'A'
        : aKey.iScanCode;
    TInt charCode = aKey.iCode;
    if (charCode >= 'A' && charCode <= 'Z')
        charCode = charCode - 'A' + 'a';

    if (scanCode == EStdKeyUpArrow || scanCode == 'W' || charCode == 'w')
        return EMenuKeyUp;
    if (scanCode == EStdKeyDownArrow || scanCode == 'S' || charCode == 's')
        return EMenuKeyDown;
    if (scanCode == EStdKeyLeftArrow || scanCode == 'A' || charCode == 'a')
        return EMenuKeyLeft;
    if (scanCode == EStdKeyRightArrow || scanCode == 'D' || charCode == 'd')
        return EMenuKeyRight;
    if (scanCode == EStdKeyEnter || scanCode == EStdKeyDevice3 ||
        scanCode == EStdKeySpace || scanCode == 'Z' ||
        charCode == EKeyEnter || charCode == EKeyDevice3 ||
        charCode == ' ' || charCode == 'z')
        return EMenuKeyDecision;
    if (scanCode == EStdKeyEscape || charCode == EKeyEscape)
        return EMenuKeyCancel;
    return EMenuKeyNone;
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
    if (iHelpVisible)
    {
        iHelpVisible = EFalse;
        iHost.GameMenuRequestRedraw();
        return ETrue;
    }
    if (iControlsVisible)
    {
        if (iCaptureControlIndex >= 0)
        {
            // Softkeys stay reserved for menu control and are never captured
            // as gameplay bindings. Back cancels the pending capture.
            if (!aLeft)
            {
                iCaptureControlIndex = -1;
                iCapturedScanCode = 0;
                iHost.GameMenuRequestRedraw();
            }
            return ETrue;
        }
        if (aLeft)
            ActivateControlSelectionL();
        else
        {
            iControlsVisible = EFalse;
            iHeldMenuKeys = 0;
            iHost.GameMenuRequestRedraw();
        }
        return ETrue;
    }
    if (aLeft)
        ActivateSelectionL();
    else
    {
        // A Symbian soft key normally arrives as KeyDown -> Key -> KeyUp.
        // Closing on KeyDown must capture the rest of that physical press,
        // otherwise the following Key event is delivered to the game as
        // CANCEL after GameMenuPauseL(EFalse) clears the runtime key state.
        iRightSoftCaptured = aType == EEventKeyDown;
        CloseL();
    }
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
        {
            if (iRightSoftCaptured)
            {
                if (aType == EEventKey || aType == EEventKeyUp)
                {
                    iRightSoftCaptured = EFalse;
                    iRightSoftHeld = EFalse;
                }
                return ETrue;
            }

            // Keep the menu's soft-key latch synchronized even while the
            // menu is closed. Without this, the KeyUp after closing is lost
            // to the menu and the next Back press appears to be already held.
            if (aType == EEventKeyDown)
                iRightSoftHeld = ETrue;
            else if (aType == EEventKey || aType == EEventKeyUp)
                iRightSoftHeld = EFalse;
            return EFalse;
        }
        return HandleSoftKeyL(EFalse, aType, aKey);
    }
    if (!iOpen)
        return EFalse;

    if (iControlsVisible)
    {
        if (HandleControlCaptureL(aType, aKey))
            return ETrue;

        const TInt controlKey = MapMenuKey(aKey);
        if (controlKey == EMenuKeyNone)
            return ETrue;
        const TBool controlTrigger = TriggerMenuKey(controlKey, aType, aKey);
        if (!controlTrigger)
            return ETrue;

        switch (controlKey)
        {
        case EMenuKeyUp:
            MoveControlSelection(-1);
            break;
        case EMenuKeyDown:
            MoveControlSelection(1);
            break;
        case EMenuKeyDecision:
            ActivateControlSelectionL();
            break;
        case EMenuKeyCancel:
            iControlsVisible = EFalse;
            iHeldMenuKeys = 0;
            iHost.GameMenuRequestRedraw();
            break;
        default:
            break;
        }
        return ETrue;
    }

    const TInt menuKey = MapMenuKey(aKey);
    if (menuKey == EMenuKeyNone)
        return ETrue;
    const TBool trigger = TriggerMenuKey(menuKey, aType, aKey);
    if (!trigger)
        return ETrue;

    if (iHelpVisible)
    {
        if (menuKey == EMenuKeyDecision || menuKey == EMenuKeyCancel)
        {
            iHelpVisible = EFalse;
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
    if (iHelpVisible)
        DrawHelpL(aGc, aDevice);
    else if (iControlsVisible)
        DrawControlsL(aGc, aDevice);
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
    const TInt rowHeight = KMenuTextHeight + 4;
    const TInt footerHeight = KMenuTextHeight + 7;
    const TInt panelWidth = Min(148, Max(1, screenSize.iWidth - 12));
    const TInt wantedHeight = itemCount * rowHeight + footerHeight + 7;
    const TInt panelHeight = Min(wantedHeight, Max(1, screenSize.iHeight - 12));
    const TPoint origin(0, screenSize.iHeight - panelHeight);
    const TRect panel(origin, TSize(panelWidth, panelHeight));

    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenColor(TRgb(255, 255, 255));
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(TRgb(0, 0, 0));
    aGc.DrawRect(panel);

    const TInt rowX = panel.iTl.iX + 5;
    const TInt rowWidth = panelWidth - 10;
    const TInt firstY = panel.iTl.iY + 4;
    const TInt visibleRows = Max(1, (panelHeight - footerHeight - 7) / rowHeight);
    TInt firstItem = 0;
    if (iSelection >= visibleRows)
        firstItem = iSelection - visibleRows + 1;
    if (firstItem + visibleRows > itemCount)
        firstItem = Max(0, itemCount - visibleRows);

    for (TInt row = 0; row < visibleRows && firstItem + row < itemCount; ++row)
    {
        const TInt item = firstItem + row;
        const TInt textY = firstY + row * rowHeight + 2;
        TBuf8<64> text8;
        ItemText(item, text8);
        if (item == iSelection)
        {
            aGc.SetPenStyle(CGraphicsContext::ENullPen);
            aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
            aGc.SetBrushColor(TRgb(255, 255, 255));
            aGc.DrawRect(TRect(TPoint(rowX, firstY + row * rowHeight),
                               TSize(rowWidth, rowHeight - 1)));
            DrawGohuText(aGc, text8, TPoint(rowX + 4, textY), TRgb(0, 0, 0));
        }
        else
        {
            DrawGohuText(aGc, text8, TPoint(rowX + 4, textY), TRgb(255, 255, 255));
        }
    }

    _LIT8(KSelect, "SELECT");
    _LIT8(KBack, "BACK");
    const TInt footerY = panel.iBr.iY - KMenuTextHeight - 4;
    DrawGohuText(aGc, KSelect, TPoint(panel.iTl.iX + 7, footerY), TRgb(255, 255, 255));
    DrawGohuText(aGc,
                 KBack,
                 TPoint(panel.iBr.iX - GohuTextWidth(KBack) - 7, footerY),
                 TRgb(255, 255, 255));
}

void CGameMenu::DrawHelpL(CGraphicsContext& aGc, CBitmapDevice& aDevice)
{
    const TSize screenSize = aDevice.SizeInPixels();
    const TInt panelWidth = Min(292, Max(1, screenSize.iWidth - 12));
    const TInt panelHeight = Min(210, Max(1, screenSize.iHeight - 12));
    const TRect panel(TPoint((screenSize.iWidth - panelWidth) / 2,
                             (screenSize.iHeight - panelHeight) / 2),
                      TSize(panelWidth, panelHeight));

    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenColor(TRgb(255, 255, 255));
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(TRgb(0, 0, 0));
    aGc.DrawRect(panel);

    TInt x = panel.iTl.iX + 10;
    TInt y = panel.iTl.iY + 8;
    const TInt step = KMenuTextHeight + 4;
    _LIT8(KHelp, "HELP");
    _LIT8(KUp, "UP / W        UP");
    _LIT8(KDown, "DOWN / S      DOWN");
    _LIT8(KLeft, "LEFT / A      LEFT");
    _LIT8(KRight, "RIGHT / D     RIGHT");
    _LIT8(KPrimary, "CENTER / Z    PRIMARY");
    _LIT8(KBack, "R SOFT / ESC  BACK");
    _LIT8(KClose, "CENTER/BACK TO CLOSE");

    DrawGohuText(aGc, KHelp, TPoint(x, y), TRgb(255, 255, 255));
    y += step;
    DrawGohuText(aGc, KUp, TPoint(x, y), TRgb(255, 255, 255));
    y += step;
    DrawGohuText(aGc, KDown, TPoint(x, y), TRgb(255, 255, 255));
    y += step;
    DrawGohuText(aGc, KLeft, TPoint(x, y), TRgb(255, 255, 255));
    y += step;
    DrawGohuText(aGc, KRight, TPoint(x, y), TRgb(255, 255, 255));
    y += step;
    DrawGohuText(aGc, KPrimary, TPoint(x, y), TRgb(255, 255, 255));
    y += step;
    DrawGohuText(aGc, KBack, TPoint(x, y), TRgb(255, 255, 255));
    DrawGohuText(aGc,
                 KClose,
                 TPoint(x, panel.iBr.iY - KMenuTextHeight - 7),
                 TRgb(255, 255, 255));
}

void CGameMenu::DrawControlsL(CGraphicsContext& aGc, CBitmapDevice& aDevice)
{
    NormalizeControlSelection();
    const TInt itemCount = EasyRpgConfigurablePlayerKeyCount() + 1;
    if (itemCount <= 0)
        return;

    const TSize screenSize = aDevice.SizeInPixels();
    const TInt rowHeight = KMenuTextHeight + 4;
    const TInt titleHeight = KMenuTextHeight + 8;
    const TInt footerHeight = KMenuTextHeight + 8;
    const TInt panelWidth = Min(292, Max(1, screenSize.iWidth - 12));
    const TInt panelHeight = Min(220, Max(1, screenSize.iHeight - 12));
    const TRect panel(TPoint((screenSize.iWidth - panelWidth) / 2,
                             (screenSize.iHeight - panelHeight) / 2),
                      TSize(panelWidth, panelHeight));

    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenColor(TRgb(255, 255, 255));
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(TRgb(0, 0, 0));
    aGc.DrawRect(panel);

    const TInt rowX = panel.iTl.iX + 7;
    const TInt rowWidth = panelWidth - 14;
    const TInt titleY = panel.iTl.iY + 7;
    _LIT8(KControls, "CONTROLS");
    DrawGohuText(aGc, KControls, TPoint(rowX + 3, titleY), TRgb(255, 255, 255));

    const TInt firstY = panel.iTl.iY + titleHeight + 5;
    const TInt usableRowsHeight = panelHeight - titleHeight - footerHeight - 10;
    const TInt visibleRows = Max(1, usableRowsHeight / rowHeight);
    TInt firstItem = 0;
    if (iControlSelection >= visibleRows)
        firstItem = iControlSelection - visibleRows + 1;
    if (firstItem + visibleRows > itemCount)
        firstItem = Max(0, itemCount - visibleRows);

    for (TInt row = 0; row < visibleRows && firstItem + row < itemCount; ++row)
    {
        const TInt item = firstItem + row;
        const TInt rowY = firstY + row * rowHeight;
        TBuf8<64> text;
        ControlItemText(item, text);
        if (item == iControlSelection)
        {
            aGc.SetPenStyle(CGraphicsContext::ENullPen);
            aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
            aGc.SetBrushColor(TRgb(255, 255, 255));
            aGc.DrawRect(TRect(TPoint(rowX, rowY), TSize(rowWidth, rowHeight - 1)));
            DrawGohuText(aGc, text, TPoint(rowX + 4, rowY + 2), TRgb(0, 0, 0));
        }
        else
        {
            DrawGohuText(aGc, text, TPoint(rowX + 4, rowY + 2), TRgb(255, 255, 255));
        }
    }

    const TInt footerY = panel.iBr.iY - KMenuTextHeight - 5;
    if (iCaptureControlIndex >= 0)
    {
        _LIT8(KPressKey, "PRESS KEY");
        _LIT8(KCancel, "CANCEL");
        DrawGohuText(aGc, KPressKey, TPoint(panel.iTl.iX + 8, footerY), TRgb(255, 255, 255));
        DrawGohuText(aGc,
                     KCancel,
                     TPoint(panel.iBr.iX - GohuTextWidth(KCancel) - 8, footerY),
                     TRgb(255, 255, 255));
    }
    else
    {
        _LIT8(KSelect, "SELECT");
        _LIT8(KBack, "BACK");
        DrawGohuText(aGc, KSelect, TPoint(panel.iTl.iX + 8, footerY), TRgb(255, 255, 255));
        DrawGohuText(aGc,
                     KBack,
                     TPoint(panel.iBr.iX - GohuTextWidth(KBack) - 8, footerY),
                     TRgb(255, 255, 255));
    }
}

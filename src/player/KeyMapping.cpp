#include <e32keys.h>

#include "KeyMapping.h"
#include "PlayerRuntime.h"

namespace
{
struct TKeyMapEntry
{
    TInt iScanCode;
    TInt iCharCode;
    TUint32 iPlayerKey;
};

struct TCustomBinding
{
    TUint32 iPlayerKey;
    TInt iScanCode;
    TInt iCharCode;
    TBool iActive;
};

const TUint32 KConfigurableKeys[] =
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
    CPlayerRuntime::EKeyPeriod
};

const TInt KConfigurableKeyCount =
    sizeof(KConfigurableKeys) / sizeof(KConfigurableKeys[0]);

TCustomBinding KCustomBindings[KConfigurableKeyCount];

// Match the desktop EasyRPG defaults where they make sense on the E72.
// Scan codes are preferred because games care about physical keyboard keys;
// char codes are a fallback for devices/drivers that do not preserve them on
// all event types.
const TKeyMapEntry KDefaultKeyMap[] =
{
    { EStdKeyUpArrow, EKeyUpArrow, CPlayerRuntime::EKeyUp },
    { 'W', 'w', CPlayerRuntime::EKeyUp },
    { 'K', 'k', CPlayerRuntime::EKeyUp },
    { EStdKeyDownArrow, EKeyDownArrow, CPlayerRuntime::EKeyDown },
    { 'S', 's', CPlayerRuntime::EKeyDown },
    { 'J', 'j', CPlayerRuntime::EKeyDown },
    { EStdKeyLeftArrow, EKeyLeftArrow, CPlayerRuntime::EKeyLeft },
    { 'A', 'a', CPlayerRuntime::EKeyLeft },
    { 'H', 'h', CPlayerRuntime::EKeyLeft },
    { EStdKeyRightArrow, EKeyRightArrow, CPlayerRuntime::EKeyRight },
    { 'D', 'd', CPlayerRuntime::EKeyRight },
    { 'L', 'l', CPlayerRuntime::EKeyRight },
    { EStdKeyEnter, EKeyEnter, CPlayerRuntime::EKeyDecision },
    { EStdKeyDevice3, EKeyDevice3, CPlayerRuntime::EKeyDecision },
    { EStdKeySpace, ' ', CPlayerRuntime::EKeyDecision },
    { 'Z', 'z', CPlayerRuntime::EKeyDecision },
    { 'Y', 'y', CPlayerRuntime::EKeyDecision },
    { EStdKeyEscape, EKeyEscape, CPlayerRuntime::EKeyCancel },
    { 'X', 'x', CPlayerRuntime::EKeyCancel },
    { 'C', 'c', CPlayerRuntime::EKeyCancel },
    { 'V', 'v', CPlayerRuntime::EKeyCancel },
    { 'B', 'b', CPlayerRuntime::EKeyCancel },
    { 'N', 'n', CPlayerRuntime::EKeyCancel },
    { EStdKeyLeftShift, EKeyLeftShift, CPlayerRuntime::EKeyShift },
    { EStdKeyRightShift, EKeyRightShift, CPlayerRuntime::EKeyShift },
    { '0', '0', CPlayerRuntime::EKeyN0 },
    { '1', '1', CPlayerRuntime::EKeyN1 },
    { '2', '2', CPlayerRuntime::EKeyN2 },
    { '3', '3', CPlayerRuntime::EKeyN3 },
    { '4', '4', CPlayerRuntime::EKeyN4 },
    { '5', '5', CPlayerRuntime::EKeyN5 },
    { '6', '6', CPlayerRuntime::EKeyN6 },
    { '7', '7', CPlayerRuntime::EKeyN7 },
    { '8', '8', CPlayerRuntime::EKeyN8 },
    { '9', '9', CPlayerRuntime::EKeyN9 },
    { EStdKeyNkpPlus, '+', CPlayerRuntime::EKeyPlus },
    { EStdKeyMinus, '-', CPlayerRuntime::EKeyMinus },
    { EStdKeyNkpMinus, 0, CPlayerRuntime::EKeyMinus },
    { EStdKeyNkpAsterisk, '*', CPlayerRuntime::EKeyMultiply },
    { EStdKeyForwardSlash, '/', CPlayerRuntime::EKeyDivide },
    { EStdKeyNkpForwardSlash, 0, CPlayerRuntime::EKeyDivide },
    { EStdKeyFullStop, '.', CPlayerRuntime::EKeyPeriod },
    { EStdKeyNkpFullStop, 0, CPlayerRuntime::EKeyPeriod }
};

const TInt KDefaultKeyMapCount = sizeof(KDefaultKeyMap) / sizeof(KDefaultKeyMap[0]);

TInt NormalizeScanCode(TInt aCode)
{
    if (aCode >= 'a' && aCode <= 'z')
        return aCode - 'a' + 'A';
    return aCode;
}

TInt NormalizeCharCode(TInt aCode)
{
    if (aCode >= 'A' && aCode <= 'Z')
        return aCode - 'A' + 'a';
    return aCode;
}

TInt ConfigurableIndexForKey(TUint32 aPlayerKey)
{
    for (TInt index = 0; index < KConfigurableKeyCount; ++index)
        if (KConfigurableKeys[index] == aPlayerKey)
            return index;
    return KErrNotFound;
}

TBool PhysicalBindingMatches(const TCustomBinding& aBinding,
                             TInt aScanCode,
                             TInt aCharCode)
{
    if (!aBinding.iActive)
        return EFalse;
    if (aScanCode != 0 && aBinding.iScanCode != 0 &&
        aBinding.iScanCode == aScanCode)
        return ETrue;
    return aCharCode != 0 && aBinding.iCharCode != 0 &&
        aBinding.iCharCode == aCharCode;
}

TUint32 CustomPlayerKeyForPhysical(TInt aScanCode, TInt aCharCode)
{
    for (TInt index = 0; index < KConfigurableKeyCount; ++index)
        if (PhysicalBindingMatches(KCustomBindings[index], aScanCode, aCharCode))
            return KCustomBindings[index].iPlayerKey;
    return 0;
}

TUint32 DefaultPlayerKeyForPhysical(TInt aScanCode, TInt aCharCode)
{
    if (aScanCode != 0)
    {
        for (TInt index = 0; index < KDefaultKeyMapCount; ++index)
        {
            const TKeyMapEntry& entry = KDefaultKeyMap[index];
            if (NormalizeScanCode(entry.iScanCode) == aScanCode &&
                !EasyRpgHasCustomPlayerKeyBinding(entry.iPlayerKey))
                return entry.iPlayerKey;
        }
    }
    if (aCharCode != 0)
    {
        for (TInt index = 0; index < KDefaultKeyMapCount; ++index)
        {
            const TKeyMapEntry& entry = KDefaultKeyMap[index];
            if (NormalizeCharCode(entry.iCharCode) == aCharCode &&
                !EasyRpgHasCustomPlayerKeyBinding(entry.iPlayerKey))
                return entry.iPlayerKey;
        }
    }
    return 0;
}

void CopyPlayerKeyName(TUint32 aPlayerKey, TDes8& aName, TBool aSettingName)
{
    aName.Zero();
    switch (aPlayerKey)
    {
    case CPlayerRuntime::EKeyUp:       aName.Copy(aSettingName ? _L8("key_up") : _L8("UP")); break;
    case CPlayerRuntime::EKeyDown:     aName.Copy(aSettingName ? _L8("key_down") : _L8("DOWN")); break;
    case CPlayerRuntime::EKeyLeft:     aName.Copy(aSettingName ? _L8("key_left") : _L8("LEFT")); break;
    case CPlayerRuntime::EKeyRight:    aName.Copy(aSettingName ? _L8("key_right") : _L8("RIGHT")); break;
    case CPlayerRuntime::EKeyDecision: aName.Copy(aSettingName ? _L8("key_primary") : _L8("PRIMARY")); break;
    case CPlayerRuntime::EKeyCancel:   aName.Copy(aSettingName ? _L8("key_back") : _L8("BACK")); break;
    case CPlayerRuntime::EKeyShift:    aName.Copy(aSettingName ? _L8("key_shift") : _L8("SHIFT")); break;
    case CPlayerRuntime::EKeyN0:       aName.Copy(aSettingName ? _L8("key_0") : _L8("0")); break;
    case CPlayerRuntime::EKeyN1:       aName.Copy(aSettingName ? _L8("key_1") : _L8("1")); break;
    case CPlayerRuntime::EKeyN2:       aName.Copy(aSettingName ? _L8("key_2") : _L8("2")); break;
    case CPlayerRuntime::EKeyN3:       aName.Copy(aSettingName ? _L8("key_3") : _L8("3")); break;
    case CPlayerRuntime::EKeyN4:       aName.Copy(aSettingName ? _L8("key_4") : _L8("4")); break;
    case CPlayerRuntime::EKeyN5:       aName.Copy(aSettingName ? _L8("key_5") : _L8("5")); break;
    case CPlayerRuntime::EKeyN6:       aName.Copy(aSettingName ? _L8("key_6") : _L8("6")); break;
    case CPlayerRuntime::EKeyN7:       aName.Copy(aSettingName ? _L8("key_7") : _L8("7")); break;
    case CPlayerRuntime::EKeyN8:       aName.Copy(aSettingName ? _L8("key_8") : _L8("8")); break;
    case CPlayerRuntime::EKeyN9:       aName.Copy(aSettingName ? _L8("key_9") : _L8("9")); break;
    case CPlayerRuntime::EKeyPlus:     aName.Copy(aSettingName ? _L8("key_plus") : _L8("PLUS")); break;
    case CPlayerRuntime::EKeyMinus:    aName.Copy(aSettingName ? _L8("key_minus") : _L8("MINUS")); break;
    case CPlayerRuntime::EKeyMultiply: aName.Copy(aSettingName ? _L8("key_multiply") : _L8("MULTIPLY")); break;
    case CPlayerRuntime::EKeyDivide:   aName.Copy(aSettingName ? _L8("key_divide") : _L8("DIVIDE")); break;
    case CPlayerRuntime::EKeyPeriod:   aName.Copy(aSettingName ? _L8("key_period") : _L8("PERIOD")); break;
    default: break;
    }
}

void PhysicalKeyName(TInt aScanCode, TInt aCharCode, TDes8& aName)
{
    aName.Zero();
    const TInt scanCode = NormalizeScanCode(aScanCode);
    const TInt charCode = NormalizeCharCode(aCharCode);
    switch (scanCode)
    {
    case EStdKeyUpArrow: aName.Copy(_L8("UP")); return;
    case EStdKeyDownArrow: aName.Copy(_L8("DOWN")); return;
    case EStdKeyLeftArrow: aName.Copy(_L8("LEFT")); return;
    case EStdKeyRightArrow: aName.Copy(_L8("RIGHT")); return;
    case EStdKeyEnter: aName.Copy(_L8("ENTER")); return;
    case EStdKeyDevice3: aName.Copy(_L8("CENTER")); return;
    case EStdKeySpace: aName.Copy(_L8("SPACE")); return;
    case EStdKeyEscape: aName.Copy(_L8("ESC")); return;
    case EStdKeyLeftShift: aName.Copy(_L8("LSHIFT")); return;
    case EStdKeyRightShift: aName.Copy(_L8("RSHIFT")); return;
    case EStdKeyNkpPlus: aName.Copy(_L8("+")); return;
    case EStdKeyMinus:
    case EStdKeyNkpMinus: aName.Copy(_L8("-")); return;
    case EStdKeyNkpAsterisk: aName.Copy(_L8("*")); return;
    case EStdKeyForwardSlash:
    case EStdKeyNkpForwardSlash: aName.Copy(_L8("/")); return;
    case EStdKeyFullStop:
    case EStdKeyNkpFullStop: aName.Copy(_L8(".")); return;
    default: break;
    }

    if (scanCode >= 'A' && scanCode <= 'Z')
    {
        aName.Append(static_cast<TUint8>(scanCode));
        return;
    }
    if (scanCode >= '0' && scanCode <= '9')
    {
        aName.Append(static_cast<TUint8>(scanCode));
        return;
    }
    if (charCode >= 32 && charCode < 127)
    {
        TInt display = charCode;
        if (display >= 'a' && display <= 'z')
            display = display - 'a' + 'A';
        aName.Append(static_cast<TUint8>(display));
        return;
    }

    aName.Format(_L8("SCAN %d"), scanCode);
}
} // namespace

TUint32 EasyRpgPlayerKeyForEvent(const TKeyEvent& aKey)
{
    const TInt scanCode = NormalizeScanCode(aKey.iScanCode);
    const TInt charCode = NormalizeCharCode(aKey.iCode);
    const TUint32 custom = CustomPlayerKeyForPhysical(scanCode, charCode);
    return custom != 0 ? custom : DefaultPlayerKeyForPhysical(scanCode, charCode);
}

void EasyRpgResetCustomPlayerKeyBindings()
{
    for (TInt index = 0; index < KConfigurableKeyCount; ++index)
    {
        KCustomBindings[index].iPlayerKey = KConfigurableKeys[index];
        KCustomBindings[index].iScanCode = 0;
        KCustomBindings[index].iCharCode = 0;
        KCustomBindings[index].iActive = EFalse;
    }
}

TBool EasyRpgHasCustomPlayerKeyBinding(TUint32 aPlayerKey)
{
    const TInt index = ConfigurableIndexForKey(aPlayerKey);
    return index >= 0 && KCustomBindings[index].iActive;
}

TBool EasyRpgGetCustomPlayerKeyBinding(TUint32 aPlayerKey,
                                       TInt& aScanCode,
                                       TInt& aCharCode)
{
    aScanCode = 0;
    aCharCode = 0;
    const TInt index = ConfigurableIndexForKey(aPlayerKey);
    if (index < 0 || !KCustomBindings[index].iActive)
        return EFalse;
    aScanCode = KCustomBindings[index].iScanCode;
    aCharCode = KCustomBindings[index].iCharCode;
    return ETrue;
}

void EasyRpgSetCustomPlayerKeyBinding(TUint32 aPlayerKey,
                                      TInt aScanCode,
                                      TInt aCharCode)
{
    const TInt target = ConfigurableIndexForKey(aPlayerKey);
    if (target < 0)
        return;

    const TInt scanCode = NormalizeScanCode(aScanCode);
    const TInt charCode = NormalizeCharCode(aCharCode);
    if (scanCode == 0 && charCode == 0)
        return;

    // A physical key belongs to at most one custom action. This prevents a
    // remap from silently activating two EasyRPG actions at once.
    for (TInt index = 0; index < KConfigurableKeyCount; ++index)
    {
        if (index == target || !KCustomBindings[index].iActive)
            continue;
        if (PhysicalBindingMatches(KCustomBindings[index], scanCode, charCode))
        {
            KCustomBindings[index].iScanCode = 0;
            KCustomBindings[index].iCharCode = 0;
            KCustomBindings[index].iActive = EFalse;
        }
    }

    KCustomBindings[target].iPlayerKey = aPlayerKey;
    KCustomBindings[target].iScanCode = scanCode;
    KCustomBindings[target].iCharCode = charCode;
    KCustomBindings[target].iActive = ETrue;
}

TInt EasyRpgConfigurablePlayerKeyCount()
{
    return KConfigurableKeyCount;
}

TUint32 EasyRpgConfigurablePlayerKeyAt(TInt aIndex)
{
    return aIndex >= 0 && aIndex < KConfigurableKeyCount
        ? KConfigurableKeys[aIndex]
        : 0;
}

void EasyRpgPlayerKeyName(TUint32 aPlayerKey, TDes8& aName)
{
    CopyPlayerKeyName(aPlayerKey, aName, EFalse);
}

void EasyRpgPlayerKeySettingName(TUint32 aPlayerKey, TDes8& aName)
{
    CopyPlayerKeyName(aPlayerKey, aName, ETrue);
}

void EasyRpgPlayerKeyBindingName(TUint32 aPlayerKey, TDes8& aName)
{
    TInt scanCode = 0;
    TInt charCode = 0;
    if (!EasyRpgGetCustomPlayerKeyBinding(aPlayerKey, scanCode, charCode))
    {
        aName.Copy(_L8("DEFAULT"));
        return;
    }
    PhysicalKeyName(scanCode, charCode, aName);
}

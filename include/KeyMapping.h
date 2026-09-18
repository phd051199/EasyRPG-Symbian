#ifndef EASYRPG_KEY_MAPPING_H
#define EASYRPG_KEY_MAPPING_H

#include <e32base.h>
#include <w32std.h>

// Resolve one physical Symbian key event to the EasyRPG player-key bit used
// by CPlayerRuntime. Custom bindings take priority over defaults.
TUint32 EasyRpgPlayerKeyForEvent(const TKeyEvent &aKey);

// Custom bindings are process-local; CGameMenu loads/saves them through the
// existing settings.ini file during player startup and menu changes.
void EasyRpgResetCustomPlayerKeyBindings();
TBool EasyRpgHasCustomPlayerKeyBinding(TUint32 aPlayerKey);
TBool EasyRpgGetCustomPlayerKeyBinding(TUint32 aPlayerKey,
                                       TInt &aScanCode,
                                       TInt &aCharCode);
void EasyRpgSetCustomPlayerKeyBinding(TUint32 aPlayerKey,
                                      TInt aScanCode,
                                      TInt aCharCode);

TInt EasyRpgConfigurablePlayerKeyCount();
TUint32 EasyRpgConfigurablePlayerKeyAt(TInt aIndex);
void EasyRpgPlayerKeyName(TUint32 aPlayerKey, TDes8 &aName);
void EasyRpgPlayerKeySettingName(TUint32 aPlayerKey, TDes8 &aName);
void EasyRpgPlayerKeyBindingName(TUint32 aPlayerKey, TDes8 &aName);

#endif

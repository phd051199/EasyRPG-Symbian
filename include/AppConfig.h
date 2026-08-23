#ifndef EASYRPG_APP_CONFIG_H
#define EASYRPG_APP_CONFIG_H

#include <e32std.h>

namespace EasyRpgApp
{
// 0xE31EA5A1 must stay in sync with _UID3 in include/App.hrh,
// target/group/Launcher.mmp and target/package/App.pkg (Symbian requires the UID
// literally in each of those; no shared include reaches all of them).
const TUid KLauncherUid = { 0xE31EA5A1 };
_LIT(KLauncherExecutable, "EasyRPG.exe");
_LIT(KPlayerExecutable, "EasyRPGPlayer.exe");
_LIT(KApplicationCaption, "EasyRPG");

_LIT(KGamesDirectorySuffix, "\\Data\\EasyRPG\\Games\\");
_LIT(KMemoryCardGamesDirectory, "E:\\EasyRPG\\Games\\");
_LIT(KSettingsDirectorySuffix, "\\Data\\EasyRPG\\");
_LIT(KSettingsFileSuffix, "\\Data\\EasyRPG\\settings.ini");
_LIT(KMovieDirectorySuffix, "\\Data\\EasyRPG\\Movie\\");
_LIT(KAudioDirectorySuffix, "\\Data\\EasyRPG\\Audio\\");
_LIT(KVirtualProjectDirectorySuffix, "\\Data\\EasyRPG\\Vfs\\");
_LIT(KSaveDirectorySuffix, "\\Data\\EasyRPG\\Saves\\");
}

#endif

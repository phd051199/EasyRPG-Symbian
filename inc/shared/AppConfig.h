#ifndef EASYRPG_APP_CONFIG_H
#define EASYRPG_APP_CONFIG_H

#include <e32std.h>

namespace EasyRpgApp
{
const TUid KLauncherUid = { 0xE31EA5A1 };
_LIT(KLauncherExecutable, "EasyRPG.exe");
_LIT(KPlayerExecutable, "EasyRPGPlayer.exe");

_LIT(KGamesDirectorySuffix, "\\Data\\EasyRPG\\Games\\");
_LIT(KMemoryCardGamesDirectory, "E:\\EasyRPG\\Games\\");
_LIT(KSettingsDirectorySuffix, "\\Data\\EasyRPG\\");
_LIT(KSettingsFileSuffix, "\\Data\\EasyRPG\\settings.ini");
_LIT(KMovieDirectorySuffix, "\\Data\\EasyRPG\\Movie\\");
_LIT(KVirtualProjectDirectorySuffix, "\\Data\\EasyRPG\\Vfs\\");
_LIT(KSaveDirectorySuffix, "\\Data\\EasyRPG\\Saves\\");
}

#endif

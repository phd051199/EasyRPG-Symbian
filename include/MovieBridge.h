#ifndef EASYRPG_MOVIE_BRIDGE_H
#define EASYRPG_MOVIE_BRIDGE_H

#include <e32std.h>
#include "CoreMovie.h"

class RWsSession;
class CWsScreenDevice;
class RWindowBase;

void EasyRpgMovieInitializeL(RWsSession &aWs,
                             CWsScreenDevice &aScreenDevice,
                             RWindowBase &aWindow);
void EasyRpgMovieShutdown(void);
TBool EasyRpgMovieIsActive(void);
void EasyRpgMovieSetForeground(TBool aForeground);
TBool EasyRpgMovieConsumeRedraw(void);

#endif

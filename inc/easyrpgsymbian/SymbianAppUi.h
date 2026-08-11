#ifndef EASYRPG_SYMBIAN_APP_UI_H
#define EASYRPG_SYMBIAN_APP_UI_H

#include <aknappui.h>

class CGameView;

class CEasyRpgAppUi : public CAknAppUi
{
public:
    CEasyRpgAppUi();
    ~CEasyRpgAppUi();

    void ConstructL();
    void EnterGameFullscreen();
    void ExitGameFullscreen();
    void HandleCommandL(TInt aCommand);
    void HandleStatusPaneSizeChange();
    void HandleForegroundEventL(TBool aForeground);

private:
    CGameView* iGameView;
};

#endif

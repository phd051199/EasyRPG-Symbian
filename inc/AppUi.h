#ifndef EASYRPG_APP_UI_H
#define EASYRPG_APP_UI_H

#include <aknappui.h>

class CGameView;
class CCoeControl;
class CEikMenuPane;
class TWsEvent;

class CEasyRpgAppUi : public CAknAppUi
{
public:
    CEasyRpgAppUi();
    ~CEasyRpgAppUi();

    void ConstructL();
    void EnterGameFullscreen();
    void ExitGameFullscreen();
    void HandleCommandL(TInt aCommand);
    void HandleWsEventL(const TWsEvent& aEvent, CCoeControl* aDestination);
    void HandleStatusPaneSizeChange();
    void HandleForegroundEventL(TBool aForeground);

private:
    CGameView* iGameView;
};

#endif

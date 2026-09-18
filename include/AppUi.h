#ifndef EASYRPG_APP_UI_H
#define EASYRPG_APP_UI_H

#include <aknappui.h>

class CLauncherView;

class CEasyRpgAppUi : public CAknAppUi
{
  public:
    CEasyRpgAppUi();
    ~CEasyRpgAppUi();

    void ConstructL();
    void HandleCommandL(TInt aCommand);
    void HandleStatusPaneSizeChange();

  private:
    CLauncherView *iLauncherView;
};

#endif

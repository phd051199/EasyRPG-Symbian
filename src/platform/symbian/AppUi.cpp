#include <aknnotewrappers.h>
#include <avkon.hrh>
#include <eikbtgpc.h>
#include <eikspane.h>

#include "App.hrh"
#include "AppUi.h"
#include "LauncherView.h"

CEasyRpgAppUi::CEasyRpgAppUi()
    : iLauncherView(NULL)
{
}

void CEasyRpgAppUi::ConstructL()
{
    BaseConstructL(CAknAppUi::EAknEnableSkin);
    SetFullScreenApp(EFalse);
    if (StatusPane())
        StatusPane()->MakeVisible(ETrue);
    if (Cba())
        Cba()->MakeVisible(ETrue);

    iLauncherView = CLauncherView::NewL(ClientRect());
    AddToStackL(iLauncherView);
}

CEasyRpgAppUi::~CEasyRpgAppUi()
{
    if (iLauncherView)
    {
        RemoveFromStack(iLauncherView);
        delete iLauncherView;
        iLauncherView = NULL;
    }
}

void CEasyRpgAppUi::HandleCommandL(TInt aCommand)
{
    switch (aCommand)
    {
    case EEasyRpgCmdLauncherPlay:
        if (iLauncherView)
            iLauncherView->PlaySelectedGameL();
        break;
    case EEasyRpgCmdLauncherAbout:
        {
            CAknInformationNote* note = new (ELeave) CAknInformationNote;
            note->ExecuteLD(_L("EasyRPG"));
        }
        break;
    case EEikCmdExit:
    case EAknSoftkeyExit:
        Exit();
        break;
    default:
        break;
    }
}

void CEasyRpgAppUi::HandleStatusPaneSizeChange()
{
    if (iLauncherView)
        iLauncherView->UpdateLayout();
}

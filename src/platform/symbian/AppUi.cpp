#include <avkon.hrh>
#include <eikbtgpc.h>
#include <eikspane.h>
#include "GameView.h"
#include "AppUi.h"

CEasyRpgAppUi::CEasyRpgAppUi() : iGameView(NULL) {}

void CEasyRpgAppUi::ConstructL()
{
    BaseConstructL(CAknAppUi::EAknEnableSkin);
    SetFullScreenApp(EFalse);
    if (StatusPane()) StatusPane()->MakeVisible(ETrue);
    if (Cba()) Cba()->MakeVisible(ETrue);
    iGameView = CGameView::NewL(ClientRect());
    AddToStackL(iGameView);
}

CEasyRpgAppUi::~CEasyRpgAppUi()
{
    if (iGameView)
    {
        RemoveFromStack(iGameView);
        delete iGameView;
        iGameView = NULL;
    }
}

void CEasyRpgAppUi::EnterGameFullscreen()
{
    SetFullScreenApp(ETrue);
    if (StatusPane()) StatusPane()->MakeVisible(EFalse);
    if (Cba()) Cba()->MakeVisible(EFalse);
}

void CEasyRpgAppUi::ExitGameFullscreen()
{
    SetFullScreenApp(EFalse);
    if (StatusPane()) StatusPane()->MakeVisible(ETrue);
    if (Cba()) Cba()->MakeVisible(ETrue);
}

void CEasyRpgAppUi::HandleCommandL(TInt aCommand)
{
    if (iGameView && iGameView->IsGameRunning())
    {
        if (aCommand == EAknSoftkeyOptions)
        {
            iGameView->HandleLeftSoftkeyL();
            return;
        }
        if (aCommand == EAknSoftkeyExit)
        {
            iGameView->HandleRightSoftkeyL();
            return;
        }
    }

    switch (aCommand)
    {
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
    if (iGameView) iGameView->UpdateLayout();
}

void CEasyRpgAppUi::HandleForegroundEventL(TBool aForeground)
{
    CAknAppUi::HandleForegroundEventL(aForeground);
    if (iGameView) iGameView->SetForeground(aForeground);
}

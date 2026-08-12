#include <aknnotewrappers.h>
#include <avkon.hrh>
#include <eikbtgpc.h>
#include <eikspane.h>
#include <w32std.h>
#include "App.hrh"
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

void CEasyRpgAppUi::HandleWsEventL(const TWsEvent& aEvent, CCoeControl* aDestination)
{
    if (iGameView && iGameView->IsGameRunning())
    {
        const TInt type = aEvent.Type();
        if (type == EEventKeyDown || type == EEventKey || type == EEventKeyUp)
        {
            const TKeyEvent* keyEvent = aEvent.Key();
            if (keyEvent &&
                iGameView->HandleKeyEventL(*keyEvent, static_cast<TEventCode>(type)) == EKeyWasConsumed)
                return;
        }
    }
    CAknAppUi::HandleWsEventL(aEvent, aDestination);
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
    case EEasyRpgCmdLauncherPlay:
        if (iGameView)
            iGameView->PlaySelectedGameL();
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
    if (iGameView) iGameView->UpdateLayout();
}

void CEasyRpgAppUi::HandleForegroundEventL(TBool aForeground)
{
    CAknAppUi::HandleForegroundEventL(aForeground);
    if (iGameView) iGameView->SetForeground(aForeground);
}

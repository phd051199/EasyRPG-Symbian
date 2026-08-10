#include <avkon.hrh>
#include <eikbtgpc.h>
#include <eikspane.h>

#include "easyrpgsymbian/GameView.h"
#include "easyrpgsymbian/SymbianAppUi.h"

CEasyRpgAppUi::CEasyRpgAppUi()
    : iGameView(NULL)
{
}

void CEasyRpgAppUi::ConstructL()
{
    BaseConstructL(CAknAppUi::EAknEnableSkin);
    SetFullScreenApp(ETrue);

    if (StatusPane())
        StatusPane()->MakeVisible(EFalse);
    if (Cba())
        Cba()->MakeVisible(EFalse);

    iGameView = CGameView::NewL(ApplicationRect());
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

void CEasyRpgAppUi::HandleCommandL(TInt aCommand)
{
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
    if (iGameView)
        iGameView->SetRect(ApplicationRect());
}

void CEasyRpgAppUi::HandleForegroundEventL(TBool aForeground)
{
    CAknAppUi::HandleForegroundEventL(aForeground);
    if (iGameView)
        iGameView->SetForeground(aForeground);
}

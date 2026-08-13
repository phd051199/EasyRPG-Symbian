#include <aknappui.h>
#include <aknlists.h>
#include <coemain.h>
#include <e32std.h>
#include <eikclbd.h>
#include <eikenv.h>
#include <eikscrlb.h>

#include "GameLocator.h"
#include "GameMetadata.h"
#include "LauncherView.h"

namespace
{
_LIT(KDoubleListItemFormat, "\t%S\t%S");
_LIT(KNoGamesFound, "No games found");
_LIT(KEmptyListSubtitle, "");

void CopyListText(const TDesC& aSource, TDes& aTarget)
{
    aTarget.Copy(aSource.Left(aTarget.MaxLength()));
    for (TInt index = 0; index < aTarget.Length(); ++index)
    {
        if (aTarget[index] == '\t' || aTarget[index] == '\r' ||
            aTarget[index] == '\n')
            aTarget[index] = ' ';
    }
}

}

CLauncherView* CLauncherView::NewL(const TRect& aRect)
{
    CLauncherView* self = new (ELeave) CLauncherView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop(self);
    return self;
}

CLauncherView::CLauncherView()
    : iListBox(NULL),
      iListItems(NULL),
      iArchivePaths(NULL)
{
}

void CLauncherView::ConstructL(const TRect& aRect)
{
    CreateWindowL();
    User::LeaveIfError(iFs.Connect());

    iListBox = new (ELeave) CAknDoubleStyleListBox;
    iListBox->SetContainerWindowL(*this);
    iListBox->ConstructL(this, EAknListBoxSelectionList);
    iListBox->SetListBoxObserver(this);
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

    iListItems = new (ELeave) CDesCArrayFlat(16);
    iArchivePaths = new (ELeave) CDesCArrayFlat(16);
    iListBox->Model()->SetItemTextArray(iListItems);
    iListBox->Model()->SetOwnershipType(ELbmDoesNotOwnItemArray);

    BuildGameListL();
    SetRect(aRect);
    SetFocus(ETrue);
    ActivateL();
    iListBox->ActivateL();
    iListBox->SetFocus(ETrue);
}

CLauncherView::~CLauncherView()
{
    delete iListBox;
    iListBox = NULL;
    delete iListItems;
    iListItems = NULL;
    delete iArchivePaths;
    iArchivePaths = NULL;
    iFs.Close();
}

void CLauncherView::BuildGameListL()
{
    iListItems->Reset();
    iArchivePaths->Reset();

    CDesCArrayFlat* displayNames = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(displayNames);
    TGameLocator::ListGameArchivesL(iFs, *displayNames, *iArchivePaths);

    for (TInt index = 0; index < displayNames->Count(); ++index)
    {
        TBuf<KMaxFileName> title;
        CopyListText((*displayNames)[index], title);

        TInt version = 0;
        TRAP_IGNORE(ReadGameMetadataL(iFs, (*iArchivePaths)[index], title, version));

        TBuf<64> subtitle;
        FormatGameSubtitle(iFs, (*iArchivePaths)[index], version, subtitle);

        TBuf<KMaxFileName + 80> item;
        item.Format(KDoubleListItemFormat, &title, &subtitle);
        iListItems->AppendL(item);
    }

    if (iListItems->Count() == 0)
    {
        TBuf<64> item;
        item.Format(KDoubleListItemFormat, &KNoGamesFound, &KEmptyListSubtitle);
        iListItems->AppendL(item);
    }

    CleanupStack::PopAndDestroy(displayNames);
    iListBox->HandleItemAdditionL();
    iListBox->SetCurrentItemIndex(0);
}

void CLauncherView::LaunchArchiveL(const TDesC& aArchivePath)
{
    RProcess process;
    const TInt error = process.Create(_L("EasyRPGGame.exe"), aArchivePath);
    if (error != KErrNone)
        User::Leave(error);
    process.Resume();
    process.Close();

    CAknAppUi* appUi = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi());
    appUi->Exit();
}

void CLauncherView::PlaySelectedGameL()
{
    if (!iListBox || !iArchivePaths)
        return;
    const TInt index = iListBox->CurrentItemIndex();
    if (index < 0 || index >= iArchivePaths->Count())
        return;
    LaunchArchiveL((*iArchivePaths)[index]);
}

void CLauncherView::UpdateLayout()
{
    CAknAppUi* appUi = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi());
    SetRect(appUi->ClientRect());
}

void CLauncherView::SizeChanged()
{
    if (iListBox)
        iListBox->SetRect(Rect());
}

TInt CLauncherView::CountComponentControls() const
{
    return iListBox ? 1 : 0;
}

CCoeControl* CLauncherView::ComponentControl(TInt aIndex) const
{
    return aIndex == 0 ? iListBox : NULL;
}

TKeyResponse CLauncherView::HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
    return OfferKeyEventL(aKeyEvent, aType);
}

TKeyResponse CLauncherView::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
    if (!iListBox)
        return EKeyWasNotConsumed;
    return iListBox->OfferKeyEventL(aKeyEvent, aType);
}

void CLauncherView::HandleListBoxEventL(CEikListBox*, TListBoxEvent aEventType)
{
    if (aEventType == EEventEnterKeyPressed || aEventType == EEventItemClicked)
        PlaySelectedGameL();
}

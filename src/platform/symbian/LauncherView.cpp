#include <aknappui.h>
#include <aknlists.h>
#include <aknnotewrappers.h>
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
    iEngineVersions.Close();
    iFs.Close();
}

void CLauncherView::BuildGameListL()
{
    iListItems->Reset();
    iArchivePaths->Reset();
    iEngineVersions.Reset();

    CDesCArrayFlat* displayNames = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(displayNames);
    TGameLocator::ListGameArchivesL(iFs, *displayNames, *iArchivePaths);

    for (TInt index = 0; index < displayNames->Count(); ++index)
    {
        TBuf<KMaxFileName> title;
        CopyListText((*displayNames)[index], title);

        TInt version = EGameEngineUnknown;
        TRAP_IGNORE(version = DetectGameEngineL(iFs, (*iArchivePaths)[index]));
        TInt metadataVersion = version;
        TRAP_IGNORE(ReadGameMetadataL(iFs,
                                      (*iArchivePaths)[index],
                                      title,
                                      metadataVersion));
        if (metadataVersion != EGameEngineUnknown)
            version = metadataVersion;
        User::LeaveIfError(iEngineVersions.Append(version));

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

void CLauncherView::LaunchArchiveL(const TDesC& aArchivePath, TInt aVersion)
{
    const TInt KGamePathParameterSlot = 1;
    _LIT(KEasyRpgRuntime, "EasyRPGGame.exe");
    _LIT(KEasyRpgRgssRuntime, "EasyRPGRgss.exe");
    const TDesC* executable = &KEasyRpgRuntime;
    if (aVersion == EGameEngineRpgXp ||
        aVersion == EGameEngineRpgVx ||
        aVersion == EGameEngineRpgVxAce)
        executable = &KEasyRpgRgssRuntime;
    else if (aVersion != EGameEngineRpg2000 &&
             aVersion != EGameEngineRpg2003)
        User::Leave(KErrNotSupported);

    const TFileName currentExecutable = RProcess().FileName();
    const TParsePtrC currentParse(currentExecutable);
    TFileName executablePath(currentParse.DriveAndPath());
    executablePath.Append(*executable);

    RProcess process;
    User::LeaveIfError(process.Create(executablePath, KNullDesC));
    const TInt parameterError = process.SetParameter(KGamePathParameterSlot, aArchivePath);
    if (parameterError != KErrNone)
    {
        process.Kill(parameterError);
        process.Close();
        User::Leave(parameterError);
    }
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
    if (index < 0 || index >= iArchivePaths->Count() ||
        index >= iEngineVersions.Count())
        return;
    TRAPD(error, LaunchArchiveL((*iArchivePaths)[index], iEngineVersions[index]));
    if (error != KErrNone)
    {
        TBuf<48> message;
        message.Format(_L("Start failed (%d)"), error);
        CAknErrorNote* note = new (ELeave) CAknErrorNote;
        note->ExecuteLD(message);
    }
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
    if (aType == EEventKey &&
        (aKeyEvent.iScanCode == EStdKeyEnter ||
         aKeyEvent.iScanCode == EStdKeyDevice3 ||
         aKeyEvent.iCode == EKeyEnter ||
         aKeyEvent.iCode == EKeyDevice3))
    {
        PlaySelectedGameL();
        return EKeyWasConsumed;
    }
    return iListBox->OfferKeyEventL(aKeyEvent, aType);
}

void CLauncherView::HandleListBoxEventL(CEikListBox*, TListBoxEvent aEventType)
{
    if (aEventType == EEventEnterKeyPressed || aEventType == EEventItemClicked)
        PlaySelectedGameL();
}

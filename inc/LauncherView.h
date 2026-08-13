#ifndef EASYRPG_LAUNCHER_VIEW_H
#define EASYRPG_LAUNCHER_VIEW_H

#include <badesca.h>
#include <coecntrl.h>
#include <eiklbo.h>
#include <f32file.h>

class CAknDoubleStyleListBox;
class CAknProgressDialog;
class CEikProgressInfo;
class CGameLaunchWatcher;

class CLauncherView : public CCoeControl,
                      public MEikListBoxObserver
{
    friend class CGameLaunchWatcher;
public:
    static CLauncherView* NewL(const TRect& aRect);
    ~CLauncherView();

    void PlaySelectedGameL();
    void UpdateLayout();
    TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

private:
    CLauncherView();
    void ConstructL(const TRect& aRect);
    void BuildGameListL();
    void LaunchArchiveL(const TDesC& aArchivePath, TInt aVersion);
    void ShowLoadingDialogL();
    void UpdateLoadingProgress(TInt aValue);
    void DismissLoadingDialog();
    void GameLaunchCompleted(TInt aError);
    void SizeChanged();
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
    void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);

private:
    RFs iFs;
    CAknDoubleStyleListBox* iListBox;
    CDesCArrayFlat* iListItems;
    CDesCArrayFlat* iArchivePaths;
    RArray<TInt> iEngineVersions;
    CAknProgressDialog* iLoadingDialog;
    CEikProgressInfo* iLoadingProgress;
    CGameLaunchWatcher* iLaunchWatcher;
    TBool iLaunching;
};

#endif

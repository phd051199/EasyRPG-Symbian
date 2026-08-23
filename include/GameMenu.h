#ifndef EASYRPG_GAME_MENU_H
#define EASYRPG_GAME_MENU_H

#include <e32base.h>
#include <f32file.h>
#include <w32std.h>

class CBitmapDevice;
class CGraphicsContext;

enum TGameMenuFeature
{
    EGameMenuFeatureSound = 1u << 0,
    EGameMenuFeatureVolume = 1u << 1,
    EGameMenuFeatureFps = 1u << 2,
    EGameMenuFeatureHelp = 1u << 3,
    EGameMenuFeatureReset = 1u << 4
};

class MGameMenuHost
{
public:
    virtual TUint32 GameMenuFeatures() const { return 0; }
    virtual void GameMenuSetSoundEnabledL(TBool) {}
    virtual void GameMenuSetVolumeL(TInt) {}
    virtual void GameMenuSetFpsEnabledL(TBool) {}
    virtual void GameMenuResetL() {}
    virtual TInt GameMenuCustomItemCount() const { return 0; }
    virtual void GameMenuCustomItemText(TInt, TDes8& aText) const { aText.Zero(); }
    virtual TBool GameMenuCustomActivateL(TInt) { return ETrue; }
    virtual void GameMenuPauseL(TBool aPaused) = 0;
    virtual void GameMenuExitL() = 0;
    virtual void GameMenuRequestRedraw() = 0;
};

class CGameMenu : public CBase
{
public:
    static CGameMenu* NewL(MGameMenuHost& aHost, RFs& aFs);
    ~CGameMenu();

    TBool IsOpen() const;
    TInt Selection() const;
    TInt Volume() const;
    TBool SoundEnabled() const;
    void OpenL();
    void CloseL();
    void ResetInputState();
    TBool HandleKeyEventL(TInt aType, const TKeyEvent& aKey);
    void DrawL(CGraphicsContext& aGc, CBitmapDevice& aDevice);

private:
    enum TItemKind
    {
        EItemInvalid = 0,
        EItemSound,
        EItemVolume,
        EItemFps,
        EItemHelp,
        EItemCustom,
        EItemReset,
        EItemExit
    };

    CGameMenu(MGameMenuHost& aHost, RFs& aFs);
    void ConstructL();
    void LoadSettings();
    void SaveSettings();
    void ApplySettingsL();
    TInt ItemCount() const;
    TItemKind ItemKindAt(TInt aIndex, TInt& aCustomIndex) const;
    void ItemText(TInt aIndex, TDes8& aText) const;
    TBool ItemAdjustable(TInt aIndex) const;
    void ActivateSelectionL();
    void AdjustSelectionL(TInt aDelta);
    void NormalizeSelection();
    void MoveSelection(TInt aDelta);
    TInt MapMenuKey(const TKeyEvent& aKey) const;
    TBool HandleSoftKeyL(TBool aLeft, TInt aType, const TKeyEvent& aKey);
    TBool TriggerMenuKey(TInt aMenuKey, TInt aType, const TKeyEvent& aKey);
    void DrawMenuL(CGraphicsContext& aGc, CBitmapDevice& aDevice);
    void DrawHelpL(CGraphicsContext& aGc, CBitmapDevice& aDevice);

private:
    MGameMenuHost& iHost;
    RFs& iFs;
    TBool iOpen;
    TBool iHelpVisible;
    TInt iSelection;
    TBool iLeftSoftHeld;
    TBool iRightSoftHeld;
    TUint32 iHeldMenuKeys;
    TInt iVolume;
    TBool iSoundEnabled;
    TBool iFpsEnabled;
};

#endif

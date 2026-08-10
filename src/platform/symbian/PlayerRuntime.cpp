#include "easyrpgsymbian/PlayerRuntime.h"
#include "easyrpgsymbian/DisplaySurface.h"
#include "easyrpgsymbian/LcfProbe.h"
#include "easyrpgsymbian/ModernCoreProbe.h"
#include "easyrpgsymbian/UpstreamLcfLinkAnchor.h"
#include "easyrpgsymbian/ZipVfs.h"

CPlayerRuntime* CPlayerRuntime::NewL(CZipVfs* aResources,
                                    const TDesC& aGameSource,
                                    CSymbianDisplaySurface& aDisplaySurface)
{
    CPlayerRuntime* self = new (ELeave) CPlayerRuntime(aResources, aDisplaySurface);
    CleanupStack::PushL(self);
    self->ConstructL(aGameSource);
    CleanupStack::Pop(self);
    return self;
}

CPlayerRuntime::CPlayerRuntime(CZipVfs* aResources,
                               CSymbianDisplaySurface& aDisplaySurface)
    : iResources(aResources),
      iDisplaySurface(aDisplaySurface),
      iPressedKeys(0)
{
}

void CPlayerRuntime::ConstructL(const TDesC&)
{
    if (!iResources)
        User::Leave(KErrNotFound);

    TLcfProjectSummary projectSummary;
    TBuf<96> gameTitle;
    User::LeaveIfError(TLcfProbe::ProbeProject(*iResources, gameTitle, projectSummary));

    if (EasyRpgModernCoreProbe(5) != 33 || EasyRpgUpstreamLmuLinkAnchor() != 0)
        User::Leave(KErrNotSupported);

    iDisplaySurface.Clear(0xff000000u);
    iDisplaySurface.PresentL();
}

CPlayerRuntime::~CPlayerRuntime()
{
}

void CPlayerRuntime::SetKeyState(TUint32 aKey, TBool aPressed)
{
    if (aPressed)
        iPressedKeys |= aKey;
    else
        iPressedKeys &= ~aKey;
}

void CPlayerRuntime::ReleaseAllKeys()
{
    iPressedKeys = 0;
}

void CPlayerRuntime::TickL(TUint32, TBool aRenderFrame)
{
    if (aRenderFrame)
        iDisplaySurface.PresentL();
}

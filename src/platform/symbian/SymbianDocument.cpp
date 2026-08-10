#include "easyrpgsymbian/SymbianAppUi.h"
#include "easyrpgsymbian/SymbianDocument.h"

CEasyRpgDocument* CEasyRpgDocument::NewL(CEikApplication& aApplication)
{
    return new (ELeave) CEasyRpgDocument(aApplication);
}

CEasyRpgDocument::CEasyRpgDocument(CEikApplication& aApplication)
    : CAknDocument(aApplication)
{
}

CEikAppUi* CEasyRpgDocument::CreateAppUiL()
{
    return new (ELeave) CEasyRpgAppUi;
}

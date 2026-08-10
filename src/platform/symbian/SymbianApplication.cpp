#include "easyrpgsymbian/SymbianApplication.h"
#include "easyrpgsymbian/SymbianDocument.h"

CApaDocument* CEasyRpgApplication::CreateDocumentL()
{
    return CEasyRpgDocument::NewL(*this);
}

TUid CEasyRpgApplication::AppDllUid() const
{
    return KUidEasyRpgSymbian;
}

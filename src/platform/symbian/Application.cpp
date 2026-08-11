#include "Application.h"
#include "Document.h"

CApaDocument* CEasyRpgApplication::CreateDocumentL()
{
    return CEasyRpgDocument::NewL(*this);
}

TUid CEasyRpgApplication::AppDllUid() const
{
    return KUidEasyRpg;
}

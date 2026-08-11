#ifndef EASYRPG_DOCUMENT_H
#define EASYRPG_DOCUMENT_H

#include <akndoc.h>

class CEasyRpgDocument : public CAknDocument
{
public:
    static CEasyRpgDocument* NewL(CEikApplication& aApplication);

private:
    explicit CEasyRpgDocument(CEikApplication& aApplication);
    CEikAppUi* CreateAppUiL();
};

#endif

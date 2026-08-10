#ifndef EASYRPG_SYMBIAN_APPLICATION_H
#define EASYRPG_SYMBIAN_APPLICATION_H

#include <aknapp.h>

const TUid KUidEasyRpgSymbian = { 0xE31EA5A1 };

class CEasyRpgApplication : public CAknApplication
{
private:
    CApaDocument* CreateDocumentL();
    TUid AppDllUid() const;
};

#endif

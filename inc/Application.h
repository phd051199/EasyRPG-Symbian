#ifndef EASYRPG_APPLICATION_H
#define EASYRPG_APPLICATION_H

#include <aknapp.h>

const TUid KUidEasyRpg = { 0xE31EA5A1 };

class CEasyRpgApplication : public CAknApplication
{
private:
    CApaDocument* CreateDocumentL();
    TUid AppDllUid() const;
};

#endif

#ifndef EASYRPG_APPLICATION_H
#define EASYRPG_APPLICATION_H

#include <aknapp.h>

#ifdef EASYRPG_RUNTIME_PROCESS
const TUid KUidEasyRpg = { 0xE31EA5A2 };
#else
const TUid KUidEasyRpg = { 0xE31EA5A1 };
#endif

class CEasyRpgApplication : public CAknApplication
{
private:
    CApaDocument* CreateDocumentL();
    TUid AppDllUid() const;
};

#endif

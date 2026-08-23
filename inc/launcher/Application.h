#ifndef EASYRPG_APPLICATION_H
#define EASYRPG_APPLICATION_H

#include <aknapp.h>

#include "AppConfig.h"

class CEasyRpgApplication : public CAknApplication
{
private:
    CApaDocument* CreateDocumentL();
    TUid AppDllUid() const;
};

#endif

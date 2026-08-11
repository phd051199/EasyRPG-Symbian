#include <eikstart.h>
#include <stdio.h>

#include "easyrpgsymbian/SymbianApplication.h"
#include "easyrpgsymbian/SymbianLogBridge.h"

namespace
{
void ExceptionHandler(TExcType aType)
{
    char message[64];
    snprintf(message, sizeof(message), "fatal exception type=%d", static_cast<int>(aType));
    EasyRpgSymbianLogDebug(message);
}
}

LOCAL_C CApaApplication* NewApplication()
{
    return new CEasyRpgApplication;
}

GLDEF_C TInt E32Main()
{
    User::SetExceptionHandler(ExceptionHandler, 0xffffffffu);
    return EikStart::RunApplication(NewApplication);
}

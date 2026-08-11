#include <eikstart.h>
#include <stdio.h>

#include "Application.h"
#include "LogBridge.h"

namespace
{
void ExceptionHandler(TExcType aType)
{
    char message[64];
    snprintf(message, sizeof(message), "fatal exception type=%d", static_cast<int>(aType));
    EasyRpgLogDebug(message);
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

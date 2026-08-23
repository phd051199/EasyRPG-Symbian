#include <eikstart.h>
#include "Application.h"
LOCAL_C CApaApplication* NewApplication()
{
    return new CEasyRpgApplication;
}

GLDEF_C TInt E32Main()
{
    return EikStart::RunApplication(NewApplication);
}

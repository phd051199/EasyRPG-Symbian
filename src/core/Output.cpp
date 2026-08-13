#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "output.h"

namespace {
void WriteLine(const char* prefix, const char* message) {
    if (prefix)
        std::fprintf(stderr, "%s", prefix);
    std::fprintf(stderr, "%s\n", message);
}

void PrintLine(const char* prefix, const char* fmt, va_list ap) {
    char message[1024];
    message[0] = 0;
    vsnprintf(message, sizeof(message), fmt, ap);
    message[sizeof(message) - 1] = 0;
    WriteLine(prefix, message);
}
}

namespace Output {
void Quit() {}
bool TakeScreenshot() { return false; }
bool TakeScreenshot(std::string const&) { return false; }
bool TakeScreenshot(std::ostream&) { return false; }
void ToggleLog() {}
void IgnorePause(bool) {}

void Post(char*, ...) {}

void Post(const char*, ...) {}

void PostStr(std::string const&) {}

void Warning(const char*, ...) {}

void WarningStr(std::string const&) {}

void Error(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    PrintLine("error: ", fmt, ap);
    va_end(ap);
    std::abort();
    for (;;) {}
}

void ErrorStr(std::string const& err) {
    WriteLine("error: ", err.c_str());
    std::abort();
    for (;;) {}
}

}

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "output.h"

namespace {
bool gLogStarted = false;
FILE* gLog = NULL;

FILE* LogFile() {
    if (gLog)
        return gLog;
    gLog = std::fopen("C:\\Data\\EasyRPG\\easyrpg.log", gLogStarted ? "a" : "w");
    if (gLog)
        gLogStarted = true;
    return gLog;
}

void CloseLog() {
    if (!gLog)
        return;
    std::fflush(gLog);
    std::fclose(gLog);
    gLog = NULL;
}

void WriteLine(const char* prefix, const char* message) {
    if (prefix)
        std::fprintf(stderr, "%s", prefix);
    std::fprintf(stderr, "%s\n", message);

    FILE* log = LogFile();
    if (!log)
        return;
    if (prefix)
        std::fprintf(log, "%s", prefix);
    std::fprintf(log, "%s\n", message);
    std::fflush(log);
}

void PrintLine(const char* prefix, const char* fmt, va_list ap) {
    char message[1024];
    message[0] = 0;
    vsnprintf(message, sizeof(message), fmt, ap);
    message[sizeof(message) - 1] = 0;
    WriteLine(prefix, message);
}
}

extern "C" void EasyRpgLogDebug(const char* message) {
    if (message)
        WriteLine("easyrpg: ", message);
}

namespace Output {
void Quit() { CloseLog(); }
bool TakeScreenshot() { return false; }
bool TakeScreenshot(std::string const&) { return false; }
bool TakeScreenshot(std::ostream&) { return false; }
void ToggleLog() {}
void IgnorePause(bool) {}

void Post(char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    PrintLine(0, fmt, ap);
    va_end(ap);
}

void Post(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    PrintLine(0, fmt, ap);
    va_end(ap);
}

void PostStr(std::string const& msg) {
    WriteLine(0, msg.c_str());
}

void Warning(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    PrintLine("warning: ", fmt, ap);
    va_end(ap);
}

void WarningStr(std::string const& warn) {
    WriteLine("warning: ", warn.c_str());
}

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

void Debug(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    PrintLine("debug: ", fmt, ap);
    va_end(ap);
}

void DebugStr(std::string const& msg) {
    WriteLine("debug: ", msg.c_str());
}
}

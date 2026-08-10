#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "output.h"

namespace {
void PrintLine(const char* prefix, const char* fmt, va_list ap) {
    if (prefix) std::fprintf(stderr, "%s", prefix);
    std::vfprintf(stderr, fmt, ap);
    std::fprintf(stderr, "\n");
}
}

namespace Output {
void Quit() {}
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
    std::fprintf(stderr, "%s\n", msg.c_str());
}

void Warning(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    PrintLine("warning: ", fmt, ap);
    va_end(ap);
}

void WarningStr(std::string const& warn) {
    std::fprintf(stderr, "warning: %s\n", warn.c_str());
}

void Error(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    PrintLine("error: ", fmt, ap);
    va_end(ap);
    std::abort();
}

void ErrorStr(std::string const& err) {
    std::fprintf(stderr, "error: %s\n", err.c_str());
    std::abort();
}

void Debug(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    PrintLine("debug: ", fmt, ap);
    va_end(ap);
}

void DebugStr(std::string const& msg) {
    std::fprintf(stderr, "debug: %s\n", msg.c_str());
}
}

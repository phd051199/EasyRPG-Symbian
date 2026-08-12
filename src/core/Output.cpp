#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>

#include "baseui.h"
#include "bitmap.h"
#include "filefinder.h"
#include "main_data.h"
#include "output.h"
#include "utils.h"

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
bool TakeScreenshot() {
    int index = 0;
    std::string path;
    do {
        path = FileFinder::MakePath(Main_Data::GetSavePath(),
                                    "screenshot_" + Utils::ToString(index++) + ".png");
    } while (FileFinder::Exists(path));
    return TakeScreenshot(path);
}
bool TakeScreenshot(std::string const& file) {
    std::shared_ptr<std::fstream> stream = FileFinder::openUTF8(
        file, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
    return stream && TakeScreenshot(*stream);
}
bool TakeScreenshot(std::ostream& stream) {
    return DisplayUi && DisplayUi->GetDisplaySurface() &&
        DisplayUi->GetDisplaySurface()->WritePNG(stream);
}
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

}

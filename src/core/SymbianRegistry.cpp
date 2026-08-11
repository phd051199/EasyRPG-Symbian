#include "registry.h"

namespace Registry {
std::string ReadStrValue(HKEY,
                         const std::string&,
                         const std::string&,
                         REGVIEW) {
    return std::string();
}

int ReadBinValue(HKEY,
                 const std::string&,
                 const std::string&,
                 unsigned char*,
                 REGVIEW) {
    return 0;
}
}

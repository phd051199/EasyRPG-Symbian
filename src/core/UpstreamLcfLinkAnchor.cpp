#include <memory>
#include <string>

#include "lmu_reader.h"

namespace {
typedef std::unique_ptr<RPG::Map> (*TLmuLoadFunction)(const std::string&,
                                                       const std::string&);

TLmuLoadFunction volatile gLmuLoadFunction = &LMU_Reader::Load;
}

extern "C" int EasyRpgUpstreamLmuLinkAnchor(void)
{
    return gLmuLoadFunction != 0 ? 0 : -1;
}

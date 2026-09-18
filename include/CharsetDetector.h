#ifndef EASYRPG_CHARSET_DETECTOR_H
#define EASYRPG_CHARSET_DETECTOR_H

#include <string>
#include <vector>

namespace EasyRpgCharsetDetector
{

struct Guess
{
    std::string encoding;
    float confidence;
};

std::vector<Guess> Detect(const std::string &sample);

} // namespace EasyRpgCharsetDetector

#endif

#include <string>
#include <vector>

#include "reader_util.h"

extern "C" int EasyRpgCoreCp932ToUtf16(const unsigned char* aSource,
                                         unsigned int aSourceLength,
                                         unsigned short* aDestination,
                                         unsigned int aDestinationCapacity,
                                         unsigned int* aDestinationLength);

namespace {
char AsciiLower(char value) {
    if (value >= 'A' && value <= 'Z')
        return static_cast<char>(value + ('a' - 'A'));
    return value;
}

bool IsEncodingSeparator(char value) {
    return value == '-' || value == '_' || value == ' ';
}

bool EncodingEquals(const std::string& value, const char* expected) {
    std::string::size_type i = 0;
    unsigned int j = 0;
    while (true) {
        while (i < value.size() && IsEncodingSeparator(value[i]))
            ++i;
        while (expected[j] != '\0' && IsEncodingSeparator(expected[j]))
            ++j;

        const bool value_done = i == value.size();
        const bool expected_done = expected[j] == '\0';
        if (value_done || expected_done)
            return value_done && expected_done;

        if (AsciiLower(value[i]) != AsciiLower(expected[j]))
            return false;
        ++i;
        ++j;
    }
}

bool IsCp932Encoding(const std::string& encoding) {
    return EncodingEquals(encoding, "932") ||
           EncodingEquals(encoding, "CP932") ||
           EncodingEquals(encoding, "MS932") ||
           EncodingEquals(encoding, "SHIFT_JIS") ||
           EncodingEquals(encoding, "WINDOWS-31J") ||
           EncodingEquals(encoding, "WINDOWS-932") ||
           EncodingEquals(encoding, "IBM-943_P15A-2003");
}

bool IsUtf8Encoding(const std::string& encoding) {
    return EncodingEquals(encoding, "UTF-8") ||
           EncodingEquals(encoding, "UTF8");
}

void AppendUtf8(std::string& output, unsigned int codepoint) {
    if (codepoint <= 0x7fu) {
        output.push_back(static_cast<char>(codepoint));
    } else if (codepoint <= 0x7ffu) {
        output.push_back(static_cast<char>(0xc0u | (codepoint >> 6)));
        output.push_back(static_cast<char>(0x80u | (codepoint & 0x3fu)));
    } else if (codepoint <= 0xffffu) {
        output.push_back(static_cast<char>(0xe0u | (codepoint >> 12)));
        output.push_back(static_cast<char>(0x80u | ((codepoint >> 6) & 0x3fu)));
        output.push_back(static_cast<char>(0x80u | (codepoint & 0x3fu)));
    } else {
        output.push_back(static_cast<char>(0xf0u | (codepoint >> 18)));
        output.push_back(static_cast<char>(0x80u | ((codepoint >> 12) & 0x3fu)));
        output.push_back(static_cast<char>(0x80u | ((codepoint >> 6) & 0x3fu)));
        output.push_back(static_cast<char>(0x80u | (codepoint & 0x3fu)));
    }
}

std::string Cp932ToUtf8(const std::string& input) {
    const unsigned int source_length = static_cast<unsigned int>(input.size());
    unsigned short* utf16 = new unsigned short[source_length];
    unsigned int utf16_length = 0;
    const int status = EasyRpgCoreCp932ToUtf16(
        reinterpret_cast<const unsigned char*>(input.data()),
        source_length,
        utf16,
        source_length,
        &utf16_length);
    if (status != 0) {
        delete[] utf16;
        return std::string();
    }

    std::string output;
    output.reserve(utf16_length * 3u);
    for (unsigned int i = 0; i < utf16_length; ++i) {
        unsigned int codepoint = utf16[i];
        if (codepoint >= 0xd800u && codepoint <= 0xdbffu && i + 1u < utf16_length) {
            const unsigned int low = utf16[i + 1u];
            if (low >= 0xdc00u && low <= 0xdfffu) {
                codepoint = 0x10000u + ((codepoint - 0xd800u) << 10) + (low - 0xdc00u);
                ++i;
            }
        }
        AppendUtf8(output, codepoint);
    }

    delete[] utf16;
    return output;
}
}  // namespace

std::string ReaderUtil::Recode(const std::string& str_to_encode,
                               const std::string& src_enc,
                               const std::string& dst_enc) {
    if (src_enc.empty() || dst_enc.empty() || str_to_encode.empty())
        return str_to_encode;

    if (IsUtf8Encoding(src_enc) && IsUtf8Encoding(dst_enc))
        return str_to_encode;

    if (IsCp932Encoding(src_enc) && IsUtf8Encoding(dst_enc))
        return Cp932ToUtf8(str_to_encode);

    // Match the non-ICU upstream behavior when iconv_open() cannot provide a
    // requested conversion: leave the original bytes untouched.
    return str_to_encode;
}

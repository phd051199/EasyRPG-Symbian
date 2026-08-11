#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "inireader.h"
#include "reader_util.h"
#include "CoreEncoding.h"
#include "GohuFont.h"

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
    return EncodingEquals(encoding, "932") || EncodingEquals(encoding, "CP932") ||
           EncodingEquals(encoding, "MS932") || EncodingEquals(encoding, "SHIFT_JIS") ||
           EncodingEquals(encoding, "WINDOWS-31J") || EncodingEquals(encoding, "WINDOWS-932") ||
           EncodingEquals(encoding, "IBM-943_P15A-2003");
}

bool IsUtf8Encoding(const std::string& encoding) {
    return EncodingEquals(encoding, "UTF-8") || EncodingEquals(encoding, "UTF8");
}

bool IsMixedVietnameseEncoding(const std::string& encoding) {
    return EncodingEquals(encoding, "EASYRPG_VI_CP932") || EncodingEquals(encoding, "TCVN3_CP932");
}

unsigned int Tcvn3Codepoint(unsigned char value) {
    const unsigned int codepoint = EasyRpgTcvn3DecodeByte(value);
    return codepoint != value ? codepoint : 0;
}

bool IsAsciiLetter(unsigned char value) {
    return (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z');
}

bool LooksLikeTcvn3(const std::string& input) {
    unsigned int ascii_letters = 0;
    for (std::string::size_type index = 0; index < input.size(); ++index) {
        if (IsAsciiLetter(static_cast<unsigned char>(input[index]))) ++ascii_letters;
    }
    unsigned int score = 0;
    unsigned int mapped = 0;
    for (std::string::size_type index = 0; index < input.size(); ++index) {
        const unsigned char value = static_cast<unsigned char>(input[index]);
        const unsigned int codepoint = Tcvn3Codepoint(value);
        if (codepoint == 0 || codepoint == 0x00a0u || (codepoint >= 0x0300u && codepoint <= 0x0323u)) continue;
        if (value < 0x20u) { ++mapped; score += 4; continue; }
        if (value < 0xa1u) continue;
        if (value >= 0xe0u && ascii_letters * 2u <= input.size()) continue;
        ++mapped;
        const bool left_letter = index > 0 && IsAsciiLetter(static_cast<unsigned char>(input[index - 1]));
        const bool right_letter = index + 1 < input.size() && IsAsciiLetter(static_cast<unsigned char>(input[index + 1]));
        if (left_letter || right_letter) score += 2;
        else if ((index > 0 && input[index - 1] == ' ') || (index + 1 < input.size() && input[index + 1] == ' ')) ++score;
    }
    return mapped != 0 && score >= 2;
}

void AppendUtf8(std::string& output, unsigned int codepoint) {
    if (codepoint <= 0x7fu) output.push_back(static_cast<char>(codepoint));
    else if (codepoint <= 0x7ffu) {
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

std::string Tcvn3ToUtf8(const std::string& input) {
    std::string output;
    output.reserve(input.size() * 3u);
    for (std::string::size_type index = 0; index < input.size(); ++index) {
        const unsigned char value = static_cast<unsigned char>(input[index]);
        const unsigned int codepoint = EasyRpgTcvn3DecodeByte(value);
        if (codepoint == value && value < 0x80u) output.push_back(static_cast<char>(value));
        else AppendUtf8(output, codepoint);
    }
    return output;
}

std::string Cp932ToUtf8(const std::string& input) {
    const unsigned int source_length = static_cast<unsigned int>(input.size());
    unsigned short* utf16 = new unsigned short[source_length];
    unsigned int utf16_length = 0;
    const int status = EasyRpgCoreCp932ToUtf16(reinterpret_cast<const unsigned char*>(input.data()), source_length, utf16, source_length, &utf16_length);
    if (status != 0) { delete[] utf16; return std::string(); }
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

bool Utf8ToUtf16(const std::string& input, std::vector<unsigned short>& output) {
    output.clear();
    for (std::string::size_type i = 0; i < input.size();) {
        const unsigned int first = static_cast<unsigned char>(input[i]);
        unsigned int codepoint = 0;
        unsigned int length = 0;
        if (first < 0x80u) { codepoint = first; length = 1; }
        else if (first >= 0xc2u && first <= 0xdfu) { codepoint = first & 0x1fu; length = 2; }
        else if (first >= 0xe0u && first <= 0xefu) { codepoint = first & 0x0fu; length = 3; }
        else return false;
        if (i + length > input.size()) return false;
        for (unsigned int offset = 1; offset < length; ++offset) {
            const unsigned int next = static_cast<unsigned char>(input[i + offset]);
            if ((next & 0xc0u) != 0x80u) return false;
            codepoint = (codepoint << 6) | (next & 0x3fu);
        }
        if ((length == 3 && codepoint < 0x800u) || (codepoint >= 0xd800u && codepoint <= 0xdfffu) || codepoint > 0xffffu) return false;
        output.push_back(static_cast<unsigned short>(codepoint));
        i += length;
    }
    return true;
}

std::string Utf8ToCp932(const std::string& input) {
    std::vector<unsigned short> utf16;
    if (!Utf8ToUtf16(input, utf16) || utf16.empty()) return std::string();
    std::vector<unsigned char> encoded(utf16.size() * 2u);
    unsigned int encoded_length = 0;
    const int status = EasyRpgCoreUtf16ToCp932(&utf16[0], static_cast<unsigned int>(utf16.size()), &encoded[0], static_cast<unsigned int>(encoded.size()), &encoded_length);
    if (status != 0) return std::string();
    return std::string(reinterpret_cast<const char*>(&encoded[0]), encoded_length);
}
}

std::string ReaderUtil::CodepageToEncoding(int codepage) {
    if (codepage == 0) return std::string();
    if (codepage == 932) return "SHIFT_JIS";
    if (codepage == 949) return "cp949";
    std::string result("CP");
    char digits[16];
    std::sprintf(digits, "%d", codepage);
    result += digits;
    return result;
}

std::string ReaderUtil::DetectEncoding(const std::string& database_file) {
    std::vector<std::string> encodings = DetectEncodings(database_file);
    return encodings.empty() ? std::string() : encodings.front();
}

std::vector<std::string> ReaderUtil::DetectEncodings(const std::string& database_file) {
    std::vector<std::string> encodings;
    if (!database_file.empty()) { encodings.push_back("EASYRPG_VI_CP932"); encodings.push_back("SHIFT_JIS"); }
    return encodings;
}

std::string ReaderUtil::GetEncoding(const std::string& ini_file) {
    if (ini_file.empty()) return std::string();
    INIReader ini(ini_file);
    if (ini.ParseError() == -1) return std::string();
    const std::string value = ini.Get("EasyRPG", "Encoding", std::string());
    if (value.empty()) return std::string();
    return CodepageToEncoding(std::atoi(value.c_str()));
}

std::string ReaderUtil::GetLocaleEncoding() { return "EASYRPG_VI_CP932"; }

std::string ReaderUtil::Recode(const std::string& str_to_encode, const std::string& source_encoding) {
    return Recode(str_to_encode, source_encoding, "UTF-8");
}

std::string ReaderUtil::Recode(const std::string& str_to_encode, const std::string& src_enc, const std::string& dst_enc) {
    if (src_enc.empty() || dst_enc.empty() || str_to_encode.empty()) return str_to_encode;
    if (IsUtf8Encoding(src_enc) && IsUtf8Encoding(dst_enc)) return str_to_encode;
    if (IsMixedVietnameseEncoding(src_enc) && IsUtf8Encoding(dst_enc)) {
        if (LooksLikeTcvn3(str_to_encode)) return Tcvn3ToUtf8(str_to_encode);
        const std::string cp932 = Cp932ToUtf8(str_to_encode);
        if (cp932.empty() && !str_to_encode.empty()) return Tcvn3ToUtf8(str_to_encode);
        return cp932;
    }
    if (IsCp932Encoding(src_enc) && IsUtf8Encoding(dst_enc)) return Cp932ToUtf8(str_to_encode);
    if (IsUtf8Encoding(src_enc) && IsCp932Encoding(dst_enc)) return Utf8ToCp932(str_to_encode);
    return str_to_encode;
}

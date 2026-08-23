#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "data.h"
#include "inireader.h"
#include "ldb_reader.h"
#include "lmu_reader.h"
#include "reader_util.h"
#include "CharsetDetector.h"
#include "CoreEncoding.h"
#include "GohuFont.h"

#ifdef EASYRPG_ENCODING_CORPUS_TEST
extern "C" void EasyRpgEncodingCorpusRecord(const char* aSourceEncoding,
                                               const char* aDestinationEncoding,
                                               const unsigned char* aSource,
                                               unsigned int aSourceLength,
                                               const unsigned char* aDestination,
                                               unsigned int aDestinationLength);
#endif

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

bool IsGbkEncoding(const std::string& encoding) {
    return EncodingEquals(encoding, "936") || EncodingEquals(encoding, "CP936") ||
           EncodingEquals(encoding, "WINDOWS-936") || EncodingEquals(encoding, "GBK") ||
           EncodingEquals(encoding, "GB2312");
}

bool IsBig5Encoding(const std::string& encoding) {
    return EncodingEquals(encoding, "950") || EncodingEquals(encoding, "CP950") ||
           EncodingEquals(encoding, "WINDOWS-950") || EncodingEquals(encoding, "BIG5");
}

bool IsUtf8Encoding(const std::string& encoding) {
    return EncodingEquals(encoding, "UTF-8") || EncodingEquals(encoding, "UTF8");
}

bool IsMixedVietnameseEncoding(const std::string& encoding) {
    return EncodingEquals(encoding, "EASYRPG_VI_CP932") || EncodingEquals(encoding, "TCVN3_CP932");
}

bool IsMixedGbkCp932Encoding(const std::string& encoding) {
    return EncodingEquals(encoding, "EASYRPG_GBK_CP932") ||
           EncodingEquals(encoding, "GBK_CP932");
}

bool IsAsciiLetter(unsigned char value) {
    return (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z');
}

struct Tcvn3Evidence {
    unsigned int ascii_letters = 0;
    unsigned int mapped_bytes = 0;
    unsigned int score = 0;
};

Tcvn3Evidence InspectTcvn3(const std::string& input) {
    Tcvn3Evidence result;
    for (std::string::size_type index = 0; index < input.size(); ++index) {
        if (IsAsciiLetter(static_cast<unsigned char>(input[index])))
            ++result.ascii_letters;
    }

    for (std::string::size_type index = 0; index < input.size(); ++index) {
        const unsigned char value = static_cast<unsigned char>(input[index]);
        const unsigned int codepoint = EasyRpgTcvn3DecodeByte(value);
        if (codepoint == value || codepoint == 0x00a0u ||
            (codepoint >= 0x0300u && codepoint <= 0x0323u))
            continue;
        if (value < 0x20u) {
            ++result.mapped_bytes;
            result.score += 4;
            continue;
        }
        if (value < 0xa1u)
            continue;
        if (value >= 0xe0u && result.ascii_letters * 2u <= input.size())
            continue;

        ++result.mapped_bytes;
        const bool left_letter = index > 0 &&
            IsAsciiLetter(static_cast<unsigned char>(input[index - 1]));
        const bool right_letter = index + 1 < input.size() &&
            IsAsciiLetter(static_cast<unsigned char>(input[index + 1]));
        if (left_letter || right_letter)
            result.score += 2;
        else if ((index > 0 && input[index - 1] == ' ') ||
                 (index + 1 < input.size() && input[index + 1] == ' '))
            ++result.score;
    }
    return result;
}

bool IsCp932LeadByte(unsigned char value) {
    return (value >= 0x81u && value <= 0x9fu) ||
           (value >= 0xe0u && value <= 0xfcu);
}

bool IsCp932TrailByte(unsigned char value) {
    return (value >= 0x40u && value <= 0x7eu) ||
           (value >= 0x80u && value <= 0xfcu);
}

struct Cp932Evidence {
    bool valid = true;
    unsigned int multibyte_chars = 0;
    unsigned int longest_multibyte_run = 0;
    unsigned int kana_lead_pairs = 0;
};

Cp932Evidence InspectCp932(const std::string& input) {
    Cp932Evidence result;
    unsigned int current_run = 0;
    for (std::string::size_type index = 0; index < input.size();) {
        const unsigned char value = static_cast<unsigned char>(input[index]);
        if (value < 0x80u || (value >= 0xa1u && value <= 0xdfu)) {
            current_run = 0;
            ++index;
            continue;
        }
        if (!IsCp932LeadByte(value) || index + 1u >= input.size() ||
            !IsCp932TrailByte(static_cast<unsigned char>(input[index + 1u]))) {
            result.valid = false;
            return result;
        }
        ++result.multibyte_chars;
        if (value == 0x82u || value == 0x83u)
            ++result.kana_lead_pairs;
        ++current_run;
        if (current_run > result.longest_multibyte_run)
            result.longest_multibyte_run = current_run;
        index += 2u;
    }
    return result;
}

struct GbkByteEvidence {
    bool valid = true;
    unsigned int multibyte_chars = 0;
    unsigned int gb2312_lead_pairs = 0;
};

bool IsGbkTrailByte(unsigned char value) {
    return (value >= 0x40u && value <= 0x7eu) ||
           (value >= 0x80u && value <= 0xfeu);
}

GbkByteEvidence InspectGbkBytes(const std::string& input) {
    GbkByteEvidence result;
    for (std::string::size_type index = 0; index < input.size();) {
        const unsigned char lead = static_cast<unsigned char>(input[index]);
        if (lead < 0x80u) {
            ++index;
            continue;
        }
        if (lead < 0x81u || lead > 0xfeu || index + 1u >= input.size() ||
            !IsGbkTrailByte(static_cast<unsigned char>(input[index + 1u]))) {
            result.valid = false;
            return result;
        }
        ++result.multibyte_chars;
        if (lead >= 0xa1u && lead <= 0xdfu)
            ++result.gb2312_lead_pairs;
        index += 2u;
    }
    return result;
}

enum class MixedCjkEncoding {
    Unknown,
    Cp932,
    Gbk
};

MixedCjkEncoding ClassifyMixedCjkText(const std::string& input) {
    const Cp932Evidence cp932 = InspectCp932(input);
    const GbkByteEvidence gbk = InspectGbkBytes(input);
    if (cp932.valid && !gbk.valid)
        return MixedCjkEncoding::Cp932;
    if (gbk.valid && !cp932.valid)
        return MixedCjkEncoding::Gbk;
    if (!cp932.valid && !gbk.valid)
        return MixedCjkEncoding::Unknown;
    // In a Chinese-primary mixed game many perfectly valid GBK byte pairs are
    // also structurally valid CP932. Only prefer CP932 when the byte pattern
    // has explicit kana evidence; otherwise GBK is the deterministic primary.
    if (cp932.kana_lead_pairs != 0u)
        return MixedCjkEncoding::Cp932;
    if (gbk.multibyte_chars != 0u)
        return MixedCjkEncoding::Gbk;
    if (cp932.multibyte_chars != 0u)
        return MixedCjkEncoding::Cp932;
    return MixedCjkEncoding::Unknown;
}

bool IsMixedGbkCp932Sample(const std::string& input) {
    unsigned int cp932_lines = 0;
    unsigned int gbk_lines = 0;
    std::string::size_type start = 0;
    while (start < input.size()) {
        std::string::size_type end = input.find('\n', start);
        if (end == std::string::npos)
            end = input.size();
        if (end > start) {
            const std::string line(input, start, end - start);
            const Cp932Evidence cp932 = InspectCp932(line);
            const GbkByteEvidence gbk = InspectGbkBytes(line);
            if (cp932.valid && cp932.kana_lead_pairs >= 2u)
                ++cp932_lines;
            if (gbk.valid && gbk.gb2312_lead_pairs >= 2u &&
                cp932.multibyte_chars <= 1u)
                ++gbk_lines;
        }
        start = end + 1u;
    }
    return cp932_lines >= 5u && gbk_lines >= 5u;
}

enum class MixedLegacyEncoding {
    Unknown,
    Cp932,
    Tcvn3
};

bool HasTcvn3Evidence(const Tcvn3Evidence& evidence) {
    return evidence.mapped_bytes != 0u && evidence.score >= 2u;
}

bool HasStrongTcvn3Evidence(const Tcvn3Evidence& evidence) {
    return evidence.mapped_bytes >= 2u && HasTcvn3Evidence(evidence);
}

bool IsVietnameseLetter(unsigned int codepoint) {
    return (codepoint >= 0x00c0u && codepoint <= 0x00c3u) ||
           (codepoint >= 0x00c8u && codepoint <= 0x00cau) ||
           (codepoint >= 0x00ccu && codepoint <= 0x00cdu) ||
           (codepoint >= 0x00d2u && codepoint <= 0x00d5u) ||
           (codepoint >= 0x00d9u && codepoint <= 0x00dau) ||
           codepoint == 0x00ddu ||
           (codepoint >= 0x00e0u && codepoint <= 0x00e3u) ||
           (codepoint >= 0x00e8u && codepoint <= 0x00eau) ||
           (codepoint >= 0x00ecu && codepoint <= 0x00edu) ||
           (codepoint >= 0x00f2u && codepoint <= 0x00f5u) ||
           (codepoint >= 0x00f9u && codepoint <= 0x00fau) ||
           codepoint == 0x00fdu ||
           (codepoint >= 0x0102u && codepoint <= 0x0103u) ||
           (codepoint >= 0x0110u && codepoint <= 0x0111u) ||
           (codepoint >= 0x0128u && codepoint <= 0x0129u) ||
           (codepoint >= 0x0168u && codepoint <= 0x0169u) ||
           (codepoint >= 0x01a0u && codepoint <= 0x01a1u) ||
           (codepoint >= 0x01afu && codepoint <= 0x01b0u) ||
           (codepoint >= 0x1ea0u && codepoint <= 0x1ef9u);
}

bool LooksLikeVietnameseLatinText(const std::string& input,
                                  const Tcvn3Evidence& evidence) {
    if (!HasTcvn3Evidence(evidence))
        return false;

    unsigned int letters = 0;
    unsigned int vietnamese_letters = 0;
    unsigned int spaces = 0;
    for (std::string::size_type index = 0; index < input.size(); ++index) {
        const unsigned char value = static_cast<unsigned char>(input[index]);
        if (IsAsciiLetter(value)) {
            ++letters;
            continue;
        }
        if (value == ' ' || value == '\t') {
            ++spaces;
            continue;
        }
        const unsigned int codepoint = EasyRpgTcvn3DecodeByte(value);
        if (IsVietnameseLetter(codepoint)) {
            ++letters;
            ++vietnamese_letters;
            continue;
        }
        if ((value >= '0' && value <= '9') || value == '-' || value == '_' ||
            value == '.' || value == ',' || value == ':' || value == ';' ||
            value == '!' || value == '?' || value == '/' || value == '\\' ||
            value == '(' || value == ')' || value == '[' || value == ']')
            continue;
        return false;
    }
    return vietnamese_letters != 0u && letters >= 3u &&
           (spaces != 0u || evidence.mapped_bytes >= 2u);
}

MixedLegacyEncoding ClassifyMixedLegacyText(const std::string& input) {
    const Tcvn3Evidence tcvn3 = InspectTcvn3(input);
    const Cp932Evidence cp932 = InspectCp932(input);

    if (!cp932.valid)
        return HasTcvn3Evidence(tcvn3) ? MixedLegacyEncoding::Tcvn3 : MixedLegacyEncoding::Unknown;
    if (cp932.longest_multibyte_run >= 2u)
        return MixedLegacyEncoding::Cp932;
    if (HasStrongTcvn3Evidence(tcvn3))
        return MixedLegacyEncoding::Tcvn3;
    if (cp932.multibyte_chars <= 1u && LooksLikeVietnameseLatinText(input, tcvn3))
        return MixedLegacyEncoding::Tcvn3;
    if (cp932.multibyte_chars != 0u)
        return MixedLegacyEncoding::Cp932;
    return HasTcvn3Evidence(tcvn3) ? MixedLegacyEncoding::Tcvn3 : MixedLegacyEncoding::Cp932;
}

unsigned int CountTcvn3Lines(const std::string& input) {
    unsigned int tcvn3_lines = 0;
    std::string::size_type start = 0;
    while (start < input.size()) {
        std::string::size_type end = input.find('\n', start);
        if (end == std::string::npos)
            end = input.size();
        if (end > start) {
            const std::string line(input, start, end - start);
            if (ClassifyMixedLegacyText(line) == MixedLegacyEncoding::Tcvn3)
                ++tcvn3_lines;
        }
        start = end + 1;
    }
    return tcvn3_lines;
}

bool IsMixedVietnameseSample(const std::string& input) {
    if (input.size() < 64u)
        return false;
    return CountTcvn3Lines(input) >= 5u;
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
    if (input.empty())
        return std::string();

    const unsigned int source_length = static_cast<unsigned int>(input.size());
    std::vector<unsigned short> utf16(source_length);
    unsigned int utf16_length = 0;
    const int status = EasyRpgCoreCp932ToUtf16(
        reinterpret_cast<const unsigned char*>(input.data()), source_length,
        &utf16[0], source_length, &utf16_length);
    if (status != 0)
        return std::string();

    std::string output;
    output.reserve(utf16_length * 3u);
    for (unsigned int index = 0; index < utf16_length; ++index)
        AppendUtf8(output, utf16[index]);
    return output;
}

std::string GbkToUtf8(const std::string& input) {
    if (input.empty())
        return std::string();

    const unsigned int source_length = static_cast<unsigned int>(input.size());
    std::vector<unsigned short> utf16(source_length);
    unsigned int utf16_length = 0;
    const int status = EasyRpgCoreGbkToUtf16(
        reinterpret_cast<const unsigned char*>(input.data()), source_length,
        &utf16[0], source_length, &utf16_length);
    if (status != 0)
        return std::string();

    std::string output;
    output.reserve(utf16_length * 3u);
    for (unsigned int index = 0; index < utf16_length; ++index)
        AppendUtf8(output, utf16[index]);
    return output;
}

bool IsValidUtf8(const std::string& value);

bool IsStrongUtf8CjkText(const std::string& input) {
    if (!IsValidUtf8(input))
        return false;

    unsigned int three_byte_sequences = 0;
    for (std::string::size_type index = 0; index < input.size();) {
        const unsigned char first = static_cast<unsigned char>(input[index]);
        if (first < 0x80u) {
            ++index;
        } else if (first >= 0xc2u && first <= 0xdfu) {
            index += 2u;
        } else if (first >= 0xe0u && first <= 0xefu) {
            ++three_byte_sequences;
            index += 3u;
        } else if (first >= 0xf0u && first <= 0xf4u) {
            index += 4u;
        } else {
            return false;
        }
    }
    return three_byte_sequences >= 2u;
}

std::string DecodeMixedGbkCp932Text(const std::string& input) {
    const std::string cp932 = Cp932ToUtf8(input);
    const std::string gbk = GbkToUtf8(input);

    // Some RPG Maker projects contain newer UTF-8 save strings alongside a
    // legacy GBK database. A strong multi-byte UTF-8 payload is unambiguous
    // enough to preserve before trying the legacy codecs.
    if (IsStrongUtf8CjkText(input))
        return input;

    if (gbk.empty() && cp932.empty())
        return IsValidUtf8(input) ? input : std::string();
    if (gbk.empty())
        return cp932;
    if (cp932.empty())
        return gbk;

    const MixedCjkEncoding encoding = ClassifyMixedCjkText(input);
    return encoding == MixedCjkEncoding::Cp932 ? cp932 : gbk;
}

std::string PlatformLegacyToUtf8(const std::string& input, unsigned int codepage) {
    if (input.empty())
        return std::string();
    std::vector<unsigned short> utf16(input.size());
    unsigned int utf16_length = 0;
    const int status = EasyRpgPlatformLegacyToUtf16(
        codepage,
        reinterpret_cast<const unsigned char*>(input.data()),
        static_cast<unsigned int>(input.size()),
        &utf16[0],
        static_cast<unsigned int>(utf16.size()),
        &utf16_length);
    if (status != 0)
        return std::string();

    std::string output;
    output.reserve(utf16_length * 3u);
    for (unsigned int index = 0; index < utf16_length; ++index)
        AppendUtf8(output, utf16[index]);
    return output;
}

std::string DecodeMixedLegacyText(const std::string& input) {
    switch (ClassifyMixedLegacyText(input)) {
    case MixedLegacyEncoding::Cp932: {
        const std::string decoded = Cp932ToUtf8(input);
        return decoded.empty() && !input.empty() ? Tcvn3ToUtf8(input) : decoded;
    }
    case MixedLegacyEncoding::Tcvn3:
        return Tcvn3ToUtf8(input);
    case MixedLegacyEncoding::Unknown:
        for (std::string::size_type index = 0; index < input.size(); ++index) {
            if (static_cast<unsigned char>(input[index]) >= 0x80u)
                return Tcvn3ToUtf8(input);
        }
        return input;
    }
    return input;
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

std::string Utf8ToGbk(const std::string& input) {
    std::vector<unsigned short> utf16;
    if (!Utf8ToUtf16(input, utf16) || utf16.empty()) return std::string();
    std::vector<unsigned char> encoded(utf16.size() * 2u);
    unsigned int encoded_length = 0;
    const int status = EasyRpgCoreUtf16ToGbk(
        &utf16[0], static_cast<unsigned int>(utf16.size()),
        &encoded[0], static_cast<unsigned int>(encoded.size()), &encoded_length);
    if (status != 0) return std::string();
    return std::string(reinterpret_cast<const char*>(&encoded[0]), encoded_length);
}

bool ContainsJapaneseKana(const std::vector<unsigned short>& utf16) {
    for (std::vector<unsigned short>::const_iterator it = utf16.begin(); it != utf16.end(); ++it) {
        if ((*it >= 0x3040u && *it <= 0x30ffu) ||
            (*it >= 0xff61u && *it <= 0xff9fu))
            return true;
    }
    return false;
}

std::string Utf8ToMixedGbkCp932(const std::string& input) {
    const std::string cp932 = Utf8ToCp932(input);
    const std::string gbk = Utf8ToGbk(input);
    if (cp932.empty())
        return gbk;
    if (gbk.empty())
        return cp932;

    std::vector<unsigned short> utf16;
    if (Utf8ToUtf16(input, utf16) && ContainsJapaneseKana(utf16))
        return cp932;
    return gbk;
}

std::string Utf8ToPlatformLegacy(const std::string& input, unsigned int codepage) {
    std::vector<unsigned short> utf16;
    if (!Utf8ToUtf16(input, utf16) || utf16.empty())
        return std::string();
    std::vector<unsigned char> encoded(utf16.size() * 2u);
    unsigned int encoded_length = 0;
    const int status = EasyRpgPlatformUtf16ToLegacy(
        codepage,
        &utf16[0],
        static_cast<unsigned int>(utf16.size()),
        &encoded[0],
        static_cast<unsigned int>(encoded.size()),
        &encoded_length);
    if (status != 0)
        return std::string();
    return std::string(reinterpret_cast<const char*>(&encoded[0]), encoded_length);
}

void AppendDetectionSample(std::string& sample, const std::string& value) {
    const std::string::size_type limit = 65536u;
    if (value.empty() || sample.size() >= limit)
        return;
    const std::string::size_type remaining = limit - sample.size();
    const std::string::size_type count = value.size() < remaining ? value.size() : remaining;
    sample.append(value.data(), count);
    if (sample.size() < limit)
        sample.push_back('\n');
}

std::string BuildDetectionSample() {
    std::string sample;
    sample.reserve(65536u);

    AppendDetectionSample(sample, Data::terms.menu_save);
    AppendDetectionSample(sample, Data::terms.menu_quit);
    AppendDetectionSample(sample, Data::terms.new_game);
    AppendDetectionSample(sample, Data::terms.load_game);
    AppendDetectionSample(sample, Data::terms.exit_game);
    AppendDetectionSample(sample, Data::terms.status);
    AppendDetectionSample(sample, Data::terms.row);
    AppendDetectionSample(sample, Data::terms.order);
    AppendDetectionSample(sample, Data::terms.wait_on);
    AppendDetectionSample(sample, Data::terms.wait_off);
    AppendDetectionSample(sample, Data::terms.level);
    AppendDetectionSample(sample, Data::terms.health_points);
    AppendDetectionSample(sample, Data::terms.spirit_points);
    AppendDetectionSample(sample, Data::terms.normal_status);
    AppendDetectionSample(sample, Data::terms.exp_short);
    AppendDetectionSample(sample, Data::terms.lvl_short);
    AppendDetectionSample(sample, Data::terms.hp_short);
    AppendDetectionSample(sample, Data::terms.sp_short);
    AppendDetectionSample(sample, Data::terms.sp_cost);
    AppendDetectionSample(sample, Data::terms.attack);
    AppendDetectionSample(sample, Data::terms.defense);
    AppendDetectionSample(sample, Data::terms.spirit);
    AppendDetectionSample(sample, Data::terms.agility);
    AppendDetectionSample(sample, Data::terms.weapon);
    AppendDetectionSample(sample, Data::terms.shield);
    AppendDetectionSample(sample, Data::terms.armor);
    AppendDetectionSample(sample, Data::terms.helmet);
    AppendDetectionSample(sample, Data::terms.accessory);
    AppendDetectionSample(sample, Data::terms.save_game_message);
    AppendDetectionSample(sample, Data::terms.load_game_message);
    AppendDetectionSample(sample, Data::terms.file);
    AppendDetectionSample(sample, Data::terms.exit_game_message);
    AppendDetectionSample(sample, Data::terms.yes);
    AppendDetectionSample(sample, Data::terms.no);

    AppendDetectionSample(sample, Data::system.boat_name);
    AppendDetectionSample(sample, Data::system.ship_name);
    AppendDetectionSample(sample, Data::system.airship_name);
    AppendDetectionSample(sample, Data::system.title_name);
    AppendDetectionSample(sample, Data::system.gameover_name);
    AppendDetectionSample(sample, Data::system.system_name);
    AppendDetectionSample(sample, Data::system.system2_name);
    AppendDetectionSample(sample, Data::system.battletest_background);
    AppendDetectionSample(sample, Data::system.frame_name);

    for (std::vector<RPG::Actor>::const_iterator it = Data::actors.begin(); it != Data::actors.end(); ++it) {
        AppendDetectionSample(sample, it->name);
        AppendDetectionSample(sample, it->title);
        AppendDetectionSample(sample, it->character_name);
        AppendDetectionSample(sample, it->face_name);
        AppendDetectionSample(sample, it->skill_name);
    }
    for (std::vector<RPG::Skill>::const_iterator it = Data::skills.begin(); it != Data::skills.end(); ++it) {
        AppendDetectionSample(sample, it->name);
        AppendDetectionSample(sample, it->description);
        AppendDetectionSample(sample, it->using_message1);
        AppendDetectionSample(sample, it->using_message2);
    }
    for (std::vector<RPG::Item>::const_iterator it = Data::items.begin(); it != Data::items.end(); ++it) {
        AppendDetectionSample(sample, it->name);
        AppendDetectionSample(sample, it->description);
    }
    for (std::vector<RPG::State>::const_iterator it = Data::states.begin(); it != Data::states.end(); ++it) {
        AppendDetectionSample(sample, it->name);
        AppendDetectionSample(sample, it->message_actor);
        AppendDetectionSample(sample, it->message_enemy);
        AppendDetectionSample(sample, it->message_already);
        AppendDetectionSample(sample, it->message_affected);
        AppendDetectionSample(sample, it->message_recovery);
    }
    for (std::vector<RPG::Enemy>::const_iterator it = Data::enemies.begin(); it != Data::enemies.end(); ++it) {
        AppendDetectionSample(sample, it->name);
        AppendDetectionSample(sample, it->battler_name);
    }
    for (std::vector<RPG::CommonEvent>::const_iterator it = Data::commonevents.begin(); it != Data::commonevents.end(); ++it) {
        AppendDetectionSample(sample, it->name);
        for (std::vector<RPG::EventCommand>::const_iterator command = it->event_commands.begin();
             command != it->event_commands.end(); ++command)
            AppendDetectionSample(sample, command->string);
    }
    return sample;
}

void PushEncoding(std::vector<std::string>& encodings, const std::string& encoding) {
    if (encoding.empty())
        return;
    for (std::vector<std::string>::const_iterator it = encodings.begin(); it != encodings.end(); ++it) {
        if (*it == encoding)
            return;
    }
    encodings.push_back(encoding);
}

bool HasNonAsciiByte(const std::string& value) {
    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it) {
        if (static_cast<unsigned char>(*it) >= 0x80u)
            return true;
    }
    return false;
}

bool IsValidUtf8(const std::string& value) {
    for (std::string::size_type i = 0; i < value.size();) {
        const unsigned char first = static_cast<unsigned char>(value[i]);
        if (first <= 0x7fu) {
            ++i;
            continue;
        }

        unsigned int count = 0;
        unsigned char second_min = 0x80u;
        unsigned char second_max = 0xbfu;
        if (first >= 0xc2u && first <= 0xdfu) {
            count = 2u;
        } else if (first >= 0xe0u && first <= 0xefu) {
            count = 3u;
            if (first == 0xe0u) second_min = 0xa0u;
            if (first == 0xedu) second_max = 0x9fu;
        } else if (first >= 0xf0u && first <= 0xf4u) {
            count = 4u;
            if (first == 0xf0u) second_min = 0x90u;
            if (first == 0xf4u) second_max = 0x8fu;
        } else {
            return false;
        }

        if (i + count > value.size())
            return false;
        const unsigned char second = static_cast<unsigned char>(value[i + 1u]);
        if (second < second_min || second > second_max)
            return false;
        for (unsigned int offset = 2u; offset < count; ++offset) {
            const unsigned char continuation = static_cast<unsigned char>(value[i + offset]);
            if (continuation < 0x80u || continuation > 0xbfu)
                return false;
        }
        i += count;
    }
    return true;
}

std::string DetectIniTitleEncoding(INIReader& ini) {
    const std::string title = ini.Get("RPG_RT", "GameTitle", std::string());
    if (title.empty() || !HasNonAsciiByte(title))
        return std::string();

    if (IsValidUtf8(title))
        return "UTF-8";

    const std::vector<EasyRpgCharsetDetector::Guess> guesses =
        EasyRpgCharsetDetector::Detect(title);
    if (!guesses.empty() && guesses.front().confidence >= 0.80f)
        return guesses.front().encoding;
    return std::string();
}

void AppendMapDetectionSamples(std::string& sample, const std::string& database_file) {
    if (sample.size() >= 65536u)
        return;
    std::string directory;
    const std::string::size_type slash = database_file.find_last_of("/\\");
    if (slash != std::string::npos)
        directory = database_file.substr(0, slash + 1);

    for (unsigned int index = 1u; index <= 12u; ++index) {
        char digits[16];
        snprintf(digits, sizeof(digits), "%04u", index);
        std::unique_ptr<RPG::Map> map = LMU_Reader::Load(directory + "Map" + digits + ".lmu", std::string());
        if (!map)
            continue;
        for (std::vector<RPG::Event>::const_iterator event = map->events.begin();
             event != map->events.end(); ++event) {
            for (std::vector<RPG::EventPage>::const_iterator page = event->pages.begin();
                 page != event->pages.end(); ++page) {
                for (std::vector<RPG::EventCommand>::const_iterator command = page->event_commands.begin();
                     command != page->event_commands.end(); ++command)
                    AppendDetectionSample(sample, command->string);
            }
        }
        if (sample.size() >= 65536u)
            break;
    }
}
}

std::string ReaderUtil::CodepageToEncoding(int codepage) {
    if (codepage == 0) return std::string();
    if (codepage == 932) return "SHIFT_JIS";
    if (codepage == 936) return "GBK";
    if (codepage == 950) return "BIG5";
    if (codepage == 65001) return "UTF-8";
    if (codepage == 949) return "cp949";
    std::string result("CP");
    char digits[16];
    snprintf(digits, sizeof(digits), "%d", codepage);
    result += digits;
    return result;
}

std::string ReaderUtil::DetectEncoding(const std::string& database_file) {
    std::vector<std::string> encodings = DetectEncodings(database_file);
    return encodings.empty() ? std::string() : encodings.front();
}

std::vector<std::string> ReaderUtil::DetectEncodings(const std::string& database_file) {
    std::vector<std::string> encodings;
    if (database_file.empty() || !LDB_Reader::Load(database_file, std::string()))
        return encodings;

    const std::string sample = BuildDetectionSample();
    if (sample.empty())
        return encodings;

    std::string detection_sample = sample;
    AppendMapDetectionSamples(detection_sample, database_file);

    const std::vector<EasyRpgCharsetDetector::Guess> guesses =
        EasyRpgCharsetDetector::Detect(detection_sample);
    const bool mixed_vietnamese = IsMixedVietnameseSample(detection_sample);
    const bool strong_chinese = !guesses.empty() &&
        (guesses.front().encoding == "GBK" || guesses.front().encoding == "BIG5") &&
        guesses.front().confidence >= 0.80f;
    const bool mixed_gbk_cp932 = strong_chinese &&
        IsMixedGbkCp932Sample(detection_sample);

    if (mixed_vietnamese && !strong_chinese)
        PushEncoding(encodings, "EASYRPG_VI_CP932");
    if (mixed_gbk_cp932)
        PushEncoding(encodings, "EASYRPG_GBK_CP932");

    for (std::vector<EasyRpgCharsetDetector::Guess>::const_iterator it = guesses.begin();
         it != guesses.end(); ++it)
        PushEncoding(encodings, it->encoding);

    if (mixed_vietnamese && !strong_chinese)
        PushEncoding(encodings, "EASYRPG_VI_CP932");

    PushEncoding(encodings, "SHIFT_JIS");
    PushEncoding(encodings, "GBK");
    PushEncoding(encodings, "BIG5");
    PushEncoding(encodings, "UTF-8");
    return encodings;
}

std::string ReaderUtil::GetEncoding(const std::string& ini_file) {
    if (ini_file.empty()) return std::string();
    INIReader ini(ini_file);
    if (ini.ParseError() == -1) return std::string();
    const std::string value = ini.Get("EasyRPG", "Encoding", std::string());
    if (!value.empty())
        return CodepageToEncoding(std::atoi(value.c_str()));
    return DetectIniTitleEncoding(ini);
}

std::string ReaderUtil::GetLocaleEncoding() { return "SHIFT_JIS"; }

std::string ReaderUtil::Recode(const std::string& str_to_encode, const std::string& source_encoding) {
    return Recode(str_to_encode, source_encoding, "UTF-8");
}

std::string ReaderUtil::Recode(const std::string& str_to_encode, const std::string& src_enc, const std::string& dst_enc) {
    if (src_enc.empty() || dst_enc.empty() || str_to_encode.empty()) return str_to_encode;
    std::string result = str_to_encode;
    if (IsUtf8Encoding(src_enc) && IsUtf8Encoding(dst_enc)) {
        result = str_to_encode;
    }
    if (IsMixedVietnameseEncoding(src_enc) && IsUtf8Encoding(dst_enc))
        result = DecodeMixedLegacyText(str_to_encode);
    else if (IsMixedGbkCp932Encoding(src_enc) && IsUtf8Encoding(dst_enc))
        result = DecodeMixedGbkCp932Text(str_to_encode);
    else if (IsCp932Encoding(src_enc) && IsUtf8Encoding(dst_enc)) result = Cp932ToUtf8(str_to_encode);
    else if (IsGbkEncoding(src_enc) && IsUtf8Encoding(dst_enc)) result = GbkToUtf8(str_to_encode);
    else if (IsBig5Encoding(src_enc) && IsUtf8Encoding(dst_enc)) result = PlatformLegacyToUtf8(str_to_encode, 950);
    else if (IsUtf8Encoding(src_enc) && IsCp932Encoding(dst_enc)) result = Utf8ToCp932(str_to_encode);
    else if (IsUtf8Encoding(src_enc) && IsGbkEncoding(dst_enc)) result = Utf8ToGbk(str_to_encode);
    else if (IsUtf8Encoding(src_enc) && IsMixedGbkCp932Encoding(dst_enc)) result = Utf8ToMixedGbkCp932(str_to_encode);
    else if (IsUtf8Encoding(src_enc) && IsBig5Encoding(dst_enc)) result = Utf8ToPlatformLegacy(str_to_encode, 950);
#ifdef EASYRPG_ENCODING_CORPUS_TEST
    EasyRpgEncodingCorpusRecord(
        src_enc.c_str(), dst_enc.c_str(),
        reinterpret_cast<const unsigned char*>(str_to_encode.data()),
        static_cast<unsigned int>(str_to_encode.size()),
        reinterpret_cast<const unsigned char*>(result.data()),
        static_cast<unsigned int>(result.size()));
#endif
    return result;
}

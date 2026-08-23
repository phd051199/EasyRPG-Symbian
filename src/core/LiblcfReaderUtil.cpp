#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "data.h"
#include "inireader.h"
#include "ldb_reader.h"
#include "lmu_reader.h"
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
        ++current_run;
        if (current_run > result.longest_multibyte_run)
            result.longest_multibyte_run = current_run;
        index += 2u;
    }
    return result;
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
    case MixedLegacyEncoding::Cp932:
        return Cp932ToUtf8(input);
    case MixedLegacyEncoding::Tcvn3:
        return Tcvn3ToUtf8(input);
    case MixedLegacyEncoding::Unknown:
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

    if (IsMixedVietnameseSample(detection_sample)) {
        PushEncoding(encodings, "EASYRPG_VI_CP932");
    }
    unsigned int codepage = 0;
    int confidence = 0;
    if (EasyRpgPlatformDetectLegacyEncoding(
            reinterpret_cast<const unsigned char*>(sample.data()),
            static_cast<unsigned int>(sample.size()),
            &codepage, &confidence) == 0 && codepage != 0)
        PushEncoding(encodings, CodepageToEncoding(static_cast<int>(codepage)));

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
    if (value.empty()) return std::string();
    return CodepageToEncoding(std::atoi(value.c_str()));
}

std::string ReaderUtil::GetLocaleEncoding() { return "SHIFT_JIS"; }

std::string ReaderUtil::Recode(const std::string& str_to_encode, const std::string& source_encoding) {
    return Recode(str_to_encode, source_encoding, "UTF-8");
}

std::string ReaderUtil::Recode(const std::string& str_to_encode, const std::string& src_enc, const std::string& dst_enc) {
    if (src_enc.empty() || dst_enc.empty() || str_to_encode.empty()) return str_to_encode;
    if (IsUtf8Encoding(src_enc) && IsUtf8Encoding(dst_enc)) return str_to_encode;
    if (IsMixedVietnameseEncoding(src_enc) && IsUtf8Encoding(dst_enc))
        return DecodeMixedLegacyText(str_to_encode);
    if (IsCp932Encoding(src_enc) && IsUtf8Encoding(dst_enc)) return Cp932ToUtf8(str_to_encode);
    if (IsGbkEncoding(src_enc) && IsUtf8Encoding(dst_enc)) return PlatformLegacyToUtf8(str_to_encode, 936);
    if (IsBig5Encoding(src_enc) && IsUtf8Encoding(dst_enc)) return PlatformLegacyToUtf8(str_to_encode, 950);
    if (IsUtf8Encoding(src_enc) && IsCp932Encoding(dst_enc)) return Utf8ToCp932(str_to_encode);
    if (IsUtf8Encoding(src_enc) && IsGbkEncoding(dst_enc)) return Utf8ToPlatformLegacy(str_to_encode, 936);
    if (IsUtf8Encoding(src_enc) && IsBig5Encoding(dst_enc)) return Utf8ToPlatformLegacy(str_to_encode, 950);
    return str_to_encode;
}

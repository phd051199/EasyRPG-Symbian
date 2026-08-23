#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "ldb_reader.h"
#include "lmt_reader.h"
#include "lmu_reader.h"
#include "lsd_reader.h"
#include "reader_lcf.h"
#include "reader_util.h"
#include "GohuFont.h"

extern "C" int EasyRpgPlatformLegacyToUtf16(unsigned int,
                                            const unsigned char*,
                                            unsigned int,
                                            unsigned short*,
                                            unsigned int,
                                            unsigned int*) {
    return -1;
}

extern "C" int EasyRpgPlatformUtf16ToLegacy(unsigned int,
                                            const unsigned short*,
                                            unsigned int,
                                            unsigned char*,
                                            unsigned int,
                                            unsigned int*) {
    return -1;
}

struct EncodingCorpusRecord {
    std::string raw;
    std::string decoded;
};

static bool g_corpus_capture = false;
static std::vector<EncodingCorpusRecord> g_corpus_records;

extern "C" void EasyRpgEncodingCorpusRecord(const char*,
                                               const char*,
                                               const unsigned char* source,
                                               unsigned int source_length,
                                               const unsigned char* destination,
                                               unsigned int destination_length) {
    if (!g_corpus_capture)
        return;
    EncodingCorpusRecord record;
    record.raw.assign(reinterpret_cast<const char*>(source), source_length);
    record.decoded.assign(reinterpret_cast<const char*>(destination), destination_length);
    g_corpus_records.push_back(record);
}

namespace {

int g_failures = 0;

std::string LowerExtension(const std::string& path) {
    const std::string::size_type dot = path.find_last_of('.');
    if (dot == std::string::npos)
        return std::string();
    std::string extension = path.substr(dot);
    for (std::string::size_type i = 0; i < extension.size(); ++i) {
        if (extension[i] >= 'A' && extension[i] <= 'Z')
            extension[i] = static_cast<char>(extension[i] - 'A' + 'a');
    }
    return extension;
}

bool LoadLcf(const std::string& source, const std::string& encoding) {
    const std::string extension = LowerExtension(source);
    if (extension == ".ldb")
        return LDB_Reader::Load(source, encoding);
    if (extension == ".lmt")
        return LMT_Reader::Load(source, encoding);
    if (extension == ".lmu") {
        std::unique_ptr<RPG::Map> map = LMU_Reader::Load(source, encoding);
        return map.get() != 0;
    }
    if (extension == ".lsd") {
        std::unique_ptr<RPG::Save> save = LSD_Reader::Load(source, encoding);
        return save.get() != 0;
    }
    return false;
}

std::string HexEncode(const std::string& value) {
    static const char kHex[] = "0123456789abcdef";
    std::string output;
    output.resize(value.size() * 2u);
    for (std::string::size_type index = 0; index < value.size(); ++index) {
        const unsigned char byte = static_cast<unsigned char>(value[index]);
        output[index * 2u] = kHex[byte >> 4];
        output[index * 2u + 1u] = kHex[byte & 0x0fu];
    }
    return output;
}

bool SupportsCoreRoundTrip(const std::string& encoding) {
    return encoding == "SHIFT_JIS" || encoding == "GBK" || encoding == "UTF-8";
}

int DumpBatch(const std::string& encoding,
              const std::string& list_file,
              const std::string& output_file) {
    std::ifstream input(list_file.c_str());
    if (!input) {
        std::fprintf(stderr, "Unable to open batch list: %s\n", list_file.c_str());
        return 2;
    }
    std::FILE* output = std::fopen(output_file.c_str(), "wb");
    if (!output) {
        std::fprintf(stderr, "Unable to open corpus output: %s\n", output_file.c_str());
        return 2;
    }
    std::fprintf(output, "source\tordinal\traw_hex\tutf8_hex\n");

    int files = 0;
    int failures = 0;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty())
            continue;
        const std::string source = line;
        ++files;
        g_corpus_records.clear();
        g_corpus_capture = true;
        const bool loaded = LoadLcf(source, encoding);
        g_corpus_capture = false;
        if (!loaded) {
            std::fprintf(stderr, "FAIL load %s: %s\n",
                         source.c_str(), LcfReader::GetError().c_str());
            ++failures;
            continue;
        }
        for (std::size_t index = 0; index < g_corpus_records.size(); ++index) {
            const EncodingCorpusRecord& record = g_corpus_records[index];
            if (!record.raw.empty() && record.decoded.empty()) {
                std::fprintf(stderr, "FAIL decode %s text #%zu became empty\n",
                             source.c_str(), index + 1u);
                ++failures;
                continue;
            }
            if (SupportsCoreRoundTrip(encoding)) {
                const std::string encoded = ReaderUtil::Recode(record.decoded, "UTF-8", encoding);
                const std::string decoded_again = ReaderUtil::Recode(encoded, encoding, "UTF-8");
                if (encoded.empty() || decoded_again != record.decoded) {
                    std::fprintf(stderr,
                                 "FAIL semantic roundtrip %s text #%zu raw=%s\n",
                                 source.c_str(), index + 1u,
                                 HexEncode(record.raw).c_str());
                    ++failures;
                    continue;
                }
            }
            const std::string raw_hex = HexEncode(record.raw);
            const std::string utf8_hex = HexEncode(record.decoded);
            std::fprintf(output, "%s\t%zu\t%s\t%s\n",
                         source.c_str(), index + 1u,
                         raw_hex.c_str(), utf8_hex.c_str());
        }
    }
    std::fclose(output);
    std::printf("DUMPED\t%d\tFAILED\t%d\n", files, failures);
    return failures == 0 ? 0 : 1;
}

void Check(bool condition, const char* description) {
    if (condition) {
        std::printf("  PASS %s\n", description);
    } else {
        std::printf("  FAIL %s\n", description);
        ++g_failures;
    }
}

std::string HexDump(const std::string& value) {
    std::string out;
    char buffer[8];
    for (std::string::size_type i = 0; i < value.size() && out.size() < 120u; ++i) {
        std::snprintf(buffer, sizeof(buffer), "%02x ",
                      static_cast<unsigned char>(value[i]));
        out += buffer;
    }
    return out;
}

const char kVietnameseTcvn3[] = "Ch\xB5o c\xCBu, d\xB9o n\xB5y v\xC9n kh\xE1" "e ch\xF8?";
const char kJapaneseCp932[] = "\x82\xB1\x82\xF1\x82\xC9\x82\xBF\x82\xCD";

void TestRecode() {
    static const unsigned char kExpectedFGlyph[11] = {
        0x00, 0x1f, 0x01, 0x01, 0x0f, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00
    };
    const unsigned char* f_glyph = EasyRpgGohuGlyphRows('F');
    Check(f_glyph != 0 && std::memcmp(f_glyph, kExpectedFGlyph, sizeof(kExpectedFGlyph)) == 0,
          "Gohu ASCII glyph F is not corrupted");

    const std::string vietnamese(kVietnameseTcvn3);
    const std::string recoded_vn = ReaderUtil::Recode(vietnamese, "EASYRPG_VI_CP932");
    Check(recoded_vn == "Chào cậu, dạo này vẫn khỏe chứ?",
          "Recode TCVN3 dialogue to UTF-8");

    const std::string japanese(kJapaneseCp932);
    const std::string recoded_jp = ReaderUtil::Recode(japanese, "EASYRPG_VI_CP932");
    Check(recoded_jp == "\xE3\x81\x93\xE3\x82\x93\xE3\x81\xAB\xE3\x81\xA1\xE3\x81\xAF",
          "Mixed mode still decodes Japanese correctly");

    Check(ReaderUtil::Recode("K\xFC n\xA8ng", "EASYRPG_VI_CP932") == "Kỹ năng",
          "Short Vietnamese menu term: Kỹ năng");
    Check(ReaderUtil::Recode("Sinh l\xF9" "c", "EASYRPG_VI_CP932") == "Sinh lực",
          "Ambiguous TCVN3 byte pair does not become Shift-JIS: Sinh lực");
    Check(ReaderUtil::Recode("Ma l\xF9" "c", "EASYRPG_VI_CP932") == "Ma lực",
          "Ambiguous short Vietnamese term does not become Shift-JIS: Ma lực");
    Check(ReaderUtil::Recode("H\xD6 th\xE8ng", "EASYRPG_VI_CP932") == "Hệ thống",
          "Ambiguous Vietnamese term does not become Shift-JIS: Hệ thống");
    Check(ReaderUtil::Recode(" Tr\xAD\xEA" "ng h\xE4" "c...", "EASYRPG_VI_CP932") ==
              " Trường học...",
          "Vietnamese map text with consecutive CP932-looking bytes: Trường học");
    Check(ReaderUtil::Recode(" s\xE8ng s\xE3t...\\|", "EASYRPG_VI_CP932") ==
              " sống sót...\\|",
          "Vietnamese map text with message control suffix: sống sót");
    Check(ReaderUtil::Recode("\\>\\C[4]H\xD6 th\xE8ng\\C[0]\\<", "EASYRPG_VI_CP932") ==
              "\\>\\C[4]Hệ thống\\C[0]\\<",
          "Vietnamese map text with color controls: Hệ thống");
    Check(ReaderUtil::Recode("\\>Ch\xE4n \xAE\xE9 kho !\\<", "EASYRPG_VI_CP932") ==
              "\\>Chọn độ kho !\\<",
          "Mixed fallback never leaks raw TCVN3 bytes");
    Check(EasyRpgGohuGlyphRows(0x1ec7u) != 0,
          "Vietnamese font contains glyph: ệ");
    Check(EasyRpgGohuGlyphRows(0x1ed1u) != 0,
          "Vietnamese font contains glyph: ố");
    Check(ReaderUtil::Recode("HP\x8B\x5A", "EASYRPG_VI_CP932") == "HP技",
          "Single Japanese glyph beside ASCII remains Shift-JIS");
    Check(ReaderUtil::Recode("Nhân vật", "EASYRPG_VI_CP932") == "Nhân vật",
          "Mixed Vietnamese preserves UTF-8 resource names from older saves");
    Check(ReaderUtil::Recode("Kỹ năng", "UTF-8", "EASYRPG_VI_CP932") == std::string("K\xFC n\xA8ng", 7),
          "Mixed Vietnamese reverse conversion writes TCVN3 for save strings");
    Check(ReaderUtil::Recode("Kỹ năng", "UTF-8", "EASYRPG_VI_CP932") != "Kỹ năng",
          "Mixed Vietnamese save writer does not leak UTF-8 when TCVN3 is available");
    const std::string saved_vietnamese = ReaderUtil::Recode("Nhân vật", "UTF-8", "EASYRPG_VI_CP932");
    Check(ReaderUtil::Recode(saved_vietnamese, "EASYRPG_VI_CP932", "UTF-8") == "Nhân vật",
          "Mixed Vietnamese resource name roundtrips through save encoding");
    const std::string saved_japanese = ReaderUtil::Recode(recoded_jp, "UTF-8", "EASYRPG_VI_CP932");
    Check(saved_japanese == japanese &&
              ReaderUtil::Recode(saved_japanese, "EASYRPG_VI_CP932", "UTF-8") == recoded_jp,
          "Mixed Vietnamese save encoding preserves CP932 Japanese strings");
    Check(ReaderUtil::Recode("\xB0\xB2\xD0\xDB", "GBK") == "安雄",
          "GBK Chinese decodes without Symbian charconv plug-in");
    Check(ReaderUtil::Recode("\xA5\xB7\xA5\xB9\xA5\xC6\xA5\xE0", "GBK") == "システム",
          "GBK Japanese resource name decodes correctly");
    Check(ReaderUtil::Recode("安雄", "UTF-8", "GBK") == std::string("\xB0\xB2\xD0\xDB", 4),
          "GBK reverse conversion works for save writing");
    Check(ReaderUtil::Recode(std::string("\xB4\xF3\xD0\xDB", 4), "EASYRPG_GBK_CP932") == "大雄",
          "Mixed CJK decodes GBK text");
    Check(ReaderUtil::Recode(std::string("\x83\x58\x83\x6C\x95\x76\x83\x5A\x83\x62\x83\x67", 12),
                             "EASYRPG_GBK_CP932") == "スネ夫セット",
          "Mixed CJK decodes CP932 text");
    Check(ReaderUtil::Recode(std::string("\xA4\xCE\xA4\xD3\xCC\xAB\x31", 7),
                             "EASYRPG_GBK_CP932") == "のび太1",
          "Mixed CJK keeps GBK Japanese text");
    Check(ReaderUtil::Recode("oxygenメーター", "EASYRPG_GBK_CP932") == "oxygenメーター",
          "Mixed CJK preserves embedded UTF-8 save text");
    if (g_failures && !recoded_vn.empty()) {
        std::printf("    VN recode bytes: %s\n", HexDump(recoded_vn).c_str());
    }
}

void TestMixedVietnameseSaveRoundTrip() {
    const char* path = "/tmp/easyrpg_symbian_mixed_vietnamese_save_test.lsd";
    std::remove(path);

    RPG::Save save;
    save.party_location.map_id = 7;
    save.map_info.events.resize(1);
    save.map_info.events[0].ID = 3;
    save.map_info.events[0].sprite_name = "Nhân vật";
    save.map_info.events[0].sprite_id = 2;

    const bool wrote = LSD_Reader::Save(path, save, "EASYRPG_VI_CP932");
    Check(wrote, "Mixed Vietnamese LSD save writes successfully");

    std::unique_ptr<RPG::Save> loaded = LSD_Reader::Load(path, "EASYRPG_VI_CP932");
    Check(loaded.get() != 0 && loaded->party_location.map_id == 7,
          "Mixed Vietnamese LSD save restores map state");
    Check(loaded.get() != 0 && loaded->map_info.events.size() == 1u &&
              loaded->map_info.events[0].sprite_name == "Nhân vật" &&
              loaded->map_info.events[0].sprite_id == 2,
          "Mixed Vietnamese LSD save restores map event sprite resource");

    std::remove(path);
}

int TestDetection(const char* game_dir, const char* expected_first, bool expect_mixed) {
    const std::string ini_file = std::string(game_dir) + "/RPG_RT.ini";
    const std::string ini_encoding = ReaderUtil::GetEncoding(ini_file);
    if (!ini_encoding.empty()) {
        std::printf("  ini encoding: %s\n", ini_encoding.c_str());
        if (expected_first != 0)
            Check(ini_encoding == expected_first,
                  (std::string("RPG_RT.ini encoding of ") + game_dir + " == " + expected_first).c_str());
    } else {
        std::printf("  ini encoding: <no charset signal>\n");
    }

    const std::string database = std::string(game_dir) + "/RPG_RT.ldb";
    const std::vector<std::string> encodings = ReaderUtil::DetectEncodings(database);
    if (encodings.empty()) {
        std::printf("  FAIL DetectEncodings returned nothing for %s\n", game_dir);
        ++g_failures;
        return 1;
    }
    std::printf("  encodings[%zu]:", encodings.size());
    for (std::size_t i = 0; i < encodings.size() && i < 4u; ++i)
        std::printf(" %s", encodings[i].c_str());
    std::printf("\n");
    if (expected_first != 0)
        Check(encodings.front() == expected_first,
              (std::string("first encoding of ") + game_dir + " == " + expected_first).c_str());
    const bool is_mixed = encodings.front() == "EASYRPG_VI_CP932";
    Check(is_mixed == expect_mixed,
          (std::string(game_dir) + (expect_mixed ? " is a mixed Vietnamese game" : " is not a mixed Vietnamese game")).c_str());
    return 0;
}

}

int main(int argc, char** argv) {
    if (argc == 3 && std::strcmp(argv[1], "--ini-detect") == 0) {
        const std::string encoding = ReaderUtil::GetEncoding(argv[2]);
        std::printf("%s\n", encoding.empty() ? "-" : encoding.c_str());
        return 0;
    }
    if (argc == 3 && std::strcmp(argv[1], "--detect") == 0) {
        const std::vector<std::string> encodings = ReaderUtil::DetectEncodings(argv[2]);
        if (encodings.empty())
            return 1;
        std::printf("%s\n", encodings.front().c_str());
        return 0;
    }
    if (argc == 5 && std::strcmp(argv[1], "--dump-batch") == 0)
        return DumpBatch(argv[2], argv[3], argv[4]);

    if (argc < 2) {
        std::fprintf(stderr, "Usage: %s <game_dir[:expect_first][:!mixed|:mixed]>...\n", argv[0]);
        return 2;
    }
    std::printf("Recode unit tests:\n");
    TestRecode();
    TestMixedVietnameseSaveRoundTrip();

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        std::string dir = arg;
        std::string expected_first;
        bool expect_mixed = false;
        std::string::size_type sep = arg.find(':');
        if (sep != std::string::npos) {
            dir = arg.substr(0, sep);
            arg.erase(0, sep + 1);
            sep = arg.find(':');
            const std::string expect = sep == std::string::npos ? arg : arg.substr(0, sep);
            const std::string mixed = sep == std::string::npos ? std::string() : arg.substr(sep + 1);
            if (expect != "-" && !expect.empty())
                expected_first = expect;
            expect_mixed = mixed == "mixed";
        }
        std::printf("Detection: %s\n", dir.c_str());
        TestDetection(dir.c_str(), expected_first.empty() ? 0 : expected_first.c_str(), expect_mixed);
    }
    std::printf(g_failures == 0 ? "ALL PASS\n" : "FAILURES: %d\n", g_failures);
    return g_failures == 0 ? 0 : 1;
}

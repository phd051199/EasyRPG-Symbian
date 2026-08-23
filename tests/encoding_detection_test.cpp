#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "ldb_reader.h"
#include "reader_util.h"

extern "C" int EasyRpgPlatformDetectLegacyEncoding(const unsigned char*,
                                                   unsigned int,
                                                   unsigned int* aCodepage,
                                                   int* aConfidence) {
    *aCodepage = 932;
    *aConfidence = 0;
    return 0;
}

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

namespace {

int g_failures = 0;

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
    Check(ReaderUtil::Recode("HP\x8B\x5A", "EASYRPG_VI_CP932") == "HP技",
          "Single Japanese glyph beside ASCII remains Shift-JIS");
    if (g_failures && !recoded_vn.empty()) {
        std::printf("    VN recode bytes: %s\n", HexDump(recoded_vn).c_str());
    }
}

int TestDetection(const char* game_dir, const char* expected_first, bool expect_mixed) {
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
    if (argc < 2) {
        std::fprintf(stderr, "Usage: %s <game_dir[:expect_first][:!mixed|:mixed]>...\n", argv[0]);
        return 2;
    }
    std::printf("Recode unit tests:\n");
    TestRecode();

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

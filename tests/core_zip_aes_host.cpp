#include "CoreZipCrypto.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <zlib.h>

namespace
{
uint16_t Read16(const std::vector<unsigned char>& data, std::size_t offset)
{
    return static_cast<uint16_t>(data[offset]) |
           static_cast<uint16_t>(data[offset + 1] << 8);
}

uint32_t Read32(const std::vector<unsigned char>& data, std::size_t offset)
{
    return static_cast<uint32_t>(data[offset]) |
           (static_cast<uint32_t>(data[offset + 1]) << 8) |
           (static_cast<uint32_t>(data[offset + 2]) << 16) |
           (static_cast<uint32_t>(data[offset + 3]) << 24);
}

bool FindAesExtra(const std::vector<unsigned char>& data,
                  std::size_t offset,
                  std::size_t length,
                  unsigned int& strength,
                  unsigned int& actualMethod)
{
    const std::size_t end = offset + length;
    while (offset + 4 <= end)
    {
        const uint16_t id = Read16(data, offset);
        const uint16_t size = Read16(data, offset + 2);
        offset += 4;
        if (offset + size > end)
            return false;
        if (id == 0x9901 && size >= 7)
        {
            strength = data[offset + 4];
            actualMethod = Read16(data, offset + 5);
            return true;
        }
        offset += size;
    }
    return false;
}
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "usage: core_zip_aes_host archive.zip password expected-file\n";
        return 2;
    }

    std::ifstream input(argv[1], std::ios::binary);
    std::vector<unsigned char> zip((std::istreambuf_iterator<char>(input)),
                                   std::istreambuf_iterator<char>());
    if (zip.size() < 22)
        return 3;

    std::size_t eocd = zip.size() - 22;
    while (eocd > 0 && Read32(zip, eocd) != 0x06054b50u)
        --eocd;
    if (Read32(zip, eocd) != 0x06054b50u)
        return 4;

    const std::size_t central = Read32(zip, eocd + 16);
    const unsigned int totalEntries = Read16(zip, eocd + 10);
    if (Read32(zip, central) != 0x02014b50u)
        return 5;

    std::string targetName(argv[3]);
    const std::size_t slash = targetName.find_last_of("/\\");
    if (slash != std::string::npos)
        targetName.erase(0, slash + 1);

    std::size_t selected = 0;
    std::size_t cursor = central;
    for (unsigned int entry = 0; entry < totalEntries; ++entry)
    {
        if (cursor + 46 > zip.size() || Read32(zip, cursor) != 0x02014b50u)
            return 5;
        const unsigned int nameLengthHere = Read16(zip, cursor + 28);
        const unsigned int extraLengthHere = Read16(zip, cursor + 30);
        const unsigned int commentLengthHere = Read16(zip, cursor + 32);
        if (cursor + 46 + nameLengthHere + extraLengthHere + commentLengthHere > zip.size())
            return 5;
        const std::string name(reinterpret_cast<const char*>(zip.data() + cursor + 46),
                               nameLengthHere);
        if (name.size() >= targetName.size() &&
            name.compare(name.size() - targetName.size(), targetName.size(), targetName) == 0)
        {
            selected = cursor;
            break;
        }
        cursor += 46 + nameLengthHere + extraLengthHere + commentLengthHere;
    }
    if (selected == 0)
        return 6;

    const unsigned int method = Read16(zip, selected + 10);
    const unsigned int compressedSize = Read32(zip, selected + 20);
    const unsigned int uncompressedSize = Read32(zip, selected + 24);
    const unsigned int nameLength = Read16(zip, selected + 28);
    const unsigned int extraLength = Read16(zip, selected + 30);
    const std::size_t localOffset = Read32(zip, selected + 42);
    if (method != 99u)
        return 6;

    unsigned int strength = 0;
    unsigned int actualMethod = 0;
    if (!FindAesExtra(zip, selected + 46 + nameLength, extraLength, strength, actualMethod))
        return 7;

    if (Read32(zip, localOffset) != 0x04034b50u)
        return 8;
    const unsigned int localNameLength = Read16(zip, localOffset + 26);
    const unsigned int localExtraLength = Read16(zip, localOffset + 28);
    const std::size_t payloadOffset = localOffset + 30 + localNameLength + localExtraLength;
    if (payloadOffset + compressedSize > zip.size())
        return 9;

    std::vector<unsigned char> plain(compressedSize);
    unsigned int plainSize = 0;
    const std::string password(argv[2]);
    const int result = EasyRpgCoreWinZipAesDecrypt(
        zip.data() + payloadOffset,
        compressedSize,
        reinterpret_cast<const unsigned char*>(password.data()),
        static_cast<unsigned int>(password.size()),
        strength,
        plain.data(),
        static_cast<unsigned int>(plain.size()),
        &plainSize);
    if (result != 0)
    {
        std::cerr << "AES decrypt failed: " << result << "\n";
        return 10;
    }

    std::ifstream expectedInput(argv[3], std::ios::binary);
    std::vector<unsigned char> expected((std::istreambuf_iterator<char>(expectedInput)),
                                        std::istreambuf_iterator<char>());
    if (uncompressedSize != expected.size())
        return 11;

    std::vector<unsigned char> decoded;
    if (actualMethod == 0u)
    {
        decoded.assign(plain.begin(), plain.begin() + plainSize);
    }
    else if (actualMethod == 8u)
    {
        decoded.resize(uncompressedSize);
        z_stream stream = {};
        stream.next_in = plain.data();
        stream.avail_in = plainSize;
        stream.next_out = decoded.data();
        stream.avail_out = uncompressedSize;
        if (inflateInit2(&stream, -MAX_WBITS) != Z_OK)
            return 12;
        const int inflateResult = inflate(&stream, Z_FINISH);
        inflateEnd(&stream);
        if (inflateResult != Z_STREAM_END || stream.total_out != uncompressedSize)
            return 13;
    }
    else
    {
        std::cerr << "unsupported actual ZIP method in regression: " << actualMethod << "\n";
        return 14;
    }

    if (decoded != expected)
        return 15;

    std::cout << "WinZip AES regression OK: strength=" << strength
              << " method=" << actualMethod
              << " encrypted=" << compressedSize
              << " compressedPlain=" << plainSize
              << " decoded=" << decoded.size() << "\n";
    return 0;
}

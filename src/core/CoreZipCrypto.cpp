#include "CoreZipCrypto.h"

namespace
{
struct Sha1Context
{
    unsigned int state[5];
    unsigned long long bitCount;
    unsigned char buffer[64];
    unsigned int bufferSize;
};

unsigned int RotateLeft(unsigned int aValue, unsigned int aBits)
{
    return (aValue << aBits) | (aValue >> (32u - aBits));
}

void Sha1Transform(Sha1Context& aContext, const unsigned char* aBlock)
{
    unsigned int words[80];
    for (unsigned int index = 0; index < 16; ++index)
    {
        const unsigned int offset = index * 4u;
        words[index] =
            (static_cast<unsigned int>(aBlock[offset]) << 24) |
            (static_cast<unsigned int>(aBlock[offset + 1]) << 16) |
            (static_cast<unsigned int>(aBlock[offset + 2]) << 8) |
            static_cast<unsigned int>(aBlock[offset + 3]);
    }
    for (unsigned int index = 16; index < 80; ++index)
        words[index] = RotateLeft(words[index - 3] ^ words[index - 8] ^ words[index - 14] ^ words[index - 16], 1);

    unsigned int a = aContext.state[0];
    unsigned int b = aContext.state[1];
    unsigned int c = aContext.state[2];
    unsigned int d = aContext.state[3];
    unsigned int e = aContext.state[4];

    for (unsigned int index = 0; index < 80; ++index)
    {
        unsigned int functionValue = 0;
        unsigned int constant = 0;
        if (index < 20)
        {
            functionValue = (b & c) | ((~b) & d);
            constant = 0x5a827999u;
        }
        else if (index < 40)
        {
            functionValue = b ^ c ^ d;
            constant = 0x6ed9eba1u;
        }
        else if (index < 60)
        {
            functionValue = (b & c) | (b & d) | (c & d);
            constant = 0x8f1bbcdcu;
        }
        else
        {
            functionValue = b ^ c ^ d;
            constant = 0xca62c1d6u;
        }

        const unsigned int temporary =
            RotateLeft(a, 5) + functionValue + e + constant + words[index];
        e = d;
        d = c;
        c = RotateLeft(b, 30);
        b = a;
        a = temporary;
    }

    aContext.state[0] += a;
    aContext.state[1] += b;
    aContext.state[2] += c;
    aContext.state[3] += d;
    aContext.state[4] += e;
}

void Sha1Init(Sha1Context& aContext)
{
    aContext.state[0] = 0x67452301u;
    aContext.state[1] = 0xefcdab89u;
    aContext.state[2] = 0x98badcfeu;
    aContext.state[3] = 0x10325476u;
    aContext.state[4] = 0xc3d2e1f0u;
    aContext.bitCount = 0;
    aContext.bufferSize = 0;
}

void Sha1Update(Sha1Context& aContext, const unsigned char* aData, unsigned int aLength)
{
    if (!aData || aLength == 0)
        return;

    aContext.bitCount += static_cast<unsigned long long>(aLength) * 8ull;
    unsigned int position = 0;
    while (position < aLength)
    {
        unsigned int copyLength = 64u - aContext.bufferSize;
        const unsigned int remaining = aLength - position;
        if (copyLength > remaining)
            copyLength = remaining;

        for (unsigned int index = 0; index < copyLength; ++index)
            aContext.buffer[aContext.bufferSize + index] = aData[position + index];
        aContext.bufferSize += copyLength;
        position += copyLength;

        if (aContext.bufferSize == 64u)
        {
            Sha1Transform(aContext, aContext.buffer);
            aContext.bufferSize = 0;
        }
    }
}

void Sha1Final(Sha1Context& aContext, unsigned char aDigest[20])
{
    aContext.buffer[aContext.bufferSize++] = 0x80u;
    if (aContext.bufferSize > 56u)
    {
        while (aContext.bufferSize < 64u)
            aContext.buffer[aContext.bufferSize++] = 0;
        Sha1Transform(aContext, aContext.buffer);
        aContext.bufferSize = 0;
    }
    while (aContext.bufferSize < 56u)
        aContext.buffer[aContext.bufferSize++] = 0;

    for (unsigned int index = 0; index < 8; ++index)
    {
        const unsigned int shift = (7u - index) * 8u;
        aContext.buffer[56u + index] =
            static_cast<unsigned char>((aContext.bitCount >> shift) & 0xffu);
    }
    Sha1Transform(aContext, aContext.buffer);

    for (unsigned int index = 0; index < 5; ++index)
    {
        aDigest[index * 4u] = static_cast<unsigned char>(aContext.state[index] >> 24);
        aDigest[index * 4u + 1] = static_cast<unsigned char>(aContext.state[index] >> 16);
        aDigest[index * 4u + 2] = static_cast<unsigned char>(aContext.state[index] >> 8);
        aDigest[index * 4u + 3] = static_cast<unsigned char>(aContext.state[index]);
    }
}

void HmacSha1(const unsigned char* aKey,
              unsigned int aKeyLength,
              const unsigned char* aData,
              unsigned int aDataLength,
              unsigned char aDigest[20])
{
    unsigned char keyBlock[64];
    for (unsigned int index = 0; index < 64; ++index)
        keyBlock[index] = 0;

    if (aKeyLength > 64u)
    {
        Sha1Context keyHash;
        Sha1Init(keyHash);
        Sha1Update(keyHash, aKey, aKeyLength);
        unsigned char hashedKey[20];
        Sha1Final(keyHash, hashedKey);
        for (unsigned int index = 0; index < 20; ++index)
            keyBlock[index] = hashedKey[index];
    }
    else
    {
        for (unsigned int index = 0; index < aKeyLength; ++index)
            keyBlock[index] = aKey[index];
    }

    unsigned char innerPad[64];
    unsigned char outerPad[64];
    for (unsigned int index = 0; index < 64; ++index)
    {
        innerPad[index] = static_cast<unsigned char>(keyBlock[index] ^ 0x36u);
        outerPad[index] = static_cast<unsigned char>(keyBlock[index] ^ 0x5cu);
    }

    unsigned char innerDigest[20];
    Sha1Context inner;
    Sha1Init(inner);
    Sha1Update(inner, innerPad, 64);
    Sha1Update(inner, aData, aDataLength);
    Sha1Final(inner, innerDigest);

    Sha1Context outer;
    Sha1Init(outer);
    Sha1Update(outer, outerPad, 64);
    Sha1Update(outer, innerDigest, 20);
    Sha1Final(outer, aDigest);
}

void Pbkdf2HmacSha1(const unsigned char* aPassword,
                    unsigned int aPasswordLength,
                    const unsigned char* aSalt,
                    unsigned int aSaltLength,
                    unsigned int aIterations,
                    unsigned char* aOutput,
                    unsigned int aOutputLength)
{
    unsigned int outputPosition = 0;
    unsigned int blockIndex = 1;
    while (outputPosition < aOutputLength)
    {
        unsigned char saltBlock[20];
        for (unsigned int index = 0; index < aSaltLength; ++index)
            saltBlock[index] = aSalt[index];
        saltBlock[aSaltLength] = static_cast<unsigned char>(blockIndex >> 24);
        saltBlock[aSaltLength + 1] = static_cast<unsigned char>(blockIndex >> 16);
        saltBlock[aSaltLength + 2] = static_cast<unsigned char>(blockIndex >> 8);
        saltBlock[aSaltLength + 3] = static_cast<unsigned char>(blockIndex);

        unsigned char u[20];
        unsigned char accumulator[20];
        HmacSha1(aPassword, aPasswordLength, saltBlock, aSaltLength + 4u, u);
        for (unsigned int index = 0; index < 20; ++index)
            accumulator[index] = u[index];

        for (unsigned int iteration = 1; iteration < aIterations; ++iteration)
        {
            unsigned char next[20];
            HmacSha1(aPassword, aPasswordLength, u, 20, next);
            for (unsigned int index = 0; index < 20; ++index)
            {
                u[index] = next[index];
                accumulator[index] ^= next[index];
            }
        }

        unsigned int copyLength = aOutputLength - outputPosition;
        if (copyLength > 20u)
            copyLength = 20u;
        for (unsigned int index = 0; index < copyLength; ++index)
            aOutput[outputPosition + index] = accumulator[index];
        outputPosition += copyLength;
        ++blockIndex;
    }
}

static const unsigned char KSbox[256] =
{
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

unsigned char Xtime(unsigned char aValue)
{
    return static_cast<unsigned char>((aValue << 1) ^ ((aValue & 0x80u) ? 0x1bu : 0u));
}

void AddRoundKey(unsigned char aState[16], const unsigned char* aRoundKey)
{
    for (unsigned int index = 0; index < 16; ++index)
        aState[index] ^= aRoundKey[index];
}

void SubBytes(unsigned char aState[16])
{
    for (unsigned int index = 0; index < 16; ++index)
        aState[index] = KSbox[aState[index]];
}

void ShiftRows(unsigned char aState[16])
{
    unsigned char temporary = 0;
    temporary = aState[1]; aState[1] = aState[5]; aState[5] = aState[9]; aState[9] = aState[13]; aState[13] = temporary;
    temporary = aState[2]; aState[2] = aState[10]; aState[10] = temporary;
    temporary = aState[6]; aState[6] = aState[14]; aState[14] = temporary;
    temporary = aState[15]; aState[15] = aState[11]; aState[11] = aState[7]; aState[7] = aState[3]; aState[3] = temporary;
}

void MixColumns(unsigned char aState[16])
{
    for (unsigned int column = 0; column < 4; ++column)
    {
        unsigned char* value = aState + column * 4u;
        const unsigned char total = static_cast<unsigned char>(value[0] ^ value[1] ^ value[2] ^ value[3]);
        const unsigned char original = value[0];
        value[0] ^= static_cast<unsigned char>(total ^ Xtime(static_cast<unsigned char>(value[0] ^ value[1])));
        value[1] ^= static_cast<unsigned char>(total ^ Xtime(static_cast<unsigned char>(value[1] ^ value[2])));
        value[2] ^= static_cast<unsigned char>(total ^ Xtime(static_cast<unsigned char>(value[2] ^ value[3])));
        value[3] ^= static_cast<unsigned char>(total ^ Xtime(static_cast<unsigned char>(value[3] ^ original)));
    }
}

unsigned char Rcon(unsigned int aIndex)
{
    unsigned char value = 1;
    if (aIndex == 0)
        return 0;
    while (aIndex > 1)
    {
        value = Xtime(value);
        --aIndex;
    }
    return value;
}

bool ExpandAesKey(const unsigned char* aKey,
                  unsigned int aKeyLength,
                  unsigned char* aExpandedKey,
                  unsigned int& aRounds)
{
    unsigned int nk = 0;
    if (aKeyLength == 16u) { nk = 4u; aRounds = 10u; }
    else if (aKeyLength == 24u) { nk = 6u; aRounds = 12u; }
    else if (aKeyLength == 32u) { nk = 8u; aRounds = 14u; }
    else return false;

    const unsigned int words = 4u * (aRounds + 1u);
    for (unsigned int index = 0; index < aKeyLength; ++index)
        aExpandedKey[index] = aKey[index];

    unsigned int wordIndex = nk;
    unsigned char temp[4];
    while (wordIndex < words)
    {
        for (unsigned int index = 0; index < 4; ++index)
            temp[index] = aExpandedKey[(wordIndex - 1u) * 4u + index];

        unsigned int remainder = 0;
        unsigned int quotient = 0;
        if (nk == 4u)
        {
            remainder = wordIndex & 3u;
            quotient = wordIndex >> 2;
        }
        else if (nk == 8u)
        {
            remainder = wordIndex & 7u;
            quotient = wordIndex >> 3;
        }
        else
        {
            remainder = wordIndex;
            while (remainder >= 6u)
            {
                remainder -= 6u;
                ++quotient;
            }
        }

        if (remainder == 0u)
        {
            const unsigned char first = temp[0];
            temp[0] = KSbox[temp[1]];
            temp[1] = KSbox[temp[2]];
            temp[2] = KSbox[temp[3]];
            temp[3] = KSbox[first];
            temp[0] ^= Rcon(quotient);
        }
        else if (nk > 6u && remainder == 4u)
        {
            for (unsigned int index = 0; index < 4; ++index)
                temp[index] = KSbox[temp[index]];
        }

        for (unsigned int index = 0; index < 4; ++index)
            aExpandedKey[wordIndex * 4u + index] =
                static_cast<unsigned char>(aExpandedKey[(wordIndex - nk) * 4u + index] ^ temp[index]);
        ++wordIndex;
    }
    return true;
}

void AesEncryptBlock(const unsigned char* aExpandedKey,
                     unsigned int aRounds,
                     const unsigned char aInput[16],
                     unsigned char aOutput[16])
{
    unsigned char state[16];
    for (unsigned int index = 0; index < 16; ++index)
        state[index] = aInput[index];
    AddRoundKey(state, aExpandedKey);

    for (unsigned int round = 1; round < aRounds; ++round)
    {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, aExpandedKey + round * 16u);
    }
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, aExpandedKey + aRounds * 16u);

    for (unsigned int index = 0; index < 16; ++index)
        aOutput[index] = state[index];
}

bool ConstantTimeEqual(const unsigned char* aLeft,
                       const unsigned char* aRight,
                       unsigned int aLength)
{
    unsigned int difference = 0;
    for (unsigned int index = 0; index < aLength; ++index)
        difference |= static_cast<unsigned int>(aLeft[index] ^ aRight[index]);
    return difference == 0;
}
}

extern "C" int EasyRpgCoreWinZipAesDecrypt(const unsigned char* aEncrypted,
                                             unsigned int aEncryptedSize,
                                             const unsigned char* aPassword,
                                             unsigned int aPasswordSize,
                                             unsigned int aStrength,
                                             unsigned char* aPlain,
                                             unsigned int aPlainCapacity,
                                             unsigned int* aPlainSize)
{
    if (!aPlainSize)
        return -1;
    *aPlainSize = 0;
    if (!aEncrypted || !aPassword || !aPlain)
        return -1;

    unsigned int keyLength = 0;
    unsigned int saltLength = 0;
    if (aStrength == 1u) { keyLength = 16u; saltLength = 8u; }
    else if (aStrength == 2u) { keyLength = 24u; saltLength = 12u; }
    else if (aStrength == 3u) { keyLength = 32u; saltLength = 16u; }
    else return -2;

    if (aEncryptedSize < saltLength + 2u + 10u)
        return -3;
    const unsigned int encryptedDataLength = aEncryptedSize - saltLength - 2u - 10u;
    if (encryptedDataLength > aPlainCapacity)
        return -4;

    unsigned char derived[66];
    const unsigned int derivedLength = keyLength * 2u + 2u;
    Pbkdf2HmacSha1(aPassword,
                   aPasswordSize,
                   aEncrypted,
                   saltLength,
                   1000u,
                   derived,
                   derivedLength);

    const unsigned char* encryptionKey = derived;
    const unsigned char* authenticationKey = derived + keyLength;
    const unsigned char* passwordVerifier = derived + keyLength * 2u;
    const unsigned char* storedVerifier = aEncrypted + saltLength;
    if (!ConstantTimeEqual(passwordVerifier, storedVerifier, 2u))
        return -5;

    const unsigned char* encryptedData = aEncrypted + saltLength + 2u;
    const unsigned char* storedAuthentication = encryptedData + encryptedDataLength;
    unsigned char authentication[20];
    HmacSha1(authenticationKey,
             keyLength,
             encryptedData,
             encryptedDataLength,
             authentication);
    if (!ConstantTimeEqual(authentication, storedAuthentication, 10u))
        return -6;

    unsigned char expandedKey[240];
    unsigned int rounds = 0;
    if (!ExpandAesKey(encryptionKey, keyLength, expandedKey, rounds))
        return -7;

    unsigned int position = 0;
    unsigned int nonce = 1u;
    while (position < encryptedDataLength)
    {
        unsigned char counterBlock[16];
        for (unsigned int index = 0; index < 16; ++index)
            counterBlock[index] = 0;
        counterBlock[0] = static_cast<unsigned char>(nonce);
        counterBlock[1] = static_cast<unsigned char>(nonce >> 8);
        counterBlock[2] = static_cast<unsigned char>(nonce >> 16);
        counterBlock[3] = static_cast<unsigned char>(nonce >> 24);

        unsigned char keystream[16];
        AesEncryptBlock(expandedKey, rounds, counterBlock, keystream);

        unsigned int blockLength = encryptedDataLength - position;
        if (blockLength > 16u)
            blockLength = 16u;
        for (unsigned int index = 0; index < blockLength; ++index)
            aPlain[position + index] = static_cast<unsigned char>(encryptedData[position + index] ^ keystream[index]);
        position += blockLength;
        ++nonce;
    }

    *aPlainSize = encryptedDataLength;
    return 0;
}

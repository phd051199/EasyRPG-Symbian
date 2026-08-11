#ifndef EASYRPG_CORE_ZIP_CRYPTO_H
#define EASYRPG_CORE_ZIP_CRYPTO_H

#ifdef __cplusplus
extern "C" {
#endif

int EasyRpgCoreWinZipAesDecrypt(const unsigned char* aEncrypted,
                                unsigned int aEncryptedSize,
                                const unsigned char* aPassword,
                                unsigned int aPasswordSize,
                                unsigned int aStrength,
                                unsigned char* aPlain,
                                unsigned int aPlainCapacity,
                                unsigned int* aPlainSize);

#ifdef __cplusplus
}
#endif

#endif

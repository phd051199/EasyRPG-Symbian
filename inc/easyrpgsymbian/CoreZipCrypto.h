#ifndef EASYRPG_SYMBIAN_CORE_ZIP_CRYPTO_H
#define EASYRPG_SYMBIAN_CORE_ZIP_CRYPTO_H

#ifdef __cplusplus
extern "C" {
#endif

// Decrypt one WinZip AES member payload. aEncrypted contains the complete
// member payload stored after the local header: salt, 2-byte password
// verifier, encrypted compressed bytes and the 10-byte authentication code.
// aStrength is 1/2/3 for AES-128/192/256. On success aPlain contains only the
// decrypted compressed bytes (ready for Stored/Deflate handling).
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

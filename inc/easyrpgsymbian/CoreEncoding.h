#ifndef EASYRPG_SYMBIAN_CORE_ENCODING_H
#define EASYRPG_SYMBIAN_CORE_ENCODING_H

#ifdef __cplusplus
extern "C" {
#endif

int EasyRpgCoreCp932ToUtf16(const unsigned char* aSource,
                            unsigned int aSourceLength,
                            unsigned short* aDestination,
                            unsigned int aDestinationCapacity,
                            unsigned int* aDestinationLength);

#ifdef __cplusplus
}
#endif

#endif

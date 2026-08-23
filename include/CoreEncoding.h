#ifndef EASYRPG_CORE_ENCODING_H
#define EASYRPG_CORE_ENCODING_H

#ifdef __cplusplus
extern "C" {
#endif

int EasyRpgCoreCp932ToUtf16(const unsigned char* aSource,
                            unsigned int aSourceLength,
                            unsigned short* aDestination,
                            unsigned int aDestinationCapacity,
                            unsigned int* aDestinationLength);
int EasyRpgCoreUtf16ToCp932(const unsigned short* aSource,
                            unsigned int aSourceLength,
                            unsigned char* aDestination,
                            unsigned int aDestinationCapacity,
                            unsigned int* aDestinationLength);
int EasyRpgCoreGbkToUtf16(const unsigned char* aSource,
                          unsigned int aSourceLength,
                          unsigned short* aDestination,
                          unsigned int aDestinationCapacity,
                          unsigned int* aDestinationLength);
int EasyRpgCoreUtf16ToGbk(const unsigned short* aSource,
                          unsigned int aSourceLength,
                          unsigned char* aDestination,
                          unsigned int aDestinationCapacity,
                          unsigned int* aDestinationLength);
int EasyRpgPlatformLegacyToUtf16(unsigned int aCodepage,
                                 const unsigned char* aSource,
                                 unsigned int aSourceLength,
                                 unsigned short* aDestination,
                                 unsigned int aDestinationCapacity,
                                 unsigned int* aDestinationLength);
int EasyRpgPlatformUtf16ToLegacy(unsigned int aCodepage,
                                 const unsigned short* aSource,
                                 unsigned int aSourceLength,
                                 unsigned char* aDestination,
                                 unsigned int aDestinationCapacity,
                                 unsigned int* aDestinationLength);

#ifdef __cplusplus
}
#endif

#endif

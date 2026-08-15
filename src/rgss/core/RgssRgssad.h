#ifndef RGSS_RGSSAD_H
#define RGSS_RGSSAD_H

/*
 * Portable reader for RPG Maker's encrypted archives (.rgssad/.rgss2a v1,
 * .rgss3a v3). Byte-exact port of mkxp-z src/crypto/rgssad.cpp:
 *  v1: magic = 0xDEADCAFE; every consumed u32/name byte XORs with advance()
 *      (old value, then magic = magic*7 + 3); payload XOR is a byte stream
 *      keyed by the advancing magic, 4 bytes per step, relative to entry start.
 *  v3: baseKey = first u32 after the 8-byte header, *9 + 3; entry table
 *      fields XOR with baseKey (which never advances); name bytes XOR with
 *      (baseKey >> 8*(i%4)) & 0xFF; payload uses the per-entry magic.
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RgssadEntry
{
    long offset;
    unsigned long size;
    unsigned int startMagic;
} RgssadEntry;

typedef struct RgssadArchive RgssadArchive;

/* Parses the whole archive held in memory (caller keeps ownership of aData).
 * Returns 0 on success, negative on a malformed archive. */
int RgssadParse(const unsigned char* aData, long aSize, RgssadArchive** aOut);
void RgssadFree(RgssadArchive* aArchive);

/* Case-insensitive lookup with '/'-separated, normalized game paths.
 * Returns the entry count for enumeration, or -1 if not found via aOut=NULL. */
int RgssadEntryCount(const RgssadArchive* aArchive);
/* Returns 0 and fills *aOut when found; name is the normalized entry name. */
int RgssadFind(const RgssadArchive* aArchive, const char* aPath,
               RgssadEntry* aOut);

/* Decrypts one entry into a malloc'd buffer; returns size or -1. */
long RgssadReadEntry(const unsigned char* aArchiveData,
                     const RgssadEntry* aEntry,
                     unsigned char** aOut);

#ifdef __cplusplus
}
#endif

#endif

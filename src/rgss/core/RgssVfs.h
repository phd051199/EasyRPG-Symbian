#ifndef RGSS_VFS_H
#define RGSS_VFS_H

/*
 * Unified game-file resolver: a mounted .rgssad/.rgss2a/.rgss3a archive
 * shadows files served by the host (zip/loose dir on Symbian via CZipVfs,
 * loose dir on the macOS harness). Portable; the archive is loaded through
 * the host FileRead op so the Symbian side needs no rgssad code.
 */

#include "RgssRgssad.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Attempts to read + parse <root>.rgssad/.rgss2a/.rgss3a (whichever exists)
 * through the host. Returns 0 when an archive was mounted, -1 otherwise. */
int RgssVfsMountRgssad(void);
void RgssVfsUnmount(void);

/* Returns 1 when the file exists (rgssad or host). */
int RgssVfsExists(const char* aPath);

/* Whole-file read; returns size or -1, *aOut is malloc'd (caller frees). */
long RgssVfsRead(const char* aPath, unsigned char** aOut);

#ifdef __cplusplus
}
#endif

#endif

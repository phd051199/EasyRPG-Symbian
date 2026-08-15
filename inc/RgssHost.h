#ifndef EASYRPG_RGSS_HOST_H
#define EASYRPG_RGSS_HOST_H

/*
 * The entire platform boundary of the portable RGSS runtime (src/rgss).
 * Two implementations: host/RgssHostSdl.cpp (macOS test harness) and
 * src/platform/symbian/RgssHostSymbian.cpp (E72). Mirrors the style of
 * ScriptVmBridge.h: pure C, set once at startup before RgssRun().
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Input bitmask: bit i is set when RGSS button code i is held.
 * RGSS1 codes: 2=Down 4=Left 6=Right 8=Up, 11..18=A..R,
 * 21=Shift 22=Ctrl 23=Alt, 25..29=F5..F9.
 */
typedef struct RgssHostVtable
{
    void* ctx;

    /* Create window/buffers for a native-resolution frame; 0 on success. */
    int (*Init)(void* ctx, int nativeWidth, int nativeHeight);
    /* Present one native-resolution RGB565 frame; host downscales itself. */
    void (*Present)(void* ctx, const unsigned short* frame, int width, int height);

    unsigned int (*PollInput)(void* ctx);

    /* Audio. Paths are game-relative; return 0 on success. */
    int (*BgmPlay)(void* ctx, const char* path, int volume, int pitch);
    void (*BgmStop)(void* ctx);
    void (*BgmFade)(void* ctx, int ms);
    int (*BgmPos)(void* ctx); /* milliseconds, -1 unknown */
    void (*BgmPause)(void* ctx);
    void (*BgmResume)(void* ctx);
    int (*BgsPlay)(void* ctx, const char* path, int volume, int pitch);
    void (*BgsStop)(void* ctx);
    void (*BgsFade)(void* ctx, int ms);
    int (*MePlay)(void* ctx, const char* path, int volume, int pitch);
    void (*MeStop)(void* ctx);
    void (*MeFade)(void* ctx, int ms);
    int (*SePlay)(void* ctx, const char* path, int volume, int pitch);
    void (*SeStop)(void* ctx);
    void (*AudioUpdate)(void* ctx);
    void (*AudioShutdown)(void* ctx);

    /* Filesystem, whole-file reads. FileRead returns size or -1; *out is
     * malloc'd and owned by the caller. Paths are game-relative, '/'-separated. */
    int (*FileExists)(void* ctx, const char* path);
    long (*FileRead)(void* ctx, const char* path, unsigned char** out);
    long (*FileWrite)(void* ctx, const char* path, const unsigned char* data, long size);

    void (*Log)(void* ctx, const char* message);

    unsigned int (*TicksMs)(void* ctx);
    void (*DelayMs)(void* ctx, unsigned int ms);

    /* Writable overlay operations. Appended to keep the existing vtable
     * prefix stable for host implementations built before save-file support. */
    int (*FileDelete)(void* ctx, const char* path);
    int (*FileRename)(void* ctx, const char* oldPath, const char* newPath);
} RgssHostVtable;

/* Portable side: install the host implementation, then call RgssRun. */
void RgssHostSet(const RgssHostVtable* vtable);
const RgssHostVtable* RgssHost(void);

#ifdef __cplusplus
}
#endif

#endif

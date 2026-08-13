#ifndef EASYRPG_RGSS_SCRIPT_ARCHIVE_BRIDGE_H
#define EASYRPG_RGSS_SCRIPT_ARCHIVE_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssScriptArchiveResult
{
    ERgssScriptArchiveOk = 0,
    ERgssScriptArchiveErrorInvalidArgument = -120,
    ERgssScriptArchiveErrorRead = -121,
    ERgssScriptArchiveErrorCorrupt = -122,
    ERgssScriptArchiveErrorUnsupported = -123,
    ERgssScriptArchiveErrorCapacity = -124,
    ERgssScriptArchiveErrorNoMemory = -125
};

typedef int (*EasyRpgRgssScriptArchiveReadFn)(void* context,
                                               int offset,
                                               void* buffer,
                                               int bytes);
typedef void* (*EasyRpgRgssScriptArchiveAllocFn)(void* context, unsigned int bytes);
typedef void (*EasyRpgRgssScriptArchiveFreeFn)(void* context, void* memory);

typedef struct EasyRpgRgssScriptArchiveReader
{
    void* context;
    int size;
    EasyRpgRgssScriptArchiveReadFn read;
} EasyRpgRgssScriptArchiveReader;

typedef struct EasyRpgRgssScriptEntry
{
    int id;
    int name_offset;
    int name_length;
    int data_offset;
    int data_length;
} EasyRpgRgssScriptEntry;

int EasyRpgRgssScriptArchiveInspect(const EasyRpgRgssScriptArchiveReader* reader,
                                     EasyRpgRgssScriptEntry* entries,
                                     int capacity,
                                     int* count);
int EasyRpgRgssScriptArchiveValidate(const EasyRpgRgssScriptArchiveReader* reader,
                                     int* count);
int EasyRpgRgssScriptArchiveInflate(const EasyRpgRgssScriptArchiveReader* reader,
                                    const EasyRpgRgssScriptEntry* entry,
                                    EasyRpgRgssScriptArchiveAllocFn allocate,
                                    EasyRpgRgssScriptArchiveFreeFn release,
                                    void* allocation_context,
                                    int max_output_bytes,
                                    unsigned char** output,
                                    int* output_length);

#ifdef __cplusplus
}
#endif

#endif

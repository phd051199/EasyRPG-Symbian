#ifndef EASYRPG_DIRENT_BRIDGE_H
#define EASYRPG_DIRENT_BRIDGE_H

#include <stddef.h>

struct __sFILE;
typedef struct __sFILE FILE;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EasyRpgDir EasyRpgDir;

typedef struct EasyRpgDirent {
    unsigned long d_fileno;
    unsigned short d_namlen;
    char* d_name;
    unsigned char d_type;
} EasyRpgDirent;

EasyRpgDir* EasyRpgOpendir(const char* path);
EasyRpgDirent* EasyRpgReaddir(EasyRpgDir* dir);
int EasyRpgClosedir(EasyRpgDir* dir);
FILE* EasyRpgFopen(const char* path, const char* mode);
int EasyRpgFclose(FILE* stream);
size_t EasyRpgFread(void* buffer, size_t size, size_t count, FILE* stream);
int EasyRpgFseek(FILE* stream, long offset, int origin);
long EasyRpgFtell(FILE* stream);
int EasyRpgFeof(FILE* stream);
int EasyRpgFerror(FILE* stream);

#ifdef __cplusplus
}

#ifdef __SYMBIAN32__
class CZipVfs;
void EasyRpgSetVirtualProject(CZipVfs* aVfs, const TDesC& aRoot);
void EasyRpgClearVirtualProject();
void EasyRpgBuildVirtualPathL(const TDesC& aRelativePath, TDes& aPath);
TBool EasyRpgIsVirtualPath(const TDesC& aPath);
HBufC8* EasyRpgReadVirtualFileLC(const TDesC& aPath, TInt aMaxBytes);
#endif
#endif

#endif

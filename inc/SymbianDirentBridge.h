#ifndef EASYRPG_SYMBIAN_DIRENT_BRIDGE_H
#define EASYRPG_SYMBIAN_DIRENT_BRIDGE_H

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

#ifdef __cplusplus
}

#ifdef __SYMBIAN32__
class CZipVfs;
void EasyRpgSetVirtualProject(CZipVfs* aVfs, const TDesC& aRoot);
void EasyRpgClearVirtualProject();
#endif
#endif

#endif

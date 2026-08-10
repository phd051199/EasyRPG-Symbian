#ifndef EASYRPG_SYMBIAN_LIBC_COMPAT_SYS_STAT_H
#define EASYRPG_SYMBIAN_LIBC_COMPAT_SYS_STAT_H

struct stat {
    unsigned long st_mode;
};

#define S_IFDIR 0040000
#define S_ISDIR(mode) (((mode) & S_IFDIR) != 0)

#ifdef __cplusplus
extern "C" {
#endif

int stat(const char* path, struct stat* buffer);
int lstat(const char* path, struct stat* buffer);

#ifdef __cplusplus
}
#endif

#endif

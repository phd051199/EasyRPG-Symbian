#ifndef EASYRPG_SYMBIAN_LIBC_COMPAT_UNISTD_H
#define EASYRPG_SYMBIAN_LIBC_COMPAT_UNISTD_H

#define F_OK 0

#ifdef __cplusplus
extern "C" {
#endif

int access(const char* path, int mode);

#ifdef __cplusplus
}
#endif

#endif

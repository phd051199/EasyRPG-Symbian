#ifndef EASYRPG_SYMBIAN_LIBC_COMPAT_ERRNO_H
#define EASYRPG_SYMBIAN_LIBC_COMPAT_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

int* __errno(void);

#ifdef __cplusplus
}
#endif

#define errno (*__errno())
#define ERANGE 34

#endif

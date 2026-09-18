#ifndef EASYRPG_SYMBIAN_LIBC_COMPAT_ERRNO_H
#define EASYRPG_SYMBIAN_LIBC_COMPAT_ERRNO_H

#ifdef __cplusplus
extern "C"
{
#endif

    int *__errno(void);

#ifdef __cplusplus
}
#endif

#define errno (*__errno())
#ifndef ENOENT
#define ENOENT 2
#endif
#ifndef EIO
#define EIO 5
#endif
#ifndef ENOMEM
#define ENOMEM 12
#endif
#ifndef EACCES
#define EACCES 13
#endif
#ifndef EEXIST
#define EEXIST 17
#endif
#ifndef EINVAL
#define EINVAL 22
#endif
#ifndef ENOSPC
#define ENOSPC 28
#endif
#ifndef ERANGE
#define ERANGE 34
#endif

#endif

#ifndef EASYRPG_SYMBIAN_LIBC_COMPAT_SYS_TYPES_H
#define EASYRPG_SYMBIAN_LIBC_COMPAT_SYS_TYPES_H

#include <stddef.h>

#ifndef _SSIZE_T_DECLARED
typedef long ssize_t;
#define _SSIZE_T_DECLARED
#endif

#ifndef _MODE_T_DECLARED
typedef unsigned short mode_t;
#define _MODE_T_DECLARED
#endif

#ifndef _PID_T_DECLARED
typedef int pid_t;
#define _PID_T_DECLARED
#endif

#ifndef _OFF_T_DECLARED
typedef long off_t;
#define _OFF_T_DECLARED
#endif

#endif

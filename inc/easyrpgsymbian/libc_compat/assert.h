#ifndef EASYRPG_SYMBIAN_LIBC_COMPAT_ASSERT_H
#define EASYRPG_SYMBIAN_LIBC_COMPAT_ASSERT_H

#ifdef NDEBUG
#define assert(expr) ((void)0)
#else
#include <stdlib.h>
#define assert(expr) ((expr) ? (void)0 : abort())
#endif

#endif

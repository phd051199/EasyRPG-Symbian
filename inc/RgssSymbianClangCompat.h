#ifndef EASYRPG_RGSS_SYMBIAN_CLANG_COMPAT_H
#define EASYRPG_RGSS_SYMBIAN_CLANG_COMPAT_H

/* Minimal compiler personality used when Clang emits ARM objects that are
 * linked by the S60 GCCE toolchain. Do not include gcce/gcce.h here: its
 * custom va_list macros describe GCCE's ABI and break Clang's builtins. */
#define __NO_CLASS_CONSTS__
#define IMPORT_C
#define EXPORT_C
#define __SOFTFP
#define __wchar_t_defined 1
#define __offsetof__(value) (value)

typedef long long Int64;
typedef unsigned long long Uint64;

#include <sys/types.h>

#endif

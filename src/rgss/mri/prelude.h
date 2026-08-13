#ifndef EASYRPG_RUBY_SYMBIAN_PRELUDE_H
#define EASYRPG_RUBY_SYMBIAN_PRELUDE_H

#include <sys/select.h>

#undef va_start
#undef va_arg
#undef va_end
#define va_start(ap, parmN) __builtin_va_start((ap).__ap, parmN)
#define va_arg(ap, type) __builtin_va_arg((ap).__ap, type)
#define va_end(ap) __builtin_va_end((ap).__ap)

#ifndef SIG_SETMASK
#define SIG_SETMASK 0
#endif
#ifndef SIG_BLOCK
#define SIG_BLOCK 1
#endif
#ifndef SIG_UNBLOCK
#define SIG_UNBLOCK 2
#endif

#endif

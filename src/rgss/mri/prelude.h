#ifndef EASYRPG_RUBY_SYMBIAN_PRELUDE_H
#define EASYRPG_RUBY_SYMBIAN_PRELUDE_H

#ifdef __clang__
typedef long long Int64;
typedef unsigned long long Uint64;
#define __wchar_t_defined
#endif

#include <sys/select.h>

struct __siginfo
{
    void* si_addr;
};
typedef struct __siginfo siginfo_t;
typedef unsigned int nfds_t;
struct pollfd
{
    int fd;
    short events;
    short revents;
};
#ifndef POLLIN
#define POLLIN 0x0001
#endif
#ifndef POLLOUT
#define POLLOUT 0x0004
#endif
#ifndef POLLERR
#define POLLERR 0x0008
#endif
#ifndef POLLHUP
#define POLLHUP 0x0010
#endif
#ifndef POLLNVAL
#define POLLNVAL 0x0020
#endif
#ifndef sa_sigaction
#define sa_sigaction __sigaction_u.__sa_sigaction
#endif
#ifdef __cplusplus
extern "C" {
#endif
int poll(struct pollfd* fds, nfds_t count, int timeout);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
int EasyRpgRubyStrcmp(const char* left, const char* right);
#ifdef __cplusplus
}
#endif
#define strcmp EasyRpgRubyStrcmp

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

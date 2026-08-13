#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef environ
#undef environ
#endif

extern "C"
{
typedef void (*TRgssSignalHandler)(int);

#ifdef environ
#undef environ
#endif
char* gEasyRpgEmptyEnvironment[] = {0};
char** gEasyRpgMriEnvironment __asm__("environ") = gEasyRpgEmptyEnvironment;

int EasyRpgRubyStrcmp(const char* left, const char* right)
{
    const unsigned char* a = reinterpret_cast<const unsigned char*>(left);
    const unsigned char* b = reinterpret_cast<const unsigned char*>(right);
    while (*a != 0 && *a == *b)
    {
        ++a;
        ++b;
    }
    return static_cast<int>(*a) - static_cast<int>(*b);
}

TRgssSignalHandler signal(int, TRgssSignalHandler handler)
{
    return handler;
}

int kill(pid_t, int)
{
    errno = ENOSYS;
    return -1;
}

pid_t fork(void)
{
    errno = ENOSYS;
    return -1;
}

int execv(const char*, char* const*)
{
    errno = ENOSYS;
    return -1;
}

int execl(const char*, const char*, ...)
{
    errno = ENOSYS;
    return -1;
}

int fchown(int, uid_t, gid_t)
{
    errno = ENOSYS;
    return -1;
}

int flock(int, int)
{
    return 0;
}

int setmode(int, int)
{
    return 0;
}

char* crypt(const char*, const char*)
{
    errno = ENOSYS;
    return 0;
}

void rb_thread_start_timer(void)
{
}

void rb_thread_stop_timer(void)
{
}

TRgssSignalHandler ruby_nativethread_signal(int, TRgssSignalHandler handler)
{
    return handler;
}
}

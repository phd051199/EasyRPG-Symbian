#include "mri/prelude.h"

#include <e32std.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef environ
#undef environ
#endif

namespace
{
volatile unsigned int gRubyAtomicLock = 0;

void RubyAtomicAcquire()
{
    unsigned int value;
    do
    {
        __asm__ volatile("swp %0, %2, [%1]"
                         : "=&r"(value)
                         : "r"(&gRubyAtomicLock), "r"(1u)
                         : "memory");
    }
    while (value != 0);
}

void RubyAtomicRelease()
{
    __asm__ volatile("" ::: "memory");
    gRubyAtomicLock = 0;
}

struct TRubyGcPage
{
    TRubyGcPage* next;
};

extern "C" void EasyRpgRgssVmTrace(const char* aText);
extern "C" void EasyRpgRgssVmTraceHeap(const char* aText);

pthread_mutex_t gRubyGcPageLock = PTHREAD_MUTEX_INITIALIZER;
TRubyGcPage* gRubyGcFreePages = NULL;
unsigned char* gRubyGcPageNext = NULL;
unsigned char* gRubyGcPageEnd = NULL;
int gRubyGcBlockCount = 0;
}

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

void* easyrpg_symbian_gc_page_alloc(size_t alignment, size_t size)
{
    if (alignment != 16384 || size != 16384)
        return NULL;
    pthread_mutex_lock(&gRubyGcPageLock);
    if (gRubyGcFreePages)
    {
        TRubyGcPage* page = gRubyGcFreePages;
        gRubyGcFreePages = page->next;
        pthread_mutex_unlock(&gRubyGcPageLock);
        return page;
    }
    if (!gRubyGcPageNext || gRubyGcPageNext + size > gRubyGcPageEnd)
    {
        const size_t blockSize = 1024 * 1024;
        void* raw = malloc(blockSize + alignment - 1);
        if (!raw)
        {
            EasyRpgRgssVmTraceHeap("gc page block alloc failed");
            pthread_mutex_unlock(&gRubyGcPageLock);
            return NULL;
        }
        const unsigned long address = reinterpret_cast<unsigned long>(raw);
        const unsigned long aligned = (address + alignment - 1) & ~(static_cast<unsigned long>(alignment) - 1);
        gRubyGcPageNext = reinterpret_cast<unsigned char*>(aligned);
        gRubyGcPageEnd = gRubyGcPageNext + blockSize;
        ++gRubyGcBlockCount;
        if ((gRubyGcBlockCount & 7) == 1)
        {
            char line[64];
            snprintf(line, sizeof(line), "gc page blocks=%d", gRubyGcBlockCount);
            EasyRpgRgssVmTraceHeap(line);
        }
    }
    void* page = gRubyGcPageNext;
    gRubyGcPageNext += size;
    pthread_mutex_unlock(&gRubyGcPageLock);
    return page;
}

void easyrpg_symbian_gc_page_free(void* memory)
{
    if (!memory)
        return;
    pthread_mutex_lock(&gRubyGcPageLock);
    TRubyGcPage* page = static_cast<TRubyGcPage*>(memory);
    page->next = gRubyGcFreePages;
    gRubyGcFreePages = page;
    pthread_mutex_unlock(&gRubyGcPageLock);
}

TRgssSignalHandler ruby_posix_signal(int, TRgssSignalHandler handler)
{
    return handler;
}

TRgssSignalHandler posix_signal(int, TRgssSignalHandler handler)
{
    return handler;
}

void rb_encdb_declare(const char* name);
int rb_encdb_alias(const char* alias, const char* original);

void Init_enc(void)
{
    rb_encdb_declare("ASCII-8BIT");
    rb_encdb_declare("US-ASCII");
    rb_encdb_declare("UTF-8");
    rb_encdb_alias("BINARY", "ASCII-8BIT");
    rb_encdb_alias("ASCII", "US-ASCII");
}

int raise(int)
{
    return 0;
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

int execve(const char*, char* const*, char* const*)
{
    errno = ENOSYS;
    return -1;
}

int execl(const char*, const char*, ...)
{
    errno = ENOSYS;
    return -1;
}

int execle(const char*, const char*, ...)
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

#ifdef sigaddset
#undef sigaddset
#endif
int sigaddset(sigset_t* set, int sig)
{
    if (!set || sig <= 0 || sig > 128)
    {
        errno = EINVAL;
        return -1;
    }
    const int index = sig - 1;
    set->__bits[index >> 5] |= 1u << (index & 31);
    return 0;
}

int sigfillset(sigset_t* set)
{
    if (!set)
    {
        errno = EINVAL;
        return -1;
    }
    memset(set, 0xff, sizeof(*set));
    return 0;
}

int sigprocmask(int, const sigset_t*, sigset_t* oldSet)
{
    if (oldSet)
        memset(oldSet, 0, sizeof(*oldSet));
    return 0;
}

int pthread_sigmask(int, const sigset_t*, sigset_t* oldSet)
{
    if (oldSet)
        memset(oldSet, 0, sizeof(*oldSet));
    return 0;
}

int pthread_kill(pthread_t, int)
{
    return 0;
}

int getrusage(int, struct rusage* usage)
{
    if (!usage)
    {
        errno = EINVAL;
        return -1;
    }
    memset(usage, 0, sizeof(*usage));
    return 0;
}

extern "C" int easyrpg_symbian_stack_bounds(void** aAddress, size_t* aSize)
{
    if (!aAddress || !aSize)
        return EINVAL;
    TThreadStackInfo info;
    if (RThread().StackInfo(info) != KErrNone || info.iBase <= info.iLimit)
        return EINVAL;
    *aAddress = reinterpret_cast<void*>(info.iBase);
    *aSize = static_cast<size_t>(info.iBase - info.iLimit);
    return 0;
}

void* mmap(void*, size_t length, int, int, int fd, off_t)
{
    if (fd != -1)
    {
        errno = ENOSYS;
        return reinterpret_cast<void*>(-1);
    }
    const size_t pageSize = 4096;
    const size_t overhead = pageSize - 1 + sizeof(void*);
    if (length > static_cast<size_t>(-1) - overhead)
    {
        errno = ENOMEM;
        return reinterpret_cast<void*>(-1);
    }
    void* memory = malloc(length + overhead);
    if (!memory)
    {
        errno = ENOMEM;
        return reinterpret_cast<void*>(-1);
    }
    const size_t base = reinterpret_cast<size_t>(memory) + sizeof(void*);
    const size_t aligned = (base + pageSize - 1) & ~(pageSize - 1);
    reinterpret_cast<void**>(aligned)[-1] = memory;
    return reinterpret_cast<void*>(aligned);
}

int mprotect(const void*, size_t, int)
{
    return 0;
}

int munmap(void* address, size_t)
{
    if (!address || address == reinterpret_cast<void*>(-1))
    {
        errno = EINVAL;
        return -1;
    }
    free(reinterpret_cast<void**>(address)[-1]);
    return 0;
}

int poll(struct pollfd* fds, nfds_t count, int timeout)
{
    if (!fds && count > 0)
    {
        errno = EINVAL;
        return -1;
    }

    fd_set readSet;
    fd_set writeSet;
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    int maxFd = -1;
    for (nfds_t index = 0; index < count; ++index)
    {
        fds[index].revents = 0;
        if (fds[index].fd < 0)
            continue;
        if (fds[index].events & POLLIN)
            FD_SET(fds[index].fd, &readSet);
        if (fds[index].events & POLLOUT)
            FD_SET(fds[index].fd, &writeSet);
        if (fds[index].fd > maxFd)
            maxFd = fds[index].fd;
    }

    timeval wait;
    timeval* waitPtr = NULL;
    if (timeout >= 0)
    {
        wait.tv_sec = timeout / 1000;
        wait.tv_usec = (timeout % 1000) * 1000;
        waitPtr = &wait;
    }

    const int result = select(maxFd + 1, &readSet, &writeSet, NULL, waitPtr);
    if (result <= 0)
        return result;

    int ready = 0;
    for (nfds_t index = 0; index < count; ++index)
    {
        if (fds[index].fd < 0)
            continue;
        if ((fds[index].events & POLLIN) && FD_ISSET(fds[index].fd, &readSet))
            fds[index].revents |= POLLIN;
        if ((fds[index].events & POLLOUT) && FD_ISSET(fds[index].fd, &writeSet))
            fds[index].revents |= POLLOUT;
        if (fds[index].revents != 0)
            ++ready;
    }
    return ready;
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

unsigned int easyrpg_atomic_fetch_add(volatile unsigned int* value, unsigned int delta)
{
    RubyAtomicAcquire();
    const unsigned int old = *value;
    *value = old + delta;
    RubyAtomicRelease();
    return old;
}

unsigned int easyrpg_atomic_fetch_sub(volatile unsigned int* value, unsigned int delta)
{
    RubyAtomicAcquire();
    const unsigned int old = *value;
    *value = old - delta;
    RubyAtomicRelease();
    return old;
}

unsigned int easyrpg_atomic_or(volatile unsigned int* value, unsigned int bits)
{
    RubyAtomicAcquire();
    const unsigned int old = *value;
    *value = old | bits;
    RubyAtomicRelease();
    return old;
}

unsigned int easyrpg_atomic_exchange(volatile unsigned int* value, unsigned int replacement)
{
    RubyAtomicAcquire();
    const unsigned int old = *value;
    *value = replacement;
    RubyAtomicRelease();
    return old;
}

unsigned int easyrpg_atomic_cas(volatile unsigned int* value,
                                unsigned int expected,
                                unsigned int replacement)
{
    RubyAtomicAcquire();
    const unsigned int old = *value;
    if (old == expected)
        *value = replacement;
    RubyAtomicRelease();
    return old;
}

void* easyrpg_atomic_ptr_exchange(void* volatile* value, void* replacement)
{
    RubyAtomicAcquire();
    void* old = *value;
    *value = replacement;
    RubyAtomicRelease();
    return old;
}

void* easyrpg_atomic_ptr_cas(void* volatile* value, void* expected, void* replacement)
{
    RubyAtomicAcquire();
    void* old = *value;
    if (old == expected)
        *value = replacement;
    RubyAtomicRelease();
    return old;
}
}

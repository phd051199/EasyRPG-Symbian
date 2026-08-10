#ifndef EASYRPG_SYMBIAN_LIBC_COMPAT_STDLIB_H
#define EASYRPG_SYMBIAN_LIBC_COMPAT_STDLIB_H

#include <stddef.h>

#ifndef RAND_MAX
#define RAND_MAX 0x7fffffff
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { int quot; int rem; } div_t;
typedef struct { long quot; long rem; } ldiv_t;
typedef struct { long long quot; long long rem; } lldiv_t;

double atof(const char*);
int atoi(const char*);
long atol(const char*);
long long atoll(const char*);
double strtod(const char*, char**);
float strtof(const char*, char**);
long double strtold(const char*, char**);
long strtol(const char*, char**, int);
long long strtoll(const char*, char**, int);
unsigned long strtoul(const char*, char**, int);
unsigned long long strtoull(const char*, char**, int);
int rand(void);
void srand(unsigned int);
void* calloc(size_t, size_t);
void free(void*);
void* malloc(size_t);
void* realloc(void*, size_t);
void abort(void);
int atexit(void (*)(void));
void exit(int);
void _Exit(int);
char* getenv(const char*);
int system(const char*);
void* bsearch(const void*, const void*, size_t, size_t, int (*)(const void*, const void*));
void qsort(void*, size_t, size_t, int (*)(const void*, const void*));
int abs(int);
long labs(long);
long long llabs(long long);
div_t div(int, int);
ldiv_t ldiv(long, long);
lldiv_t lldiv(long long, long long);
int mblen(const char*, size_t);
int mbtowc(wchar_t*, const char*, size_t);
int wctomb(char*, wchar_t);
size_t mbstowcs(wchar_t*, const char*, size_t);
size_t wcstombs(char*, const wchar_t*, size_t);
int at_quick_exit(void (*)(void));
void quick_exit(int);
void* aligned_alloc(size_t, size_t);

#ifdef __cplusplus
}
#endif

#endif

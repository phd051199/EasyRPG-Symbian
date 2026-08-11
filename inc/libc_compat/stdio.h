#ifndef EASYRPG_SYMBIAN_LIBC_COMPAT_STDIO_H
#define EASYRPG_SYMBIAN_LIBC_COMPAT_STDIO_H

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

struct __sFILE;
typedef struct __sFILE FILE;
typedef long long fpos_t;

FILE* __stdin(void);
FILE* __stdout(void);
FILE* __stderr(void);

#define stdin (__stdin())
#define stdout (__stdout())
#define stderr (__stderr())

#ifndef EOF
#define EOF (-1)
#endif
#ifndef BUFSIZ
#define BUFSIZ 1024
#endif
#ifndef FILENAME_MAX
#define FILENAME_MAX 256
#endif
#ifndef FOPEN_MAX
#define FOPEN_MAX 20
#endif
#ifndef L_tmpnam
#define L_tmpnam 1024
#endif
#ifndef TMP_MAX
#define TMP_MAX 308915776
#endif
#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif
#ifndef _IOFBF
#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2
#endif

int remove(const char*);
int rename(const char*, const char*);
FILE* tmpfile(void);
char* tmpnam(char*);
int fclose(FILE*);
int fflush(FILE*);
FILE* fopen(const char*, const char*);
FILE* freopen(const char*, const char*, FILE*);
void setbuf(FILE*, char*);
int setvbuf(FILE*, char*, int, size_t);
int fprintf(FILE*, const char*, ...);
int fscanf(FILE*, const char*, ...);
int printf(const char*, ...);
int scanf(const char*, ...);
int snprintf(char*, size_t, const char*, ...);
int sprintf(char*, const char*, ...);
int sscanf(const char*, const char*, ...);
int vfprintf(FILE*, const char*, va_list);
int vfscanf(FILE*, const char*, va_list);
int vprintf(const char*, va_list);
int vscanf(const char*, va_list);
int vsnprintf(char*, size_t, const char*, va_list);
int vsprintf(char*, const char*, va_list);
int vsscanf(const char*, const char*, va_list);
int fgetc(FILE*);
char* fgets(char*, int, FILE*);
int fputc(int, FILE*);
int fputs(const char*, FILE*);
int getc(FILE*);
int getchar(void);
char* gets(char*);
int putc(int, FILE*);
int putchar(int);
int puts(const char*);
int ungetc(int, FILE*);
size_t fread(void*, size_t, size_t, FILE*);
size_t fwrite(const void*, size_t, size_t, FILE*);
int fgetpos(FILE*, fpos_t*);
int fseek(FILE*, long, int);
int fsetpos(FILE*, const fpos_t*);
long ftell(FILE*);
void rewind(FILE*);
void clearerr(FILE*);
int feof(FILE*);
int ferror(FILE*);
void perror(const char*);

#ifdef __cplusplus
}
#endif

#endif

#ifndef EASYRPG_RUBY_SYMBIAN_CONFIG_H
#define EASYRPG_RUBY_SYMBIAN_CONFIG_H

#ifndef IMPORT_C
#define IMPORT_C
#endif
#ifndef EXPORT_C
#define EXPORT_C
#endif
#include <sys/select.h>

#define STDC_HEADERS 1
#define HAVE_LONG_LONG 1
#define HAVE_OFF_T 1
#define SIZEOF_INT 4
#define SIZEOF_SHORT 2
#define SIZEOF_LONG 4
#define SIZEOF_LONG_LONG 8
#define SIZEOF___INT64 0
#define SIZEOF_OFF_T 8
#define SIZEOF_VOIDP 4
#define SIZEOF_FLOAT 4
#define SIZEOF_DOUBLE 8
#define SIZEOF_TIME_T 4
#define SIZEOF_RLIM_T 8
#define rb_pid_t pid_t
#define rb_gid_t gid_t
#define rb_uid_t uid_t
#define HAVE_PROTOTYPES 1
#define TOKEN_PASTE(x,y) x##y
#define HAVE_STDARG_PROTOTYPES 1
#define NORETURN(x) __attribute__((noreturn)) x
#define NOINLINE(x) __attribute__((noinline)) x
#define RUBY_EXTERN extern
#define HAVE_DIRENT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_WAIT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_UNISTD_H 1
#define HAVE_LIMITS_H 1
#define HAVE_SYS_FILE_H 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_FCNTL_H 1
#define HAVE_SYS_FCNTL_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TIMES_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_PWD_H 1
#define HAVE_GRP_H 1
#define HAVE_UTIME_H 1
#define HAVE_MEMORY_H 1
#define HAVE_SYS_RESOURCE_H 1
#define HAVE_FLOAT_H 1
#define HAVE_TIME_H 1
#define HAVE_STDINT_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_INT8_T 1
#define SIZEOF_INT8_T 1
#define HAVE_UINT8_T 1
#define SIZEOF_UINT8_T 1
#define HAVE_INT16_T 1
#define SIZEOF_INT16_T 2
#define HAVE_UINT16_T 1
#define SIZEOF_UINT16_T 2
#define HAVE_INT32_T 1
#define SIZEOF_INT32_T 4
#define HAVE_UINT32_T 1
#define SIZEOF_UINT32_T 4
#define HAVE_INT64_T 1
#define SIZEOF_INT64_T 8
#define HAVE_UINT64_T 1
#define SIZEOF_UINT64_T 8
#define HAVE_STRUCT_TIMEZONE 1
#define GETGROUPS_T gid_t
#define RETSIGTYPE void
#define HAVE_ALLOCA 1
#define alloca __builtin_alloca
#define HAVE_DUP2 1
#define HAVE_MEMMOVE 1
#define HAVE_STRCASECMP 1
#define HAVE_STRNCASECMP 1
#define HAVE_STRERROR 1
#define HAVE_STRFTIME 1
#define HAVE_STRCHR 1
#define HAVE_STRSTR 1
#define HAVE_STRTOUL 1
#define HAVE_VSNPRINTF 1
#define HAVE_ISNAN 1
#define HAVE_ISINF 1
#define HAVE_HYPOT 1
#define HAVE_ACOSH 1
#define HAVE_ERF 1
#define HAVE_FMOD 1
#define HAVE_WAITPID 1
#define HAVE_FSYNC 1
#define HAVE_GETCWD 1
#define HAVE_FTRUNCATE 1
#define HAVE_FCNTL 1
#define HAVE_LSTAT 1
#define HAVE_GETPGRP 1
#define HAVE_GETGROUPS 1
#define HAVE_SYSCONF 1
#define HAVE_MKTIME 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_COSH 1
#define HAVE_SINH 1
#define HAVE_TANH 1
#define HAVE_SETENV 1
#define HAVE_UNSETENV 1
#define RUBY_SETJMP(env) setjmp(env)
#define RUBY_LONGJMP(env,val) longjmp(env,val)
#define RUBY_JMP_BUF jmp_buf
#define RSHIFT(x,y) ((x)>>(int)(y))
#define STACK_GROW_DIRECTION -1
#define DEFAULT_KCODE KCODE_NONE
#define DLEXT_MAXLEN 4
#define DLEXT ".so"
#define RUBY_LIB ""
#define RUBY_SITE_LIB ""
#define RUBY_SITE_LIB2 ""
#define RUBY_VENDOR_LIB ""
#define RUBY_VENDOR_LIB2 ""
#define RUBY_PLATFORM "arm-symbian"
#define RUBY_ARCHLIB ""
#define RUBY_SITE_ARCHLIB ""
#define RUBY_VENDOR_ARCHLIB ""
#define RUBY_SEARCH_PATH ""

#endif

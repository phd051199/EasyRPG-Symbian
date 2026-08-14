root = ARGV.fetch(0)

def replace(root, relative, old_text, new_text)
  path = File.join(root, relative)
  text = File.binread(path)
  return if text.include?(new_text)
  abort("Unexpected Ruby 3.0.7 source: #{relative}") unless text.include?(old_text)
  File.binwrite(path, text.sub(old_text, new_text))
end

replace(root, "thread_pthread.h",
        "#elif defined(__GNUC__)\n",
        "#elif defined(__GNUC__) && !defined(RB_THREAD_LOCAL_SPECIFIER_IS_UNSUPPORTED)\n")
replace(root, "thread_pthread.c",
        "#if defined(SIGVTALRM) && !defined(__CYGWIN__)\n",
        "#if defined(SIGVTALRM) && !defined(__CYGWIN__) && !defined(__SYMBIAN32__)\n")
replace(root, "include/ruby/atomic.h",
        "#elif defined HAVE_GCC_ATOMIC_BUILTINS\n",
        "#elif defined(__SYMBIAN32__)\ntypedef unsigned int rb_atomic_t;\nunsigned int easyrpg_atomic_fetch_add(volatile unsigned int *, unsigned int);\nunsigned int easyrpg_atomic_fetch_sub(volatile unsigned int *, unsigned int);\nunsigned int easyrpg_atomic_or(volatile unsigned int *, unsigned int);\nunsigned int easyrpg_atomic_exchange(volatile unsigned int *, unsigned int);\nunsigned int easyrpg_atomic_cas(volatile unsigned int *, unsigned int, unsigned int);\nvoid *easyrpg_atomic_ptr_exchange(void *volatile *, void *);\nvoid *easyrpg_atomic_ptr_cas(void *volatile *, void *, void *);\n# define RUBY_ATOMIC_FETCH_ADD(var, val) easyrpg_atomic_fetch_add((volatile unsigned int *)&(var), (unsigned int)(val))\n# define RUBY_ATOMIC_FETCH_SUB(var, val) easyrpg_atomic_fetch_sub((volatile unsigned int *)&(var), (unsigned int)(val))\n# define RUBY_ATOMIC_OR(var, val) easyrpg_atomic_or((volatile unsigned int *)&(var), (unsigned int)(val))\n# define RUBY_ATOMIC_EXCHANGE(var, val) easyrpg_atomic_exchange((volatile unsigned int *)&(var), (unsigned int)(val))\n# define RUBY_ATOMIC_CAS(var, oldval, newval) ((__typeof__(var))easyrpg_atomic_cas((volatile unsigned int *)&(var), (unsigned int)(oldval), (unsigned int)(newval)))\n# define RUBY_ATOMIC_PTR_EXCHANGE(var, val) easyrpg_atomic_ptr_exchange((void *volatile *)&(var), (void *)(val))\n# define RUBY_ATOMIC_PTR_CAS(var, oldval, newval) easyrpg_atomic_ptr_cas((void *volatile *)&(var), (void *)(oldval), (void *)(newval))\n#elif defined HAVE_GCC_ATOMIC_BUILTINS\n")
replace(root, "thread_pthread.c",
        "#if defined HAVE_PTHREAD_GETATTR_NP || defined HAVE_PTHREAD_ATTR_GET_NP\n#define STACKADDR_AVAILABLE 1\n",
        "#if defined(__SYMBIAN32__)\n#define STACKADDR_AVAILABLE 1\n#undef MAINSTACKADDR_AVAILABLE\n#define MAINSTACKADDR_AVAILABLE 1\n#elif defined HAVE_PTHREAD_GETATTR_NP || defined HAVE_PTHREAD_ATTR_GET_NP\n#define STACKADDR_AVAILABLE 1\n")
replace(root, "thread_pthread.c",
        "#ifdef HAVE_PTHREAD_GETATTR_NP /* Linux */\n",
        "#ifdef __SYMBIAN32__\n    extern int easyrpg_symbian_stack_bounds(void **, size_t *);\n    return easyrpg_symbian_stack_bounds(addr, size);\n#elif defined HAVE_PTHREAD_GETATTR_NP /* Linux */\n")
replace(root, "thread_pthread.c",
        "rb_thread_create_timer_thread(void)\n{\n    /* we only create the pipe, and lazy-spawn */",
        "rb_thread_create_timer_thread(void)\n{\n#ifdef __SYMBIAN32__\n    return;\n#else\n    /* we only create the pipe, and lazy-spawn */")
replace(root, "thread_pthread.c",
        "        signal_self_pipe.owner_process = current;\n    }\n}\n\nstatic void\nubf_timer_disarm(void)",
        "        signal_self_pipe.owner_process = current;\n    }\n#endif\n}\n\nstatic void\nubf_timer_disarm(void)")
replace(root, "thread_pthread.c",
        "native_sleep(rb_thread_t *th, rb_hrtime_t *rel)\n{\n    int sigwait_fd = rb_sigwait_fd_get(th);",
        "native_sleep(rb_thread_t *th, rb_hrtime_t *rel)\n{\n#ifdef __SYMBIAN32__\n    rb_ractor_blocking_threads_inc(th->ractor, __FILE__, __LINE__);\n    native_cond_sleep(th, rel);\n    rb_ractor_blocking_threads_dec(th->ractor, __FILE__, __LINE__);\n    return;\n#endif\n    int sigwait_fd = rb_sigwait_fd_get(th);")
replace(root, "vm_insnhelper.c",
        "ALWAYS_INLINE(static VALUE vm_call_iseq_setup_normal(rb_execution_context_t *ec, rb_control_frame_t *cfp, struct rb_calling_info *calling, const rb_callable_method_entry_t *me, int opt_pc, int param_size, int local_size));",
        "static inline VALUE vm_call_iseq_setup_normal(rb_execution_context_t *ec, rb_control_frame_t *cfp, struct rb_calling_info *calling, const rb_callable_method_entry_t *me, int opt_pc, int param_size, int local_size);")
replace(root, "vm_eval.c",
        "vm_ci_new(mid, 0, argc, false);",
        "vm_ci_new(mid, 0, argc, NULL);")
replace(root, "gc.c",
        "    res = _aligned_malloc(size, alignment);\n#elif defined(HAVE_POSIX_MEMALIGN)\n",
        "    res = _aligned_malloc(size, alignment);\n#elif defined(__SYMBIAN32__)\n    extern void *easyrpg_symbian_gc_page_alloc(size_t, size_t);\n    res = easyrpg_symbian_gc_page_alloc(alignment, size);\n#elif defined(HAVE_POSIX_MEMALIGN)\n")
replace(root, "gc.c",
        "#elif defined(HAVE_MEMALIGN) || defined(HAVE_POSIX_MEMALIGN)\n    free(ptr);",
        "#elif defined(__SYMBIAN32__)\n    extern void easyrpg_symbian_gc_page_free(void *);\n    easyrpg_symbian_gc_page_free(ptr);\n#elif defined(HAVE_MEMALIGN) || defined(HAVE_POSIX_MEMALIGN)\n    free(ptr);")
replace(root, "gc.c",
        "    res = malloc(alignment + size + sizeof(void*));\n    aligned = (char*)res + alignment + sizeof(void*);",
        "    res = malloc(alignment + size + sizeof(void*));\n    if (!res) return NULL;\n    aligned = (char*)res + alignment + sizeof(void*);")
replace(root, "coroutine/arm32/Context.S", "\t# Save caller state", "\t@ Save caller state")
replace(root, "coroutine/arm32/Context.S", "\t# Save caller stack pointer", "\t@ Save caller stack pointer")
replace(root, "coroutine/arm32/Context.S", "\t# Restore callee stack pointer", "\t@ Restore callee stack pointer")
replace(root, "coroutine/arm32/Context.S", "\t# Restore callee state", "\t@ Restore callee state")

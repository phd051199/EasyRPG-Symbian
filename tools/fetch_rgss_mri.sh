#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
WINE_PREFIX=${EASYRPG_WINE_PREFIX:-"$HOME/.wine-symbian"}
SDK_ROOT=${EASYRPG_SYMBIAN_SDK_ROOT:-"$WINE_PREFIX/drive_c/S60/devices/S60_3rd_FP2_SDK_v1.1"}
WINE_BIN=${EASYRPG_WINE_BIN:-}
CSL_BIN="$WINE_PREFIX/drive_c/Program Files/CSL Arm Toolchain/bin"
CC="$CSL_BIN/arm-none-symbianelf-gcc.exe"
CXX="$CSL_BIN/arm-none-symbianelf-g++.exe"

RUBY_VERSION=3.0.7
RUBY_SERIES=3.0
RUBY_SHA256="2a3411977f2850431136b0fab8ad53af09fb74df2ee2f4fb7f11b378fe034388"
RUBY_ARCHIVE="ruby-${RUBY_VERSION}.tar.gz"
RUBY_URL="https://cache.ruby-lang.org/pub/ruby/${RUBY_SERIES}/${RUBY_ARCHIVE}"
CACHE_DIR="$PROJECT_DIR/build/vendor/cache"
WORK_ROOT="$PROJECT_DIR/build/vendor/ruby-${RUBY_VERSION}"
DEST_ROOT="$PROJECT_DIR/third_party/ruby-${RUBY_VERSION}"
WIN_PROJECT="C:\\work\\EasyRPG"
WIN_RUBY="$WIN_PROJECT\\build\\vendor\\ruby-${RUBY_VERSION}"
WIN_SDK="C:\\S60\\devices\\S60_3rd_FP2_SDK_v1.1\\epoc32\\include"

find_wine() {
    if [[ -n $WINE_BIN && -x $WINE_BIN ]]; then
        printf '%s\n' "$WINE_BIN"
        return
    fi
    local candidate
    for candidate in \
        "$HOME/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine" \
        "/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine"; do
        if [[ -x $candidate ]]; then
            printf '%s\n' "$candidate"
            return
        fi
    done
    command -v wine 2>/dev/null || true
}

WINE_BIN=$(find_wine)
if [[ -z $WINE_BIN || ! -x $WINE_BIN ]]; then
    echo "Wine executable not found." >&2
    exit 1
fi
for compiler in "$CC" "$CXX"; do
    if [[ ! -e $compiler ]]; then
        echo "Missing CSL ARM compiler: $compiler" >&2
        exit 1
    fi
done

mkdir -p "$CACHE_DIR" "$PROJECT_DIR/build/vendor"
archive="$CACHE_DIR/$RUBY_ARCHIVE"
if [[ ! -f $archive ]]; then
    curl -L --fail --silent --show-error "$RUBY_URL" -o "$archive"
fi

actual=$(shasum -a 256 "$archive" | awk '{print $1}')
if [[ $actual != "$RUBY_SHA256" ]]; then
    rm -f "$archive"
    echo "SHA256 mismatch for $RUBY_ARCHIVE: $actual" >&2
    exit 1
fi

rm -rf "$WORK_ROOT"
tar -xzf "$archive" -C "$PROJECT_DIR/build/vendor"
if [[ ! -f "$WORK_ROOT/ruby.c" || ! -f "$WORK_ROOT/include/ruby.h" ]]; then
    echo "Unexpected Ruby archive layout: $WORK_ROOT" >&2
    exit 1
fi

cleanup() {
    rm -rf "$WORK_ROOT"
}
trap cleanup EXIT

ruby - "$WORK_ROOT" <<'RUBY'
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
RUBY

tmp=$(mktemp -d "${TMPDIR:-/tmp}/easyrpg-ruby-deps.XXXXXX")
trap 'rm -rf "$tmp"; cleanup' EXIT
source_list="$tmp/sources.txt"
deps_raw="$tmp/deps.raw"
deps_list="$tmp/deps.txt"

ruby - "$PROJECT_DIR/tools/build_rgss_mri.sh" >"$source_list" <<'RUBY'
text = File.read(ARGV.fetch(0))
body = text[/^SOURCES=\(\n(.*?)^\)\n/m, 1] or abort("Cannot find Ruby SOURCES block")
body.each_line do |line|
  source = line.strip
  next if source.empty? || source.start_with?("#")
  puts source
end
RUBY

: >"$deps_raw"
while IFS= read -r source; do
    [[ -n $source ]] || continue
    if [[ $source == *.S ]]; then
        continue
    fi
    WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
        "$WINE_BIN" "$CC" \
        -march=armv5t \
        -msoft-float \
        -std=gnu99 \
        -D__SYMBIAN32__ \
        -DDTRACE_PROBES_DISABLED=1 \
        -DRUBY_EXPORT \
        -D_REENTRANT \
        -DDISABLE_RUBYGEMS=1 \
        -DCROSS_COMPILING=1 \
        -include "$WIN_SDK\\gcce\\gcce.h" \
        -include "$WIN_PROJECT\\src\\rgss\\mri\\prelude.h" \
        -I "$WIN_PROJECT\\src\\rgss\\mri" \
        -I "$WIN_RUBY" \
        -I "$WIN_RUBY\\include" \
        -I "$WIN_RUBY\\enc\\unicode\\12.1.0" \
        -I "$WIN_SDK" \
        -I "$WIN_SDK\\stdapis" \
        -I "$WIN_SDK\\libc" \
        -M "$WIN_RUBY\\$source" >>"$deps_raw"
done <"$source_list"

# ABLD compiles the MRI bridge as C++, which enables Ruby's C++ compatibility
# headers (for example ruby/backward/cxxanyargs.hpp) that are invisible to the
# C-only MRI dependency scan above. Include that exact closure as well.
WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
    "$WINE_BIN" "$CXX" \
    -march=armv5t \
    -msoft-float \
    -fexceptions \
    -nostdinc \
    -D__SYMBIAN32__ \
    -D__S60_32__ \
    -D__S60_3X__ \
    -D__SERIES60_3X__ \
    -D__GCCE__ \
    -D__EPOC32__ \
    -D__MARM__ \
    -D__EABI__ \
    -D__MARM_ARMV5__ \
    -D__EXE__ \
    -DEASYRPG_LAUNCHER_PROCESS \
    -DEASYRPG_RGSS_PROCESS \
    -x c++ \
    -include "$WIN_SDK\\gcce\\gcce.h" \
    -I "$WIN_PROJECT\\src\\rgss" \
    -I "$WIN_PROJECT\\inc" \
    -I "$WIN_PROJECT\\src\\rgss\\mri" \
    -I "$WIN_RUBY" \
    -I "$WIN_RUBY\\include" \
    -I "$WIN_RUBY\\include\\ruby" \
    -I "$WIN_SDK" \
    -I "$WIN_SDK\\stdapis" \
    -I "$WIN_SDK\\libc" \
    -M "$WIN_PROJECT\\src\\rgss\\RgssVmMri.cpp" >>"$deps_raw"

ruby - "$deps_raw" "$WORK_ROOT" "$source_list" >"$deps_list" <<'RUBY'
raw_path, root, sources_path = ARGV
text = File.binread(raw_path).gsub("\\", "/").gsub(%r{/+}, "/")
prefix = "C:/work/EasyRPG/build/vendor/ruby-3.0.7/"
files = {}
text.scan(/#{Regexp.escape(prefix)}([^\s]+)/) do |match|
  relative = match.fetch(0).sub(%r{/+\z}, "")
  files[relative] = true if File.file?(File.join(root, relative))
end
File.foreach(sources_path) do |line|
  relative = line.strip
  next if relative.empty?
  files[relative] = true if File.file?(File.join(root, relative))
end
%w[COPYING LEGAL include/ruby/backward.h].each do |relative|
  files[relative] = true if File.file?(File.join(root, relative))
end
files.keys.sort.each { |relative| puts relative }
RUBY

rm -rf "$DEST_ROOT"
mkdir -p "$DEST_ROOT"
while IFS= read -r relative; do
    [[ -n $relative ]] || continue
    if [[ ! -f "$WORK_ROOT/$relative" ]]; then
        echo "Missing dependency selected for vendoring: $relative" >&2
        exit 1
    fi
    mkdir -p "$DEST_ROOT/$(dirname "$relative")"
    cp "$WORK_ROOT/$relative" "$DEST_ROOT/$relative"
done <"$deps_list"

file_count=$(find "$DEST_ROOT" -type f | wc -l | tr -d ' ')
printf 'Vendored Ruby %s ARMv5 core: %s (%s files, ' "$RUBY_VERSION" "$DEST_ROOT" "$file_count"
du -sh "$DEST_ROOT" | awk '{print $1 ")"}'

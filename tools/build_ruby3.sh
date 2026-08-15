#!/usr/bin/env bash
set -Eeuo pipefail

PROFILE=${1:-build}
case "$PROFILE" in
    build) SDK_CFG=UREL; OPT=(-O2 -DNDEBUG) ;;
    debug) SDK_CFG=UDEB; OPT=(-O0 -g) ;;
    *) echo "Usage: build_ruby3.sh [build|debug]" >&2; exit 2 ;;
esac

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
WINE_PREFIX=${EASYRPG_WINE_PREFIX:-"$HOME/.wine-symbian"}
SDK_ROOT=${EASYRPG_SYMBIAN_SDK_ROOT:-"$WINE_PREFIX/drive_c/S60/devices/S60_3rd_FP2_SDK_v1.1"}
WINE_BIN=${EASYRPG_WINE_BIN:-}
CSL_BIN="$WINE_PREFIX/drive_c/Program Files/CSL Arm Toolchain/bin"
CC="$CSL_BIN/arm-none-symbianelf-gcc.exe"
AR="$CSL_BIN/arm-none-symbianelf-ar.exe"
NM="$CSL_BIN/arm-none-symbianelf-nm.exe"
RUBY_VERSION=3.0.7
LIBRARY_NAME=EasyRpgRuby3.lib
C_STANDARD=gnu99
RUBY_EXTRA_FLAGS="-DRUBY_EXPORT -D_REENTRANT -DDISABLE_RUBYGEMS=1 -DCROSS_COMPILING=1"
RUBY_ROOT="$PROJECT_DIR/build/work/third-party/ruby-$RUBY_VERSION"
BUILD_DIR="$PROJECT_DIR/build/ruby3-$RUBY_VERSION/$SDK_CFG"
SDK_LIBRARY_DIR="$SDK_ROOT/epoc32/release/ARMV5/$SDK_CFG"
LIBRARY="$BUILD_DIR/$LIBRARY_NAME"
WIN_PROJECT="C:\\work\\EasyRPG"
WIN_RUBY="$WIN_PROJECT\\build\\work\\third-party\\ruby-$RUBY_VERSION"
WIN_BUILD="$WIN_PROJECT\\build\\ruby3-$RUBY_VERSION\\$SDK_CFG"
WIN_SDK="C:\\S60\\devices\\S60_3rd_FP2_SDK_v1.1\\epoc32\\include"
RUBY_EXTRA_INCLUDE="-I $WIN_RUBY\\enc\\unicode\\12.1.0"

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
for tool in "$CC" "$AR" "$NM"; do
    if [[ ! -e $tool ]]; then
        echo "Missing CSL ARM tool: $tool" >&2
        exit 1
    fi
done

if [[ ! -f "$RUBY_ROOT/ruby.c" || ! -f "$RUBY_ROOT/include/ruby.h" ]]; then
    echo "Missing vendored Ruby core: $RUBY_ROOT" >&2
    echo "Run tools/prepare_third_party.sh before building." >&2
    exit 1
fi
mkdir -p "$BUILD_DIR" "$SDK_LIBRARY_DIR"
rm -f "$LIBRARY"

SOURCES=(
    array.c
    ast.c
    bignum.c
    builtin.c
    class.c
    compar.c
    compile.c
    complex.c
    cont.c
    debug.c
    debug_counter.c
    dir.c
    dmydln.c
    dln_find.c
    encoding.c
    enum.c
    enumerator.c
    error.c
    eval.c
    file.c
    gc.c
    hash.c
    inits.c
    io.c
    iseq.c
    load.c
    localeinit.c
    loadpath.c
    marshal.c
    math.c
    missing/explicit_bzero.c
    missing/nan.c
    missing/setproctitle.c
    missing/tgamma.c
    memory_view.c
    node.c
    numeric.c
    object.c
    pack.c
    parse.c
    proc.c
    process.c
    ractor.c
    random.c
    range.c
    rational.c
    re.c
    regcomp.c
    regenc.c
    regerror.c
    regexec.c
    regparse.c
    regsyntax.c
    ruby.c
    scheduler.c
    signal.c
    sprintf.c
    st.c
    strftime.c
    string.c
    struct.c
    symbol.c
    thread.c
    time.c
    transcode.c
    transient_heap.c
    util.c
    variable.c
    version.c
    vm.c
    vm_backtrace.c
    vm_dump.c
    vm_sync.c
    vm_trace.c
    dmyext.c
    enc/ascii.c
    enc/unicode.c
    enc/us_ascii.c
    enc/utf_8.c
    enc/trans/newline.c
    coroutine/arm32/Context.S
)

objects=()
for source in "${SOURCES[@]}"; do
    name=${source%.*}
    name=${name//\//_}
    output="$BUILD_DIR/$name.o"
    win_output="$WIN_BUILD\\$name.o"
    source_path="$RUBY_ROOT/$source"
    if [[ -f $output && $output -nt $source_path && \
          $output -nt "$PROJECT_DIR/src/script/ruby3/prelude.h" && \
          $output -nt "$PROJECT_DIR/src/script/ruby3/ruby/config.h" ]]; then
        objects+=("$win_output")
        continue
    fi
    if [[ $source == *.S ]]; then
        WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
            "$WINE_BIN" "$CC" \
            -march=armv5t \
            -msoft-float \
            -DSYMBOL_PREFIX= \
            -c "$WIN_RUBY\\$source" \
            -o "$win_output"
    else
        WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
            "$WINE_BIN" "$CC" \
            -march=armv5t \
            -msoft-float \
            -std="$C_STANDARD" \
            -fno-unit-at-a-time \
            -fno-strict-aliasing \
            "${OPT[@]}" \
            -D__SYMBIAN32__ \
            -DDTRACE_PROBES_DISABLED=1 \
            $RUBY_EXTRA_FLAGS \
            -include "$WIN_SDK\\gcce\\gcce.h" \
            -include "$WIN_PROJECT\\src\\script\\ruby3\\prelude.h" \
            -I "$WIN_PROJECT\\src\\script\\ruby3" \
            -I "$WIN_RUBY" \
            -I "$WIN_RUBY\\include" \
            $RUBY_EXTRA_INCLUDE \
            -I "$WIN_SDK" \
            -I "$WIN_SDK\\stdapis" \
            -I "$WIN_SDK\\libc" \
            -c "$WIN_RUBY\\$source" \
            -o "$win_output"
    fi
    objects+=("$win_output")
done

WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
    "$WINE_BIN" "$AR" crs "$WIN_BUILD\\$LIBRARY_NAME" "${objects[@]}"

symbols=$(WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all "$WINE_BIN" "$NM" -g "$WIN_BUILD\\$LIBRARY_NAME" | tr -d '\r')
for symbol in ruby_init ruby_cleanup rb_eval_string_protect rb_define_class rb_define_module rb_define_method rb_define_singleton_method rb_marshal_load; do
    if ! grep -q "[[:space:]]$symbol$" <<<"$symbols"; then
        echo "Ruby 3 archive does not export $symbol." >&2
        exit 1
    fi
done

cp -f "$LIBRARY" "$SDK_LIBRARY_DIR/$LIBRARY_NAME"
printf 'Ruby %s for E72: %s (%s bytes, %s objects)\n' "$RUBY_VERSION" "$LIBRARY" "$(stat -f %z "$LIBRARY")" "${#objects[@]}"
printf 'Installed for ABLD: %s\n' "$SDK_LIBRARY_DIR/$LIBRARY_NAME"

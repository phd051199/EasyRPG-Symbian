#!/usr/bin/env bash
set -Eeuo pipefail

PROFILE=${1:-build}
case "$PROFILE" in
    build) SDK_CFG=UREL; OPT=(-O2 -DNDEBUG) ;;
    debug) SDK_CFG=UDEB; OPT=(-O0 -g) ;;
    *) echo "Usage: build_rgss_mri.sh [build|debug]" >&2; exit 2 ;;
esac

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
WINE_PREFIX=${EASYRPG_WINE_PREFIX:-"$HOME/.wine-symbian"}
SDK_ROOT=${EASYRPG_SYMBIAN_SDK_ROOT:-"$WINE_PREFIX/drive_c/S60/devices/S60_3rd_FP2_SDK_v1.1"}
WINE_BIN=${EASYRPG_WINE_BIN:-}
CSL_BIN="$WINE_PREFIX/drive_c/Program Files/CSL Arm Toolchain/bin"
CC="$CSL_BIN/arm-none-symbianelf-gcc.exe"
AR="$CSL_BIN/arm-none-symbianelf-ar.exe"
NM="$CSL_BIN/arm-none-symbianelf-nm.exe"
RUBY_ROOT="$PROJECT_DIR/build/vendor/ruby-1.8.7-p374"
BUILD_DIR="$PROJECT_DIR/build/rgss-mri/$SDK_CFG"
SDK_LIBRARY_DIR="$SDK_ROOT/epoc32/release/ARMV5/$SDK_CFG"
LIBRARY="$BUILD_DIR/EasyRpgRuby187.lib"
WIN_PROJECT="C:\\work\\EasyRPG"
WIN_RUBY="$WIN_PROJECT\\build\\vendor\\ruby-1.8.7-p374"
WIN_BUILD="$WIN_PROJECT\\build\\rgss-mri\\$SDK_CFG"
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
for tool in "$CC" "$AR" "$NM"; do
    if [[ ! -e $tool ]]; then
        echo "Missing CSL ARM tool: $tool" >&2
        exit 1
    fi
done

bash "$PROJECT_DIR/tools/fetch_rgss_mri.sh" >/dev/null
mkdir -p "$BUILD_DIR" "$SDK_LIBRARY_DIR"
rm -f "$LIBRARY"

SOURCES=(
    array.c
    bignum.c
    class.c
    compar.c
    dir.c
    dln.c
    enum.c
    enumerator.c
    error.c
    eval.c
    file.c
    gc.c
    hash.c
    inits.c
    io.c
    marshal.c
    math.c
    numeric.c
    object.c
    pack.c
    parse.c
    process.c
    prec.c
    random.c
    range.c
    re.c
    regex.c
    ruby.c
    signal.c
    sprintf.c
    st.c
    string.c
    struct.c
    time.c
    util.c
    variable.c
    version.c
    dmyext.c
)

objects=()
for source in "${SOURCES[@]}"; do
    name=${source%.c}
    output="$BUILD_DIR/$name.o"
    win_output="$WIN_BUILD\\$name.o"
    WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
        "$WINE_BIN" "$CC" \
        -march=armv5t \
        -msoft-float \
        -std=gnu89 \
        -fno-unit-at-a-time \
        -fno-strict-aliasing \
        "${OPT[@]}" \
        -D__SYMBIAN32__ \
        -include "$WIN_SDK\\gcce\\gcce.h" \
        -include "$WIN_PROJECT\\src\\rgss\\mri\\prelude.h" \
        -I "$WIN_PROJECT\\src\\rgss\\mri" \
        -I "$WIN_RUBY" \
        -I "$WIN_SDK" \
        -I "$WIN_SDK\\stdapis" \
        -I "$WIN_SDK\\libc" \
        -c "$WIN_RUBY\\$source" \
        -o "$win_output"
    objects+=("$win_output")
done

WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
    "$WINE_BIN" "$AR" crs "$WIN_BUILD\\EasyRpgRuby187.lib" "${objects[@]}"

symbols=$(WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all "$WINE_BIN" "$NM" -g "$WIN_BUILD\\EasyRpgRuby187.lib" | tr -d '\r')
for symbol in ruby_init ruby_cleanup rb_eval_string_protect rb_define_class rb_define_module rb_define_method rb_define_singleton_method rb_marshal_load; do
    if ! grep -q "[[:space:]]$symbol$" <<<"$symbols"; then
        echo "MRI archive does not export $symbol." >&2
        exit 1
    fi
done

cp -f "$LIBRARY" "$SDK_LIBRARY_DIR/EasyRpgRuby187.lib"
printf 'MRI 1.8.7-p374: %s (%s bytes, %s objects)\n' "$LIBRARY" "$(stat -f %z "$LIBRARY")" "${#objects[@]}"
printf 'Installed for ABLD: %s\n' "$SDK_LIBRARY_DIR/EasyRpgRuby187.lib"

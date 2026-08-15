#!/usr/bin/env bash
# Build the portable RGSS runtime as an ARM static library using modern Clang
# for C++11 parsing/codegen and the original S60 CSL assembler for linkable
# Symbian ELF objects. EasyRPGScript.mmp links the resulting library.
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
WINE_PREFIX=${WINEPREFIX:-$HOME/.wine-symbian}
SDK_ROOT=${S60_SDK_ROOT:-$WINE_PREFIX/drive_c/S60/devices/S60_3rd_FP2_SDK_v1.1}
SDK_INCLUDE="$SDK_ROOT/epoc32/include"
SDK_LIBRARY_DIR="$SDK_ROOT/epoc32/release/armv5/urel"
TOOLCHAIN_ROOT=${CSL_ARM_ROOT:-$WINE_PREFIX/drive_c/Program Files/CSL Arm Toolchain}
ASSEMBLER="$TOOLCHAIN_ROOT/bin/arm-none-symbianelf-as.exe"
ARCHIVER="$TOOLCHAIN_ROOT/bin/arm-none-symbianelf-ar.exe"
NM="$TOOLCHAIN_ROOT/bin/arm-none-symbianelf-nm.exe"
CLANGXX=${CLANGXX:-/usr/bin/clang++}
THIRD_PARTY="$PROJECT_DIR/build/work/third-party"
RUBY_ROOT="$THIRD_PARTY/ruby-3.0.7"
PIXMAN_ROOT="$THIRD_PARTY/pixman-0.34.0/pixman"
LIBCXX_ROOT="$THIRD_PARTY/libcxx-4.0.1/include"
LIBCXX_COMPAT="$PROJECT_DIR/inc/libcxx_compat"
BUILD_DIR="$PROJECT_DIR/build/rgss/UREL"
WIN_BUILD_DIR='C:\work\EasyRPG\build\rgss\UREL'
LIBRARY="$BUILD_DIR/EasyRpgRgss.lib"

find_wine() {
    local candidate
    for candidate in "${WINE_BIN:-}" \
        "$HOME/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine" \
        "/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine"; do
        if [[ -n $candidate && -x $candidate ]]; then printf '%s\n' "$candidate"; return; fi
    done
    command -v wine 2>/dev/null || true
}
WINE_BIN=$(find_wine)
[[ -x $CLANGXX ]] || { echo "Clang not found: $CLANGXX" >&2; exit 1; }
[[ -n $WINE_BIN && -x $WINE_BIN ]] || { echo "Wine not found" >&2; exit 1; }
for tool in "$ASSEMBLER" "$ARCHIVER" "$NM"; do
    [[ -e $tool ]] || { echo "Missing CSL tool: $tool" >&2; exit 1; }
done
for path in "$RUBY_ROOT/include/ruby.h" "$PIXMAN_ROOT/pixman.h" "$LIBCXX_ROOT/vector"; do
    [[ -e $path ]] || { echo "Prepared third party missing: $path" >&2; exit 1; }
done

SOURCES=(
    "$PROJECT_DIR/src/rgss/RgssRun.cpp"
    "$PROJECT_DIR/src/rgss/RgssBootstrap.cpp"
    "$PROJECT_DIR/src/rgss/binding/RgssBindingInit.cpp"
    "$PROJECT_DIR/src/rgss/binding/RgssTable.cpp"
    "$PROJECT_DIR/src/rgss/binding/RgssEtc.cpp"
    "$PROJECT_DIR/src/rgss/binding/RgssFileInt.cpp"
    "$PROJECT_DIR/src/rgss/binding/RgssZlib.cpp"
    "$PROJECT_DIR/src/rgss/binding/RgssRenderer.cpp"
    "$PROJECT_DIR/src/rgss/binding/RgssInputBinding.cpp"
    "$PROJECT_DIR/src/rgss/binding/RgssAudioBinding.cpp"
    "$PROJECT_DIR/src/rgss/binding/RgssM4Binding.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssTypes.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssRgssad.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssVfs.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssBitmapCore.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssImageDecode.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssScene.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssSpriteCore.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssPlaneCore.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssWindowCore.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssTilemapCore.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssGraphicsCore.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssFontCore.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssInput.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssDownscale.cpp"
    "$PROJECT_DIR/src/rgss/core/RgssCxxCompat.cpp"
    "$THIRD_PARTY/mkxp-z-2.4.2/binding/module_rpg.cpp"
)

python3 "$PROJECT_DIR/tools/extract_rgss_vx_autotiles.py" --check >/dev/null

mkdir -p "$BUILD_DIR" "$SDK_LIBRARY_DIR"
rm -f "$LIBRARY"
objects=()
win_objects=()
index=0
for source in "${SOURCES[@]}"; do
    [[ -f $source ]] || { echo "RGSS source missing: $source" >&2; exit 1; }
    name=$(basename -- "$source" .cpp)
    base=$(printf '%03d_%s' "$index" "$name")
    index=$((index + 1))
    raw="$BUILD_DIR/$base.clang.s"
    asm="$BUILD_DIR/$base.gcce.s"
    obj="$BUILD_DIR/$base.o"
    win_asm="$WIN_BUILD_DIR\\$base.gcce.s"
    win_obj="$WIN_BUILD_DIR\\$base.o"

    "$CLANGXX" \
        --target=arm-none-eabi -mcpu=arm1136jf-s -mfloat-abi=soft -fshort-wchar \
        -std=c++11 -O2 -DNDEBUG -fomit-frame-pointer -ffreestanding \
        -fno-exceptions -fno-rtti -fno-threadsafe-statics \
        -fno-c++-static-destructors -fno-stack-protector \
        -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-addrsig \
        -fvisibility=hidden -fdata-sections -ffunction-sections \
        -D_LIBCPP_HAS_NO_THREADS -DEASYRPG_SYMBIAN -D__SYMBIAN32__ \
        -include "$PROJECT_DIR/inc/RgssSymbianClangCompat.h" \
        -nostdinc++ \
        -I "$LIBCXX_COMPAT" -I "$LIBCXX_ROOT" \
        -I "$PROJECT_DIR/inc/libc_compat" -I "$PROJECT_DIR/inc" \
        -I "$PROJECT_DIR/src" -I "$PROJECT_DIR/src/rgss" \
        -I "$PROJECT_DIR/src/rgss/core" -I "$PROJECT_DIR/src/rgss/binding" \
        -I "$PROJECT_DIR/src/script/ruby3" \
        -I "$RUBY_ROOT" -I "$RUBY_ROOT/include" \
        -I "$PIXMAN_ROOT" \
        -I "$SDK_INCLUDE" -I "$SDK_INCLUDE/stdapis" -I "$SDK_INCLUDE/libc" \
        -S "$source" -o "$raw"

    sed -E \
        -e '/^[[:space:]]*\.(eabi_attribute|cpu)([[:space:]]|$)/d' \
        -e 's/^([[:space:]]*)stm(eq|ne|cs|cc|mi|pl|vs|vc|hi|ls|ge|lt|gt|le)([[:space:]])/\1stm\2ia\3/' \
        -e 's/^([[:space:]]*)ldm(eq|ne|cs|cc|mi|pl|vs|vc|hi|ls|ge|lt|gt|le)([[:space:]])/\1ldm\2ia\3/' \
        -e 's/^([[:space:]]*)stm([[:space:]])/\1stmia\2/' \
        -e 's/^([[:space:]]*)ldm([[:space:]])/\1ldmia\2/' \
        -e 's/^([[:space:]]*)lsl([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, lsl \6/' \
        -e 's/^([[:space:]]*)lsr([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, lsr \6/' \
        -e 's/^([[:space:]]*)asr([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, asr \6/' \
        -e 's/^([[:space:]]*)ror([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, ror \6/' \
        -e 's/^([[:space:]]*)rrx([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,[:space:]]+)[[:space:]]*$/\1mov\2\3\4, \5, rrx/' \
        -e 's/^([[:space:]]*)\.inst([[:space:]]+)/\1.word\2/' \
        -e 's/"awGR"/"awG"/g' \
        "$raw" > "$asm"

    WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
        "$WINE_BIN" "$ASSEMBLER" -mcpu=arm1136jf-s -o "$win_obj" "$win_asm"
    objects+=("$obj")
    win_objects+=("$win_obj")
done

WIN_LIBRARY="$WIN_BUILD_DIR\\EasyRpgRgss.lib"
chunk_size=32
for ((start=0; start<${#win_objects[@]}; start+=chunk_size)); do
    chunk=("${win_objects[@]:start:chunk_size}")
    flags=rs
    (( start == 0 )) && flags=crs
    WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all "$WINE_BIN" "$ARCHIVER" \
        "$flags" "$WIN_LIBRARY" "${chunk[@]}"
done

symbols=$(WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all "$WINE_BIN" "$NM" -g "$WIN_LIBRARY" | tr -d '\r')
for symbol in RgssRun RgssHostSet; do
    grep -q "$symbol" <<<"$symbols" || { echo "RGSS archive missing $symbol" >&2; exit 1; }
done

cp -f "$LIBRARY" "$SDK_LIBRARY_DIR/.EasyRpgRgss.lib.$$"
mv -f "$SDK_LIBRARY_DIR/.EasyRpgRgss.lib.$$" "$SDK_LIBRARY_DIR/EasyRpgRgss.lib"
printf 'EasyRPG RGSS: %s (%s objects)\n' "$LIBRARY" "${#objects[@]}"
printf 'Installed for ABLD: %s\n' "$SDK_LIBRARY_DIR/EasyRpgRgss.lib"

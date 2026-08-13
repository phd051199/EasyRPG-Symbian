#!/usr/bin/env bash
set -Eeuo pipefail

PROFILE=${1:-build}
case "$PROFILE" in
    build) SDK_CFG=UREL ;;
    debug) SDK_CFG=UDEB ;;
    *) echo "Usage: build_rgss_core.sh [build|debug]" >&2; exit 2 ;;
esac

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
WINE_PREFIX=${EASYRPG_WINE_PREFIX:-"$HOME/.wine-symbian"}
SDK_ROOT=${EASYRPG_SYMBIAN_SDK_ROOT:-"$WINE_PREFIX/drive_c/S60/devices/S60_3rd_FP2_SDK_v1.1"}
CLANGXX=${EASYRPG_CORE_CLANGXX:-/usr/bin/clang++}
WINE_BIN=${EASYRPG_WINE_BIN:-}
CSL_BIN="$WINE_PREFIX/drive_c/Program Files/CSL Arm Toolchain/bin"
ASSEMBLER="$CSL_BIN/arm-none-symbianelf-as.exe"
ARCHIVER="$CSL_BIN/arm-none-symbianelf-ar.exe"
NM="$CSL_BIN/arm-none-symbianelf-nm.exe"
BUILD_DIR="$PROJECT_DIR/build/rgss-core/$SDK_CFG"
SDK_LIBRARY_DIR="$SDK_ROOT/epoc32/release/ARMV5/$SDK_CFG"
LIBRARY="$BUILD_DIR/EasyRpgRgssCore.lib"
WIN_BUILD_DIR="C:\\work\\EasyRPG\\build\\rgss-core\\$SDK_CFG"
WIN_LIBRARY="$WIN_BUILD_DIR\\EasyRpgRgssCore.lib"
SOURCES=(
    "$PROJECT_DIR/src/rgss/RgssCore.cpp"
    "$PROJECT_DIR/src/rgss/RgssGraphics.cpp"
    "$PROJECT_DIR/src/rgss/RgssBitmap.cpp"
    "$PROJECT_DIR/src/rgss/RgssInput.cpp"
    "$PROJECT_DIR/src/rgss/RgssRender.cpp"
    "$PROJECT_DIR/src/rgss/RgssScriptArchive.cpp"
    "$PROJECT_DIR/src/rgss/RgssPlane.cpp"
    "$PROJECT_DIR/src/rgss/RgssSprite.cpp"
    "$PROJECT_DIR/src/rgss/RgssViewport.cpp"
    "$PROJECT_DIR/src/rgss/RgssTable.cpp"
    "$PROJECT_DIR/src/rgss/RgssVmStub.cpp"
    "$PROJECT_DIR/src/core/GohuFontData.cpp"
    "$PROJECT_DIR/src/core/Aeabi.cpp"
)

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
    echo "Wine executable not found for CSL binutils." >&2
    exit 1
fi
if [[ ! -x $CLANGXX ]]; then
    echo "Clang compiler not found: $CLANGXX" >&2
    exit 1
fi
for tool in "$ASSEMBLER" "$ARCHIVER" "$NM"; do
    if [[ ! -e $tool ]]; then
        echo "Missing CSL ARM binutil: $tool" >&2
        exit 1
    fi
done
if [[ ! -d $SDK_ROOT/epoc32 ]]; then
    echo "Invalid S60 SDK: $SDK_ROOT" >&2
    exit 1
fi

mkdir -p "$BUILD_DIR" "$SDK_LIBRARY_DIR"
rm -f "$LIBRARY"
win_objects=()
index=0
for source in "${SOURCES[@]}"; do
    name=$(basename "$source" .cpp)
    assembly_raw="$BUILD_DIR/${name}.clang.s"
    assembly_pre="$BUILD_DIR/${name}.pre.s"
    assembly="$BUILD_DIR/${name}.gcce.s"
    win_assembly="$WIN_BUILD_DIR\\${name}.gcce.s"
    win_object="$WIN_BUILD_DIR\\${name}.o"

    "$CLANGXX" \
        --target=arm-none-eabi \
        -mcpu=arm1136jf-s \
        -mfloat-abi=soft \
        -fshort-wchar \
        -xc++ \
        -std=c++03 \
        -O3 \
        -DNDEBUG \
        -fomit-frame-pointer \
        -ffreestanding \
        -fno-exceptions \
        -fno-rtti \
        -fno-threadsafe-statics \
        -fno-c++-static-destructors \
        -fno-stack-protector \
        -fno-unwind-tables \
        -fno-asynchronous-unwind-tables \
        -fno-addrsig \
        -fvisibility=hidden \
        -fdata-sections \
        -ffunction-sections \
        -I "$PROJECT_DIR/inc" \
        -I "$SDK_ROOT/epoc32/include/stdapis" \
        -S "$source" \
        -o "$assembly_raw"

    sed -E \
        -e '/^[[:space:]]*\.(eabi_attribute|cpu)([[:space:]]|$)/d' \
        -e 's/^([[:space:]]*)stm([[:space:]])/\1stmia\2/' \
        -e 's/^([[:space:]]*)ldm([[:space:]])/\1ldmia\2/' \
        -e 's/^([[:space:]]*)lsl([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, lsl \6/' \
        -e 's/^([[:space:]]*)lsr([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, lsr \6/' \
        -e 's/^([[:space:]]*)asr([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, asr \6/' \
        -e 's/^([[:space:]]*)ror([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, ror \6/' \
        -e 's/^([[:space:]]*)rrx([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,[:space:]]+)[[:space:]]*$/\1mov\2\3\4, \5, rrx/' \
        -e 's/^([[:space:]]*)mul([a-z]{0,2})([[:space:]]+)r0,[[:space:]]*r0,[[:space:]]*([^,[:space:]]+)[[:space:]]*$/\1mul\2\3r0, \4, r0/' \
        -e 's/^([[:space:]]*)\.inst([[:space:]]+)/\1.word\2/' \
        -e 's/"awGR"/"awG"/g' \
        "$assembly_raw" > "$assembly_pre"
    python3 "$PROJECT_DIR/tools/normalize_arm_assembly.py" "$assembly_pre" "$assembly"

    WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
        "$WINE_BIN" "$ASSEMBLER" -mcpu=arm1136jf-s -o "$win_object" "$win_assembly"
    win_objects+=("$win_object")
    index=$((index + 1))
done

WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
    "$WINE_BIN" "$ARCHIVER" crs "$WIN_LIBRARY" "${win_objects[@]}"

symbol_output=$(WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
    "$WINE_BIN" "$NM" -g "$WIN_LIBRARY")
for required_symbol in \
    EasyRpgRgssDefaultConfig \
    EasyRpgRgssDetectProject \
    EasyRpgRgssCoreConfigure \
    EasyRpgRgssCoreBindFramebuffer \
    EasyRpgRgssCoreLogicFps \
    EasyRpgRgssCoreMapRect \
    EasyRpgRgssGraphicsInitialize \
    EasyRpgRgssGraphicsUpdate \
    EasyRpgRgssGraphicsSetFrameRate \
    EasyRpgRgssBitmapInitialize \
    EasyRpgRgssBitmapCreate \
    EasyRpgRgssBitmapBlt \
    EasyRpgRgssBitmapStretchBlt \
    EasyRpgRgssBitmapHueChange \
    EasyRpgRgssBitmapBlit \
    EasyRpgRgssBitmapBlitEx \
    EasyRpgRgssBitmapBlitExClipped \
    EasyRpgRgssBitmapBlitEffects \
    EasyRpgRgssBitmapTile \
    EasyRpgRgssBitmapTextSize \
    EasyRpgRgssBitmapDrawText \
    EasyRpgRgssBitmapShutdown \
    EasyRpgRgssInputReset \
    EasyRpgRgssInputUpdate \
    EasyRpgRgssInputRepeat \
    EasyRpgRgssRenderInitialize \
    EasyRpgRgssRenderRegister \
    EasyRpgRgssRenderAll \
    EasyRpgRgssRenderShutdown \
    EasyRpgRgssScriptArchiveInspect \
    EasyRpgRgssScriptArchiveValidate \
    EasyRpgRgssScriptArchiveInflate \
    EasyRpgRgssPlaneInitialize \
    EasyRpgRgssPlaneCreate \
    EasyRpgRgssPlaneShutdown \
    EasyRpgRgssSpriteInitialize \
    EasyRpgRgssSpriteCreate \
    EasyRpgRgssSpriteSetTone \
    EasyRpgRgssSpriteSetColor \
    EasyRpgRgssSpriteSetBush \
    EasyRpgRgssSpriteRenderRange \
    EasyRpgRgssSpriteShutdown \
    EasyRpgRgssViewportInitialize \
    EasyRpgRgssViewportCreate \
    EasyRpgRgssViewportRevision \
    EasyRpgRgssViewportShutdown \
    EasyRpgRgssTableInitialize \
    EasyRpgRgssTableCreate \
    EasyRpgRgssTableResize \
    EasyRpgRgssTableShutdown \
    EasyRpgRgssVmAvailable \
    EasyRpgRgssVmInitialize \
    EasyRpgRgssVmLoadScripts \
    EasyRpgRgssVmTick \
    EasyRpgRgssVmShutdown; do
    if ! grep -q "$required_symbol" <<<"$symbol_output"; then
        echo "RGSS core archive does not export $required_symbol." >&2
        printf '%s\n' "$symbol_output" >&2
        exit 1
    fi
done

SDK_LIBRARY="$SDK_LIBRARY_DIR/EasyRpgRgssCore.lib"
SDK_LIBRARY_TMP="$SDK_LIBRARY_DIR/.EasyRpgRgssCore.lib.$$"
cp -f "$LIBRARY" "$SDK_LIBRARY_TMP"
mv -f "$SDK_LIBRARY_TMP" "$SDK_LIBRARY"

printf 'RGSS core: %s (%s bytes, %s objects)\n' \
    "$LIBRARY" "$(stat -f %z "$LIBRARY")" "${#win_objects[@]}"
printf 'Installed for ABLD: %s\n' "$SDK_LIBRARY"

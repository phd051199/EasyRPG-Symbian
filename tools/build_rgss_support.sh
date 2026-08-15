#!/usr/bin/env bash
# Build the small support archive used only by EasyRPGScript. Reuses the exact
# ARM objects already produced from the canonical Player sources; this avoids
# linking the entire 2k/2k3 engine into the RGSS process.
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
WINE_PREFIX=${WINEPREFIX:-$HOME/.wine-symbian}
SDK_ROOT=${S60_SDK_ROOT:-$WINE_PREFIX/drive_c/S60/devices/S60_3rd_FP2_SDK_v1.1}
SDK_LIBRARY_DIR="$SDK_ROOT/epoc32/release/armv5/urel"
TOOLCHAIN_ROOT=${CSL_ARM_ROOT:-$WINE_PREFIX/drive_c/Program Files/CSL Arm Toolchain}
ARCHIVER="$TOOLCHAIN_ROOT/bin/arm-none-symbianelf-ar.exe"
NM="$TOOLCHAIN_ROOT/bin/arm-none-symbianelf-nm.exe"
PLAYER_BUILD="$PROJECT_DIR/build/player/UREL"
BUILD_DIR="$PROJECT_DIR/build/rgss-support/UREL"
LIBRARY="$BUILD_DIR/EasyRpgRgssSupport.lib"
WIN_LIBRARY='C:\work\EasyRPG\build\rgss-support\UREL\EasyRpgRgssSupport.lib'

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
[[ -n $WINE_BIN && -x $WINE_BIN ]] || { echo "Wine not found" >&2; exit 1; }
for tool in "$ARCHIVER" "$NM"; do
    [[ -e $tool ]] || { echo "Missing CSL tool: $tool" >&2; exit 1; }
done

objects=(
    "$PLAYER_BUILD/030_MidiLite.o"
    "$PLAYER_BUILD/031_midisequencer.o"
    "$PLAYER_BUILD/032_Output.o"
    "$PLAYER_BUILD/033_Aeabi.o"
    "$PLAYER_BUILD/034_Image.o"
    "$PLAYER_BUILD/037_GohuFontData.o"
    "$PLAYER_BUILD/038_pixman-implementation.c.o"
    "$PLAYER_BUILD/039_pixman-general.c.o"
    "$PLAYER_BUILD/040_pixman-utils.c.o"
    "$PLAYER_BUILD/041_pixman-noop.c.o"
    "$PLAYER_BUILD/042_pixman-solid-fill.c.o"
    "$PLAYER_BUILD/043_pixman-arm.c.o"
    "$PLAYER_BUILD/044_pixman-x86.c.o"
    "$PLAYER_BUILD/045_pixman-ppc.c.o"
    "$PLAYER_BUILD/046_pixman-mips.c.o"
    "$PLAYER_BUILD/047_pixman-matrix.c.o"
    "$PLAYER_BUILD/048_pixman-image.c.o"
    "$PLAYER_BUILD/049_pixman-bits-image.c.o"
    "$PLAYER_BUILD/050_pixman-combine32.c.o"
    "$PLAYER_BUILD/051_pixman-fast-path.c.o"
    "$PLAYER_BUILD/052_pixman.c.o"
    "$PLAYER_BUILD/053_PixmanCompat.c.o"
    "$PLAYER_BUILD/167_CoreCp932Table.o"
    "$PLAYER_BUILD/231_shinonome_gothic.o"
)

for object in "${objects[@]}"; do
    [[ -f $object ]] || {
        echo "Missing Player support object: $object" >&2
        echo "Run tools/build_player.sh build first." >&2
        exit 1
    }
done

mkdir -p "$BUILD_DIR" "$SDK_LIBRARY_DIR"
rm -f "$LIBRARY"
win_objects=()
for object in "${objects[@]}"; do
    relative=${object#"$PROJECT_DIR/"}
    windows="C:\\work\\EasyRPG\\${relative//\//\\}"
    win_objects+=("$windows")
done

chunk_size=24
for ((start=0; start<${#win_objects[@]}; start+=chunk_size)); do
    chunk=("${win_objects[@]:start:chunk_size}")
    flags=rs
    (( start == 0 )) && flags=crs
    WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all "$WINE_BIN" "$ARCHIVER" \
        "$flags" "$WIN_LIBRARY" "${chunk[@]}"
done

symbols=$(WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all "$WINE_BIN" "$NM" -g "$WIN_LIBRARY" | tr -d '\r')
for symbol in ReadPNGBytes EasyRpgMidiLiteCreate EasyRpgGohuGlyphRows SHINONOME_GOTHIC pixman_image_composite EasyRpgCoreCp932ToUtf16 Output; do
    grep -q "$symbol" <<<"$symbols" || { echo "Support archive missing $symbol" >&2; exit 1; }
done

cp -f "$LIBRARY" "$SDK_LIBRARY_DIR/.EasyRpgRgssSupport.lib.$$"
mv -f "$SDK_LIBRARY_DIR/.EasyRpgRgssSupport.lib.$$" "$SDK_LIBRARY_DIR/EasyRpgRgssSupport.lib"
printf 'EasyRPG RGSS support: %s (%s objects)\n' "$LIBRARY" "${#objects[@]}"
printf 'Installed for ABLD: %s\n' "$SDK_LIBRARY_DIR/EasyRpgRgssSupport.lib"

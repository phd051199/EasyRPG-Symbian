#!/usr/bin/env bash
# Host golden tests for the RGSS bitmap font path (Gohu + Shinonome).
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
PIXMAN="$PROJECT_DIR/third_party/pixman-0.34.0/pixman"
EASYRPG_SRC="$PROJECT_DIR/third_party/easyrpg-player-0.5.0/src"
WORK="$PROJECT_DIR/build/renderer-test"
CXX=${CXX:-clang++}
CC=${CC:-clang}
mkdir -p "$WORK"

PIXMAN_SOURCES=(
    pixman pixman-image pixman-bits-image pixman-combine32 pixman-fast-path
    pixman-general pixman-implementation pixman-utils pixman-solid-fill
    pixman-noop pixman-matrix pixman-arm pixman-x86 pixman-mips pixman-ppc
)
OBJECTS=()
build_obj() {
    local source=$1 obj="$WORK/$2.o"
    OBJECTS+=("$obj")
    if [[ ! -f "$obj" || "$source" -nt "$obj" ]]; then
        "$CC" -c -O1 -DPACKAGE=1 -DPIXMAN_NO_TLS -I "$PIXMAN" "$source" -o "$obj"
    fi
}
for name in "${PIXMAN_SOURCES[@]}"; do
    build_obj "$PIXMAN/$name.c" "$name"
done
build_obj "$PROJECT_DIR/src/core/PixmanCompat.c" pixmancompat

BIN="$WORK/rgss_font_test"
"$CXX" -std=c++11 -O0 -g \
    -I "$PROJECT_DIR/inc" -I "$PROJECT_DIR/src" -I "$PIXMAN" -I "$EASYRPG_SRC" \
    "$PROJECT_DIR/src/rgss/core/RgssBitmapCore.cpp" \
    "$PROJECT_DIR/src/rgss/core/RgssTypes.cpp" \
    "$PROJECT_DIR/src/rgss/core/RgssFontCore.cpp" \
    "$PROJECT_DIR/src/core/GohuFontData.cpp" \
    "$EASYRPG_SRC/shinonome_gothic.cpp" \
    "$PROJECT_DIR/tests/rgss_font_test.cpp" \
    "${OBJECTS[@]}" -o "$BIN"
"$BIN"

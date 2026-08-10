#!/usr/bin/env bash
set -Eeuo pipefail

PROFILE=${1:-build}
case "$PROFILE" in
    build) SDK_CFG=UREL ;;
    debug) SDK_CFG=UDEB ;;
    *) echo "Usage: build_modern_core.sh [build|debug]" >&2; exit 2 ;;
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
BUILD_DIR="$PROJECT_DIR/build/modern-core/$SDK_CFG"
LIBRARY="$BUILD_DIR/EasyRpgModernCore.lib"
SDK_LIBRARY_DIR="$SDK_ROOT/epoc32/release/ARMV5/$SDK_CFG"
WIN_BUILD_DIR="C:\\work\\EasyRPGSymbian\\build\\modern-core\\$SDK_CFG"
GENERATED_DIR="$PROJECT_DIR/build/generated"
GENERATED_CP932="$GENERATED_DIR/CoreCp932Table.cpp"
GENERATED_READER_STRUCT="$GENERATED_DIR/reader_struct.cpp"
LIBLCF_ROOT="$PROJECT_DIR/third_party/liblcf-0.5.0/src"
LIBLCF_GENERATED_ROOT="$LIBLCF_ROOT/generated"
LIBCXX_COMPAT_INCLUDE="$PROJECT_DIR/inc/easyrpgsymbian/libcxx_compat"
PLAYER_ROOT="$PROJECT_DIR/third_party/easyrpg-player-0.5.0/src"
PIXMAN_ROOT="$PROJECT_DIR/third_party/pixman-0.34.0/pixman"
SOURCES=(
    "$PROJECT_DIR/src/core/ModernCoreProbe.cpp"
    "$PROJECT_DIR/src/core/CoreZipCrypto.cpp"
    "$LIBLCF_ROOT/reader_lcf.cpp"
    "$LIBLCF_ROOT/data.cpp"
    "$LIBLCF_ROOT/rpg_setup.cpp"
    "$LIBLCF_ROOT/ldb_reader.cpp"
    "$LIBLCF_ROOT/lmt_reader.cpp"
    "$LIBLCF_ROOT/lmu_reader.cpp"
    "$LIBLCF_ROOT/writer_lcf.cpp"
    "$LIBLCF_ROOT/writer_xml.cpp"
    "$GENERATED_READER_STRUCT"
    "$PROJECT_DIR/third_party/libcxx-4.0.1/src/string.cpp"
    "$PROJECT_DIR/src/core/LibcxxVectorRuntime.cpp"
    "$PROJECT_DIR/src/core/LiblcfReaderUtilSymbian.cpp"
    "$PROJECT_DIR/src/core/LiblcfXmlReaderSymbian.cpp"
    "$PROJECT_DIR/src/core/UpstreamLmuBridge.cpp"
    "$PROJECT_DIR/src/core/UpstreamLcfLinkAnchor.cpp"
    "$PLAYER_ROOT/color.cpp"
    "$PLAYER_ROOT/rect.cpp"
    "$PLAYER_ROOT/tone.cpp"
    "$PLAYER_ROOT/bitmap.cpp"
    "$PLAYER_ROOT/effects.cpp"
    "$PLAYER_ROOT/tilemap_layer.cpp"
    "$PLAYER_ROOT/tilemap.cpp"
    "$PLAYER_ROOT/sprite.cpp"
    "$PLAYER_ROOT/graphics.cpp"
    "$GENERATED_CP932"
)

while IFS= read -r generated_source; do
    SOURCES+=("$generated_source")
done < <(find "$LIBLCF_GENERATED_ROOT" -maxdepth 1 -type f -name '*.cpp' | sort)

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

mkdir -p "$BUILD_DIR" "$SDK_LIBRARY_DIR" "$GENERATED_DIR"
python3 "$PROJECT_DIR/tools/generate_cp932_table.py" --output "$GENERATED_CP932"
python3 "$PROJECT_DIR/tools/generate_symbian_reader_struct.py" "$GENERATED_READER_STRUCT"
rm -f "$LIBRARY"

objects=()
win_objects=()
for source in "${SOURCES[@]}"; do
    source_name=$(basename -- "$source")
    base=${source_name%.cpp}
    assembly_raw="$BUILD_DIR/$base.clang.s"
    assembly="$BUILD_DIR/$base.gcce.s"
    object="$BUILD_DIR/$base.o"
    win_assembly="$WIN_BUILD_DIR\\$base.gcce.s"
    win_object="$WIN_BUILD_DIR\\$base.o"

    if [[ ! -f $source ]]; then
        echo "Modern core source missing: $source" >&2
        exit 1
    fi

    # GCCE 3.4.3 cannot parse EasyRPG 0.5-era C++11. Modern Clang is used
    # only as frontend/optimizer/code generator. The final object is produced
    # by the original S60 CSL assembler so its ELF/EABI format is linkable by
    # the SDK's arm-none-symbianelf-ld and elf2e32.
    source_define=
    if [[ $source == "$PLAYER_ROOT/"* ]]; then
        source_define=-D_3DS
    fi

    "$CLANGXX" \
        --target=arm-none-eabi \
        -march=armv5t \
        -mfloat-abi=soft \
        -fshort-wchar \
        -std=c++11 \
        -O2 \
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
        -D_LIBCPP_HAS_NO_THREADS \
        $source_define \
        -nostdinc++ \
        -I "$LIBCXX_COMPAT_INCLUDE" \
        -I "$PROJECT_DIR/third_party/libcxx-4.0.1/include" \
        -I "$PROJECT_DIR/inc/easyrpgsymbian/libc_compat" \
        -I "$PLAYER_ROOT" \
        -I "$PIXMAN_ROOT" \
        -I "$LIBLCF_ROOT" \
        -I "$LIBLCF_GENERATED_ROOT" \
        -I "$PROJECT_DIR/inc" \
        -S "$source" \
        -o "$assembly_raw"

    # CSL's old assembler predates these metadata directives and some modern
    # GNU assembler spellings. ARM's default stm/ldm alias is increment-after.
    # Clang also emits three-operand register shifts (e.g. lsl r2,r9,r1), while
    # CSL requires the equivalent data-processing spelling (mov r2,r9,lsl r1).
    # Normalize syntax only; instruction semantics remain unchanged.
    sed -E \
        -e '/^[[:space:]]*\.(eabi_attribute|cpu)([[:space:]]|$)/d' \
        -e 's/^([[:space:]]*)stm([[:space:]])/\1stmia\2/' \
        -e 's/^([[:space:]]*)ldm([[:space:]])/\1ldmia\2/' \
        -e 's/^([[:space:]]*)lsl([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, lsl \6/' \
        -e 's/^([[:space:]]*)lsr([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, lsr \6/' \
        -e 's/^([[:space:]]*)asr([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, asr \6/' \
        -e 's/^([[:space:]]*)ror([a-z]{0,2})([[:space:]]+)([^,]+),[[:space:]]*([^,]+),[[:space:]]*((r(1[0-5]|[0-9]))|ip|lr|sp)[[:space:]]*$/\1mov\2\3\4, \5, ror \6/' \
        -e 's/"awGR"/"awG"/g' \
        "$assembly_raw" > "$assembly"

    WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
        "$WINE_BIN" "$ASSEMBLER" -march=armv5t -o "$win_object" "$win_assembly"

    objects+=("$object")
    win_objects+=("$win_object")
done

WIN_LIBRARY="$WIN_BUILD_DIR\\EasyRpgModernCore.lib"
WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
    "$WINE_BIN" "$ARCHIVER" crs "$WIN_LIBRARY" "${win_objects[@]}"

symbol_output=$(WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
    "$WINE_BIN" "$NM" -g "$WIN_LIBRARY")
for required_symbol in EasyRpgModernCoreProbe EasyRpgCoreCp932ToUtf16 EasyRpgCoreWinZipAesDecrypt EasyRpgUpstreamLoadLmtSummary EasyRpgUpstreamLoadLdbSummary EasyRpgUpstreamReadLmuSummary EasyRpgUpstreamReadLmuLayers EasyRpgUpstreamLmuLinkAnchor _ZN10ReaderUtil6Recode _ZN9LcfReaderC1 _ZN10LMU_Reader4Load; do
    if ! grep -q "$required_symbol" <<<"$symbol_output"; then
        echo "Modern core archive does not export $required_symbol." >&2
        printf '%s\n' "$symbol_output" >&2
        exit 1
    fi
done

cp -f "$LIBRARY" "$SDK_LIBRARY_DIR/EasyRpgModernCore.lib"

printf 'Modern C++11 -> CSL EABI4 core: %s (%s bytes, %s objects)\n' \
    "$LIBRARY" "$(stat -f %z "$LIBRARY")" "${#objects[@]}"
printf 'Verified symbols: upstream LDB/LMT/LMU read path + CP932/ZIP platform adapters\n'
printf 'Installed for ABLD: %s\n' "$SDK_LIBRARY_DIR/EasyRpgModernCore.lib"

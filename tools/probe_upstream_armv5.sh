#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
WINE_PREFIX=${EASYRPG_WINE_PREFIX:-"$HOME/.wine-symbian"}
SDK_ROOT=${EASYRPG_SYMBIAN_SDK_ROOT:-"$WINE_PREFIX/drive_c/S60/devices/S60_3rd_FP2_SDK_v1.1"}
CLANGXX=${EASYRPG_CORE_CLANGXX:-/usr/bin/clang++}
WINE_BIN=${EASYRPG_WINE_BIN:-}
CSL_BIN="$WINE_PREFIX/drive_c/Program Files/CSL Arm Toolchain/bin"
ASSEMBLER="$CSL_BIN/arm-none-symbianelf-as.exe"
NM="$CSL_BIN/arm-none-symbianelf-nm.exe"
BUILD_DIR="$PROJECT_DIR/build/upstream-armv5-probe"
WIN_BUILD_DIR="C:\\work\\EasyRPGSymbian\\build\\upstream-armv5-probe"
SYMBIAN_READER_STRUCT="$BUILD_DIR/generated/reader_struct.cpp"
LIBLCF_ROOT="$PROJECT_DIR/third_party/liblcf-0.5.0/src"
LIBCXX_ROOT="$PROJECT_DIR/third_party/libcxx-4.0.1"
LIBLCF_INCLUDE="$LIBLCF_ROOT"
LIBLCF_GENERATED_INCLUDE="$LIBLCF_ROOT/generated"
LIBCXX_INCLUDE="$LIBCXX_ROOT/include"
LIBC_COMPAT_INCLUDE="$PROJECT_DIR/inc/easyrpgsymbian/libc_compat"
SOURCES=(
    "$LIBLCF_ROOT/reader_lcf.cpp"
    "$LIBLCF_ROOT/lmu_reader.cpp"
    "$SYMBIAN_READER_STRUCT"
    "$LIBCXX_ROOT/src/string.cpp"
    "$PROJECT_DIR/src/core/LibcxxVectorRuntime.cpp"
    "$PROJECT_DIR/src/core/LiblcfReaderUtilSymbian.cpp"
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
    echo "UPSTREAM ARMv5 PROBE: Wine executable not found for CSL assembler." >&2
    exit 20
fi
if [[ ! -x $CLANGXX ]]; then
    echo "UPSTREAM ARMv5 PROBE: Clang compiler not found: $CLANGXX" >&2
    exit 20
fi
if [[ ! -e $ASSEMBLER || ! -e $NM ]]; then
    echo "UPSTREAM ARMv5 PROBE: CSL ARM binutils are missing under: $CSL_BIN" >&2
    exit 20
fi
if [[ ! -d $SDK_ROOT/epoc32 ]]; then
    echo "UPSTREAM ARMv5 PROBE: invalid S60 SDK: $SDK_ROOT" >&2
    exit 20
fi
mkdir -p "$(dirname "$SYMBIAN_READER_STRUCT")"
python3 "$PROJECT_DIR/tools/generate_symbian_reader_struct.py" "$SYMBIAN_READER_STRUCT"
for source in "${SOURCES[@]}"; do
    if [[ ! -f $source ]]; then
        echo "UPSTREAM ARMv5 PROBE: upstream source missing: $source" >&2
        exit 20
    fi
done

# reader_lcf.cpp itself only names string/vector directly, but libc++ 4.0.1
# reaches the following public/internal headers while instantiating those types.
# Keep this preflight explicit so an incomplete vendor tree fails with an exact
# actionable list instead of silently falling back to the host C++ library.
required_libcxx_headers=(
    __bit_reference
    algorithm
    cerrno
    iterator
    limits
    memory
    string
    string_view
    tuple
    utility
    vector
)
missing_headers=()
for header in "${required_libcxx_headers[@]}"; do
    if [[ ! -f "$LIBCXX_INCLUDE/$header" ]]; then
        missing_headers+=("$header")
    fi
done
if (( ${#missing_headers[@]} != 0 )); then
    echo "UPSTREAM ARMv5 PROBE: vendored libc++ 4.0.1 dependency closure is incomplete." >&2
    echo "Missing exact upstream headers under third_party/libcxx-4.0.1/include/:" >&2
    printf '  %s\n' "${missing_headers[@]}" >&2
    echo >&2
    echo "Do not use the host Xcode libc++ headers and do not replace upstream liblcf with a local STL shim." >&2
    echo "Vendor the matching LLVM libc++ 4.0.1 files, then rerun this probe." >&2
    exit 21
fi

mkdir -p "$BUILD_DIR"

printf 'UPSTREAM ARMv5 PROBE frontend: %s\n' "$CLANGXX"
"$CLANGXX" --version | head -1

compile_object() {
    local source=$1
    local stem=$2
    local assembly_raw="$BUILD_DIR/$stem.clang.s"
    local assembly="$BUILD_DIR/$stem.gcce.s"
    local object="$BUILD_DIR/$stem.o"
    local win_assembly="$WIN_BUILD_DIR\\$stem.gcce.s"
    local win_object="$WIN_BUILD_DIR\\$stem.o"

    printf 'Compiling %s\n' "${source#$PROJECT_DIR/}"
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
        -nostdinc++ \
        -I "$LIBCXX_INCLUDE" \
        -I "$LIBC_COMPAT_INCLUDE" \
        -I "$LIBLCF_INCLUDE" \
        -I "$LIBLCF_GENERATED_INCLUDE" \
        -S "$source" \
        -o "$assembly_raw"

    # Keep this syntax normalization identical to the modern-core lane. The CSL
    # assembler is the producer of the final EABI4 object; Clang is only the
    # C++11 frontend/optimizer/code generator.
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

    printf 'ARMv5 EABI4 object OK: %s (%s bytes)\n' \
        "$object" "$(stat -f %z "$object" 2>/dev/null || stat -c %s "$object")"
}

compile_object "$LIBLCF_ROOT/reader_lcf.cpp" reader_lcf
compile_object "$LIBLCF_ROOT/lmu_reader.cpp" lmu_reader
compile_object "$SYMBIAN_READER_STRUCT" reader_struct
compile_object "$LIBCXX_ROOT/src/string.cpp" libcxx_string
compile_object "$PROJECT_DIR/src/core/LibcxxVectorRuntime.cpp" libcxx_vector_runtime
compile_object "$PROJECT_DIR/src/core/LiblcfReaderUtilSymbian.cpp" liblcf_reader_util_symbian

generated_stems=()
while IFS= read -r source; do
    base=$(basename "$source" .cpp)
    stem="liblcf_generated_$base"
    compile_object "$source" "$stem"
    generated_stems+=("$stem")
done < <(find "$LIBLCF_GENERATED_INCLUDE" -maxdepth 1 -type f -name '*.cpp' | sort)
printf 'Generated RPG model objects: %s\n' "${#generated_stems[@]}"

for stem in reader_lcf lmu_reader reader_struct libcxx_string libcxx_vector_runtime liblcf_reader_util_symbian; do
    printf '\nUndefined symbols: %s.o\n' "$stem"
    WINEPREFIX="$WINE_PREFIX" WINEDEBUG=-all \
        "$WINE_BIN" "$NM" -u "$WIN_BUILD_DIR\\$stem.o" || true
done

printf '\nUPSTREAM ARMv5 PROBE OK: upstream reader + required libc++ runtime objects compile and assemble as CSL EABI4.\n'

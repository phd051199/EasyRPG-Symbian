#!/usr/bin/env bash
# Host regression test for encoding detection (tests/encoding_detection_test.cpp).
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
LIBLCF_SRC="$PROJECT_DIR/build/work/third-party/liblcf-0.5.0/src"
LIBLCF_GEN="$LIBLCF_SRC/generated"
GENERATED_DIR="$PROJECT_DIR/build/generated"
WORK="$PROJECT_DIR/build/encoding-test"
CXX=${CXX:-clang++}

if [[ ! -f "$LIBLCF_SRC/ldb_reader.cpp" ]]; then
    echo "Missing prepared liblcf. Run tools/prepare_third_party.sh." >&2
    exit 1
fi
mkdir -p "$WORK" "$GENERATED_DIR"

python3 "$PROJECT_DIR/tools/generate_cp932_table.py" --output "$GENERATED_DIR/CoreCp932Table.cpp"

SOURCES=(
    "$LIBLCF_SRC/reader_lcf.cpp"
    "$LIBLCF_SRC/data.cpp"
    "$LIBLCF_SRC/rpg_setup.cpp"
    "$LIBLCF_SRC/rpg_fixup.cpp"
    "$LIBLCF_SRC/ldb_reader.cpp"
    "$LIBLCF_SRC/ldb_equipment.cpp"
    "$LIBLCF_SRC/ldb_eventcommand.cpp"
    "$LIBLCF_SRC/ldb_parameters.cpp"
    "$LIBLCF_SRC/lmt_reader.cpp"
    "$LIBLCF_SRC/lmt_rect.cpp"
    "$LIBLCF_SRC/lmt_treemap.cpp"
    "$LIBLCF_SRC/lmu_reader.cpp"
    "$LIBLCF_SRC/lmu_movecommand.cpp"
    "$LIBLCF_SRC/ini.cpp"
    "$LIBLCF_SRC/inireader.cpp"
    "$LIBLCF_SRC/reader_flags.cpp"
    "$LIBLCF_SRC/writer_lcf.cpp"
    "$LIBLCF_SRC/writer_xml.cpp"
    "$GENERATED_DIR/reader_struct.cpp"
    "$GENERATED_DIR/CoreCp932Table.cpp"
    "$PROJECT_DIR/src/core/LiblcfReaderUtil.cpp"
    "$PROJECT_DIR/src/core/LiblcfXmlReader.cpp"
    "$PROJECT_DIR/src/core/GohuFontData.cpp"
    "$PROJECT_DIR/tests/encoding_detection_test.cpp"
)
while IFS= read -r generated_source; do
    SOURCES+=("$generated_source")
done < <(find "$LIBLCF_GEN" -maxdepth 1 -type f -name '*.cpp' | sort)

BIN="$WORK/encoding_detection_test"
echo "Compiling host test (${#SOURCES[@]} sources)..."
"$CXX" -std=c++11 -O0 -g \
    -I "$LIBLCF_SRC" -I "$LIBLCF_GEN" -I "$GENERATED_DIR" -I "$PROJECT_DIR/include" \
    -o "$BIN" "${SOURCES[@]}"
echo "Built $BIN"

extract_game() {
    local zip_path=$1 case_dir=$2
    rm -rf "$case_dir"
    mkdir -p "$case_dir"
    python3 - "$zip_path" "$case_dir" <<'PY'
import sys, zipfile
zf = zipfile.ZipFile(sys.argv[1])
for n in zf.namelist():
    if n.startswith('__MACOSX/') or n.endswith('/'):
        continue
    zf.extract(n, sys.argv[2])
PY
}

run_case() {
    local zip_path=$1 label=$2 expect=$3
    extract_game "$zip_path" "$WORK/$label"
    local ldb
    ldb=$(find "$WORK/$label" -maxdepth 3 -name RPG_RT.ldb | head -1)
    if [[ -z $ldb ]]; then
        echo "FAIL: RPG_RT.ldb not found in $zip_path"
        return 1
    fi
    "$BIN" "$(dirname "$ldb"):$expect"
}

resolve_game_archive() {
    local stem=$1
    local plain="$PROJECT_DIR/games/$stem.zip"
    local legacy="$PROJECT_DIR/games/$stem.store.zip"
    if [[ -f $plain ]]; then
        printf '%s\n' "$plain"
        return
    fi
    if [[ -f $legacy ]]; then
        printf '%s\n' "$legacy"
        return
    fi
    echo "Missing game archive: $stem(.zip|.store.zip)" >&2
    return 1
}

echo
echo "=== Unit + real games (expect: VH = mixed; Doraemon II, WitchsHeart = Japanese) ==="
STATUS=0
run_case "$(resolve_game_archive 'Nobihaza VH')" "vh" "EASYRPG_VI_CP932:mixed" || STATUS=1
# Doraemon II ships under the Nobihaza brand but contains no Vietnamese text.
run_case "$(resolve_game_archive 'Nobihaza Code Doraemon II')" "doraemon2" "SHIFT_JIS:!mixed" || STATUS=1
run_case "$(resolve_game_archive 'WitchsHeart203')" "witchheart" "SHIFT_JIS:!mixed" || STATUS=1

exit $STATUS

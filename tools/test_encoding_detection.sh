#!/usr/bin/env bash
# Host regression test for encoding detection (tests/encoding_detection_test.cpp).
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
LIBLCF_SRC="$PROJECT_DIR/build/work/third-party/liblcf-0.5.0/src"
LIBLCF_GEN="$LIBLCF_SRC/generated"
UCHARDET_SRC="$PROJECT_DIR/build/work/third-party/uchardet-0.0.8/src"
GENERATED_DIR="$PROJECT_DIR/build/generated"
WORK="$PROJECT_DIR/build/encoding-test/run-$$"
CXX=${CXX:-clang++}

if [[ ! -f "$LIBLCF_SRC/ldb_reader.cpp" || ! -f "$UCHARDET_SRC/nsSJISProber.cpp" ]]; then
    echo "Missing prepared dependencies. Run tools/prepare_third_party.sh." >&2
    exit 1
fi
mkdir -p "$WORK" "$GENERATED_DIR"

python3 "$PROJECT_DIR/tools/generate_cp932_table.py" --output "$GENERATED_DIR/CoreCp932Table.cpp"
python3 "$PROJECT_DIR/tools/generate_gbk_table.py" --output "$GENERATED_DIR/CoreGbkTable.cpp"

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
    "$LIBLCF_SRC/lsd_reader.cpp"
    "$LIBLCF_SRC/ini.cpp"
    "$LIBLCF_SRC/inireader.cpp"
    "$LIBLCF_SRC/reader_flags.cpp"
    "$LIBLCF_SRC/writer_lcf.cpp"
    "$LIBLCF_SRC/writer_xml.cpp"
    "$GENERATED_DIR/reader_struct.cpp"
    "$GENERATED_DIR/CoreCp932Table.cpp"
    "$GENERATED_DIR/CoreGbkTable.cpp"
    "$PROJECT_DIR/src/core/CharsetDetector.cpp"
    "$PROJECT_DIR/src/core/EncodingPipeline.cpp"
    "$PROJECT_DIR/src/core/LiblcfXmlReader.cpp"
    "$PROJECT_DIR/src/core/GohuFontData.cpp"
    "$PROJECT_DIR/tests/encoding_detection_test.cpp"
    "$UCHARDET_SRC/CharDistribution.cpp"
    "$UCHARDET_SRC/JpCntx.cpp"
    "$UCHARDET_SRC/nsBig5Prober.cpp"
    "$UCHARDET_SRC/nsGB2312Prober.cpp"
    "$UCHARDET_SRC/nsMBCSSM.cpp"
    "$UCHARDET_SRC/nsSJISProber.cpp"
    "$UCHARDET_SRC/nsUTF8Prober.cpp"
)
while IFS= read -r generated_source; do
    SOURCES+=("$generated_source")
done < <(find "$LIBLCF_GEN" -maxdepth 1 -type f -name '*.cpp' | sort)

BIN="$WORK/encoding_detection_test"
echo "Compiling host test (${#SOURCES[@]} sources)..."
"$CXX" -std=c++11 -O0 -g \
    -DEASYRPG_ENCODING_CORPUS_TEST \
    -I "$LIBLCF_SRC" -I "$LIBLCF_GEN" -I "$GENERATED_DIR" -I "$PROJECT_DIR/include" -I "$UCHARDET_SRC" \
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
echo "=== Unit + real games (expect: VH = mixed; YasuoStory = GBK; Doraemon II, WitchsHeart = Japanese) ==="
STATUS=0
run_case "$(resolve_game_archive 'Nobihaza VH')" "vh" "EASYRPG_VI_CP932:mixed" || STATUS=1
run_case "$(resolve_game_archive 'Nobihaza YasuoStory')" "yasuo" "GBK:!mixed" || STATUS=1
run_case "$(resolve_game_archive 'Nobihaza Dead Space 2')" "deadspace2" "EASYRPG_GBK_CP932:!mixed" || STATUS=1
run_case "$(resolve_game_archive 'Nobihaza In The Dark')" "inthedark" "EASYRPG_GBK_CP932:!mixed" || STATUS=1
# Doraemon II ships under the Nobihaza brand but contains no Vietnamese text.
run_case "$(resolve_game_archive 'Nobihaza Code Doraemon II')" "doraemon2" "SHIFT_JIS:!mixed" || STATUS=1
run_case "$(resolve_game_archive 'WitchsHeart203')" "witchheart" "SHIFT_JIS:!mixed" || STATUS=1

echo
echo "=== Full source-text corpus: every LDB/LMT/LMU in games/*.zip ==="
CORPUS_ARGS=(
    --binary "$BIN"
    --games "$PROJECT_DIR/games"
    --work "$WORK/corpus-work"
    --manifest "$PROJECT_DIR/tests/game_text_corpus_manifest.tsv"
)
if [[ ${UPDATE_GAME_TEXT_CORPUS:-0} == 1 ]]; then
    CORPUS_ARGS+=(--update)
fi
python3 "$PROJECT_DIR/tools/test_game_text_corpus.py" "${CORPUS_ARGS[@]}" || STATUS=1

exit $STATUS

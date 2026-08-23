#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
CACHE_DIR="$PROJECT_DIR/build/cache/upstream"
IMPORT_DIR="$PROJECT_DIR/build/import/third-party"
MANIFEST_DIR="$PROJECT_DIR/tools/vendor"

mkdir -p "$CACHE_DIR" "$IMPORT_DIR"

fetch_archive() {
    local source_url=$1 archive_name=$2 expected_sha=$3
    local archive_file="$CACHE_DIR/$archive_name"
    if [[ ! -f $archive_file ]]; then
        curl -L --fail --silent --show-error "$source_url" -o "$archive_file"
    fi
    local actual_sha
    actual_sha=$(shasum -a 256 "$archive_file" | awk '{print $1}')
    if [[ $actual_sha != "$expected_sha" ]]; then
        echo "SHA-256 mismatch for $archive_name: $actual_sha" >&2
        exit 1
    fi
}

replace_component() {
    local source_root=$1 component=$2 manifest=$3
    local destination="$PROJECT_DIR/third_party/$component"
    local staged="$IMPORT_DIR/stage-$component"

    if [[ $destination != "$PROJECT_DIR/third_party/"* ||
          $staged != "$PROJECT_DIR/build/import/third-party/"* ]]; then
        echo "Refusing unsafe vendor destination: $destination" >&2
        exit 1
    fi

    rm -rf -- "$staged"
    mkdir -p "$staged"
    while IFS= read -r relative || [[ -n $relative ]]; do
        [[ -n $relative && $relative != \#* ]] || continue
        case "$relative" in
            /*|../*|*/../*|*/..)
                echo "Unsafe allowlist entry in $manifest: $relative" >&2
                exit 1
                ;;
        esac
        if [[ ! -f "$source_root/$relative" ]]; then
            echo "Pinned upstream is missing $component/$relative" >&2
            exit 1
        fi
        mkdir -p "$staged/$(dirname -- "$relative")"
        cp -p "$source_root/$relative" "$staged/$relative"
    done < "$manifest"

    rm -rf -- "$destination"
    mv "$staged" "$destination"
}

extract_tgz() {
    local archive_name=$1 extracted_root=$2
    local destination="$IMPORT_DIR/$extracted_root"
    rm -rf -- "$destination"
    tar -xzf "$CACHE_DIR/$archive_name" -C "$IMPORT_DIR"
}

extract_txz() {
    local archive_name=$1 extracted_root=$2
    local destination="$IMPORT_DIR/$extracted_root"
    rm -rf -- "$destination"
    tar -xJf "$CACHE_DIR/$archive_name" -C "$IMPORT_DIR"
}

fetch_archive \
    "https://easyrpg.org/downloads/player/0.5.0/easyrpg-player-0.5.0.tar.gz" \
    "easyrpg-player-0.5.0.tar.gz" \
    "21c9b95536fb6348e84b11ac8e5908688c6e61e9b4eb426247d546a44869241e"
fetch_archive \
    "https://www.cairographics.org/releases/pixman-0.34.0.tar.gz" \
    "pixman-0.34.0.tar.gz" \
    "21b6b249b51c6800dc9553b65106e1e37d0e25df942c90531d4c3997aa20a88e"
fetch_archive \
    "https://easyrpg.org/downloads/player/0.5.0/liblcf-0.5.0.tar.gz" \
    "liblcf-0.5.0.tar.gz" \
    "5f77e40bac5b48ab3163c9628846825e44ae8b5ecb4367de2d55845b2ba48524"
fetch_archive \
    "https://releases.llvm.org/4.0.1/libcxx-4.0.1.src.tar.xz" \
    "libcxx-4.0.1.src.tar.xz" \
    "520a1171f272c9ff82f324d5d89accadcec9bc9f3c78de11f5575cdb99accc4c"
fetch_archive \
    "https://gitlab.freedesktop.org/uchardet/uchardet/-/archive/v0.0.8/uchardet-v0.0.8.tar.gz" \
    "uchardet-0.0.8.tar.gz" \
    "5aa402a1b5b1dbb8d81096f141ff1224e079f4d3a1db0f79ecca782756d3a416"

extract_tgz "easyrpg-player-0.5.0.tar.gz" "easyrpg-player-0.5.0"
extract_tgz "pixman-0.34.0.tar.gz" "pixman-0.34.0"
extract_tgz "liblcf-0.5.0.tar.gz" "liblcf-0.5.0"
extract_txz "libcxx-4.0.1.src.tar.xz" "libcxx-4.0.1.src"
extract_tgz "uchardet-0.0.8.tar.gz" "uchardet-v0.0.8"

replace_component "$IMPORT_DIR/easyrpg-player-0.5.0" \
    "easyrpg-player-0.5.0" "$MANIFEST_DIR/easyrpg-player-0.5.0.files"
replace_component "$IMPORT_DIR/pixman-0.34.0" \
    "pixman-0.34.0" "$MANIFEST_DIR/pixman-0.34.0.files"
replace_component "$IMPORT_DIR/liblcf-0.5.0" \
    "liblcf-0.5.0" "$MANIFEST_DIR/liblcf-0.5.0.files"
replace_component "$IMPORT_DIR/libcxx-4.0.1.src" \
    "libcxx-4.0.1" "$MANIFEST_DIR/libcxx-4.0.1.files"
replace_component "$IMPORT_DIR/uchardet-v0.0.8" \
    "uchardet-0.0.8" "$MANIFEST_DIR/uchardet-0.0.8.files"

"$PROJECT_DIR/tools/verify_third_party.sh"

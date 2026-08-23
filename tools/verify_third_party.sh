#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
CACHE_DIR="$PROJECT_DIR/build/cache/upstream"
MANIFEST_DIR="$PROJECT_DIR/tools/vendor"

verify_component() {
    local component=$1 archive_name=$2 archive_root=$3 archive_mode=$4
    local manifest="$MANIFEST_DIR/$component.files"
    local destination="$PROJECT_DIR/third_party/$component"
    local expected_sha=$5

    if [[ ! -f "$CACHE_DIR/$archive_name" ]]; then
        echo "Missing cached upstream archive: $archive_name" >&2
        exit 1
    fi
    local actual_sha
    actual_sha=$(shasum -a 256 "$CACHE_DIR/$archive_name" | awk '{print $1}')
    if [[ $actual_sha != "$expected_sha" ]]; then
        echo "SHA-256 mismatch for $archive_name: $actual_sha" >&2
        exit 1
    fi

    local actual_inventory expected_inventory
    actual_inventory=$(find "$destination" -type f -print | sed "s#^$destination/##" | LC_ALL=C sort)
    expected_inventory=$(sed '/^[[:space:]]*\(#\|$\)/d' "$manifest" | LC_ALL=C sort)
    if [[ $actual_inventory != "$expected_inventory" ]]; then
        echo "Unexpected files in third_party/$component" >&2
        diff -u <(printf '%s\n' "$expected_inventory") \
                <(printf '%s\n' "$actual_inventory") >&2 || true
        exit 1
    fi

    local count=0
    while IFS= read -r relative || [[ -n $relative ]]; do
        [[ -n $relative && $relative != \#* ]] || continue
        if [[ $archive_mode == tgz ]]; then
            tar -xOzf "$CACHE_DIR/$archive_name" "$archive_root/$relative" |
                cmp -s - "$destination/$relative"
        else
            tar -xOJf "$CACHE_DIR/$archive_name" "$archive_root/$relative" |
                cmp -s - "$destination/$relative"
        fi
        if [[ ${PIPESTATUS[1]} -ne 0 ]]; then
            echo "Vendored file differs from upstream: $component/$relative" >&2
            exit 1
        fi
        count=$((count + 1))
    done < "$manifest"
    printf 'verified %-25s %4d pristine files\n' "$component" "$count"
}

verify_component "easyrpg-player-0.5.0" \
    "easyrpg-player-0.5.0.tar.gz" "easyrpg-player-0.5.0" tgz \
    "21c9b95536fb6348e84b11ac8e5908688c6e61e9b4eb426247d546a44869241e"
verify_component "pixman-0.34.0" \
    "pixman-0.34.0.tar.gz" "pixman-0.34.0" tgz \
    "21b6b249b51c6800dc9553b65106e1e37d0e25df942c90531d4c3997aa20a88e"
verify_component "liblcf-0.5.0" \
    "liblcf-0.5.0.tar.gz" "liblcf-0.5.0" tgz \
    "5f77e40bac5b48ab3163c9628846825e44ae8b5ecb4367de2d55845b2ba48524"
verify_component "libcxx-4.0.1" \
    "libcxx-4.0.1.src.tar.xz" "libcxx-4.0.1.src" txz \
    "520a1171f272c9ff82f324d5d89accadcec9bc9f3c78de11f5575cdb99accc4c"
verify_component "uchardet-0.0.8" \
    "uchardet-0.0.8.tar.gz" "uchardet-v0.0.8" tgz \
    "5aa402a1b5b1dbb8d81096f141ff1224e079f4d3a1db0f79ecca782756d3a416"

verify_source_omissions() {
    local component=$1 archive_name=$2 archive_root=$3 manifest=$4 expected_file=$5
    local archive="$CACHE_DIR/$archive_name"
    local upstream_inventory actual_inventory

    upstream_inventory=$(tar -tf "$archive" |
        sed "s#^$archive_root/##" |
        grep -E '^src/.*\.(cpp|h)$' |
        LC_ALL=C sort)
    actual_inventory=$(sed '/^[[:space:]]*\(#\|$\)/d' "$manifest" |
        grep -E '^src/.*\.(cpp|h)$' |
        LC_ALL=C sort)

    if ! diff -u "$expected_file" \
        <(comm -23 <(printf '%s\n' "$upstream_inventory") \
                   <(printf '%s\n' "$actual_inventory")); then
        echo "Unexpected source omission in third_party/$component" >&2
        echo "Update the vendored allowlist or the intentional-omission list explicitly." >&2
        exit 1
    fi
}

verify_source_omissions \
    "easyrpg-player-0.5.0" \
    "easyrpg-player-0.5.0.tar.gz" \
    "easyrpg-player-0.5.0" \
    "$MANIFEST_DIR/easyrpg-player-0.5.0.files" \
    "$MANIFEST_DIR/easyrpg-player-0.5.0.omitted-src"

verify_source_omissions \
    "liblcf-0.5.0" \
    "liblcf-0.5.0.tar.gz" \
    "liblcf-0.5.0" \
    "$MANIFEST_DIR/liblcf-0.5.0.files" \
    "$MANIFEST_DIR/liblcf-0.5.0.omitted-src"

unexpected=$(find "$PROJECT_DIR/third_party" -mindepth 1 -maxdepth 1 -type d \
    ! -name easyrpg-player-0.5.0 \
    ! -name pixman-0.34.0 \
    ! -name liblcf-0.5.0 \
    ! -name libcxx-4.0.1 \
    ! -name uchardet-0.0.8 -print)
if [[ -n $unexpected ]]; then
    echo "Unexpected third_party component(s):" >&2
    printf '%s\n' "$unexpected" >&2
    exit 1
fi

echo "third_party is allowlisted and byte-identical to pinned upstream archives."
echo "EasyRPG/liblcf source omissions match the reviewed Symbian replacement/platform list."

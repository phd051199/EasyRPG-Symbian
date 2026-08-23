#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
WORK_DIR="$PROJECT_DIR/build/work/third-party"

prepare_component() {
    local component=$1
    local source="$PROJECT_DIR/third_party/$component"
    local destination="$WORK_DIR/$component"
    if [[ ! -d $source ]]; then
        echo "Missing vendored component: $component" >&2
        exit 1
    fi
    rm -rf -- "$destination"
    mkdir -p "$WORK_DIR"
    cp -Rp "$source" "$destination"
}

apply_patch_series() {
    local series="$PROJECT_DIR/patches/series.conf"
    [[ -f $series ]] || { echo "Missing patch series: $series" >&2; exit 1; }

    while IFS='|' read -r group component expected_sha relative_path || [[ -n ${group:-} ]]; do
        [[ -n ${group:-} && ${group:0:1} != '#' ]] || continue
        local patch_file="$PROJECT_DIR/$relative_path"
        local destination="$WORK_DIR/$component"
        [[ -d $destination ]] || { echo "Unknown patch component: $component" >&2; exit 1; }
        [[ -f $patch_file ]] || { echo "Missing patch: $relative_path" >&2; exit 1; }

        local actual_sha
        actual_sha=$(shasum -a 256 "$patch_file" | awk '{print $1}')
        [[ $actual_sha == "$expected_sha" ]] || {
            echo "Patch SHA-256 mismatch: $relative_path" >&2
            exit 1
        }

        if ! patch --batch --forward -p1 -d "$destination" < "$patch_file"; then
            echo "Unable to apply $relative_path" >&2
            exit 1
        fi
    done < "$series"
}

components=(
    easyrpg-player-0.5.0
    pixman-0.34.0
    liblcf-0.5.0
    libcxx-4.0.1
    uchardet-0.0.8
)
for component in "${components[@]}"; do
    prepare_component "$component"
done

apply_patch_series

echo "Prepared patched build-only dependency trees under $WORK_DIR"

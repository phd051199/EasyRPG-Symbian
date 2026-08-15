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

apply_component_patch() {
    local component=$1 patch_file=$2
    local destination="$WORK_DIR/$component"
    if ! patch --batch --forward -p1 -d "$destination" < "$patch_file"; then
        echo "Unable to apply $patch_file" >&2
        exit 1
    fi
}

components=(
    easyrpg-player-0.5.0
    pixman-0.34.0
    liblcf-0.5.0
    libcxx-4.0.1
    ruby-3.0.7
    mkxp-z-2.4.2
)
for component in "${components[@]}"; do
    prepare_component "$component"
done

apply_component_patch easyrpg-player-0.5.0 \
    "$PROJECT_DIR/patches/easyrpg-player-0.5.0-symbian.patch"
apply_component_patch pixman-0.34.0 \
    "$PROJECT_DIR/patches/pixman-0.34.0-symbian.patch"
apply_component_patch libcxx-4.0.1 \
    "$PROJECT_DIR/patches/libcxx-4.0.1-symbian.patch"
apply_component_patch ruby-3.0.7 \
    "$PROJECT_DIR/patches/ruby-3.0.7-symbian.patch"

echo "Prepared patched build-only dependency trees under $WORK_DIR"

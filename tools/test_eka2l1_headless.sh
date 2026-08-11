#!/usr/bin/env bash
set -Eeuo pipefail

ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
BUILD=0
ARGS=()

for ARG in "$@"; do
    if [[ $ARG == --build ]]; then
        BUILD=1
    else
        ARGS+=("$ARG")
    fi
done

if [[ $BUILD == 1 ]]; then
    bash "$ROOT/build-wine.sh" build
fi

exec node "$ROOT/tools/test_eka2l1_headless.mjs" "${ARGS[@]}"

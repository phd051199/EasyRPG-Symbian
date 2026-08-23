#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
WORK="$PROJECT_DIR/build/control-variable-range-test"
BIN="$WORK/control_variable_range_test"
mkdir -p "$WORK"

${CXX:-c++} -std=c++11 -O2 \
    -I"$PROJECT_DIR/include" \
    "$PROJECT_DIR/tests/control_variable_range_test.cpp" \
    -o "$BIN"

"$BIN"
echo "Control variable range regression: PASS"

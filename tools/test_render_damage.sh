#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
WORK="$PROJECT_DIR/build/render-damage-test"
BIN="$WORK/render_damage_test"
mkdir -p "$WORK"

${CXX:-c++} -std=c++11 -O2 -DUSE_SDL \
    -I"$PROJECT_DIR/include" \
    -I"$PROJECT_DIR/third_party/easyrpg-player-0.5.0/src" \
    "$PROJECT_DIR/tests/render_damage_test.cpp" \
    "$PROJECT_DIR/src/core/RenderDamage.cpp" \
    "$PROJECT_DIR/third_party/easyrpg-player-0.5.0/src/rect.cpp" \
    -o "$BIN"

"$BIN"
echo "Render damage regression: PASS"

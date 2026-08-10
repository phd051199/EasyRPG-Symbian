#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
VENDOR_DIR=${1:-"$PROJECT_DIR/build/vendor"}
CACHE_DIR="$VENDOR_DIR/cache"

PLAYER_VERSION=0.5.0
PLAYER_ARCHIVE="easyrpg-player-${PLAYER_VERSION}.tar.gz"
PLAYER_URL="https://easyrpg.org/downloads/player/${PLAYER_VERSION}/${PLAYER_ARCHIVE}"
PLAYER_SHA256="21c9b95536fb6348e84b11ac8e5908688c6e61e9b4eb426247d546a44869241e"
PLAYER_ROOT="$VENDOR_DIR/easyrpg-player-${PLAYER_VERSION}"

PIXMAN_VERSION=0.34.0
PIXMAN_ARCHIVE="pixman-${PIXMAN_VERSION}.tar.gz"
PIXMAN_URL="https://www.cairographics.org/releases/${PIXMAN_ARCHIVE}"
PIXMAN_SHA256="21b6b249b51c6800dc9553b65106e1e37d0e25df942c90531d4c3997aa20a88e"
PIXMAN_ROOT="$VENDOR_DIR/pixman-${PIXMAN_VERSION}"

mkdir -p "$CACHE_DIR" "$VENDOR_DIR"

fetch_archive() {
    local url=$1
    local archive=$2
    local expected=$3
    local path="$CACHE_DIR/$archive"

    if [[ ! -f $path ]]; then
        curl -L --fail --silent --show-error "$url" -o "$path"
    fi

    local actual
    actual=$(shasum -a 256 "$path" | awk '{print $1}')
    if [[ $actual != "$expected" ]]; then
        rm -f "$path"
        echo "SHA256 mismatch for $archive: $actual" >&2
        exit 1
    fi
}

fetch_archive "$PLAYER_URL" "$PLAYER_ARCHIVE" "$PLAYER_SHA256"
fetch_archive "$PIXMAN_URL" "$PIXMAN_ARCHIVE" "$PIXMAN_SHA256"

if [[ ! -f "$PLAYER_ROOT/src/bitmap.cpp" ]]; then
    rm -rf "$PLAYER_ROOT"
    tar -xzf "$CACHE_DIR/$PLAYER_ARCHIVE" -C "$VENDOR_DIR"
fi

if [[ ! -f "$PIXMAN_ROOT/pixman/pixman.c" ]]; then
    rm -rf "$PIXMAN_ROOT"
    tar -xzf "$CACHE_DIR/$PIXMAN_ARCHIVE" -C "$VENDOR_DIR"
fi

printf 'EasyRPG Player source: %s\n' "$PLAYER_ROOT"
printf 'pixman source: %s\n' "$PIXMAN_ROOT"

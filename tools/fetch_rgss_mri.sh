#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
VENDOR_DIR=${1:-"$PROJECT_DIR/build/vendor"}
CACHE_DIR="$VENDOR_DIR/cache"

RUBY_VERSION=1.8.7-p374
RUBY_ARCHIVE="ruby-${RUBY_VERSION}.tar.gz"
RUBY_URL="https://cache.ruby-lang.org/pub/ruby/1.8/${RUBY_ARCHIVE}"
RUBY_SHA256="876eeeaaeeab10cbf4767833547d66d86d6717ef48fd3d89e27db8926a65276c"
RUBY_ROOT="$VENDOR_DIR/ruby-${RUBY_VERSION}"

mkdir -p "$CACHE_DIR" "$VENDOR_DIR"

archive="$CACHE_DIR/$RUBY_ARCHIVE"
if [[ ! -f $archive ]]; then
    curl -L --fail --silent --show-error "$RUBY_URL" -o "$archive"
fi

actual=$(shasum -a 256 "$archive" | awk '{print $1}')
if [[ $actual != "$RUBY_SHA256" ]]; then
    rm -f "$archive"
    echo "SHA256 mismatch for $RUBY_ARCHIVE: $actual" >&2
    exit 1
fi

if [[ ! -f "$RUBY_ROOT/ruby.h" || ! -f "$RUBY_ROOT/eval.c" ]]; then
    rm -rf "$RUBY_ROOT"
    tar -xzf "$archive" -C "$VENDOR_DIR"
fi

printf 'Ruby MRI source: %s\n' "$RUBY_ROOT"

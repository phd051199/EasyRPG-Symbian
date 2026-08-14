#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
VENDOR_DIR=${1:-"$PROJECT_DIR/build/vendor"}
CACHE_DIR="$VENDOR_DIR/cache"

RUBY_VERSION=${EASYRPG_RGSS_RUBY_VERSION:-1.8.7-p374}
case "$RUBY_VERSION" in
    1.8.7-p374)
        RUBY_SERIES=1.8
        RUBY_SHA256="876eeeaaeeab10cbf4767833547d66d86d6717ef48fd3d89e27db8926a65276c"
        RUBY_HEADER=ruby.h
        ;;
    3.0.7)
        RUBY_SERIES=3.0
        RUBY_SHA256="2a3411977f2850431136b0fab8ad53af09fb74df2ee2f4fb7f11b378fe034388"
        RUBY_HEADER=include/ruby.h
        ;;
    *)
        echo "Unsupported RGSS Ruby version: $RUBY_VERSION" >&2
        exit 2
        ;;
esac
RUBY_ARCHIVE="ruby-${RUBY_VERSION}.tar.gz"
RUBY_URL="https://cache.ruby-lang.org/pub/ruby/${RUBY_SERIES}/${RUBY_ARCHIVE}"
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

if [[ ! -f "$RUBY_ROOT/$RUBY_HEADER" || ! -f "$RUBY_ROOT/ruby.c" ]]; then
    rm -rf "$RUBY_ROOT"
    tar -xzf "$archive" -C "$VENDOR_DIR"
fi

if [[ $RUBY_VERSION == 3.0.7 ]]; then
    ruby "$PROJECT_DIR/tools/patch_rgss_mri300.rb" "$RUBY_ROOT"
fi

printf 'Ruby MRI source: %s\n' "$RUBY_ROOT"

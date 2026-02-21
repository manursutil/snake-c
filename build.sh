#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"
OUTPUT_BIN="$BUILD_DIR/snake"

if ! command -v pkg-config >/dev/null 2>&1; then
  echo "Error: pkg-config is required but not installed." >&2
  exit 1
fi

if ! pkg-config --exists raylib; then
  echo "Error: raylib is not available via pkg-config." >&2
  echo "Install raylib first: https://github.com/raysan5/raylib" >&2
  exit 1
fi

SOURCES=()
while IFS= read -r file; do
  SOURCES+=("$file")
done < <(find "$ROOT_DIR/src" -type f -name "*.c" | sort)

if [ "${#SOURCES[@]}" -eq 0 ]; then
  echo "Error: no C sources found under $ROOT_DIR/src" >&2
  exit 1
fi

mkdir -p "$BUILD_DIR"

gcc \
  -Wall -Wextra -Wpedantic -std=c11 \
  -I"$ROOT_DIR" \
  "${SOURCES[@]}" \
  $(pkg-config --cflags --libs raylib) \
  -o "$OUTPUT_BIN"

echo "Built: $OUTPUT_BIN"

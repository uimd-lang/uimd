#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd "$(dirname "$0")" && pwd)
ROOT_DIR=$(CDPATH= cd "$SCRIPT_DIR/.." && pwd)

if [ -z "${UIMD_LIBSIXEL_PATH:-}" ] && [ -z "${UIMD_LIBSIXEL_DIR:-}" ] && command -v brew >/dev/null 2>&1; then
    if UIMD_HOMEBREW_LIBSIXEL_PREFIX=$(brew --prefix libsixel 2>/dev/null); then
        if [ -d "$UIMD_HOMEBREW_LIBSIXEL_PREFIX/lib" ]; then
            UIMD_LIBSIXEL_DIR="$UIMD_HOMEBREW_LIBSIXEL_PREFIX/lib"
            export UIMD_LIBSIXEL_DIR
        fi
    fi
fi

cd "$ROOT_DIR"
python3 tools/uimd_dev.py test-all "$@"

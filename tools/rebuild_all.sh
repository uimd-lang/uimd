#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd "$(dirname "$0")" && pwd)
ROOT_DIR=$(CDPATH= cd "$SCRIPT_DIR/.." && pwd)

cd "$ROOT_DIR"

echo "==> Compile Python dialogs"
./uimd generate python/dialogs --target python

echo "==> Compile Python examples"
./uimd generate python/examples --target python

echo "==> Compile package dialogs"
./uimd generate src/uimd/dialogs --target python

echo "==> Compile package tester UIs"
./uimd generate src/uimd/testing --target python

echo "==> Compile C++ dialogs and examples"
./uimd generate cpp/dialogs --target cpp
./uimd generate cpp/examples --target cpp

if [ ! -f cpp/build/CMakeCache.txt ]
then
    echo "==> Configure C++ build"
    cmake -S cpp -B cpp/build
fi

echo "==> Build C++ targets"
cmake --build cpp/build

echo "==> Byte-compile Python sources"
python3 -m compileall python src tests tools

echo "==> Full rebuild complete"

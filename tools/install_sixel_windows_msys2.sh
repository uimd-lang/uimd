#!/usr/bin/env bash
set -euo pipefail

export PATH=/ucrt64/bin:/usr/bin:${PATH}
export ACLOCAL_PATH=/ucrt64/share/aclocal
export PKG_CONFIG_PATH=/ucrt64/lib/pkgconfig

dll=/ucrt64/bin/libsixel-1.dll
if [[ -f "${dll}" ]]; then
    echo "libsixel already installed: ${dll}"
    exit 0
fi

echo "Updating MSYS2 package databases..."
pacman -Syu --noconfirm

echo "Installing MSYS2/UCRT64 build dependencies..."
pacman -S --needed --noconfirm \
    git \
    autoconf-wrapper \
    automake-wrapper \
    libtool \
    make \
    m4 \
    perl \
    mingw-w64-ucrt-x86_64-gcc \
    mingw-w64-ucrt-x86_64-pkgconf \
    mingw-w64-ucrt-x86_64-libpng \
    mingw-w64-ucrt-x86_64-libjpeg-turbo

build=/tmp/uimd-libsixel
if [[ -e "${build}" && ! -d "${build}/.git" ]]; then
    echo "error: ${build} exists but is not a git checkout" >&2
    exit 1
fi

if [[ ! -d "${build}/.git" ]]; then
    git clone https://github.com/saitoha/libsixel.git "${build}"
else
    git -C "${build}" pull --ff-only || true
fi

cd "${build}"
./autogen.sh
./configure --prefix=/ucrt64 --disable-python --enable-shared --disable-static
make -j"$(nproc)"
make install

if [[ ! -f "${dll}" ]]; then
    echo "error: libsixel build finished but ${dll} was not found" >&2
    exit 1
fi

echo "libsixel installed: ${dll}"

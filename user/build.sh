#!/bin/sh
set -e

WORK_DIR="$(pwd)"
BUILD_DIR="$WORK_DIR/build"
INSTALL_DIR="$WORK_DIR/install"
PICOLIBC_DIR="$WORK_DIR/picolibc"

if [ ! -d "picolibc" ]; then
    echo "picolibc not found, cloning now..."
    git clone https://github.com/picolibc/picolibc.git
fi

rm -rf "$BUILD_DIR"
rm -rf "$INSTALL_DIR"
mkdir "$BUILD_DIR"

cd "$BUILD_DIR"
meson setup \
    --cross-file "$WORK_DIR/cross.txt" \
    --prefix="$INSTALL_DIR" \
    -Dmultilib=false \
    -Dpicocrt=true \
    -Dpicocrt-lib=true \
    -Dsemihost=false \
    -Dos-linux=true \
    -Dposix-console=true \
    -Dtests=false \
    -Dthread-local-storage=false \
    "$PICOLIBC_DIR"

ninja
ninja install

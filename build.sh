#!/bin/sh
set -e
set -x

ROOT_DIR="$(pwd)"
QEMU_DIR="$ROOT_DIR/qemu-10.2.0"
LINUX_DIR="$ROOT_DIR/linux-7.0"
BOOTLOADER_DIR="$ROOT_DIR/bootloader"
BUILDROOT_DIR="$ROOT_DIR/user/buildroot-2026.02"

fetch_sources() {
    if [ ! -d "$QEMU_DIR" ]; then
        set +x
        echo "QEMU is not installed, fetching QEMU source"
        set -x
        wget https://download.qemu.org/qemu-10.2.0.tar.xz
        tar xvJf qemu-10.2.0.tar.xz
        cd qemu-10.2.0
        patch -p1  < ../qemu-10.2.0.patch
        cd "$ROOT_DIR"
    fi

    if [ ! -d "$LINUX_DIR" ]; then
        set +x
        echo "Linux is not installed, fetching Linux source"
        set -x
        wget https://cdn.kernel.org/pub/linux/kernel/v7.x/linux-7.0.tar.xz
        tar xvf linux-7.0.tar.xz
        cd linux-7.0
        patch -p1  < ../linux-7.0.patch
        cd "$ROOT_DIR"
    fi

    if [ ! -d "$BUILDROOT_DIR" ]; then
        set +x
        echo "Buildroot is not installed, fetching buildroot source"
        set -x
        cd "$ROOT_DIR/user"
        wget https://buildroot.org/downloads/buildroot-2026.02.tar.gz
        tar xvf buildroot-2026.02.tar.gz
        cd "$ROOT_DIR"
    fi

    echo "Fetch sources done"
}

build_qemu() {
    cd "$QEMU_DIR"
    ./configure --target-list=arm-softmmu --without-default-devices
    ninja -C build
    cd "$ROOT_DIR"
}

build_linux() {
    make linux
}

build_bootloader() {
    cd "$BOOTLOADER_DIR"
    make
    cd "$ROOT_DIR"
}

build_busybox() {
    cd "$ROOT_DIR/user"
    ./build.sh
    cd "$ROOT_DIR"
}

build_rootfs() {
    mkdir -p rootfs
    mkdir -p rootfs/lib
    mkdir -p rootfs/bin


    cp "$BUILDROOT_DIR/output/target/lib/libuClibc-1.0.56.so" rootfs/lib/libc.so.0
    cp "$BUILDROOT_DIR/output/target/lib/ld-uClibc-1.0.56.so" rootfs/lib/ld-uClibc.so.0

    cp "$BUILDROOT_DIR/output/build/busybox-1.37.0/busybox" rootfs/bin/
    if [ ! -f "rootfs/bin/sh" ]; then
        ln -s busybox rootfs/bin/sh
    fi
    if [ ! -f "rootfs/bin/cd" ]; then
        ln -s busybox rootfs/bin/cd
    fi
    if [ ! -f "rootfs/bin/ls" ]; then
        ln -s busybox rootfs/bin/ls
    fi
    if [ ! -f "rootfs/bin/pwd" ]; then
        ln -s busybox rootfs/bin/pwd
    fi
    if [ ! -f "rootfs/bin/uname" ]; then
        ln -s busybox rootfs/bin/uname
    fi
    make rootfs
}

build() {
    set +x
    echo "Start building"
    set -x
    build_qemu
    build_linux
    build_bootloader
    build_busybox
    build_rootfs
    make kernel
}


fetch_sources
build

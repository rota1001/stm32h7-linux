#!/bin/sh
set -e
set -x

ROOT_DIR="$(pwd)"
QEMU_DIR="$ROOT_DIR/qemu-10.2.0"
LINUX_DIR="$ROOT_DIR/linux-6.19"
BOOTLOADER_DIR="$ROOT_DIR/bootloader"
BUILDROOT_DIR="$ROOT_DIR/user/buildroot-2025.02"

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
        wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.19.tar.xz
        tar xvf linux-6.19.tar.xz
        cd linux-6.19
        patch -p1  < ../linux-6.19.patch
        chmod +x build.sh
        cd "$ROOT_DIR"
    fi

    if [ ! -d "$BUILDROOT_DIR" ]; then
        set +x
        echo "Buildroot is not installed, fetching buildroot source"
        set -x
        cd "$ROOT_DIR/user"
        wget https://buildroot.org/downloads/buildroot-2025.02.tar.gz
        tar xvf buildroot-2025.02.tar.gz
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
    cd "$LINUX_DIR"
    ./build.sh
    cd "$ROOT_DIR"
}

build_bootloader() {
    cd "$BOOTLOADER_DIR"
    make
    cd "$ROOT_DIR"
}

build_toybox() {
    cd "$ROOT_DIR/user"
    ./build.sh
    cd "$ROOT_DIR"
}

build_rootfs() {
    mkdir -p rootfs
    mkdir -p rootfs/dev
    mkdir -p rootfs/bin
    if [ ! -e "rootfs/dev/console" ]; then
        sudo mknod rootfs/dev/console c 5 1
    fi
    if [ ! -e "rootfs/dev/null" ]; then
        sudo mknod rootfs/dev/null c 1 3
    fi
    cp user/toybox/generated/unstripped/toybox rootfs/bin/toybox
    cp user/toybox/generated/unstripped/toybox.gdb init.gdb
    if [ ! -f "rootfs/bin/sh" ]; then
        ln -s toybox rootfs/bin/sh
    fi
    if [ ! -f "rootfs/bin/cd" ]; then
        ln -s toybox rootfs/bin/cd
    fi
    if [ ! -f "rootfs/bin/ls" ]; then
        ln -s toybox rootfs/bin/ls
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
    build_toybox
    build_rootfs
    make kernel
}


fetch_sources
build

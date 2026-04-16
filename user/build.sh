#!/bin/sh
set -e

WORK_DIR="$(pwd)"
BUILDROOT_DIR="$WORK_DIR/buildroot-2026.02"


cp busybox-minimal.config "$BUILDROOT_DIR/package/busybox"
cp uClibc-ng.config "$BUILDROOT_DIR/package/uclibc"
cp buildroot.config "$BUILDROOT_DIR/configs/custom_defconfig"

cd "$BUILDROOT_DIR"
make custom_defconfig
make -j`nproc`

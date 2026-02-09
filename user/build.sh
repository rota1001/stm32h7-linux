#!/bin/sh
set -e

WORK_DIR="$(pwd)"
BUILDROOT_DIR="$WORK_DIR/buildroot-2025.02"

cp buildroot.config "$BUILDROOT_DIR"
cp uclibc.mk "$BUILDROOT_DIR/package/uclibc"
cd "$BUILDROOT_DIR"
make uclibc-dirclean
make uclibc -j`nproc`

cp ../Makefile.commonarch output/build/uclibc-1.0.51/libc/sysdeps/linux/
cp ../crt1.S output/build/uclibc-1.0.51/libc/sysdeps/linux/arm/
cd output/build/uclibc-1.0.51
rm -f lib/crt1.o
make lib/crt1.o
cp lib/crt1.o ../../host/arm-buildroot-uclinux-uclibcgnueabi/sysroot/usr/lib/crt1.o


cd "$WORK_DIR/toybox"
make clean
cp ../toybox.config .config
CROSS_COMPILE=../buildroot-2025.02/output/host/bin/arm-linux- \
CFLAGS="-Os -mthumb -mcpu=cortex-m7 -fpic -mpic-register=r10 -mno-pic-data-is-text-relative" \
LDFLAGS="-Wl,--gc-sections" make

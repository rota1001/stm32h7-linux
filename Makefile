BOOTLOADER_BIN = bootloader/build/bootloader.bin

all: BOOTLOADER_BIN kernel

qemu-10.2.0/build/qemu-system-arm:
	cd qemu-10.2.0 \
	&& ./configure --target-list=arm-softmmu --without-default-devices \
	&& ninja -C build


debug: qemu-10.2.0/build/qemu-system-arm
	foot -e zsh -c "gdb-multiarch -x gdbscript.py" &
	qemu-10.2.0/build/qemu-system-arm -machine stm32h750 -s -S \
	-kernel $(BOOTLOADER_BIN) -serial stdio -display none \
	-device loader,file=build/kernel.bin,addr=0x90000000

BOOTLOADER_BIN:
	make -C bootloader

rootfs:
	genromfs -d rootfs -f rootfs.img

linux:
	cd linux-6.19 && ./build.sh

linux-6.19/arch/arm/boot/xipImage:
	cd linux-6.19 && ./build.sh

kernel: rootfs linux-6.19/arch/arm/boot/xipImage
	mkdir -p build
	cp linux-6.19/arch/arm/boot/xipImage build/kernel.bin
	truncate -s 4M build/kernel.bin
	cat linux-6.19/arch/arm/boot/dts/st/stm32h750vbt6.dtb >> build/kernel.bin
	truncate -s 6M build/kernel.bin
	cat rootfs.img >> build/kernel.bin

dtb:
	make -C linux-6.19 ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- dtbs

flash-dummy: dummy.bin
	st-flash --reset write dummy.bin 0x8000000

flash-kernel: kernel
	make flash-dummy
	minipro -p 'W25Q128JV@SOIC8' --spi_clock=30 -w build/kernel.bin -s

flash-bootloader:
	make -C bootloader flash

flash: flash-kernel flash-bootloader

init:
	cd user && ./build.sh

clean:
	make -C bootloader clean

.PHONY: qemu kernel flash flash-kernel flash-bootloader rootfs kernel linux dtb init


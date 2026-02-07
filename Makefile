BOOTLOADER_BIN = bootloader/build/bootloader.bin

all: BOOTLOADER_BIN

qemu-10.2.0/build/qemu-system-arm:
	cd qemu-10.2.0 \
	&& ./configure --target-list=arm-softmmu --without-default-devices \
	&& ninja -C build

qemu: qemu-10.2.0/build/qemu-system-arm
	qemu-10.2.0/build/qemu-system-arm -machine stm32h750 -s  \
	-kernel $(BOOTLOADER_BIN) -serial stdio -display none \
	-device loader,file=linux-6.18.7/arch/arm/boot/xipImage,addr=0x90000000 \
	-device loader,file=linux-6.18.7/arch/arm/boot/dts/st/stm32h750vbt6.dtb,addr=0x90400000 \
	-device loader,file=rootfs.img,addr=0x90600000

debug: qemu-10.2.0/build/qemu-system-arm
	foot -e zsh -c "gdb-multiarch -x gdbscript.py" &
	qemu-10.2.0/build/qemu-system-arm -machine stm32h750 -s -S \
	-kernel $(BOOTLOADER_BIN) -serial stdio -display none \
	-device loader,file=build/kernel.bin,addr=0x90000000

BOOTLOADER_BIN:
	make -C bootloader

kernel:
	mkdir -p build
	cp linux-6.18.7/arch/arm/boot/xipImage build/kernel.bin
	truncate -s 4M build/kernel.bin
	cat linux-6.18.7/arch/arm/boot/dts/st/stm32h750vbt6.dtb >> build/kernel.bin
	truncate -s 6M build/kernel.bin
	cat rootfs.img >> build/kernel.bin

init:
	make -C user
	cp user/init rootfs
	cp user/init.gdb .
	genromfs -d rootfs -f rootfs.img
	make kernel

flash-dummy: dummy.bin
	st-flash --reset write dummy.bin 0x8000000

flash-kernel: kernel
	make flash-dummy
	minipro -p 'W25Q128JV@SOIC8' --spi_clock=30 -w build/kernel.bin -s
	make flash

clean:
	make -C bootloader clean

.PHONY: qemu kernel flash-kernel

BOOTLOADER_BIN = bootloader/build/bootloader.bin

all: BOOTLOADER_BIN

qemu-10.2.0/build/qemu-system-arm:
	cd qemu-10.2.0 \
	&& ./configure --target-list=arm-softmmu --without-default-devices \
	&& ninja -C build

qemu: qemu-10.2.0/build/qemu-system-arm
	qemu-10.2.0/build/qemu-system-arm -machine stm32h750 -s -kernel $(BOOTLOADER_BIN) -serial stdio -display none
	echo "yee"

BOOTLOADER_BIN:
	make -C bootloader

clean:
	make -C bootloader clean

.PHONY: qemu

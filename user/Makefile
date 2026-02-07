TOOLCHAIN_PATH=`pwd`/../armv7m--uclibc--bleeding-edge-2025.08-1
CC=$(TOOLCHAIN_PATH)/bin/arm-linux-gcc
ELF2FLT_LD=$(TOOLCHAIN_PATH)/arm-buildroot-uclinux-uclibcgnueabi/lib/elf2flt.ld

PICOLIBC_SPECS=build/picolibc.specs
PICOLIBC_CRT0=install/lib/crt0-linux.o

ARCH_FLAGS=-mcpu=cortex-m7 -mthumb
PIC_FLAGS=-static -fpic -mpic-register=r10

CFLAGS=-specs=$(PICOLIBC_SPECS) $(ARCH_FLAGS) $(PIC_FLAGS)
LDFLAGS=-T$(ELF2FLT_LD) -nostartfiles $(PICOLIBC_CRT0)
LDFLAGS+=-Wl,--start-group -lgcc -lc -llinux -los-fallback -Wl,--end-group

init: init.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f init init.gdb

.PHONY: clean

CROSS_COMPILE ?= arm-none-eabi-
.PHONY: all clean

SRCS = src/boot.c src/usart.c src/rcc.c src/qspi.c
LINKER = linker.ld

OBJS = $(SRCS:%.c=build/%.o)

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy


CFLAGS = -mcpu=cortex-m7 -mthumb
CFLAGS += -nostartfiles -g -nostdlib -ffreestanding
CFLAGS += -Iinclude

TARGET = bootloader

all: build/$(TARGET).bin

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/$(TARGET).out: $(OBJS) $(LINKER)
	$(CC) $(CFLAGS) -T $(LINKER) -Wl,-Map=build/main.map -Wl,--gc-sections -o $@ \
	-Wl,--start-group $(OBJS) -lgcc -Wl,--end-group

build/$(TARGET).bin: build/$(TARGET).out
	$(OBJCOPY) -O binary $< $@

flash: build/$(TARGET).bin
	st-flash --reset write build/$(TARGET).bin 0x8000000


clean:
	rm -rf build/*

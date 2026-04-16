# Linux on STM32H750
> My note: https://hackmd.io/@rota1001/stm32h750-linux

In this project, I successfully run **Linux 7.0** on STM32H750, which only has **1MB of RAM**, and run **dynamically linked Busybox** with **uClibc** support.

<img src="./image.png" >

## Features
- A **QEMU SoC model** with a core peripheral subset (UART, Memory and Timer), successfully booted Linux kernel and run a user program (With some function hooking in the gdb script)
- A **minimal bootloader (only 12KB)** to load the linux kernel
- Used **SPARSEMEM memory model** to utilize non-contiguous memory regions
- POSIX library function support with **uClibc**
- **Execute-in-Place** user program, running without loading readonly segments to RAM
- Run **dynamically linked Busybox** in FDPIC format backed by uClibc support

## Dependencies
There are some dependencies that needed to be installed.
```
sudo apt update
sudo apt install -y gcc-arm-linux-gnueabi 
```

## How to Play the Pre-build Binary
I provide a pre-build binary, if you trust me.

First, you should have `gdb-multiarch`.

Second, download the pre-build binary, and have fun!
```
wget https://github.com/rota1001/stm32h7-linux/releases/download/pre-build/pre-build.tar.gz
tar zxvf pre-build.tar.gz
cd pre-build
chmod +x run.sh
./run.sh
```

If you want to use gdb to do the debug, you can run the following command in the pre-build directory:
```
./qemu-system-arm -machine stm32h750 -s -S \
-kernel bootloader.bin -serial stdio -display none \
-device loader,file=kernel.bin,addr=0x90000000
```
This will open a gdb server at localhost:1234.
You can open another shell and run the following command also in the pre-build directory:
```
gdb-multiarch -x gdbscript.py
```
Then you can start the kernel debug.

Also, you can take a look at the implementation in gdbscript.py to go further.

## Build it in one click
I provide a simple build script, you can run it directly:
```
git clone --depth 1 https://github.com/rota1001/stm32h7-linux.git
cd stm32h7-linux
./build.sh
```

## How to Build it Step by Step
If you are curious about the compile process, here it is.

It is a little bit complex, you can see [my note](https://hackmd.io/@rota1001/stm32h750-linux) for the details.

Here is the process:
- Build the qemu
  
  ```
  cd qemu-10.2.0
  ninja -C build
  ```
- Build the bootloader
  ```
  cd bootloader
  make
  ```
- Get kernel source and patch it
  ```
  wget https://cdn.kernel.org/pub/linux/kernel/v7.x/linux-7.0.tar.xz
  tar xvf linux-7.0.tar.xz
  cd linux-7.0
  patch -p1  < ../linux-7.0.patch
  ```
- Build the kernel
  ```
  make linux
  ```
- Create the rootfs directory
  ```
  mkdir -p rootfs
  mkdir -p rootfs/lib
  mkdir -p rootfs/bin
  ```
- Build the uClibc and the Busybox
  ```
  cd user
  ./build.sh
  cd ..
  cp user/buildroot-2026.02/output/target/lib/libuClibc-1.0.56.so rootfs/lib/libc.so.0
  cp user/buildroot-2026.02/output/target/lib/ld-uClibc-1.0.56.so rootfs/lib/ld-uClibc.so.0
  cp user/buildroot-2026.02/output/build/busybox-1.37.0/busybox rootfs/bin/
  ln -s busybox rootfs/bin/sh
  ln -s busybox rootfs/bin/ls
  ln -s busybox rootfs/bin/uname
  make rootfs
  ```
- Build the kernel image
  ```
  make kernel
  ```
- Run it
  ```
  make debug
  ```
## How to Play it on the Real Board
First, you should have a board with STM32H750 SoC and the QSPI external flash.

Second, flash the kernel image (build/kernel.bin) to QSPI external flash, you can use tools like CH341 or T48 programmer. You can also program a firmware to write the flash directly.

Third, use tools like ST-LINK or J-Link to flash the bootloader to the internal flash.

Last, connect USART1 to your computer through a TTL-to-USB module, and use tools like `minicon`, `neocon` to get a console.

If you do these perfectly, it will work.

# Linux on STM32H750
> My note: https://hackmd.io/@rota1001/stm32h750-linux

In this project, I successfully run **Linux 6.19** on STM32H750, which only has **1MB of RAM**, and run toybox with uClibc support.

<img src="./image.png" width="50%">

## Features
- A **QEMU SoC model** with a core peripheral subset (UART, Memory and Timer), successfully booted Linux kernel and run a user program (With some function hooking in the gdb script)
- A **minimal bootloader (only 12KB)** to load the linux kernel
- Used **SPARSEMEM memory model** to utilize non-contiguous memory regions
- POSIX library function support with **uClibc**
- Run toybox, which is a lightweight busybox

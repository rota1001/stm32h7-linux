# Linux on STM32H750
> My note: https://hackmd.io/@rota1001/stm32h750-linux

This project aims to port Linux to the STM32H750, a platform with a highly constrained 1MB internal SRAM. This is currently a work-in-progress. The kernel now successfully boots and the scheduler is functional; the next thing to do is to execute the first init program.

## Current Progress
- A **QEMU SoC model** with a core peripheral subset (UART, Memory and Timer), successfully booted Linux kernel and run a user program (With some function hooking in the gdb script)
- A **minimal bootloader (only 12KB)** to load the linux kernel
- Reduced Linux kernel .bss and .data usage to 100KB
- Used **SPARSEMEM memory model** to utilize non-contiguous memory regions
- Successfully booted, started scheduling and run a minimal user program

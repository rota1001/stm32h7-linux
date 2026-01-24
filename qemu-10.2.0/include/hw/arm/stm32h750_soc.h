#ifndef HW_ARM_STM32H750_SOC_H
#define HW_ARM_STM32H750_SOC_H

#include "hw/char/stm32l4x5_usart.h"
#include "hw/or-irq.h"
#include "hw/arm/armv7m.h"
#include "qom/object.h"

#define TYPE_STM32H750_SOC "stm32h750-soc"
OBJECT_DECLARE_SIMPLE_TYPE(STM32H750State, STM32H750_SOC)

#define FLASH_BASE 0x8000000
#define FLASH_SIZE (128 * 1024)
#define DTCMRAM_BASE 0x20000000
#define DTCMRAM_SIZE (128 * 1024)
#define SRAM1_BASE 0x30000000
#define SRAM1_SIZE (288 * 1024)
#define SRAM4_BASE 0x38000000
#define SRAM4_SIZE (64 * 1024)
#define AXISRAM_BASE 0x24000000
#define AXISRAM_SIZE (512 * 1024)


struct STM32H750State {
    SysBusDevice parent_obj;
    ARMv7MState armv7m;
    Stm32l4x5UsartBaseState usart1;
    
    MemoryRegion flash;
    MemoryRegion dtcmram;
    MemoryRegion sram1;
    MemoryRegion sram4;
    MemoryRegion axisram;

    MemoryRegion flash_alias;

    Clock *sysclk;
};

#endif

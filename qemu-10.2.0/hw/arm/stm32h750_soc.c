#include "qemu/osdep.h"
#include "qapi/error.h"
#include "system/address-spaces.h"
#include "system/system.h"
#include "hw/arm/stm32h750_soc.h"
#include "hw/qdev-clock.h"
#include "hw/misc/unimp.h"


static void stm32h750_soc_initfn(Object *obj)
{
    STM32H750State *s = STM32H750_SOC(obj);
    object_initialize_child(obj, "armv7m", &s->armv7m, TYPE_ARMV7M);
    object_initialize_child(obj, "usart1", &s->usart1, TYPE_STM32L4X5_USART);

    s->sysclk = qdev_init_clock_in(DEVICE(s), "sysclk", NULL, NULL, 0);
}

static void stm32h750_soc_realize(DeviceState *dev_soc, Error **errp)
{
    STM32H750State *s = STM32H750_SOC(dev_soc);
    MemoryRegion *system_memory = get_system_memory();
    DeviceState *armv7m;

    armv7m = DEVICE(&s->armv7m);
    qdev_prop_set_string(armv7m, "cpu-type", ARM_CPU_TYPE_NAME("cortex-m7"));
    qdev_prop_set_uint32(armv7m, "num-irq", 150);
    object_property_set_link(OBJECT(&s->armv7m), "memory",
                            OBJECT(system_memory), &error_abort);
    qdev_connect_clock_in(armv7m, "cpuclk", s->sysclk);
    qdev_connect_clock_in(DEVICE(&s->usart1), "clk", s->sysclk);

    if (!sysbus_realize(SYS_BUS_DEVICE(&s->armv7m), errp))
        return;

    memory_region_init_rom(&s->flash, OBJECT(dev_soc), "flash", FLASH_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, FLASH_BASE, &s->flash);

    memory_region_init_ram(&s->dtcmram, OBJECT(dev_soc), "dtcmram", DTCMRAM_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, DTCMRAM_BASE, &s->dtcmram);

    memory_region_init_ram(&s->sram1, OBJECT(dev_soc), "sram1", SRAM1_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, SRAM1_BASE, &s->sram1);

    memory_region_init_ram(&s->sram4, OBJECT(dev_soc), "sram4", SRAM4_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, SRAM4_BASE, &s->sram4);

    memory_region_init_ram(&s->axisram, OBJECT(dev_soc), "axisram", AXISRAM_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, AXISRAM_BASE, &s->axisram);

    memory_region_init_alias(&s->flash_alias, OBJECT(dev_soc), "flash_alias", &s->flash, 0, FLASH_SIZE);
    memory_region_add_subregion(system_memory, 0, &s->flash_alias);

    DeviceState *dev = DEVICE(&s->usart1);
    qdev_prop_set_chr(dev, "chardev", serial_hd(0));
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->usart1), errp))
        return;
    
    SysBusDevice *busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, 0x40011000);
    sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, 37));


}

static void stm32h750_soc_class_init(ObjectClass *oc, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    dc->realize = stm32h750_soc_realize;
}

static const TypeInfo stm32h750_soc_info = {
    .name          = TYPE_STM32H750_SOC,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(STM32H750State),
    .instance_init = stm32h750_soc_initfn,
    .class_init    = stm32h750_soc_class_init,
};

static void stm32h750_soc_types(void)
{
    type_register_static(&stm32h750_soc_info);
}

type_init(stm32h750_soc_types)

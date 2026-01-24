#include "qemu/osdep.h"
#include "qapi/error.h"
#include "hw/boards.h"
#include "hw/qdev-properties.h"
#include "hw/qdev-clock.h"
#include "hw/arm/stm32h750_soc.h"
#include "hw/arm/boot.h"
#include "hw/arm/machines-qom.h"

struct STM32H750MachineState {
    MachineState parent_obj;
    STM32H750State soc;
};

#define SYSCLK_FRQ 25000000ULL

static void stm32h750_board_init(MachineState *machine)
{
    DeviceState *dev = qdev_new(TYPE_STM32H750_SOC);
    object_property_add_child(OBJECT(machine), "soc", OBJECT(dev));

    Clock *sysclk = clock_new(OBJECT(machine), "SYSCLK");
    clock_set_hz(sysclk, SYSCLK_FRQ);
    qdev_connect_clock_in(dev, "sysclk", sysclk);

    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    armv7m_load_kernel(STM32H750_SOC(dev)->armv7m.cpu, machine->kernel_filename, 0x08000000, 128 * 1024);
}

static void stm32h750_machine_init(MachineClass *mc)
{
    static const char * const valid_cpu_types[] = {
        ARM_CPU_TYPE_NAME("cortex-m7"),
        NULL
    };
    mc->desc = "Custom STM32H750 Board";
    mc->init = stm32h750_board_init;
    mc->valid_cpu_types = valid_cpu_types;
}

DEFINE_MACHINE_ARM("stm32h750", stm32h750_machine_init)

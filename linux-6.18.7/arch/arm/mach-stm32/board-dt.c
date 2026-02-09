// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/memblock.h>
#include <linux/mm.h>
#include <linux/gfp.h>
#include <asm/mach/arch.h>
#ifdef CONFIG_ARM_SINGLE_ARMV7M
#include <asm/v7m.h>
#endif

#define BODYGUARD_PHYS_START  0x30000000
#define BODYGUARD_SIZE        (128 * 1024)

static void __init stm32_reserve(void)
{
    memblock_reserve(BODYGUARD_PHYS_START, BODYGUARD_SIZE);
}

static int __init release_bodyguard_memory(void)
{
    unsigned long start_pfn = PHYS_PFN(BODYGUARD_PHYS_START);
    unsigned long end_pfn   = PHYS_PFN(BODYGUARD_PHYS_START + BODYGUARD_SIZE);
    unsigned long pfn;
    pr_info("[BODYGUARD] Release: 0x%08x - 0x%08x\n", BODYGUARD_PHYS_START, BODYGUARD_PHYS_START + BODYGUARD_SIZE);
    memblock_free(BODYGUARD_PHYS_START, BODYGUARD_SIZE);

    for (pfn = start_pfn; pfn < end_pfn; pfn++) {
        struct page *page = pfn_to_page(pfn);
        
        if (PageReserved(page)) {
            ClearPageReserved(page);
            init_page_count(page);
            __free_page(page);
        }
    }

    return 0;
}
late_initcall(release_bodyguard_memory);

static const char *const stm32_compat[] __initconst = {
    "st,stm32f429",
    "st,stm32f469",
    "st,stm32f746",
    "st,stm32f769",
    "st,stm32h743",
    "st,stm32h747",
    "st,stm32h750",
    "st,stm32mp131",
    "st,stm32mp133",
    "st,stm32mp135",
    "st,stm32mp151",
    "st,stm32mp157",
    NULL
};

DT_MACHINE_START(STM32DT, "STM32 (Device Tree Support)")
    .dt_compat = stm32_compat,
#ifdef CONFIG_ARM_SINGLE_ARMV7M
    .restart = armv7m_restart,
#endif
    .reserve = stm32_reserve,
MACHINE_END

#include <regs.h>
#include <usart.h>
#include <rcc.h>
#include <qspi.h>

extern unsigned long _stack_top, _data_lma,
    _data_vma_start, _data_vma_end, _bss_vma_start, _bss_vma_end;

static void sram_init(void)
{
    unsigned long *src = &_data_lma;
    unsigned long *dst = &_data_vma_start;
    while (dst < &_data_vma_end)
        *dst++ = *src++;

    dst = &_bss_vma_start;
    while (dst < &_bss_vma_end)
        *dst++ = 0;
}

static void enable_icache(void)
{
    asm volatile("dsb");
    asm volatile("isb");
    SCB_ICIALLU = 0;
    asm volatile ("dsb");
    asm volatile ("isb");
    SCB_CCR |= (1 << 17);
    asm volatile ("dsb");
    asm volatile ("isb");
}


__attribute__((section(".reset_isr"))) void reset_isr()
{
    RCC_AHB2ENR |=
        RCC_AHB2ENR_SRAM1EN | RCC_AHB2ENR_SRAM2EN | RCC_AHB2ENR_SRAM3EN;
    __asm volatile("dsb");
    rcc_init();
    qspi_init();
    sram_init();
    enable_icache();
    usart_init(115200);
    void (*kernel)(unsigned long, unsigned long, unsigned long) =(void (*)(unsigned long, unsigned long, unsigned long))0x90000001;
    kernel(0, ~0UL, 0x90400000);

    while (1)
        ;
}


#define MSP 0
#define RESET 1


__attribute__((section(".isr_vector"))) unsigned long isr_vec[] = {
    [MSP](unsigned long) & _stack_top,
    [RESET](unsigned long) reset_isr,
};

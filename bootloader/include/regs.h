#pragma once

#define BIT(x) (1UL << (x))

#define D1_AHB1PERIPH_BASE 0x52000000UL
#define AHB4PERIPH_BASE 0x58020000UL
#define APB2PERIPH_BASE 0x40010000UL

#define RCC_BASE 0x58024400UL
#define RCC_CR (*(volatile unsigned long *) (RCC_BASE + 0x000UL))
#define RCC_CFGR (*(volatile unsigned long *) (RCC_BASE + 0x010UL))
#define RCC_D1CFGR (*(volatile unsigned long *) (RCC_BASE + 0x01CUL))
#define RCC_D2CFGR (*(volatile unsigned long *) (RCC_BASE + 0x020UL))
#define RCC_D3CFGR (*(volatile unsigned long *) (RCC_BASE + 0x024UL))
#define RCC_PLLCKSELR (*(volatile unsigned long *) (RCC_BASE + 0x028UL))
#define RCC_PLLCFGR (*(volatile unsigned long *) (RCC_BASE + 0x02CUL))
#define RCC_PLL1DIVR (*(volatile unsigned long *) (RCC_BASE + 0x030UL))
#define RCC_PLL1FRACR (*(volatile unsigned long *) (RCC_BASE + 0x034UL))
#define RCC_PLL3DIVR (*(volatile unsigned long *) (RCC_BASE + 0x040UL))
#define RCC_PLL3FRACR (*(volatile unsigned long *) (RCC_BASE + 0x044UL))
#define RCC_D3CCIPR (*(volatile unsigned long *) (RCC_BASE + 0x058UL))
#define RCC_AHB3RSTR (*(volatile unsigned long *) (RCC_BASE + 0x07CUL))
#define RCC_APB3RSTR (*(volatile unsigned long *) (RCC_BASE + 0x088UL))
#define RCC_AHB2ENR (*(volatile unsigned long *) (RCC_BASE + 0x0DCUL))
#define RCC_AHB3ENR (*(volatile unsigned long *) (RCC_BASE + 0x0D4))
#define RCC_AHB4ENR (*(volatile unsigned long *) (RCC_BASE + 0x0E0UL))
#define RCC_APB2ENR (*(volatile unsigned long *) (RCC_BASE + 0x0F0UL))
#define RCC_APB3ENR (*(volatile unsigned long *) (RCC_BASE + 0x0E4UL))
#define RCC_APB4ENR (*(volatile unsigned long *) (RCC_BASE + 0x0F4UL))
#define RCC_D2CCIP2R (*(volatile unsigned long *) (RCC_BASE + 0x054UL))

#define RCC_CR_HSION BIT(0)
#define RCC_CR_HSIRDY BIT(2)
#define RCC_CR_HSEON BIT(16)
#define RCC_CR_HSERDY BIT(17)
#define RCC_CR_PLL1ON BIT(24)
#define RCC_CR_PLL1RDY BIT(25)
#define RCC_CR_PLL3ON BIT(28)
#define RCC_CR_PLL3RDY BIT(29)
#define RCC_CR_HSIDIV_MASK (3UL << 3)
#define RCC_CR_HSIDIV_DIV1 (0UL << 3)
#define RCC_CR_HSIDIV_DIV2 (1UL << 3)
#define RCC_CR_HSIDIV_DIV4 (2UL << 3)

#define RCC_CFGR_SW_MASK (7UL << 0)
#define RCC_CFGR_SW_HSI (0UL << 0)
#define RCC_CFGR_SW_CSI (1UL << 0)
#define RCC_CFGR_SW_HSE (2UL << 0)
#define RCC_CFGR_SW_PLL1 (3UL << 0)

#define RCC_CFGR_SWS_MASK (7UL << 3)
#define RCC_CFGR_SWS_HSI (0UL << 3)
#define RCC_CFGR_SWS_CSI (1UL << 3)
#define RCC_CFGR_SWS_HSE (2UL << 3)
#define RCC_CFGR_SWS_PLL1 (3UL << 3)


#define RCC_PLLCKSELR_PLLSRC_MASK (3UL << 0)
#define RCC_PLLCKSELR_PLLSRC_HSI (0UL << 0)
#define RCC_PLLCKSELR_PLLSRC_CSI (1UL << 0)
#define RCC_PLLCKSELR_PLLSRC_HSE (2UL << 0)
#define RCC_PLLCKSELR_PLLSRC_NONE (3UL << 0)

#define RCC_PLLCKSELR_DIVM1_MASK (0x3FUL << 4)
#define RCC_PLLCKSELR_DIVM1_Pos (4U)
#define RCC_PLLCKSELR_DIVM3_MASK (0x3FUL << 20)
#define RCC_PLLCKSELR_DIVM3_Pos (20U)

#define RCC_PLLCFGR_PLL1RGE_Pos (2U)
#define RCC_PLLCFGR_PLL1RGE_MASK (3UL << RCC_PLLCFGR_PLL1RGE_Pos)
#define RCC_PLL1VCIRANGE_0 (0UL << RCC_PLLCFGR_PLL1RGE_Pos)  // 1-2 MHz
#define RCC_PLL1VCIRANGE_1 (1UL << RCC_PLLCFGR_PLL1RGE_Pos)  // 2-4 MHz
#define RCC_PLL1VCIRANGE_2 (2UL << RCC_PLLCFGR_PLL1RGE_Pos)  // 4-8 MHz
#define RCC_PLL1VCIRANGE_3 (3UL << RCC_PLLCFGR_PLL1RGE_Pos)  // 8-16 MHz

#define RCC_PLLCFGR_PLL3RGE_Pos (10U)
#define RCC_PLLCFGR_PLL3RGE_MASK (3UL << RCC_PLLCFGR_PLL3RGE_Pos)
#define RCC_PLL3VCIRANGE_0 (0UL << RCC_PLLCFGR_PLL3RGE_Pos)  // 1-2 MHz
#define RCC_PLL3VCIRANGE_1 (1UL << RCC_PLLCFGR_PLL3RGE_Pos)  // 2-4 MHz
#define RCC_PLL3VCIRANGE_2 (2UL << RCC_PLLCFGR_PLL3RGE_Pos)  // 4-8 MHz
#define RCC_PLL3VCIRANGE_3 (3UL << RCC_PLLCFGR_PLL3RGE_Pos)  // 8-16 MHz


#define RCC_PLLCFGR_PLL1VCOSEL BIT(1)  // 0: 192-960MHz, 1: 150-420MHz
#define RCC_PLLCFGR_PLL1FRACEN BIT(0)
#define RCC_PLLCFGR_PLL3VCOSEL BIT(9)
#define RCC_PLLCFGR_PLL3FRACEN BIT(8)
#define RCC_PLLCFGR_DIVP1EN BIT(16)
#define RCC_PLLCFGR_DIVQ1EN BIT(17)
#define RCC_PLLCFGR_DIVR1EN BIT(18)
#define RCC_PLLCFGR_DIVR3EN BIT(24)

#define RCC_PLL1DIVR_N1_MASK (0x1FFUL << 0)
#define RCC_PLL1DIVR_P1_MASK (0x7FUL << 9)
#define RCC_PLL1DIVR_Q1_MASK (0x7FUL << 16)
#define RCC_PLL1DIVR_R1_MASK (0x7FUL << 24)
#define RCC_PLL1FRACR_FRACN_MASK (0x1FFFUL << 3)

#define RCC_D1CFGR_HPRE_Pos (0U)
#define RCC_D1CFGR_HPRE_MASK (0xFUL << RCC_D1CFGR_HPRE_Pos)
#define RCC_D1CFGR_HPRE_DIV1 (0x0UL << RCC_D1CFGR_HPRE_Pos)
#define RCC_D1CFGR_HPRE_DIV2 (0x8UL << RCC_D1CFGR_HPRE_Pos)
#define RCC_D1CFGR_D1PPRE_Pos (4U)
#define RCC_D1CFGR_D1PPRE_MASK (0x7UL << RCC_D1CFGR_D1PPRE_Pos)
#define RCC_D1CFGR_D1PPRE_DIV1 (0x0UL << RCC_D1CFGR_D1PPRE_Pos)
#define RCC_D1CFGR_D1PPRE_DIV2 (0x4UL << RCC_D1CFGR_D1PPRE_Pos)
#define RCC_D1CFGR_D1CPRE_Pos (8U)
#define RCC_D1CFGR_D1CPRE_MASK (0xFUL << RCC_D1CFGR_D1CPRE_Pos)
#define RCC_D1CFGR_D1CPRE_DIV1 (0x0UL << RCC_D1CFGR_D1CPRE_Pos)

#define RCC_D2CFGR_D2PPRE1_Pos (4U)
#define RCC_D2CFGR_D2PPRE1_MASK (0x7UL << RCC_D2CFGR_D2PPRE1_Pos)
#define RCC_D2CFGR_D2PPRE1_DIV1 (0x0UL << RCC_D2CFGR_D2PPRE1_Pos)
#define RCC_D2CFGR_D2PPRE1_DIV2 (0x4UL << RCC_D2CFGR_D2PPRE1_Pos)

#define RCC_D2CFGR_D2PPRE2_Pos (8U)
#define RCC_D2CFGR_D2PPRE2_MASK (0x7UL << RCC_D2CFGR_D2PPRE2_Pos)
#define RCC_D2CFGR_D2PPRE2_DIV1 (0x0UL << RCC_D2CFGR_D2PPRE2_Pos)
#define RCC_D2CFGR_D2PPRE2_DIV2 (0x4UL << RCC_D2CFGR_D2PPRE2_Pos)

#define RCC_D3CFGR_D3PPRE_Pos (4U)
#define RCC_D3CFGR_D3PPRE_MASK (0x7UL << RCC_D3CFGR_D3PPRE_Pos)
#define RCC_D3CFGR_D3PPRE_DIV1 (0x0UL << RCC_D3CFGR_D3PPRE_Pos)
#define RCC_D3CFGR_D3PPRE_DIV2 (0x4UL << RCC_D3CFGR_D3PPRE_Pos)


#define RCC_APB3RSTR_LTDCRST BIT(3)
#define RCC_D3CCIPR_LTDCSEL_MASK (0x7UL << 24)


#define RCC_AHB2ENR_SRAM1EN BIT(29)
#define RCC_AHB2ENR_SRAM2EN BIT(30)
#define RCC_AHB2ENR_SRAM3EN BIT(31)

#define RCC_AHB4ENR_GPIOAEN BIT(0)
#define RCC_AHB4ENR_GPIOBEN BIT(1)
#define RCC_AHB4ENR_GPIOCEN BIT(2)
#define RCC_AHB4ENR_GPIODEN BIT(3)
#define RCC_AHB4ENR_GPIOEEN BIT(4)
#define RCC_APB4ENR_PWREN BIT(1)
#define RCC_APB2ENR_USART1EN BIT(4)
#define RCC_APB3ENR_LTDCEN BIT(3)

#define RCC_AHB3ENR_QSPIEN BIT(14)


#define FLASH_ACR (*(volatile unsigned long *) (D1_AHB1PERIPH_BASE + 0x2000UL))
#define FLASH_ACR_LATENCY_Pos (0U)
#define FLASH_ACR_LATENCY_Msk (0xFUL << FLASH_ACR_LATENCY_Pos)
#define FLASH_ACR_WRHIGHFREQ_Pos (4U)
#define FLASH_ACR_WRHIGHFREQ_MASK (3UL << FLASH_ACR_WRHIGHFREQ_Pos)
#define FLASH_ACR_WRHIGHFREQ_0 (0UL << FLASH_ACR_WRHIGHFREQ_Pos)
#define FLASH_ACR_WRHIGHFREQ_1 (1UL << FLASH_ACR_WRHIGHFREQ_Pos)


#define PWR_BASE (0x58024800UL)
#define PWR_CR3 (*(volatile unsigned long *) (PWR_BASE + 0x0CUL))
#define PWR_CSR1 (*(volatile unsigned long *) (PWR_BASE + 0x04UL))
#define PWR_D3CR (*(volatile unsigned long *) (PWR_BASE + 0x18UL))

#define PWR_CR3_LDOEN BIT(1)
#define PWR_CR3_SMPSEN BIT(2)
#define PWR_CSR1_ACTVOSRDY BIT(13)

#define PWR_D3CR_VOS_Pos (14U)
#define PWR_D3CR_VOS_MASK (3UL << PWR_D3CR_VOS_Pos)
#define PWR_D3CR_VOS_SCALE1 (3UL << PWR_D3CR_VOS_Pos)
#define PWR_D3CR_VOS_SCALE0 (0UL << PWR_D3CR_VOS_Pos)

#define SYSCFG_BASE (0x58000400UL)
#define SYSCFG_PWRCR (*(volatile unsigned long *) (SYSCFG_BASE + 0x20UL))
#define SYSCFG_PWRCR_ODEN BIT(0)

#define GPIOA_BASE (AHB4PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE (AHB4PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE (AHB4PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE (AHB4PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE (AHB4PERIPH_BASE + 0x1000UL)

struct gpio {
    volatile unsigned long MODER;
    volatile unsigned long OTYPER;
    volatile unsigned long OSPEEDR;
    volatile unsigned long PUPDR;
    volatile unsigned long IDR;
    volatile unsigned long ODR;
    volatile unsigned long BSRR;
    volatile unsigned long LCKR;
    volatile unsigned long AFRL;
    volatile unsigned long AFRH;
    volatile unsigned long BRR;
    volatile unsigned long RES[1];
};

#define GPIOA ((struct gpio *) GPIOA_BASE)
#define GPIOB ((struct gpio *) GPIOB_BASE)
#define GPIOC ((struct gpio *) GPIOC_BASE)
#define GPIOD ((struct gpio *) GPIOD_BASE)
#define GPIOE ((struct gpio *) GPIOE_BASE)

#define GPIO_MODER_SET(GPIOx, pin, val)                         \
    do {                                                        \
        GPIOx->MODER &= ~(3UL << ((pin) * 2));                  \
        GPIOx->MODER |= ((unsigned long) (val) << ((pin) * 2)); \
    } while (0)

#define GPIO_OTYPER_SET(GPIOx, pin, val)                   \
    do {                                                   \
        GPIOx->OTYPER &= ~(1UL << (pin));                  \
        GPIOx->OTYPER |= ((unsigned long) (val) << (pin)); \
    } while (0)

#define GPIO_OSPEEDR_SET(GPIOx, pin, val)                         \
    do {                                                          \
        GPIOx->OSPEEDR &= ~(3UL << ((pin) * 2));                  \
        GPIOx->OSPEEDR |= ((unsigned long) (val) << ((pin) * 2)); \
    } while (0)

#define GPIO_PUPDR_SET(GPIOx, pin, val)                         \
    do {                                                        \
        GPIOx->PUPDR &= ~(3UL << ((pin) * 2));                  \
        GPIOx->PUPDR |= ((unsigned long) (val) << ((pin) * 2)); \
    } while (0)

#define GPIO_AFR_SET(GPIOx, pin, val)                                    \
    do {                                                                 \
        if ((pin) < 8) {                                                 \
            GPIOx->AFRL &= ~(0xFUL << (((pin) & 7) * 4));                \
            GPIOx->AFRL |= ((unsigned long) (val) << (((pin) & 7) * 4)); \
        } else {                                                         \
            GPIOx->AFRH &= ~(0xFUL << (((pin) & 7) * 4));                \
            GPIOx->AFRH |= ((unsigned long) (val) << (((pin) & 7) * 4)); \
        }                                                                \
    } while (0)

#define GPIO_MODER_INPUT 0
#define GPIO_MODER_OUTPUT 1
#define GPIO_MODER_ALT 2
#define GPIO_MODER_ANALOG 3

#define GPIO_OTYPER_PUSH_PULL 0
#define GPIO_OTYPER_OPEN_DRAIN 1

#define GPIO_OSPEEDR_LOW 0
#define GPIO_OSPEEDR_MEDIUM 1
#define GPIO_OSPEEDR_HIGH 2
#define GPIO_OSPEEDR_VERY_HIGH 3

#define GPIO_PUPDR_NONE 0
#define GPIO_PUPDR_PULL_UP 1
#define GPIO_PUPDR_PULL_DOWN 2

#define USART1_BASE 0x40011000UL

struct usart {
    volatile unsigned long CR1;
    volatile unsigned long CR2;
    volatile unsigned long CR3;
    volatile unsigned long BRR;
    volatile unsigned long GTPR;
    volatile unsigned long RTOR;
    volatile unsigned long RQR;
    volatile unsigned long ISR;
    volatile unsigned long ICR;
    volatile unsigned long RDR;
    volatile unsigned long TDR;
    volatile unsigned long PRESC;
};

#define USART1 ((struct usart *) USART1_BASE)

#define USART_CR1_UE BIT(0)
#define USART_CR1_RE BIT(2)
#define USART_CR1_TE BIT(3)
#define USART_CR1_RXNEIE BIT(5)
#define USART_ISR_TXE BIT(7)
#define USART_ISR_TC BIT(6)
#define USART_ISR_RXNE BIT(5)


#define HSI_FREQ 64000000UL

#define LTDC_BASE 0x50001000UL

struct ltdc_layer {
    volatile unsigned long CR;
    volatile unsigned long WHPCR;
    volatile unsigned long WVPCR;
    volatile unsigned long CKCR;
    volatile unsigned long PFCR;
    volatile unsigned long CACR;
    volatile unsigned long DCCR;
    volatile unsigned long BFCR;
    volatile unsigned long RESERVED_1[2];
    volatile unsigned long CFBAR;
    volatile unsigned long CFBLR;
    volatile unsigned long CFBLNR;
    volatile unsigned long RESERVED_2[3];
    volatile unsigned long CLUTWR;
};

struct ltdc {
    volatile unsigned long RESERVED_0[2];
    volatile unsigned long SSCR;
    volatile unsigned long BPCR;
    volatile unsigned long AWCR;
    volatile unsigned long TWCR;
    volatile unsigned long GCR;
    volatile unsigned long RESERVED_1[2];
    volatile unsigned long SRCR;
    volatile unsigned long RESERVED_2;
    volatile unsigned long BCCR;
    volatile unsigned long RESERVED_3;
    volatile unsigned long IER;
    volatile unsigned long ISR;
    volatile unsigned long ICR;
    volatile unsigned long LIPCR;
    volatile unsigned long CPSR;
    volatile unsigned long CDSR;
};

#define LTDC ((struct ltdc *) LTDC_BASE)
#define LTDC_layer1 ((struct ltdc_layer *) (LTDC_BASE + 0x84))
#define LTDC_layer2 ((struct ltdc_layer *) (LTDC_BASE + 0x104))

#define LTDC_GCR_LTDCEN BIT(0)
#define LTDC_GCR_HSPOL BIT(31)
#define LTDC_GCR_VSPOL BIT(30)
#define LTDC_GCR_DEPOL BIT(29)
#define LTDC_GCR_PCPOL BIT(28)

#define LTDC_SRCR_IMR BIT(0)
#define LTDC_SRCR_VBR BIT(1)

#define LTDC_LxCR_LEN BIT(0)
#define LTDC_LxCR_CLUTEN BIT(4)

#define LTDC_IER_LIE BIT(0)

#define LTDC_ICR_CLIF BIT(0)

#define LTDC_PF_L8 0x05UL
#define LTDC_PF_RGB565 0x02UL

#define LTDC_ISR_LIF BIT(0)

#define QUADSPI_BASE 0x52005000UL

struct qspi {
    volatile unsigned long CR;
    volatile unsigned long DCR;
    volatile unsigned long SR;
    volatile unsigned long FCR;
    volatile unsigned long DLR;
    volatile unsigned long CCR;
    volatile unsigned long AR;
    volatile unsigned long ABR;
    volatile unsigned long DR;
    volatile unsigned long PSMKR;
    volatile unsigned long PSMAR;
    volatile unsigned long PIR;
    volatile unsigned long LPTR;
};

#define QUADSPI ((struct qspi *) QUADSPI_BASE)

#define QUADSPI_SR_BUSY BIT(5)
#define QUADSPI_SR_TCF BIT(1)

#define QUADSPI_CR_EN BIT(0)
#define QUADSPI_CR_DIV2 BIT(24)
#define QUADSPI_CR_SSHIFT BIT(4)

#define SYSTICK_BASE        (0xE000E010UL)
#define SYSTICK_CSR         (*((volatile unsigned long *)(SYSTICK_BASE + 0x00)))
#define SYSTICK_RVR         (*((volatile unsigned long *)(SYSTICK_BASE + 0x04)))
#define SYSTICK_CVR         (*((volatile unsigned long *)(SYSTICK_BASE + 0x08)))

#define SYSCLK_FREQ 200000000UL
#define SYSTICK_ENABLE      BIT(0)
#define SYSTICK_TICKINT     BIT(1)
#define SYSTICK_CLKSOURCE   BIT(2)

#define NVIC_IPR ((volatile uint8_t *)(0xE000E400))
#define NVIC_ISER2 (*(volatile unsigned long *)(0xE000E100 + 0x08))

#define SCB_CCR (*(volatile unsigned long *)0xE000ED14)
#define SCB_ICIALLU (*(volatile unsigned long *)0xE000ED50)

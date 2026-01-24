#include <rcc.h>
#include <usart.h>

void usart_init(unsigned long baud_rate)
{
    RCC_AHB4ENR |= (1UL << 0);
    RCC_APB2ENR |= (1UL << 4);

    RCC_D2CCIP2R &= ~(7UL << 3);
    RCC_D2CCIP2R |= (3UL << 3);

    GPIO_MODER_SET(GPIOA, 9, 2);
    GPIO_MODER_SET(GPIOA, 10, 2);

    GPIO_OSPEEDR_SET(GPIOA, 9, 3);
    GPIO_OSPEEDR_SET(GPIOA, 10, 3);

    GPIO_AFR_SET(GPIOA, 9, 7);
    GPIO_AFR_SET(GPIOA, 10, 7);

    USART1->CR1 &= ~USART_CR1_UE;

    USART1->BRR = HSI_FREQ / baud_rate;

    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}


void usart_putc(char c)
{
    while (!(USART1->ISR & USART_ISR_TXE))
        ;
    USART1->TDR = c;
}

void print_uint(unsigned int n)
{
    if (!n) {
        usart_putc('0');
        return;
    }

    char buf[12];
    int i = 0;

    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }

    while (i)
        usart_putc(buf[--i]);
}

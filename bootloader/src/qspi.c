#include <qspi.h>

static void qspi_gpio_init(void)
{
    RCC_AHB4ENR |=
        RCC_AHB4ENR_GPIOBEN | RCC_AHB4ENR_GPIODEN | RCC_AHB4ENR_GPIOEEN;

    /**
     * PB2: CLK
     * PB10: CS
     * D11: DI(IO0)
     * D12: DO(IO1)
     * E2:  WP(IO2)
     * D13: HOLD/RESET(IO3)
     */

    GPIO_MODER_SET(GPIOB, 2, GPIO_MODER_ALT);
    GPIO_MODER_SET(GPIOB, 10, GPIO_MODER_ALT);
    GPIO_MODER_SET(GPIOD, 11, GPIO_MODER_ALT);
    GPIO_MODER_SET(GPIOD, 12, GPIO_MODER_ALT);
    GPIO_MODER_SET(GPIOE, 2, GPIO_MODER_ALT);
    GPIO_MODER_SET(GPIOD, 13, GPIO_MODER_ALT);

    GPIO_OSPEEDR_SET(GPIOB, 2, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_OSPEEDR_SET(GPIOB, 10, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_OSPEEDR_SET(GPIOD, 11, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_OSPEEDR_SET(GPIOD, 12, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_OSPEEDR_SET(GPIOE, 2, GPIO_OSPEEDR_VERY_HIGH);
    GPIO_OSPEEDR_SET(GPIOD, 13, GPIO_OSPEEDR_VERY_HIGH);

    GPIO_AFR_SET(GPIOB, 2, 9);
    GPIO_AFR_SET(GPIOB, 10, 9);
    GPIO_AFR_SET(GPIOD, 11, 9);
    GPIO_AFR_SET(GPIOD, 12, 9);
    GPIO_AFR_SET(GPIOE, 2, 9);
    GPIO_AFR_SET(GPIOD, 13, 9);
}

static void qspi_send_cmd(uint8_t instruction,
                          uint32_t data_mode,
                          uint32_t dummy)
{
    while (QUADSPI->SR & QUADSPI_SR_BUSY)
        ;
    QUADSPI->CCR = (data_mode << 24) | (dummy << 18) | (1 << 8) | instruction;

    if (data_mode) {
        while (QUADSPI->SR & QUADSPI_SR_BUSY)
            ;
    }
}

static void w25q_wait_busy(void)
{
    uint8_t status;
    do {
        while (QUADSPI->SR & QUADSPI_SR_BUSY)
            ;
        QUADSPI->DLR = 0;  // 1 byte
        qspi_send_cmd(W25Q128_CMD_READ_SR1, 1, 0);

        while (!(QUADSPI->SR & QUADSPI_SR_TCF))
            ;

        status = *(uint8_t *) &QUADSPI->DR;

        QUADSPI->FCR |= QUADSPI_SR_TCF;

    } while (status & W25Q128_SR_BUSY);
}

static void w25q_enable_quad(void)
{
    qspi_send_cmd(0x06, 0, 0);
    while (QUADSPI->SR & QUADSPI_SR_BUSY)
        ;


    QUADSPI->DLR = 0;  // 1 byte
    qspi_send_cmd(W25Q128_CMD_WRITE_SR2, 1, 0);
    *(uint8_t *) &QUADSPI->DR = 0x02;

    while (QUADSPI->SR & QUADSPI_SR_BUSY)
        ;
    w25q_wait_busy();
}

void qspi_init(void)
{
    RCC_AHB3ENR |= RCC_AHB3ENR_QSPIEN;
    (void)RCC_AHB3ENR;
    qspi_gpio_init();


    QUADSPI->CR = QUADSPI_CR_DIV2 | QUADSPI_CR_EN;
    QUADSPI->CR |= QUADSPI_CR_SSHIFT;
    /**
     * 16 MB and 16 cycles chip select high time
     */
    QUADSPI->DCR = (23 << 16) | (4 << 8);

    w25q_enable_quad();

    while (QUADSPI->SR & QUADSPI_SR_BUSY)
        ;


    QUADSPI->CCR = (3 << 26) |  // Memory-mapped mode
                   (2 << 12) |  // 24 bit address
                   (1 << 8) |   // 1 line instruction
                   (3 << 10) |  // 4 lines address
                   (3 << 14) |  // 4 lines alternate byte mode
                   (4 << 18) |  // 4 dummy cycles
                   (3 << 24) |  // 4 lines I/O
                   W25Q128_CMD_FAST_READ_QUAD;
    for (volatile int i = 0; i < 1000000; i++);
}

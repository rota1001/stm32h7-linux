#include <regs.h>
#include <stdint.h>

#define W25Q128_CMD_WRITE_ENABLE 0x06
#define W25Q128_CMD_READ_SR1 0x05
#define W25Q128_CMD_WRITE_SR2 0x31
#define W25Q128_CMD_FAST_READ_QUAD 0xEB

#define W25Q128_SR_BUSY BIT(0)

void qspi_init(void);

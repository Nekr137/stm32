
#include <stm32f0xx.h>


#define SPI2_DR8 (*(__IO uint8_t *)(uint32_t)(SPI2_BASE+0x0C))

uint8_t SPI_SentByte_8(uint8_t byte);		//for gyr
void SPI_SentByte_16(uint16_t data);		//for matrix
void SPIconfig(void);



#ifndef SPI_DEVICE_H__
#define SPI_DEVICE_H__

#include <stdint.h>



typedef enum{
  SPI_CLK_DIV_2 = 0,
  SPI_CLK_DIV_4,
  SPI_CLK_DIV_8,
  SPI_CLK_DIV_32,
  SPI_CLK_DIV_64,
  SPI_CLK_DIV_16,
  SPI_CLK_DIV_128,
  SPI_CLK_DIV_256,
  SPI_CLK_DIV_512,
  SPI_CLK_DIV_1024,
  SPI_CLK_DIV_2048,
  SPI_CLK_DIV_4096,
}SpiClkDevider_t;


int8_t SPI_Init(SpiClkDevider_t devider);
int8_t SPI_SendBuff(uint8_t * const buff, uint16_t length);
int8_t SPI_ReadBuff(uint8_t * const buff, uint16_t length);




























#endif
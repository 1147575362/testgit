/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file spi_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <appConfig.h>
#include <gpio_device.h>
#include <spi_device.h>
#include <atomic.h>
#include <isrfuncs.h>

int8_t SPI_Init(SpiClkDevider_t devider)
{
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_3].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_4].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_5].DIR   = (uint8_t)GPIO_DIR_INPUT;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_3].HWMODE = 5U;/* GPIO_PORT_3: SPI Master Clock Out.*/
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_4].HWMODE = 4U;/* GPIO_PORT_4: SPI Master MOSI*/
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_5].HWMODE = 4U;/* GPIO_PORT_5: SPI Master MISO.*/
  
    SPI_REG_CTRL.FIFOSOFTRESET = 1U;
    SPI_REG_CTRL.LPBK = 0U;
    SPI_REG_CTRL.SCKEXT = ((uint8_t)devider >> 2);
    SPI_REG_CTRL.SPR    = (uint8_t)devider &0x03U;
    SPI_REG_CTRL.ENA_REQ = 1U;
    return 0;
}

/*#pragma optimize=none*/
int8_t SPI_SendBuff(uint8_t * const buff, uint16_t length)
{
    uint8_t data = 0U;
    SPI_REG_CTRL.FIFOSOFTRESET = 1U;
    for (uint16_t i = 0U; i < length; i++){
        SPI_REG_TXDATA = buff[i];
        while(SPI_REG_STATUS.XFERCNT == 0U){} /* waiting for tx completed */
        data = SPI_REG_RXDATA;
        
    }
    return (int8_t)data;
}

int8_t SPI_ReadBuff(uint8_t * const buff, uint16_t length)
{
    SPI_REG_CTRL.FIFOSOFTRESET = 1U;
    for (uint16_t i = 0; i < length; i++){
        SPI_REG_TXDATA = 0xFFU;
        while(SPI_REG_STATUS.XFERCNT == 0U){}/* waiting for tx completed */
        buff[i] = SPI_REG_RXDATA;
    }
    return 0;
}


void SPI_Handler(void)
{
  
  
}
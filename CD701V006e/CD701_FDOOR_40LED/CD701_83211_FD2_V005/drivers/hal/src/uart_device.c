/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file uart_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <appConfig.h>
#include <isrfuncs.h>
#include <gpio_device.h>
#include <uart_device.h>
#include <atomic.h>



static uint8_t  rxBuff[RX_DATA_SIZE];
volatile static uint8_t  rxBuffCount = 0;
volatile static uint8_t  uartAddress = 0;

static uartRx_cb_func_t uartRxDataCallback = NULL;

int8_t UART_Init(Baudrate_DIV_16MHz_t baudRate, BitSize_t bitSize, Parity_t parity,StopBits_t stopBits)
{
    int8_t result = 0;
    
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_4].DIR   = (uint8_t)GPIO_DIR_INPUT;
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_5].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_4].HWMODE = 2U;/* GPIO_PORT_4: RXD*/
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_5].HWMODE = 2U;/* GPIO_PORT_5: TXD*/
    
    if (baudRate == BAUDRATE_DIV_16MHz_115200){
        UART_SFRS->BAUD.OSR       = BAUDRATE_OSR_16MHZ;
        UART_SFRS->BAUD.BAUDDIV   = (uint8_t)baudRate;
    }else{
        UART_SFRS->BAUD.OSR       = BAUDRATE_OSR_16MHZ_HIGH_SPEED;
        UART_SFRS->BAUD.BAUDDIV   = (uint8_t)baudRate;
    }
    
    UART_SFRS->MSGCTRL.SIZE   = (uint8_t)bitSize;
    UART_SFRS->MSGCTRL.STOP   = (uint8_t)stopBits;
    UART_SFRS->MSGCTRL.PARITY = (uint8_t)parity;
    UART_SFRS->MSGCTRL.ENABLE = 1U;
    
    return result;
}


void UART_RegisterIRQ(uint8_t address,uartRx_cb_func_t rxDowncallback)
{
    uartRxDataCallback = rxDowncallback;
    UART_SFRS->MSGCTRL.ENA_ADDR_MATCH   = 0U;
    UART_SFRS->MSGCTRL.ADDR_MATCH       = address;
    UART_SFRS->MSGCTRL.MAX_BYTES_RXD    = 3U;
    uartAddress = address;
    UART_SFRS->FIFOLEVELCTL.RXMULTIPLEXFERDONECNT = 9;
    
    UART_SFRS->MSGCTRL.UFIFOSOFTRESET = 1U;// FIFO reset
    
    UART_SFRS->INT.CLEAR.RXMULTDONE = 1U;
    UART_SFRS->INT2.CLEAR.RXTOUT    = 1U;
    
    UART_SFRS->INT.ENABLE.RXMULTDONE = 1U;
    UART_SFRS->INT2.ENABLE.RXTOUT    = 1U;
    
    NVIC_EnableIRQ(UART_IRQn);
}

void UART_UnRegisterIRQ(void)
{
    uartRxDataCallback = NULL;
    UART_SFRS->INT.ENABLE.RXMULTDONE = 0U;
    UART_SFRS->INT.ENABLE.TXDONE     = 0U;
    UART_SFRS->INT.ENABLE.BREAKKERR  = 0U;
    NVIC_DisableIRQ(UART_IRQn);
}


int8_t UART_SendBuff(uint8_t *buff, uint16_t length)
{
    int8_t result = 0;
    
    for (uint8_t i = 0; i < length; i++){
        UART_SFRS->DATA.BYTE = buff[i];
        while(UART_SFRS->INT.STATUS.TXDONE == 0U){}
        UART_SFRS->INT.CLEAR.TXDONE = 1U;
    }
    
    return result;
}


void UART_Handler(void)
{
    uint16_t mateData;
    uint8_t fifoCount = UART_SFRS->FIFOSTATUS.RXCOUNT;
    for (uint8_t i = 0; i < fifoCount; i++){
        mateData = UART_REG_BREAK_DATA;
        if ((mateData & 0x0800U) != 0U){// it's a break signal
            rxBuffCount = 0U;
        }else{// it's data 
            if (rxBuffCount < VALID_RX_PACKAGE_SIZE){
                rxBuff[rxBuffCount++] = (uint8_t)mateData;
                if (rxBuff[0] == uartAddress){
                    if (rxBuffCount == VALID_RX_PACKAGE_SIZE){
                        uartRxDataCallback(rxBuff,rxBuffCount);
                    }
                }
            }
        }
    }
    UART_SFRS->INT.CLEAR.BYTE = 0xFFU;
    UART_SFRS->INT2.CLEAR.RXTOUT = 0xFFU;
}





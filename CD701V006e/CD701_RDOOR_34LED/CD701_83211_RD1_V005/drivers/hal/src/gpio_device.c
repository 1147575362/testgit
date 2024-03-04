/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file gpio_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <gpio_device.h>
#include <isrfuncs.h>

#ifdef GPIO_SFRS
static gpio_cb_func_t gpioCallback[9]= {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};


void GPIO_Handler( void )
{
    for (uint8_t i = (uint8_t)GPIO_PORT_1; i<= (uint8_t)GPIO_PORT_9; i++ ){
        if (gpioCallback[i] !=NULL){
            if (GPIO_SFRS->GPIO_CFG[i].ACTDET == 1U){
                GPIO_SFRS->GPIO_CFG[i].CLR = 1U;
                gpioCallback[i]();
            }
        }
    }
}


void GPIO_Init(GpioPort_t port,GpioDir_t dir,GpioPullMode_t pullMode)
{
    IOCTRLA_SFRS->GPIO[(uint8_t)port].HWMODE   = (uint8_t)GPIO_MUX_GPIO;
    IOCTRLA_SFRS->GPIO[(uint8_t)port].PULLMODE = (uint8_t)pullMode;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].DIR     = (uint8_t)dir;
    if (dir == GPIO_DIR_INPUT){
        IOCTRLA_SFRS->GPIO[(uint8_t)port].RDENA = 1U;
    }
}


void GPIO_RegisterIRQ(GpioPort_t port, GpioISREdge_t edge, gpio_cb_func_t callback)
{
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].CLR = 1U;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].IE = 1U;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].EDGE = (uint8_t)edge;
    gpioCallback[(uint8_t)port] = callback;
    NVIC_EnableIRQ(GPIO_IRQn);
}

void GPIO_UnRegisterIRQ(GpioPort_t port)
{
    gpioCallback[(uint8_t)port] = NULL;
}

void GPIO_Set(GpioPort_t port, GpioSetState_t state)
{
    if(port <= GPIO_PORT_8){
        GPIO_SFRS->GPADATA[1U << (uint8_t)port] =  (uint8_t)state << (uint8_t)port;
    }else{
        GPIO_SFRS->GPBDATA[1U << ((uint8_t)port - (uint8_t)GPIO_PORT_9)]=  (uint8_t)state << ((uint8_t)port - (uint8_t)GPIO_PORT_9);
    }
}

void GPIO_SetGPIOs(uint8_t portsMask, uint8_t value)
{
    GPIO_SFRS->GPADATA[portsMask] = value;/* value */
}

uint8_t GPIO_Read(GpioPort_t port)
{
    uint8_t data;
    if(port <= GPIO_PORT_8){
        data = GPIO_SFRS->GPADATA[1U << (uint8_t)port];
    }else{
        data = GPIO_SFRS->GPBDATA[1U << ((uint8_t)port - (uint8_t)GPIO_PORT_9)];
    }
    return (uint8_t)(data != 0U);
}

#endif
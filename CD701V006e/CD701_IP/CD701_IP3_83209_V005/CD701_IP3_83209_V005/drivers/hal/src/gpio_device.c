/**
 * @copyright 2017 IndieMicro.
 *
 * This file is proprietary to IndieMicro.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of IndieMicro.
 *
 * @file gpio_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/09/10
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <gpio_device.h>
#include <isrfuncs.h>

#ifdef GPIO_SFRS
static gpio_cb_func_t gpioCallback[8]= {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};


void GPIO_Handler( void )
{
    for (uint8_t i = (uint8_t)GPIO_PORT_1; i<= (uint8_t)GPIO_PORT_6; i++ ){
        if (GPIO_SFRS->GPIO_CFG[i].ACTDET == 1U){
            GPIO_SFRS->GPIO_CFG[i].CLR = 1U;
            if (gpioCallback[i] !=NULL){
                gpioCallback[i]();
            }
        }
    }
}


void GPIO_Init(GpioInitPort_t port,GpioDir_t dir,GpioPullMode_t pullMode)
{
    IOCTRLA_SFRS->GPIO[(uint8_t)port].HWMODE   = (uint8_t)GPIO_MUX_GPIO;
    IOCTRLA_SFRS->GPIO[(uint8_t)port].PULLMODE = (uint8_t)pullMode;
    if (port <= GPIO_INIT_PORT_2){
        GPIO_SFRS->GPIO_CFG[(uint8_t)port].DIR     = (uint8_t)dir;
    }else{
        GPIO_SFRS->GPIO_CFG[(uint8_t)port + 2U].DIR= (uint8_t)dir;
    }
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
     GPIO_SFRS->GPADATA[1U << (uint8_t)port] =  (uint8_t)state << (uint8_t)port;
}

uint8_t GPIO_Read(GpioPort_t port)
{
    uint8_t data = GPIO_SFRS->GPADATA[1U << (uint8_t)port];
    return (uint8_t)(data != 0U);
}

#endif
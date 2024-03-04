/**
 * @copyright 2017 IndieMicro.
 *
 * This file is proprietary to IndieMicro.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of IndieMicro.
 *
 * @file gpio_device.h
 */

#ifndef GPIO_DEVICE_H__
#define GPIO_DEVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <realplumPro.h>
#include "errno.h"

/* for IO control*/
typedef enum{
  GPIO_PORT_1 = 0U,
  GPIO_PORT_2,
  GPIO_PORT_LINS,
  GPIO_PORT_LINM,
  GPIO_PORT_3,
  GPIO_PORT_4,
  GPIO_PORT_5,
  GPIO_PORT_6,
}GpioPort_t;

/* for IO init*/
typedef enum{
  GPIO_INIT_PORT_1 = 0U,
  GPIO_INIT_PORT_2,
  GPIO_INIT_PORT_3,
  GPIO_INIT_PORT_4,
  GPIO_INIT_PORT_5,
  GPIO_INIT_PORT_6,
}GpioInitPort_t;


typedef enum{
  GPIO_DIR_INPUT  = 0U,
  GPIO_DIR_OUTPUT,
}GpioDir_t;

/* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
typedef enum{
  GPIO_PULL_UP          = 0U,
  GPIO_PULL_NONE        = 1U,
  GPIO_PULL_UP_DOWN     = 2U,
  GPIO_PULL_DOWN        = 3U,
}GpioPullMode_t;

typedef enum{
  GPIO_LOW = 0U,
  GPIO_HIGH,
}GpioSetState_t;

typedef enum{
  GPIO_MUX_GPIO = 0U,
  GPIO_MUX_PWM,
  GPIO_MUX_UART,
}GpioMuxMode_t;


typedef enum{
  GPIO_PWM_CH1 = 0U,
  GPIO_PWM_CH2,
  GPIO_PWM_CH3,
  GPIO_PWM_NONE,
}GpioPWMchannel_t;

/* 00: disable, 01: rising, 10: falling, 11: both */
typedef enum{
  GPIO_EDGE_NODE = 0U,
  GPIO_EDGE_RISING,
  GPIO_EDGE_FALLING,
  GPIO_EDGE_BOTH,
}GpioISREdge_t;


typedef void (*gpio_cb_func_t)(void);


void GPIO_Init(GpioInitPort_t port,GpioDir_t dir,GpioPullMode_t pullMode);
void GPIO_RegisterIRQ(GpioPort_t port, GpioISREdge_t edge, gpio_cb_func_t callback);
void GPIO_UnRegisterIRQ(GpioPort_t port);
void GPIO_Set(GpioPort_t port, GpioSetState_t state);
uint8_t GPIO_Read(GpioPort_t port);

#endif /* __GPIO_DEVICE_H__ */
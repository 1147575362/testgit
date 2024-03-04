/**
 * @copyright 2016 IndieMicro.
 *
 * This file is proprietary to IndieMicro.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of IndieMicro.
 *
 * @file systick_device.h
 */

#ifndef SYSTICK_DEVICE_H
#define SYSTICK_DEVICE_H

#include <stdint.h>

typedef void (*sysTick_cb_func_t)(void);


void SysTick_Init(uint32_t ticks, sysTick_cb_func_t callback);
uint32_t SysTick_GetCount(void);
uint32_t SysTick_GetLoad(void);

























#endif /* ___SYSTICK_DEVICE_H__ */


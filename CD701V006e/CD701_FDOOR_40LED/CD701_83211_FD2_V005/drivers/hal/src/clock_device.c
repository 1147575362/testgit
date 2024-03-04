/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file clock_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <clock_device.h>

#define SYSTEM_CLOCK_HIGH_FREQ  (16000U)/* KHz */
#define SYSTEM_CLOCK_LOW_FREQ   (16U)   /* KHz */
static uint32_t systemClock = SYSTEM_CLOCK_HIGH_FREQ;

void Clock_SystemMainClockInit(uint8_t divider)
{
    CRGA_SFRS->SYSCLKCTRL.HFRCENA = 1U;
    while(CRGA_SFRS->SYSCLKCTRL.HFRCSTS == 0U){}
    /* Enable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x0EU;
    SYSCTRLA_SFRS->HF_OSC_TRIM.TRIM_HF_RC = HWCFG_GetRCHFClockCalibValue();
    SYSCTRLA_SFRS->LF_OSC_TRIM = HWCFG_GetRCLFClockCalibValue();
    CRGA_SFRS->LFCLKCTRL.CLKLFSEL = 1;
    SYSCTRLA_SFRS->PMU_TRIM.RESISTOR_TRIM =  HWCFG_GetV2ITrimValue();
    /* Disable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0U;
    CRGA_SFRS->SYSCLKCTRL.SYSCLKSEL = (uint8_t)CLOCK_RC_16MHz;
    CRGA_SFRS->SYSCLKCTRL.DIVSYSCLK = (uint8_t)divider;
    /*  frequency spread    */
    SYSCTRLA_SFRS->HF_OSC_TRIM.SSCDEEP  = 3U;
    SYSCTRLA_SFRS->HF_OSC_TRIM.SSCDIV   = 15U;
    SYSCTRLA_SFRS->HF_OSC_TRIM.SSCENA   = 1U;
    
    systemClock = SYSTEM_CLOCK_HIGH_FREQ;
    systemClock = systemClock >> (uint8_t)divider;
    
    SYSCTRLA_SFRS->PMU_TRIM.TRIM = HWCFG_GetMajorVBGCode();
}

uint32_t Clock_GetSystemClock(void)
{
    return systemClock;/* KHz */
}

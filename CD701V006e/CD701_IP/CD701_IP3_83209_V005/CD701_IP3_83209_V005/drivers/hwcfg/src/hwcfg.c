/**
 * @copyright 2017 IndieMicro.
 *
 * This file is proprietary to IndieMicro.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of IndieMicro.
 *
 * @file hwcfg.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/09/10
 */

#include <stddef.h>
#include <stdint.h>
#include <realplumPro.h>
#include "hwcfg.h"


uint8_t HWCFG_GetRCHFClockCalibValue(void)
{
    uint8_t trim = HWCFG_SFRS->CLK_RC_HF_16MHz_TRIMCODE;
    if (trim == 0xFFU){
        trim = 0x43U;
    }
    return trim;
}

  
uint8_t HWCFG_GetRCLFClockCalibValue(void)
{
    uint8_t trim =  HWCFG_SFRS->CLK_RC_LF_256KHz_TRIMCODE;
    if (trim == 0xFFU){
        trim = 0xAFU;
    }
    return trim;
}


uint16_t HWCFG_GetVBG(void)
{
    uint16_t volt = HWCFG_SFRS->BANDGAP_VOLT_mV;
    
    if (volt == 0xFFU){
        volt = E_HWCFG_ADC_VBG;
    }
    return volt;
}

/**
 * @brief Get the version of calibration data.
 *
 * @return The version for this calibration data block.
 */
uint32_t HWCFG_GetCalibrationVersion(void)
{
    uint32_t version = HWCFG_SFRS->CHIP.VERSION;
    return version;
  
}

uint8_t HWCFG_GetV2ITrimValue(void)
{
    uint8_t trimValue = (uint8_t)HWCFG_SFRS->V2I_TRIMCODE;
    if (trimValue == 0xFFU){
        trimValue =  0x20U;
    }
    return trimValue;
}


uint16_t HWCFG_GetLEDTrimValue(uint8_t ledIndex)
{
    uint16_t trimValue = HWCFG_SFRS->LED[ledIndex].LED_30mA_TRIMCODE;
    if (trimValue > 0x1FFU){
        trimValue =  250U; /* 120uA*250 = 30mA */
    }
    return trimValue;
}


uint8_t HWCFG_GetOffLEDTrimValue(void)
{
    uint8_t trimValue = (uint8_t)HWCFG_SFRS->LED[0].LED_2mA_TRIMCODE;
    
    if (trimValue >= 255U){
        trimValue = 200U; 
    }
    return trimValue;
}


void HWCFG_TrimAccessUnlock(void)
{
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x0EU;
}


void HWCFG_TrimAccessLockUntilReset(void)
{
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.LOCK = 1U;
}


    

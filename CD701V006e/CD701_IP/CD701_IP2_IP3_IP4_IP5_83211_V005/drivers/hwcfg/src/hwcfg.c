/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file hwcfg.c
 */

#include <stddef.h>
#include <stdint.h>
#include <rugbyPro.h>
#include "hwcfg.h"
#include <appConfig.h>

uint8_t HWCFG_GetRCHFClockCalibValue(void)
{
    uint8_t trim = HWCFG_SFRS->CLK_RC_HF;
    if (trim == 0xFFU){
        trim = 0x43U;
    }
    return trim;
}

  
uint8_t HWCFG_GetRCLFClockCalibValue(void)
{
    uint8_t trim =  HWCFG_SFRS->CLK_RC_LF;
    if (trim == 0xFFU){
        trim = 0x96U;
    }
    return trim;
}


uint16_t HWCFG_GetVBG(void)
{
    uint16_t volt = HWCFG_SFRS->BANDGAP_VOLT;
    
    if (volt == 0xFFU){
        volt = E_HWCFG_ADC_VBG;
    }
    return volt;
}

uint8_t HWCFG_GetMajorVBGCode(void)
{
    uint8_t code = HWCFG_SFRS->MAJOR_BANDGAP_CODE;
    
    if (code == 0xFFU){
        code = 4U;
    }
    return code;
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
    uint8_t trimValue = HWCFG_SFRS->V2I_30mA_25C;
    if (trimValue == 0xFFU){
        trimValue =  0x20U;
    }
    return trimValue;
}


uint16_t HWCFG_GetLEDTrimValue(uint8_t ledChannel)
{
    uint16_t trimValue =  300U;
    if (ledChannel < 24U){
        trimValue = HWCFG_SFRS->LED[ledChannel].TRIM_CODE_30mA;
        if (trimValue > 0x1FFU){
            trimValue =  300U; /* 100uA*300 = 30mA */
        }
    }
    return trimValue;
}


uint8_t HWCFG_GetOffLEDTrimValue(void)
{
    uint8_t trimValue = HWCFG_SFRS->TYPICAL_2mA_TRIM_CODE;
    
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


    

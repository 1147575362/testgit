/**
 * @copyright 2017 IndieMicro.
 *
 * This file is proprietary to IndieMicro.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of IndieMicro.
 *
 * @file hwcfg.h
 */

#ifndef HWCFG_H__
#define HWCFG_H__

#include <realplumPro.h>

typedef struct{
  uint16_t LED_2mA_TRIMCODE;
  uint16_t LED_2mA_Current_Value;/*  uA */
  uint16_t LED_30mA_TRIMCODE;
  uint16_t LED_30mA_Current_Value;/*  uA */
  uint16_t LED_PN1P5V_VOLT;
  uint16_t LED_PN1P5V_CODE;
  uint16_t LED_PN4P0V_VOLT;
  uint16_t LED_PN4P0V_CODE;
}LedChannelParam_t;


/**
 * @brief A structure to represent the data in hardware config block.
 */
typedef union {
    struct {
        struct {
            uint32_t crc32;
            struct {
                uint8_t  NAME[4];
                uint8_t  VERSION;
                uint8_t  LOT_NO[3];
                uint32_t ATPR;
                uint32_t CHIPID;
                uint32_t TP_SVN_R;
            }CHIP;/*20*/
            uint8_t  CUSTOMER_INFO[24];
            uint8_t  BANDGAP_CODE_UNUSED;
            uint8_t  RESVED0;
            uint16_t BANDGAP_VOLT_mV;           /* in mV */
            uint16_t ADC_0V5_CODE;              /* ADC count @0.5V */
            uint16_t ADC_1V0_CODE;              /* ADC count @1.0V */
            uint16_t TSENSOR_mV_25C;            
            uint16_t TSENSOR_CODE_25C__UNUSED;
            uint16_t VBAT_CODE_13V5;            /* ADC count @VBAT=13.5V*/
            uint16_t VBAT_CODE_8V0;             /* ADC count @VBAT=8.0V */
            uint8_t CLK_RC_LF_32KHz_TRIMCODE;
            uint8_t  RESVED1;
            uint8_t CLK_RC_LF_256KHz_TRIMCODE;
            uint8_t  RESVED2;
            uint8_t CLK_RC_HF_16MHz_TRIMCODE;
            uint8_t RESVED3;
            uint16_t V2I_TRIMCODE;
            uint16_t CLK_RC_HF_16MHz_FREQ;      /* KHz   */
            uint8_t RESVED4[6];
        };
        LedChannelParam_t  LED[3];
    };
    uint8_t payload[512];

} HWCFG_SFRS_t;

/**
 * @brief The starting address of hardware config block.
 */
#define HWCFG_SFRS ((__IO HWCFG_SFRS_t *) (0x00010200))

#define E_HWCFG_ADC_RANGE_FULL       255
#define E_HWCFG_ADC_RANGE_ZERO       0
#define E_HWCFG_ADC_VBG              1220U       /* 1.22V */

/**
 * @brief Get High Frequency RC clock trim value.
 *
 * @return The trim value stored in flash memory.
 */
uint8_t HWCFG_GetRCHFClockCalibValue(void);

/**
 * @brief Get Low Frequency RC clock trim value.
 *
 * @return The trim value stored in flash memory.
 */
uint8_t HWCFG_GetRCLFClockCalibValue(void);

/**
 * @brief Get band gap voltage.
 *
 * @return The trim value stored in flash memory.
 */
uint16_t HWCFG_GetVBG(void);

/**
 * @brief Get the version of calibration data.
 *
 * @return The version for this calibration data block.
 */
uint32_t HWCFG_GetCalibrationVersion(void);

uint8_t HWCFG_GetV2ITrimValue(void);
uint16_t HWCFG_GetLEDTrimValue(uint8_t ledIndex);
uint8_t HWCFG_GetOffLEDTrimValue(void);


void HWCFG_TrimAccessUnlock(void);
void HWCFG_TrimAccessLockUntilReset(void);


#endif /* __HWCFG_H__ */


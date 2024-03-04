/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file hwcfg.h
 */

#ifndef HWCFG_H__
#define HWCFG_H__

#include <rugbyPro.h>

#define E_HWCFG_SYSTEM_CLOCK_MAIN_RC_HF        (16000000U)
#define E_HWCFG_SYSTEM_CLOCK_MAIN_RC_LF        (10000U)

typedef enum {
  CURR_30MA = 0U,
}CurrValue_t;

typedef enum {
  OFF_CURR_2MA = 0U,
}OffCurrValue_t;

typedef struct{
  uint16_t PN_CODE_VOLT_1500mV;
  uint16_t PN_CODE_VOLT_4000mV;
  uint16_t REAL_CURR_300000uA;
  uint16_t REAL_CURR_20000uA;
  uint16_t TRIM_CODE_30mA;
  uint16_t RESVD[3];
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
            uint8_t  MAJOR_BANDGAP_CODE;
            uint8_t  RESVED0;
            uint16_t BANDGAP_VOLT;              /* mV */
            uint16_t rev;
            uint16_t ADC_0V5;                   /* 0.5V */
            uint16_t ADC_1V0;                   /* 1.0V */
            uint16_t TSENSOR_mV_25C;            
            uint16_t TSENSOR_CODE_25C;
            uint16_t VBAT_CODE_13V5;            /* 13.5V adc count */
            uint8_t CLK_RC_LF;
            uint8_t RESVED2;
            uint8_t CLK_RC_HF;
            uint8_t RESVED3;
            uint8_t BUCK_5V_DAC_CODE;
            uint8_t RESVED4;
            uint16_t BUCK_5V_ADC_CODE;
            uint16_t VBAT_CODE_8V0;
            uint8_t  PEAK_ILIM_CODE;
            uint8_t  RESVED5;
            uint16_t CLK_RC_HF_16MHz_FREQ;      /* KHz   */
            uint8_t  RESVED6[18];
        };
        
        /*  32bytes */
        uint16_t RESSVD6;
        uint16_t RESSVD7;
        uint16_t TYPICAL_30mA_TRIM_CODE;
        uint8_t TYPICAL_2mA_TRIM_CODE;
        uint8_t _RESVD_50;
        uint8_t V2I_30mA_25C;
        uint8_t _RESVD_51;
        uint16_t TYPICAL_RESVD[11];
        /*  384 bytes */
        LedChannelParam_t  LED[24];
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
 * @brief Get major bandgap code trim value only for internal circuit .
 *
 * @return The trim value stored in flash memory.
 */
uint8_t HWCFG_GetMajorVBGCode(void);
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
uint16_t HWCFG_GetLEDTrimValue(uint8_t ledChannel);
uint8_t HWCFG_GetOffLEDTrimValue(void);


void HWCFG_TrimAccessUnlock(void);
void HWCFG_TrimAccessLockUntilReset(void);


#endif /* __HWCFG_H__ */


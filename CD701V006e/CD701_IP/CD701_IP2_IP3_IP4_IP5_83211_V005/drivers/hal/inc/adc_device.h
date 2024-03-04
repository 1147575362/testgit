/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file adc_device.h
 */

#ifndef ADC_DEVICE_H__
#define ADC_DEVICE_H__

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <rugbyPro.h>
#include <pdsTask.h>

typedef enum {
  ADC_MEASURE_ITEM_NONE = 0U,
  ADC_MEASURE_ITEM_VBG,
  ADC_MEASURE_ITEM_VBAT,                /* 1/32x  */   
  ADC_MEASURE_ITEM_BUCK,                /* 1/8x   */
  ADC_MEASURE_ITEM_VDD1V5,
  ADC_MEASURE_ITEM_VTEMP,
  ADC_MEASURE_ITEM_GPIO,
  ADC_MEASURE_ITEM_BUCK_VBAT,           
  ADC_MEASURE_ITEM_LED_BUCK_BAT,          /* LED: 1/5.25x  */
  ADC_MEASURE_ITEM_LED,                   /* LED: 1/5.25x  */
}AdcMeasureItem_t;

typedef struct{
  AdcMeasureItem_t item;
  uint8_t channel;
}ADCMeasureParam_t;


typedef enum {
  ADC_MODE_DIFF = 0U,
  ADC_MODE_SINGLE_END,
}AdcMode_t;

#define CONF_MODE_POS                   (0U)
#define CONF_AUTOEN_POS                 (1U)
#define CONF_ATTEN_POS                  (2U)
#define CONF_SAMPLECYC_POS              (3U)
#define CONF_SWSYNCIN_POS               (6U)
#define CONF_TRMLEDBUFFER4ADC_POS       (16U)


typedef enum {
  ADC_STARTUP_DELAY_1US = 0U,
  ADC_STARTUP_DELAY_8US,
  ADC_STARTUP_DELAY_12US,
  ADC_STARTUP_DELAY_16US,
}AdcStartupDelay_t;

typedef enum {
  SYNC_MODE_NONE        = 0x00U,
  SYNC_MODE_RISING      = 0x01U,
  SYNC_MODE_FALLING     = 0x02U,
  SYNC_MODE_PWM_PERIOD  = 0x04U,
  SYNC_MODE_SOFT_INPUT  = 0x08U,
}AdcSyncMode_t;


typedef enum {
  ADC_CH1_CH3_SEL_VBAT = 0U,
  ADC_CH1_VH3_SEL_VBUCK,
  ADC_CH1_CH3_SEL_TEMP_SENSOR,
}AdcCh1_3Select_t;

typedef enum {
  ADC_CH2_SEL_LED0 = 0U,
  ADC_CH2_SEL_LED1,
  ADC_CH2_SEL_LED2,
  ADC_CH2_SEL_LED3,
  ADC_CH2_SEL_LED4,
  ADC_CH2_SEL_LED5,
  ADC_CH2_SEL_LED6,
  ADC_CH2_SEL_LED7,
  ADC_CH2_SEL_LED8,
  ADC_CH2_SEL_LED9,
  ADC_CH2_SEL_LED10,
  ADC_CH2_SEL_LED11,
  ADC_CH2_SEL_LED12,
  ADC_CH2_SEL_LED13,
  ADC_CH2_SEL_LED14,
  ADC_CH2_SEL_LED15,
  ADC_CH2_SEL_LED16,
  ADC_CH2_SEL_LED17,
  ADC_CH2_SEL_LED18,
  ADC_CH2_SEL_LED19,
  ADC_CH2_SEL_LED20,
  ADC_CH2_SEL_LED21,
  ADC_CH2_SEL_LED22,
  ADC_CH2_SEL_LED23,
}AdcCh2Select_t;

typedef enum {
  ADC_CH4_SEL_GPIO1 = 0U,
  ADC_CH4_SEL_GPIO2,
  ADC_CH4_SEL_GPIO3,
  ADC_CH4_SEL_GPIO4,
  ADC_CH4_SEL_GPIO5,
  ADC_CH4_SEL_GPIO6,
  ADC_CH4_SEL_GPIO7,
  ADC_CH4_SEL_GPIO8,
  ADC_CH4_SEL_GPIO9
}AdcCh4Select_t;

typedef enum {
  ADC_CH0 = 0x00U,
  ADC_CH1 = 0x01U,
  ADC_CH2 = 0x02U,
  ADC_CH3 = 0x04U,
  ADC_CH4 = 0x08U,
  ADC_CH5 = 0x10U,
  ADC_CH1_THEN_CH2 = 0x21U,
  ADC_CH1_THEN_CH3 = 0x29U,
  ADC_CH2_THEN_CH3 = 0x22U,
  ADC_CH2_THEN_CH1 = 0x2AU,
  ADC_CH3_THEN_CH1 = 0x24U,
  ADC_CH3_THEN_CH2 = 0x2CU,
  ADC_CH1_THEN_CH2_THEN_CH3 = 0x31U,
  ADC_CH1_THEN_CH3_THEN_CH2 = 0x39U,
  ADC_CH2_THEN_CH3_THEN_CH1 = 0x32U,
  ADC_CH2_THEN_CH1_THEN_CH3 = 0x3AU,
  ADC_CH3_THEN_CH1_THEN_CH2 = 0x34U,
  ADC_CH3_THEN_CH2_THEN_CH1 = 0x3CU,
}AdcChannelSequence_t;

#define CNTRL_CONVERT_POS       (0U)
#define CNTRL_CONT_POS          (1U)
#define CNTRL_SYNCENA_POS       (2U)
#define CNTRL_STUPDLY_POS       (4U)
#define CNTRL_IRQENA_POS        (6U)
#define CNTRL_IRQCLR_POS        (7U)
#define CNTRL_CHSEQ_POS         (8U)
#define CNTRL_CH2SEL_POS        (16U)
#define CNTRL_CH4SEL_POS        (20U)


typedef enum {
  ADC_WAIT_TIME_250NS = 0x00U,
  ADC_WAIT_TIME_500NS,
  ADC_WAIT_TIME_750NS,
  ADC_WAIT_TIME_1000NS,
  ADC_WAIT_TIME_1250NS,
  ADC_WAIT_TIME_1500NS,
  ADC_WAIT_TIME_1750NS,
  ADC_WAIT_TIME_2000NS,
  ADC_WAIT_TIME_2250NS,
  ADC_WAIT_TIME_2500NS,
  ADC_WAIT_TIME_2750NS,
  ADC_WAIT_TIME_3000NS,
  ADC_WAIT_TIME_3250NS,
}AdcWaittingTime_t;

#define TSET_TCURR_POS       (0U)
#define TSET_TCHNL_POS       (4U)
#define TSET_TGUARD_POS      (8U)

#define INVALID_ADC_CODE        (0x7FFFU)

void ADC_GeneralInit(void);
void ADC_UnInit(AdcMeasureItem_t item);
void ADC_RegisterIRQ(void);
void ADC_UnregisterIRQ(void);


#endif /* __ADC_DEVICE_H__ */
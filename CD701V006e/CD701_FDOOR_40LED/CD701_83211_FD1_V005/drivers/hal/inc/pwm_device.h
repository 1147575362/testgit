/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pwm_device.h
  */

#ifndef PWM_DEVICE_H__
#define PWM_DEVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include <rugbyPro.h>
#include <hwcfg.h>

typedef void (*PwmIsrCallback_t)(void);

typedef enum{
  PWM_PRESCALER_DIVIDE_1 = 0U,
  PWM_PRESCALER_DIVIDE_2,
  PWM_PRESCALER_DIVIDE_4,
  PWM_PRESCALER_DIVIDE_8,
  PWM_PRESCALER_DIVIDE_16,
  PWM_PRESCALER_DIVIDE_64,
  PWM_PRESCALER_DIVIDE_256,
  PWM_PRESCALER_DIVIDE_1024,
}PwmPrescaler_t;

typedef enum{
  PWM_CHANNEL_0 = 0U,
  PWM_CHANNEL_1,
  PWM_CHANNEL_2,
  PWM_CHANNEL_3,
  PWM_CHANNEL_4,
  PWM_CHANNEL_5,
  PWM_CHANNEL_6,
  PWM_CHANNEL_7,
  PWM_CHANNEL_8,
  PWM_CHANNEL_9,
  PWM_CHANNEL_10,
  PWM_CHANNEL_11,
  PWM_CHANNEL_12,
  PWM_CHANNEL_13,
  PWM_CHANNEL_14,
  PWM_CHANNEL_15,
  PWM_CHANNEL_16,
  PWM_CHANNEL_17,
  PWM_CHANNEL_18,
  PWM_CHANNEL_19,
  PWM_CHANNEL_20,
  PWM_CHANNEL_21,
  PWM_CHANNEL_22,
  PWM_CHANNEL_23,
}PwmChannel_t;

typedef enum{
  PWM_TRIGGER_POS_EDGE = 0U,
  PWM_TRIGGER_NEG_EDGE,
}PWMTriggerEdge_t;

typedef struct{
  PwmPrescaler_t divide;
  uint16_t       period;
  uint8_t        invertEn;
  uint8_t        enable;
}PwmConfig_t;

void PWM_Init(void);
void PWM_SetRGBValue(uint8_t channel, uint16_t red, uint16_t green, uint16_t blue);
 
void PWM_RegisterPWMReloadIRQ(void);
void PWM_UnRegisterPWMReloadIRQ(void);

int8_t PWM_SetMaxChannelCurrent(void);
void PWM_TurnOffChannelCurrent(void);

uint8_t PWM_UpdateFinished(void);
void PWM_Reset(void);
uint8_t PWM_LedIsOn(uint8_t ledNo);

uint16_t ADC_GetLedPNCode(uint8_t ledNo, uint16_t *pnCodeR,uint16_t *pnCodeG,uint16_t *pnCodeB);
uint16_t ADC_GetVBuckCode(void);
uint16_t ADC_GetVBattCode(void);
uint16_t ADC_GetVTempCode(void);


#endif /* __PWM_DEVICE_H__  */
/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file wdt_device.c
 */
#ifndef BUCK_DEVICE_H__
#define BUCK_DEVICE_H__

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <rugbyPro_sfr.h>

typedef void (*BuckIsrCallback_t)(uint8_t status);

typedef enum{
  BUCK_OVK_THRES_1_44V = 0,
  BUCK_OVK_THRES_1_65V,
  BUCK_OVK_THRES_1_85V,
  BUCK_OVK_THRES_2_06V,
  BUCK_OVK_THRES_2_26V,
  BUCK_OVK_THRES_2_47V,
  BUCK_OVK_THRES_2_68V,
  BUCK_OVK_THRES_2_87V,
  BUCK_OVK_THRES_3_08V,
  BUCK_OVK_THRES_3_29V,
  BUCK_OVK_THRES_3_50V,
  BUCK_OVK_THRES_3_69V,
  BUCK_OVK_THRES_3_90V,
  BUCK_OVK_THRES_4_11V,
  BUCK_OVK_THRES_4_31V,
  BUCK_OVK_THRES_4_52V,
  BUCK_OVK_THRES_4_72V,
  BUCK_OVK_THRES_4_93V,
  BUCK_OVK_THRES_5_12V,
  BUCK_OVK_THRES_5_22V,
  BUCK_OVK_THRES_5_32V,
  BUCK_OVK_THRES_5_42V,
  BUCK_OVK_THRES_5_53V,
  BUCK_OVK_THRES_5_65V,
  BUCK_OVK_THRES_5_77V,
  BUCK_OVK_THRES_5_90V,
  BUCK_OVK_THRES_6_03V,
  BUCK_OVK_THRES_6_16V,
  BUCK_OVK_THRES_6_31V,
  BUCK_OVK_THRES_6_46V,
  BUCK_OVK_THRES_6_61V,
  BUCK_OVK_THRES_6_78V,
}BuckOverOKVoltThres_t;

typedef enum{
  BUCK_UV_THRES_4_43V = 0,
  BUCK_UV_THRES_4_21V,
  BUCK_UV_THRES_4_02V,
  BUCK_UV_THRES_3_84V,
  BUCK_UV_THRES_3_62V,
  BUCK_UV_THRES_3_43V,
  BUCK_UV_THRES_3_24V,
  BUCK_UV_THRES_3_05V,
  BUCK_UV_THRES_2_85V,
  BUCK_UV_THRES_2_64V,
  BUCK_UV_THRES_2_46V,
  BUCK_UV_THRES_2_25V,
  BUCK_UV_THRES_2_07V,
  BUCK_UV_THRES_1_86V,
  BUCK_UV_THRES_1_67V,
  BUCK_UV_THRES_1_47V,
}BuckUnderVoltThres_t;


typedef enum{
  BUCK_OUTPUT_3000mV = 0,
  BUCK_OUTPUT_3100mV,
  BUCK_OUTPUT_3200mV,
  BUCK_OUTPUT_3300mV,
  BUCK_OUTPUT_3400mV,
  BUCK_OUTPUT_3500mV,
  BUCK_OUTPUT_3600mV,
  BUCK_OUTPUT_3700mV,
  BUCK_OUTPUT_3800mV,
  BUCK_OUTPUT_3900mV,
  BUCK_OUTPUT_4000mV,
  BUCK_OUTPUT_4100mV,
  BUCK_OUTPUT_4200mV,
  BUCK_OUTPUT_4300mV,
  BUCK_OUTPUT_4400mV,
  BUCK_OUTPUT_4500mV,
  BUCK_OUTPUT_4600mV,
  BUCK_OUTPUT_4700mV,
  BUCK_OUTPUT_4800mV,
  BUCK_OUTPUT_4900mV,
  BUCK_OUTPUT_5000mV,
  BUCK_OUTPUT_5100mV,
  BUCK_OUTPUT_5200mV,
  BUCK_OUTPUT_5300mV,
  BUCK_OUTPUT_5400mV,
  BUCK_OUTPUT_5500mV,
  BUCK_OUTPUT_5600mV,
  BUCK_OUTPUT_5700mV,
  BUCK_OUTPUT_5800mV,
}BuckOutputVoltage_t;

void BUCK_Init(BuckOutputVoltage_t enumBuckVolt);
void BUCK_RegisterEmergencyIRQ(BuckIsrCallback_t callback);
void BUCK_DisableBuck(void);
void BUCK_EnableBuck(void);



#endif














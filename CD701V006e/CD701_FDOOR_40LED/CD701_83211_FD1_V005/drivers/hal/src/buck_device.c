/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file buck_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <string.h>
#include <rugbyPro.h>
#include <buck_device.h>
#include <clock_device.h>
#include "errno.h"
#include <isrfuncs.h>


typedef struct {
    uint16_t                    outputVolt;
    BuckOverOKVoltThres_t       overVolt;
    BuckOverOKVoltThres_t       okVolt;
} BuckVoltSetting_t;


static const BuckVoltSetting_t buckSettingTable[] = {
  [BUCK_OUTPUT_3000mV] = {3000U, BUCK_OVK_THRES_3_50V, BUCK_OVK_THRES_2_68V},
  [BUCK_OUTPUT_3100mV] = {3100U, BUCK_OVK_THRES_3_69V, BUCK_OVK_THRES_2_87V},
  [BUCK_OUTPUT_3200mV] = {3200U, BUCK_OVK_THRES_3_69V, BUCK_OVK_THRES_2_87V},
  [BUCK_OUTPUT_3300mV] = {3300U, BUCK_OVK_THRES_3_90V, BUCK_OVK_THRES_3_08V},
  [BUCK_OUTPUT_3400mV] = {3400U, BUCK_OVK_THRES_3_90V, BUCK_OVK_THRES_3_08V},
  [BUCK_OUTPUT_3500mV] = {3500U, BUCK_OVK_THRES_3_90V, BUCK_OVK_THRES_3_29V},
  [BUCK_OUTPUT_3600mV] = {3600U, BUCK_OVK_THRES_3_90V, BUCK_OVK_THRES_3_29V},
  [BUCK_OUTPUT_3700mV] = {3700U, BUCK_OVK_THRES_4_11V, BUCK_OVK_THRES_3_29V},
  [BUCK_OUTPUT_3800mV] = {3800U, BUCK_OVK_THRES_4_31V, BUCK_OVK_THRES_3_50V},
  [BUCK_OUTPUT_3900mV] = {3900U, BUCK_OVK_THRES_4_31V, BUCK_OVK_THRES_3_50V},
  [BUCK_OUTPUT_4000mV] = {4000U, BUCK_OVK_THRES_4_52V, BUCK_OVK_THRES_3_69V},
  [BUCK_OUTPUT_4100mV] = {4100U, BUCK_OVK_THRES_4_52V, BUCK_OVK_THRES_3_69V},
  [BUCK_OUTPUT_4200mV] = {4200U, BUCK_OVK_THRES_4_72V, BUCK_OVK_THRES_3_90V},
  [BUCK_OUTPUT_4300mV] = {4300U, BUCK_OVK_THRES_4_72V, BUCK_OVK_THRES_3_90V},
  [BUCK_OUTPUT_4400mV] = {4400U, BUCK_OVK_THRES_4_93V, BUCK_OVK_THRES_4_11V},
  [BUCK_OUTPUT_4500mV] = {4500U, BUCK_OVK_THRES_4_93V, BUCK_OVK_THRES_4_11V},
  [BUCK_OUTPUT_4600mV] = {4600U, BUCK_OVK_THRES_5_12V, BUCK_OVK_THRES_4_31V},
  [BUCK_OUTPUT_4700mV] = {4700U, BUCK_OVK_THRES_5_12V, BUCK_OVK_THRES_4_31V},
  [BUCK_OUTPUT_4800mV] = {4800U, BUCK_OVK_THRES_5_22V, BUCK_OVK_THRES_4_52V},
  [BUCK_OUTPUT_4900mV] = {4900U, BUCK_OVK_THRES_5_22V, BUCK_OVK_THRES_4_72V},
  [BUCK_OUTPUT_5000mV] = {5000U, BUCK_OVK_THRES_5_42V, BUCK_OVK_THRES_4_72V},
  [BUCK_OUTPUT_5100mV] = {5100U, BUCK_OVK_THRES_5_53V, BUCK_OVK_THRES_4_93V},
  [BUCK_OUTPUT_5200mV] = {5200U, BUCK_OVK_THRES_5_65V, BUCK_OVK_THRES_4_93V},
  [BUCK_OUTPUT_5300mV] = {5300U, BUCK_OVK_THRES_5_77V, BUCK_OVK_THRES_4_93V},
  [BUCK_OUTPUT_5400mV] = {5400U, BUCK_OVK_THRES_5_65V, BUCK_OVK_THRES_5_12V},
  [BUCK_OUTPUT_5500mV] = {5500U, BUCK_OVK_THRES_5_90V, BUCK_OVK_THRES_5_12V},
  [BUCK_OUTPUT_5600mV] = {5600U, BUCK_OVK_THRES_6_03V, BUCK_OVK_THRES_5_22V},
  [BUCK_OUTPUT_5700mV] = {5700U, BUCK_OVK_THRES_6_16V, BUCK_OVK_THRES_5_22V},
  [BUCK_OUTPUT_5800mV] = {5800U, BUCK_OVK_THRES_6_31V, BUCK_OVK_THRES_5_32V},
};



#define BUCK_FREQ_KHZ   (400U) /*  800KHz- 300KHz */

static BuckIsrCallback_t buckIsrCallback = NULL;

/* freqiency: KHz vBuckVolt: 3000- 5800mV*/
void BUCK_Init(BuckOutputVoltage_t enumBuckVolt)
{
    uint8_t index = (uint8_t)enumBuckVolt;
    if (index > (uint8_t)BUCK_OUTPUT_5800mV){
        index = (uint8_t)BUCK_OUTPUT_4500mV;
    }
  
    BUCKCTRL_REG_CLOCK.PERIOD      = (uint8_t)(16000U/BUCK_FREQ_KHZ -1U);
    BUCKCTRL_REG_CLOCK.SSCDEEP     = 4U; /* Buck SSC Freq = Buck_Freq/[(SSC_DIV+1)*(SSC_DEEP+1)*4]  */
    BUCKCTRL_REG_CLOCK.SSCDIV      = 4U;
    BUCKCTRL_REG_CLOCK.SPREAD      = 1U; /* Enable frequency spread */

    /* Enable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x0EU;
    BUCKCTRL_REG_VDACCODE.VDACTRIM = (uint8_t)(((uint32_t)buckSettingTable[index].outputVolt *256U *32U)/(11U*25000U) -1U); /* vBuckVolt: 6.25*1.375*(VDACTRIM+1)/256 */

    uint8_t BUCK_IPEAK = HWCFG_SFRS->PEAK_ILIM_CODE;
    if (BUCK_IPEAK == 0xFFU){
        BUCK_TRIM_REG_DATA4.SETILIM = 0x8;
    }else{
        BUCK_IPEAK = BUCK_IPEAK + 6U;
        BUCK_TRIM_REG_DATA4.SETILIM  = (BUCK_IPEAK >> 1);
        BUCK_TRIM_REG_DATA4.TRIMILIM = BUCK_IPEAK & 0x01U;
    }

    BUCKCTRL_REG_VBUCKCTRL.OVLEVEL_SEL  = (uint8_t)buckSettingTable[index].overVolt;     /* over volt monitor level */
    BUCKCTRL_REG_VBUCKCTRL.OKLEVEL_SEL  = (uint8_t)buckSettingTable[index].okVolt;       /* ok volt monitor level */
    BUCKCTRL_REG_VBUCKCTRL.UVLEVEL_SEL  = (uint8_t)BUCK_UV_THRES_1_47V;    /* under volt monitor level*/

    BUCKCTRL_REG_VBUCKCTRL.OV_MONITOR_ENA  = 1U;        /* enable over volt monitor */
    BUCKCTRL_REG_VBUCKCTRL.OK_MONITOR_ENA  = 1U;        /* enable OK volt monitor */

    BUCKCTRL_REG_VBUCKDBNC.BUCKSTRBDBNCSEL = 3U;        
    BUCKCTRL_REG_VBUCKDBNC.OVTHRES1 = 7U;               /* over volt debouncing settings  */
    BUCKCTRL_REG_VBUCKDBNC.UVTHRES1 = 7U;               /* under volt debouncing settings  */
    BUCKCTRL_REG_VBUCKDBNC.LFENA       = 0x00U;         /* Low frequency strobing enable for debouncing */
    BUCKCTRL_REG_VBUCKDBNC.BUCKDBNCENA = 0x0FU;         /* enable debouncing */
    BUCKCTRL_REG_VBUCKDBNC.EXTRADBNC   = 0x07U;

    BUCKCTRL_REG_VBUCKTIMEOUT.PREENADWELL       = 0x0FU;
    BUCKCTRL_REG_VBUCKTIMEOUT.DETLOWDWELL       = 0x08U;
    BUCKCTRL_REG_VBUCKTIMEOUT.STARTLDWELL       = 0x08U;
    BUCKCTRL_REG_VBUCKTIMEOUT.STARTDBNCDWELL    = 0x06U;
    BUCKCTRL_REG_VBUCKTIMEOUT.UVDBNCDWELL       = 0x01U;
    BUCKCTRL_REG_VBUCKTIMEOUT.OVDBNCDWELL       = 0x0FU;
    BUCKCTRL_REG_VBUCKTIMEOUT.HICCUPDWELL       = 0x0FU;
    BUCKCTRL_REG_CTRL.HICCUPENA  = 1U;
    BUCKCTRL_REG_CTRL.ENAREQ     = 1U;                  /* enable buck */
}

void BUCK_RegisterEmergencyIRQ(BuckIsrCallback_t callback)
{
    buckIsrCallback = callback;
    BUCKCTRL_REG_BUCKIRQ.ENABLE.BUCK_OV = 1U;
    BUCKCTRL_REG_BUCKIRQ.ENABLE.BUCK_UV = 1U;
    BUCKCTRL_REG_BUCKIRQ.ENABLE.OVERCUR = 1U;
    BUCKCTRL_REG_BUCKIRQ.ENABLE.START_ERROR = 1U;
    
    BUCKCTRL_REG_BUCKIRQ.ENABLE.DISABLED = 1U;
    NVIC_EnableIRQ(BUCK_IRQn);
}


void BUCK_Handler(void)
{
    uint8_t status = BUCKCTRL_REG_BUCKIRQ.STATUS.BYTE;
    if (buckIsrCallback != NULL){
        buckIsrCallback(status);
    }
    BUCKCTRL_REG_BUCKIRQ.CLEAR.BYTE = 0xFFU;
}


void BUCK_DisableBuck(void)
{
    BUCKCTRL_REG_CTRL.ENAREQ     = 0U;                  /* disable buck */
}

void BUCK_EnableBuck(void)
{
    BUCKCTRL_REG_CTRL.ENAREQ     = 1U;                  /* enable buck */
}












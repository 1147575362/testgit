/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file hwProtection.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */
#include <hwProtection.h>
#include <appConfig.h>
#include <isrfuncs.h>

static hwProtIsrCallback_t otCallback = NULL;
static hwProtIsrCallback_t uvCallback = NULL;
static hwProtIsrCallback_t ovCallback = NULL;


void OVTEMP_Handler(void)
{
    if (otCallback != NULL){
        otCallback();
    }
    CRGA_SFRS->RESETCTRL.OVTEMPFLAGCLR = 1U;
    NVIC_DisableIRQ(OVTEMP_IRQn);
}


void VBAT_Handler(void)
{
    if (PMUA_SFRS->PMUIRQ.STATUS.UV == 1U){
        if (uvCallback != NULL){
            uvCallback();
        }
        PMUA_SFRS->PMUIRQ.CLEAR.UV =1U;
    }
    if (PMUA_SFRS->PMUIRQ.STATUS.OV == 1U){
        if (ovCallback != NULL){
            ovCallback();
        }
        PMUA_SFRS->PMUIRQ.CLEAR.OV =1U;
    }
    NVIC_DisableIRQ(VBAT_IRQn);
}


void HW_PROT_SetOverTmperature(OverTempThres_t threshold, OverTempAction_t action)
{
    CRGA_SFRS->OVTEMPCONFIG.OVTEMPLEVEL = (uint8_t)threshold;
    CRGA_SFRS->OVTEMPACTION.OVTEMP      = (uint8_t)action;
    CRGA_SFRS->OVTEMPCONFIG.DISTEMPSENS = 0U;    /* enable temperature sensor */
    CRGA_SFRS->OVTEMPCONFIG.OVTEMPENA   = 1U;    /* enable ov temperature action */ 
}

void HW_PROT_RegisterOVTemperatureIRQ(hwProtIsrCallback_t callback)
{
    otCallback = callback;
    CRGA_SFRS->RESETCTRL.OVTEMPFLAGCLR = 1U;
    NVIC_EnableIRQ(OVTEMP_IRQn);
}

void HW_PROT_UnRegisterOVTemperatureIRQ(void)
{
    NVIC_DisableIRQ(OVTEMP_IRQn);
    CRGA_SFRS->RESETCTRL.OVTEMPFLAGCLR = 1U;
    otCallback = NULL;
}


void HW_PROT_SetOverVoltage(OverVoltThres_t threshold,OverVoltHysteresis_t hys, VoltDebouceTime_t triggerTime, VoltDebouceTime_t releaseTime)
{
    PMUA_SFRS->VBATTRIM.OVLEVEL = (uint8_t)threshold;
    PMUA_SFRS->VBATTRIM.OVHYS   = (uint8_t)hys;
    
    PMUA_SFRS->VBATDBNC.OVSTRB0SEL = 1U; /*  16KHz source  */
    PMUA_SFRS->VBATDBNC.OVSTRB1SEL = 1U; /*  16KHz source  */
    
    PMUA_SFRS->VBATDBNCTHRES.OVTHRES1 = (uint8_t)triggerTime;/* value*62.5us when 16Khz*/
    PMUA_SFRS->VBATDBNCTHRES.OVTHRES0 = (uint8_t)releaseTime;/* value*62.5us when 16Khz*/
    
    PMUA_SFRS->VBATCTRL.OV_MONITOR_ENA = 1U;
    
}

void HW_PROT_RegisterOverVoltageIRQ(hwProtIsrCallback_t callback)
{
    ovCallback = callback;
    PMUA_SFRS->PMUIRQ.CLEAR.OV  = 1U;
    PMUA_SFRS->PMUIRQ.ENABLE.OV = 1U;
    NVIC_EnableIRQ(VBAT_IRQn);
}

void HW_PROT_UnRegisterOverVoltageIRQ(void)
{
    if (uvCallback == NULL){
        NVIC_DisableIRQ(VBAT_IRQn);
    }
    PMUA_SFRS->PMUIRQ.ENABLE.OV = 0U;
    PMUA_SFRS->PMUIRQ.CLEAR.OV  = 1U;
    ovCallback = NULL;
}


void HW_PROT_SetUnderVoltage(UnderVoltThres_t threshold,UnderVoltHysteresis_t hys, VoltDebouceTime_t triggerTime, VoltDebouceTime_t releaseTime)
{
    PMUA_SFRS->VBATTRIM.UVLEVEL = (uint8_t)threshold;
    PMUA_SFRS->VBATTRIM.UVHYS   = (uint8_t)hys;
    
    PMUA_SFRS->VBATDBNC.UVSTRB0SEL = 1U; /*  16KHz source  */
    PMUA_SFRS->VBATDBNC.UVSTRB1SEL = 1U; /*  16KHz source  */
    
    PMUA_SFRS->VBATDBNCTHRES.UVTHRES1 = (uint8_t)triggerTime; /* value*62.5us when 16Khz*/
    PMUA_SFRS->VBATDBNCTHRES.UVTHRES0 = (uint8_t)releaseTime; /* value*62.5us when 16Khz*/
    
    PMUA_SFRS->VBATCTRL.LOW_MONITOR_ENA = 1U;
}

void HW_PROT_RegisterUnderVoltageIRQ(hwProtIsrCallback_t callback)
{
    uvCallback = callback;
    PMUA_SFRS->PMUIRQ.CLEAR.UV  = 1U;
    PMUA_SFRS->PMUIRQ.ENABLE.UV = 1U;
    NVIC_EnableIRQ(VBAT_IRQn);
}

void HW_PROT_UnRegisterUnderVoltageIRQ(void)
{
    if (ovCallback == NULL){
        NVIC_DisableIRQ(VBAT_IRQn);
    }
    PMUA_SFRS->PMUIRQ.ENABLE.UV = 0U;
    PMUA_SFRS->PMUIRQ.CLEAR.UV  = 1U;
    uvCallback = NULL;
}


uint8_t HW_PROT_GetBattVoltState(void)
{
    uint8_t result = (uint8_t)PMUA_SFRS->VBATCTRL.BATT_STATE;
    return result;
}

uint8_t HW_PROT_ChipIsOverHeat(void)
{
    uint8_t result = (uint8_t)CRGA_SFRS->RESETCTRL.OVTEMPFLAG;
    return result;
}





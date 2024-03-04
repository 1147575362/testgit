/**
 * @copyright 2015 IndieMicro.
 *
 * This file is proprietary to IndieMicro.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of IndieMicro.
 *
 * @file adc_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/09/10
 */

#include <adc_device.h>
#include <isrfuncs.h>
#include <appConfig.h>
static ADCMeasureParam_t adcMeasParamm = {ADC_MEASURE_ITEM_NONE, 0};
static adc_cb_func_t adcCallback = NULL;
static uint16_t adcResult[3];


void ADC_Handler(void)
{
    if (adcCallback !=NULL){
        adcResult[0] = (uint16_t)(ADC_SFRS->DATA0345 & 0x0FFFU);        /*Vtemp */
        adcResult[1] = (uint16_t)(ADC_SFRS->DATA1 & 0x0FFFU);           /*Battery volt, */
        adcResult[2] = (uint16_t)(ADC_SFRS->DATA2 & 0x0FFFU);           /*Led PN */
       adcCallback(adcMeasParamm, adcResult);
    }
    ADC_SFRS->CNTRL.IRQCLR = 1U;
}

void ADC_GeneralInit(void)
{
    ADC_SFRS->CONF.AUTOEN       = 0U; 
    ADC_SFRS->CONF.SAMPCYC      = 10U;
    ADC_SFRS->CONF.MODE         = (uint8_t)ADC_MODE_SINGLE_END;
    ADC_SFRS->CNTRL.STUPDLY     = (uint8_t)ADC_STARTUP_DELAY_1US;
    ADC_SFRS->CONF.SYNCENA      = (uint8_t)SYNC_MODE_NONE; /* disable sync convert mode */
    ADC_SFRS->CNTRL.CONT        = 0U; /* singel convert mode */
    ADC_REG_TSET.WORD           = (uint32_t)(((uint32_t)ADC_WAIT_TIME_1500NS << TSET_TCURR_POS) | ((uint32_t)ADC_WAIT_TIME_250NS << TSET_TCHNL_POS) | ((uint32_t)ADC_WAIT_TIME_250NS << TSET_TGUARD_POS));
}


void ADC_Init(AdcMeasureItem_t item, uint8_t channel, LedType_t type, AdcSyncMode_t syncMode)
{
    adcMeasParamm.item       = item;
    adcMeasParamm.channel    = channel;
    if(item == ADC_MEASURE_ITEM_VBAT_VTEMP){
        ADC_SFRS->CONF.SAMPCYC      = 44U;
    }else{
        ADC_SFRS->CONF.SAMPCYC      = 10U;
    }
    ADC_SFRS->CONF.ATTEN = 0U;
    
    switch(item){
    case ADC_MEASURE_ITEM_LED_VBAT_VTEMP:            /*LED: 1/5.5x, BUCK: 1/8x, VBAT:1/32x*/
      ADC_SFRS->CONF.ATTEN = 0U;
      IOCTRLA_SFRS->LEDPIN.VFW_ENA = 1U;        /* Enable 2mA led current source */
      IOCTRLA_SFRS->LEDPIN.SENSE_ENA = 1U;      /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
      ADC_SFRS->CNTRL.CH1SEL = (uint8_t)ADC_CH1_SEL_VBAT;
      if (type == LED_RED){
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_R;/*  measure red PN volt */
      }else if (type == LED_GREEN){
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_G;/*  measure green PN volt */
      }else{
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_B;/*  measure blue PN volt */
      }
      ADC_SFRS->CNTRL.CH3SEL = (uint8_t)ADC_CH3_SEL_TEMP_SENSOR;
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH2_THEN_CH1_THEN_CH3;
      
      break;
      
    case ADC_MEASURE_ITEM_VBAT_VTEMP:/* 1x */
      ADC_SFRS->CONF.ATTEN = 0U;
      ADC_SFRS->CNTRL.CH1SEL = (uint8_t)ADC_CH1_SEL_VBAT;
      ADC_SFRS->CNTRL.CH3SEL = (uint8_t)ADC_CH3_SEL_TEMP_SENSOR;
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH1_THEN_CH3;
      break;
      
    case ADC_MEASURE_ITEM_LED:
      ADC_SFRS->CONF.ATTEN = 0U;
      IOCTRLA_SFRS->LEDPIN.VFW_ENA = 1U;        /* Enable 2mA led current source */
      IOCTRLA_SFRS->LEDPIN.SENSE_ENA = 1U;      /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
      if (type == LED_RED){
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_R;/*  measure red PN volt */
      }else if (type == LED_GREEN){
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_G;/*  measure green PN volt */
      }else{
          ADC_SFRS->CNTRL.CH2SEL = (uint8_t)PHY_CHANNEL_B;/*  measure blue PN volt */
      }
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH2;
      break;
    case ADC_MEASURE_ITEM_VBG:  /*1x */
      ADC_SFRS->CNTRL.CHSEQ = (uint8_t)ADC_CH0;
      break; 
      
    case ADC_MEASURE_ITEM_VBAT: /*1/32x */
      ADC_SFRS->CNTRL.CH1SEL = (uint8_t)ADC_CH1_SEL_VBAT;
      ADC_SFRS->CNTRL.CHSEQ = (uint8_t)ADC_CH1;
      break;
      
    case ADC_MEASURE_ITEM_VDD1V5:/* 1/3x */
      ADC_SFRS->CONF.ATTEN = 1U;
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH5;
      break;
      
    case ADC_MEASURE_ITEM_GPIO:/*1/3x,*/
      ADC_SFRS->CNTRL.CH4SEL = channel;
      ADC_SFRS->CONF.ATTEN = 1U;
      ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH4;
      break;

    default:
      break;
    }
    ADC_SFRS->CONF.SYNCENA = (uint8_t)syncMode;
}

void ADC_UnInit(AdcMeasureItem_t item)
{
    
  
}

/**
 * @brief Register a callback function to ADC interrupt handler.
 *
 * @param dev Pointer to the ADC device.
 * @param cb The callback function to be registered.
 * @return 0 for success or error code upon a failure.
 */
void ADC_RegisterIRQ(adc_cb_func_t callback)
{
    adcCallback = callback;
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.IRQENA = 1U;
    NVIC_EnableIRQ(ADC_IRQn);
    
}

/**
 * @brief Unregister a callback function to ADC interrupt handler.
 *
 * @param dev Pointer to the ADC device.
 * @param cb The callback function to be unregistered.
 * @return 0 for success or error code upon a failure.
 */
void ADC_UnregisterIRQ(void)
{
    adcCallback = NULL;
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.IRQENA = 0U;
    NVIC_DisableIRQ(ADC_IRQn);
}

void ADC_Start(void)
{
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.CONVERT = 1U;
}

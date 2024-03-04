/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file adc_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <adc_device.h>
#include <isrfuncs.h>
#include <appConfig.h>


void ADC_GeneralInit(void)
{
    ADC_SFRS->CONF.AUTOEN       = 0U; 
    ADC_SFRS->CONF.SAMPCYC      = 8U;
    ADC_SFRS->CONF.MODE         = (uint8_t)ADC_MODE_SINGLE_END;
    ADC_SFRS->CNTRL.STUPDLY     = (uint8_t)ADC_STARTUP_DELAY_1US;
    ADC_SFRS->CNTRL.CONT        = 0U; /* singel convert mode */
    ADC_REG_TSET.WORD           = (uint32_t)(((uint32_t)ADC_WAIT_TIME_250NS << TSET_TCURR_POS) | ((uint32_t)ADC_WAIT_TIME_250NS << TSET_TCHNL_POS) | ((uint32_t)ADC_WAIT_TIME_250NS << TSET_TGUARD_POS));
    
    ADC_SFRS->CNTRL.CH2SEL = (uint8_t)ADC_CH2_SEL_LED0;
    ADC_SFRS->CNTRL.CH3SEL = (uint8_t)ADC_CH1_CH3_SEL_VBAT;
    ADC_SFRS->CONF.SYNCENA = (uint8_t)SYNC_MODE_PWM_PERIOD;
    IOCTRLA_SFRS->LEDPIN.VFW_ENA = 1U;        /* Enable led pn measurement current source */
    IOCTRLA_SFRS->LEDPIN.SENSE_ENA = 1U;      /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
    
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.IRQENA = 1U;
    NVIC_EnableIRQ(ADC_IRQn);

}

/**
 * @brief Register a callback function to ADC interrupt handler.
 *
 * @param dev Pointer to the ADC device.
 * @param cb The callback function to be registered.
 * @return 0 for success or error code upon a failure.
 */
void ADC_RegisterIRQ(void)
{
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
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.IRQENA = 0U;
    NVIC_DisableIRQ(ADC_IRQn);
}

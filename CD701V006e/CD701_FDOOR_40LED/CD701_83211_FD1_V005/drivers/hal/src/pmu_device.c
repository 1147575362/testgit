/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pmu_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <stddef.h>
#include <pmu_device.h>
#include <errno.h>
#include <gpio_device.h>
#include <lin_device.h>
#include <wica_sfr.h>
#include <pwm_device.h>
#include <buck_device.h>
#include <isrfuncs.h>
#include "wdt_device.h"
   
static __INLINE void WDTA_Start(void);

void BOR_Handler(void) 
{
    
}
   
void PMU_BORInit(Bor1V5Thres_t lowThreshold, Bor3V3Thres_t highThreshold)
{
    CRGA_SFRS->BORCONFIG.BOR1V5THRESH = (uint8_t)lowThreshold;
    CRGA_SFRS->BORCONFIG.BOR3V3THRESH = (uint8_t)highThreshold;
    CRGA_SFRS->BORCONFIG.BORBIASOVERRIDESEL = 0U;
    CRGA_SFRS->BORACTION.VDD1V5 = (uint8_t)PMU_BROWNOUT_RESET;
    CRGA_SFRS->BORACTION.VDD3V3 = (uint8_t)PMU_BROWNOUT_RESET;
    
    CRGA_SFRS->BORACTION.BOR_1V5_LOCK = 1U;
    CRGA_SFRS->BORACTION.BOR_3V3_LOCK = 1U;
    
    CRGA_SFRS->OVTEMPCONFIG.DISTEMPSENS = 0U; /* enable temperature sensor */
    
    EVTHOLD_SFRS->HOLD = 0U; /* clear  Lullaby_Handler after wake up  */
}

void PMU_WakeTimerInit(PMU_WAKEUP_TIMEER_MODE_t mode, PMU_WAKEUP_TIMEER_Interval_t interval)
{
  if (mode == WAKEUP_TIMEER_DISABLE){
      WICA_SFRS->CTRL.TIMERENA = 0U;
  }else{
      WICA_SFRS->CTRL.TIMERTAPSEL = (uint8_t)interval;
      WICA_SFRS->CTRL.TIMERENA    = 1U;
  }
}

void PMU_EnterDeepSleepMode(void)
{
    BUCK_DisableBuck();                 /* disable buck*/
    CRGA_SFRS->MODULERSTREQ.BYTE = MODUE_SOFT_RESET_PWM;
    /* Add GPIO function here for low power mode*/
    /* set gpios to input with power up resistor */
    
    for (uint8_t i = (uint8_t)GPIO_PORT_1; i<= (uint8_t)GPIO_PORT_9; i++ ){
        GPIO_Init((GpioPort_t)i, GPIO_DIR_INPUT,GPIO_PULL_DOWN);
    }
    /*  end of GPIO settigns   */
    CRGA_SFRS->OVTEMPCONFIG.OVTEMPENA = 0U;     /* disbale temperature sensor when sleep */
    CRGA_SFRS->OVTEMPCONFIG.DISTEMPSENS = 1U;
    
    /* clear all of wake up flags */
    /* Enable wakeup detect and IRQ  when LINS is in sleep mode(LINS_REG_CTRL.SLEEP = 1U;)  */
    WICA_REG_CTRL.LINSIRQCLR = 1U;
    WICA_REG_CTRL.LINSIRQENA = 1U;
    WICA_REG_CTRL.LINSENA    = 1U;
    NVIC_EnableIRQ(WULIN_IRQn);

    NVIC_EnableIRQ(Lullaby_IRQn);               /*  Enable  Lullaby interrupt*/
    EVTHOLD_SFRS->HOLD = 1U;                    /*  Trigger ISR then entering sleep in ISR */
    /* Waiting to enter sleep mode */
    for(;;){
    }
}

/* Please make sure here and don't delete these functions!!!! or it would cause severe error*/
/* hibernate wake up interrupt ISR */
void Lullaby_Handler(void)
{
    /* Set lin to sleep mode */
    LINS_REG_CTRL.SLEEP = 1U;
    /*  Enter sleep mode  */
    PMUA_SFRS->CTRL.HIBERNATE = 1U;
}



#pragma optimize=none
static __INLINE void WDTA_Start(void)
{
    WDTA_SFRS->CTRL.WDTA_TIMEOUT_SEL    = (uint8_t)WDTA_INTERVAL_8S;
    WDTA_SFRS->CLEAR = 0x3c574ad6U;
    WDTA_SFRS->STOP  = 0U;/* start wdt*/
}


void NMI_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void HardFault_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void SVC_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void PendSV_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void BTE_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void SDIO_Handler(void)
{
    WDTA_Start();
    for(;;){}
}



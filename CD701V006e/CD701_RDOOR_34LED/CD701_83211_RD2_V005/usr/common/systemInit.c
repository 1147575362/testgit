/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file systemInit.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <systemInit.h>
#include <rugbyPro.h>
#include <buck_device.h>
#include <pwmAux_device.h>
#include <uart_device.h>
#include <linSlaveTask.h>
#include <linStackTask.h>
#include <adc_device.h>
#include <applicationTask.h>

void leds_driver_Init(void);
void gpios_init(void);
void pmu_init(void);


void leds_driver_Init(void)
{
    (void)PWM_SetMaxChannelCurrent();
    ADC_GeneralInit();
    PWM_Init();
}



void gpios_init(void)
{
    /* set gpios to input with power up resistor */
    GPIO_Init(GPIO_PORT_2, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
    GPIO_Init(GPIO_PORT_3, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
    GPIO_Init(GPIO_PORT_6, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
    GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6), (0U << GPIO_PORT_2) | (0U << GPIO_PORT_3) | (0U << GPIO_PORT_6));
    
    GPIO_Init(GPIO_PORT_9, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
    GPIO_Set(GPIO_PORT_9, GPIO_LOW);
}

void pmu_init(void)
{
    /* Init set BOR voltage level for cpu low voltage safety*/
    PMU_BORInit(BOR_1V5_THRS_1385mV, BOR_3V3_THRS_3034mV);
    /* Disable wake up timer */
    PMU_WakeTimerInit(WAKEUP_TIMEER_DISABLE, WAKEUP_TIMEER_INTERVAL_32768ms);
}


void SYS_Init(void)
{
    /* Enable trim revise access enable*/
    HWCFG_TrimAccessUnlock();
    CRGA_SFRS->MODULERSTREQ.BYTE = 0xFFU;
    /* Init system clock */
    Clock_SystemMainClockInit(SYS_MAIN_CLOCK_DIV);
    pmu_init();
    /* Init global timer engine for driving soft timer */
    SysTick_Init(SOFT_TIMER_INTERVAL *1000U * MAIN_CPU_CLOCK, SoftTimer_ExpireCallback);
#if WATCH_DOG_EN == 1U
      WDTA_Enable(WDTA_INTERVAL_8S); /* 8s */
#endif
    /* Init gpios settings */
    gpios_init();
    /* Init LED current and PWM settings */
    leds_driver_Init();
    
#if (CODE_DEBUG_EN == 1U) || (UART_MILKY_WAY_EN == 1U)
    (void)UART_Init(BAUDRATE_DIV_16MHz_1000000, BITSIZE_8BITS, PARITY_NONE,STOPS_1BITS);
    UART_RegisterIRQ(0,APPL_UART_ISR);
#endif
    /* Init buck settings */
//#if (UART_MILKY_WAY_EN == 1U)
    BUCK_Init(BUCK_OUTPUT_5000mV);
//#else
//
//    BUCK_Init(BUCK_OUTPUT_3800mV);
//#endif  
    
    /* Disable trim revise access until reset*/
    /*HWCFG_TrimAccessLockUntilReset();*/
    
    /* tasks init must be called before use. */
    TM_PostTask(TASK_ID_SOFT_TIMER);
  //  TM_PostTask(TASK_ID_SAFETY_MONITOR);
    TM_PostTask(TASK_ID_LINS);
    TM_PostTask(TASK_ID_PDS);
#if LIN_MASTER_EN == 1
    TM_PostTask(TASK_ID_LINM);
#endif
    TM_PostTask(TASK_ID_ADC_MEASURE);
    TM_PostTask(TASK_ID_COLOR_MIXING);
  //  TM_PostTask(TASK_ID_COLOR_COMPENSATION);
    TM_PostTask(TASK_ID_APPL);
}




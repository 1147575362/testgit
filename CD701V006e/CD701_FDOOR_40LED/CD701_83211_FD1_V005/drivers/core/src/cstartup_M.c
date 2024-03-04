/**************************************************
 *
 * This file contains an interrupt vector 
 * KamCho. (Cortex-M written in C)
 * The actual interrupt functions must be provided 
 * by the application developer.
 *
 * Copyright 2007 IAR Systems. All rights reserved.
 *
 * $Revision: 3998 $
 *
 **************************************************/

#pragma language=extended
#pragma segment="CSTACK"

#include <isrfuncs.h>

typedef void( *intfunc_t )( void );

void __iar_program_start( void );

typedef struct{
    void * __ptr;
    intfunc_t _iar_program_start;
    intfunc_t _NMI_Handler;
    intfunc_t _HardFault_Handler;
    intfunc_t _MemManage_Handler;
    intfunc_t _BusFault_Handler;
    intfunc_t _UsageFault_Handler;
    intfunc_t _rsv0;
    intfunc_t _rsv1;
    intfunc_t _rsv2;
    intfunc_t _rsv3;
    intfunc_t _SVC_Handler;
    intfunc_t _DebugMon_Handler;
    intfunc_t _rsv4;
    intfunc_t _PendSV_Handler;
    intfunc_t _SysTick_Handler;
    
    intfunc_t _WULIN_Handler;           /* ASIC irq 0 - Wake Up LIN */
    intfunc_t _PWMAUX_Handler;          /* ASIC irq 1 - PWM Aux -GPIO PWMs*/
    intfunc_t _WUTIMER_Handler;         /* ASIC irq 2 - Wake Up TIMER */
    intfunc_t _BOR_Handler;             /* ASIC irq 3 - BOR */
    intfunc_t _VBAT_Handler;            /* ASIC irq 4 - OVer/Under Voltage Event*/
    intfunc_t _OVTEMP_Handler;          /* ASIC irq 5 - OVTEMP */
    intfunc_t _WatchdogA_Handler;       /* ASIC irq 6 - Watchdog Timer */
    intfunc_t _GPIO_Handler;            /* ASIC irq 7 - GPIO */
    intfunc_t _LINS_Handler;            /* ASIC irq 8 - LIN Slave */
    intfunc_t _LINM_Handler;            /* ASIC irq 9 - LIN Master */
    intfunc_t _UART_Handler;            /* ASIC irq 10 - UART data ready*/
    intfunc_t _ADC_Handler;             /* ASIC irq 11 - ADC data ready */
    intfunc_t _PWM_Handler;             /* ASIC irq 12 - PWM LEDs Ready */
    intfunc_t _BUCK_Handler;            /* ASIC irq 13 - BUCK */
    intfunc_t _SPI_Handler;             /* ASIC irq 14 - SPI */
    intfunc_t _Lullaby_Handler;         /* ASIC irq 15 - lullaby */
    intfunc_t _Timer0_Handler;          /* vern irq 16 */
    intfunc_t _Timer1_Handler;          /* vern irq 17 */
    intfunc_t _Timer2_Handler;          /* vern irq 18 */
    intfunc_t _Watchdog_Handler;        /* vern irq 19 */
    intfunc_t _BTE_Handler;             /* vern irq 20 */
    intfunc_t _SDIO_Handler;            /* vern irq 21 */
    intfunc_t _rsv22;                   /* vern irq 22 */
    intfunc_t _rsv23;                   /* vern irq 23 */
    intfunc_t _rsv24;                   /* vern irq 24 */
    intfunc_t _rsv25;                   /* vern irq 25 */
    intfunc_t _rsv26;                   /* vern irq 26 */
    intfunc_t _rsv27;                   /* vern irq 27 */
    intfunc_t _rsv28;                   /* vern irq 28 */
    intfunc_t _rsv29;                   /* vern irq 29 */
    intfunc_t _rsv30;                   /* vern irq 30 */
    intfunc_t _rsv31;                   /* vern irq 31 */
#ifdef BOOTLOADER_EN
    intfunc_t hwcode;                   /* chip code offset = 0xC0  */
#endif  
}intvecTable_t;

/*
 The vector table is normally located at address 0.
 When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
 If you need to define interrupt service routines,
 make a copy of this file and include it in your project.
 The name "__vector_table" has special meaning for C-SPY:
 it is where the SP start value is found, and the NVIC vector
 table register (VTOR) is initialized to this address if != 0.
*/

#pragma location = ".intvec"
static const intvecTable_t __vector_table =
{
  .__ptr = __sfe( "CSTACK" ),
  ._iar_program_start           = __iar_program_start,
  ._NMI_Handler                 = NMI_Handler,
  ._HardFault_Handler           = HardFault_Handler,
  ._MemManage_Handler           = 0U,
  ._BusFault_Handler            = 0U,
  ._UsageFault_Handler          = 0U,
  ._rsv0                        = 0U,
  ._rsv1                        = 0U,
  ._rsv2                        = 0U,
  ._rsv3                        = 0U,
  ._SVC_Handler                 = SVC_Handler,
  ._DebugMon_Handler            = 0U,
  ._rsv4                        = 0U,
  ._PendSV_Handler              = PendSV_Handler,
  ._SysTick_Handler             = SysTick_Handler,
  ._WULIN_Handler               = WULIN_Handler,        /* ASIC irq 0 - Wake Up LIN - WICA*/
  ._PWMAUX_Handler              = PWMAUX_Handler,       /* ASIC irq 1 - PWM Aux -GPIO PWMs*/
  ._WUTIMER_Handler             = WUTIMER_Handler,      /* ASIC irq 2 - Wake Up TIMER */
  ._BOR_Handler                 = BOR_Handler,          /* ASIC irq 3 - BOR */
  ._VBAT_Handler                = VBAT_Handler,         /* ASIC irq 4 - Under Voltage Event*/
  ._OVTEMP_Handler              = OVTEMP_Handler,       /* ASIC irq 5 - OVTEMP */
  ._WatchdogA_Handler           = WatchdogA_Handler,    /* ASIC irq 6 - Watchdog Timer */
  ._GPIO_Handler                = GPIO_Handler,         /* ASIC irq 7 - GPIO */
  ._LINS_Handler                = LINS_Handler,         /* ASIC irq 8 - LIN Slave */
  ._LINM_Handler                = LINM_Handler,         /* ASIC irq 9 - LIN Master */
  ._UART_Handler                = UART_Handler,         /* ASIC irq 10 - UART data ready*/
  ._ADC_Handler                 = ADC_Handler,          /* ASIC irq 11 - ADC data ready */
  ._PWM_Handler                 = PWM_Handler,          /* ASIC irq 12 - PWM LEDs Ready */
  ._BUCK_Handler                = BUCK_Handler,         /* ASIC irq 13 - BUCK */
  ._SPI_Handler                 = SPI_Handler,          /* ASIC irq 14 - SPI */
  ._Lullaby_Handler             = Lullaby_Handler,      /* ASIC irq 15 - lullaby */
  ._Timer0_Handler              = Timer0_Handler,       /* vern irq 16 */
  ._Timer1_Handler              = Timer1_Handler,       /* vern irq 17 */
  ._Timer2_Handler              = Timer2_Handler,       /* vern irq 18 */
  ._Watchdog_Handler            = Watchdog_Handler,     /* vern irq 19 */
  ._BTE_Handler                 = BTE_Handler,          /* vern irq 20 */
  ._SDIO_Handler                = SDIO_Handler,         /* vern irq 21 */
  ._rsv22                       = 0U,                   /* vern irq 22 */
  ._rsv23                       = 0U,                   /* vern irq 23 */
  ._rsv24                       = 0U,                   /* vern irq 24 */
  ._rsv25                       = 0U,                   /* vern irq 25 */
  ._rsv26                       = 0U,                   /* vern irq 26 */
  ._rsv27                       = 0U,                   /* vern irq 27 */
  ._rsv28                       = 0U,                   /* vern irq 28 */
  ._rsv29                       = 0U,                   /* vern irq 29 */
  ._rsv30                       = 0U,                   /* vern irq 30 */
  ._rsv31                       = 0U,                   /* vern irq 31 */
#ifdef BOOTLOADER_EN
 .hwcode                       = (intfunc_t)83211,      
#endif
};

void __cmain( void );

__weak void __iar_init_core( void );
__weak void __iar_init_vfp( void );

#pragma required=__vector_table
void __iar_program_start( void )
{
    __iar_init_core();
    __iar_init_vfp();
    __cmain();
}





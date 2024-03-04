#ifndef APP_CONFIG_H__
#define APP_CONFIG_H__

#include <clock_device.h>
#include <pwm_device.h>
#include <string.h>

#define PRAGMA(x) _Pragma(#x)
#define BEGIN_PACK PRAGMA(pack(push, 1))
#define END_PACK   PRAGMA(pack(pop))


#define LIN_STACK_TYPE_LIN2_2A          (0U)
#define LIN_STACK_TYPE_SAEJ2602         (1U)
#define LIN_STACK_TYPE_PRIVATE          (2U)

#define LIN_STACK_TYPE                  LIN_STACK_TYPE_LIN2_2A

#define AUTO_ADDRESSING_EN              (0U)

#define SDK_APPL_VERSION        (0x0105U)

/* ************************hardware config ********************************/
#define MAIN_CPU_CLOCK          (16U)       /*MHz*/

#if MAIN_CPU_CLOCK == 16U
    #define SYS_MAIN_CLOCK_DIV    CLOCK_DIV_1
#else
    #error MAIN_CPU_CLOCK MUST BE 16MHz!
#endif


#define ROOM_TEMPERATURE        (24)  /*Celsius degree*/

#define BOARD_TYPE_DP           (0)
#define BOARD_TYPE_SY           (1)
#define BOARD_TYPE_EVB_24LED           (2)



/* ************************PWMs config ********************************/
/* pwm resolution settings*/
#define PWM_VALUE_MAX_POS               (15U)
#define PWM_VALUE_MAX                   ((1U << PWM_VALUE_MAX_POS) -1U)

#define LED_BOARD_TYPE          BOARD_TYPE_EVB_24LED
/* rgb channels settings*/
#if LED_BOARD_TYPE == BOARD_TYPE_EVB_24LED
  #define LED_NUM    (24U)
  #define PHY_CHANNEL_R    PWM_CHANNEL_0
  #define PHY_CHANNEL_G    PWM_CHANNEL_2
  #define PHY_CHANNEL_B    PWM_CHANNEL_1
#else
  #error invalid LED_BOARD_TYPE definition!
#endif

#if LED_NUM > 24U || LED_NUM < 17U
  #error LED_NUM should be within 17-24!
#endif

/* ************************System config ******************************/
#if AUTO_ADDRESSING_EN == 1U
    #define LIN_MASTER_EN                       (0U)
#else
    #define LIN_MASTER_EN                       (0U)
#endif

#define WATCH_DOG_EN                            (0U)

/* ************************debug config *******************************/
#define ENABLE_FUNCTION_VALIDATION              (0U)


#define UART_MILKY_WAY_EN                       (0U)
#define CODE_DEBUG_EN                           (0U)

#if CODE_DEBUG_EN == 1U
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #define DEBUG_OUT(...) printf(__VA_ARGS__)
#else
  #define DEBUG_OUT(...)      
#endif

#endif
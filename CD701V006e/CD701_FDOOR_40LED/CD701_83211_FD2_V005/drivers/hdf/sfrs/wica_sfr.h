/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file wica_sfr.h
 */

#ifndef WICA_SFR_H__
#define WICA_SFR_H__

#include <stdint.h>


typedef union{
  struct {
    uint8_t  LINSENA                  :  1; /*!< LIN Slave Wakeup Enable */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  TIMERENA                 :  1; /*!< Wakeup Timer Enable */
    uint8_t  LINMENA                  :  1; /*!< LIN Master Wakeup Enable */
    uint8_t  TIMERTAPSEL              :  4; /*!< WakeUp Timer Tap Select */
    uint8_t  LINSIRQENA               :  1; /*!< LIN Slave Wakeup Interrupt Enable */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  TIMERIRQENA              :  1; /*!< Timer Wakeup Interrupt Enable */
    uint8_t  LINMIRQENA               :  1; /*!< LIN Master Wakeup Interrupt Enable */
    uint8_t  LINSIRQCLR               :  1; /*!< clear the wulin_irq */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  TIMERIRQCLR              :  1; /*!< clear the wutimer_irq */
    uint8_t  LINMIRQCLR               :  1; /*!< clear the wulin_irq */
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}WICA_CTRL_t;

typedef union{
  struct {
    uint8_t  LINS                     :  1; /*!< LIN Slave Wakeup Status */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  TIMER                    :  1; /*!< Wakeup Timer Status */
    uint8_t  LINM                     :  1; /*!< LIN Master Wakeup Status */
    uint8_t                           :  4; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint16_t TIMERCNT                 : 16; /*!< Wakeup Timer Counter Value */
  };
  uint32_t WORD;
}WICA_STATUS_t;

/**
 * @brief A structure to represent Special Function Registers for WICA.
 */
typedef struct {
  WICA_CTRL_t   CTRL; /* +0x000 */
  WICA_STATUS_t STATUS; /* +0x004 */
}WICA_SFRS_t;

/**
 * @brief The starting address of WICA SFRS.
 */
#define WICA_REG_CTRL           (*(__IO   WICA_CTRL_t *)(0x50010100))
#define WICA_REG_STATUS         (*(__IO WICA_STATUS_t *)(0x50010104))

#define WICA_SFRS ((__IO WICA_SFRS_t *)0x50010100)

#endif /* end of __WICA_SFR_H__ section */



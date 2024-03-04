/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file wdta_sfr.h
 */

#ifndef WDTA_SFR_H__
#define WDTA_SFR_H__

#include <stdint.h>

typedef union{
  struct {
    uint8_t  WDTA_RUNNING             :  1; /*!< Analog-Watchdog Running status */
    uint8_t  UPDATE                   :  1; /*!< Window Mode Enable */
    uint8_t                           :  6; /*   (reserved) */
    uint8_t  WDTA_TIMEOUT_SEL         :  3; /*!< Timeout select */
    uint8_t  WDTA_WINOPEN_ENA         :  1; /*!< Window Mode Enable */
    uint8_t  WDTA_WINOPEN_SEL         :  2; /*!< Window Mode open select */
    uint8_t  WDTA_WINOPEN_FLAG        :  1; /*!< Window open flag */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  7; /*   (reserved) */
    uint8_t  STOP_LOCK                :  1;
  };
  uint32_t WORD;
}WDTA_CTRL_t;


typedef  union {
  struct {
    struct {
      uint8_t  WDTA                 :  1; /*!< WDTA timeout Interrupt Enable */
      uint8_t  WINOPEN              :  1; /*!< Window Open Interrupt Enable */
      uint8_t                       :  6; /*   (reserved) */
    }ENABLE;
    struct {
      uint8_t  WDTA                 :  1; /*!< WDTA timeout Interrupt Clear */
      uint8_t  WINOPEN              :  1; /*!< Window Open Interrupt Clear */
      uint8_t                       :  6; /*   (reserved) */
    }CLEAR;
    struct {
      uint8_t  WDTA                 :  1; /*!< WDTA timeout Status */
      uint8_t  WINOPEN              :  1; /*!< Window Open Status */
      uint8_t                       :  6; /*   (reserved) */
    }STATUS;
    struct {
      uint8_t  WDTA                 :  1; /*!< WDTA timeout Interrupt */
      uint8_t  WINOPEN              :  1; /*!< Window Open Interrupt */
      uint8_t                       :  6; /*   (reserved) */
    }IRQ;
  };
  uint32_t WORD;
}WDTA_INT_t; /* +0x010 */



/**
 * @brief A structure to represent Special Function Registers for WDTA.
 */
typedef struct {
  WDTA_CTRL_t   CTRL;   /* +0x000 */
  uint32_t      STOP;   /* +0x004 */
  uint32_t      CLEAR;  /* +0x008 */
  uint32_t      CNTVAL; /*<! Counter value +0x00C */
  WDTA_INT_t    INT;
} WDTA_SFRS_t;

/**
 * @brief The starting address of WDTA SFRS.
 */
#define WDTA_REG_CTRL   (*(__IO WDTA_CTRL_t *)(0x50010200))
#define WDTA_REG_STOP   (*(__IO WDTA_STOP_t *)(0x50010204))
#define WDTA_REG_CLEAR  (*(__IO    uint32_t *)(0x50010208))
#define WDTA_REG_CNTVAL (*(__IO    uint32_t *)(0x5001020C))
#define WDTA_REG_INT    (*(__IO  WDTA_INT_t *)(0x50010210))

#define WDTA_SFRS ((__IO WDTA_SFRS_t *)0x50010200)

#endif /* end of __WDTA_SFR_H__ section */



/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file gpio_sfr.h
 */

#ifndef GPIO_SFR_H__
#define GPIO_SFR_H__

#include <stdint.h>

typedef union {
  struct {
    uint8_t  GPAENA                   :  1;
    uint8_t                           :  7; /*   (reserved) */
    uint32_t                          : 24; /*   (reserved) */
  };
  uint32_t WORD;
}GPIO_GPENA_t;

/*
    uint8_t  RE                   :  1; Pin x rising edge enable
    uint8_t  FE                   :  1; Pin x falling edge enable
*/
typedef union {
  struct {
    uint8_t  DIR                  :  1; /*!< Pin x output enable */
    uint8_t  IE                   :  1; /*!< Pin x interrupt enable */
    uint8_t  EDGE                 :  2; /* 00: disable, 01: rising, 10: falling, 11: both */
    uint8_t  CLR                  :  1; /*!< Pin x interrupt clear */
    uint8_t  ACTDET               :  1; /*!< Pin x activity interrupt */
    uint8_t  RISING_STATUS        :  1; /*!<PIN activity rising status */
    uint8_t  FALL_STATUS          :  1; /*!<PIN activity falling status */
  };
  uint8_t BYTE;
}GPIO_Cfg_t;

/**
 * @brief A structure to represent Special Function Registers for GPIO.
 */
typedef struct {

  uint8_t       GPADATA[1024U];          /*<! Port A data +0x000 */
  uint8_t       RESVD0[1024U];            /*<! resevd data + */
  GPIO_GPENA_t  GPENA;
  uint8_t       RESVD1[2044];            /*<! resevd data + */
  GPIO_Cfg_t    GPIO_CFG[8U];            /* only 0-7 valid */
} GPIO_SFRS_t;

/**
 * @brief The starting address of GPIO SFRS.
 */
#define GPIO_REG_GPENA          (*(__IO  GPIO_GPENA_t *)(0x50018800))
#define GPIO_REG_GPIO_CFG(x)    (*(__IO    GPIO_Cfg_t *)(0x50019000 + x))

#define GPIO_SFRS ((__IO GPIO_SFRS_t *)0x50018000)

#endif /* end of __GPIO_SFR_H__ section */



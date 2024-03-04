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
    uint8_t  GPBENA                   :  1;
    uint8_t                           :  6; /*   (reserved) */
    uint32_t                           : 24; /*   (reserved) */
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
    uint8_t                       :  2; /*   (reserved) */
  };
  uint8_t BYTE;
}GPIO_Cfg_t;

typedef union {
  struct {
    uint8_t  GPADIR0                  :  1; /*!< Pin 0 output enable */
    uint8_t  GPAIE0                   :  1; /*!< Pin 0 interrupt enable */
    uint8_t  GPARE0                   :  1; /*!< Pin 0 rising edge enable */
    uint8_t  GPAFE0                   :  1; /*!< Pin 0 falling edge enable */
    uint8_t  GPACLR0                  :  1; /*!< Pin 0 interrupt clear */
    uint8_t  GPAACTDET0               :  1; /*!< Pin 0 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  GPADIR1                  :  1; /*!< Pin 1 output enable */
    uint8_t  GPAIE1                   :  1; /*!< Pin 1 interrupt enable */
    uint8_t  GPARE1                   :  1; /*!< Pin 1 rising edge enable */
    uint8_t  GPAFE1                   :  1; /*!< Pin 1 falling edge enable */
    uint8_t  GPACLR1                  :  1; /*!< Pin 1 interrupt clear */
    uint8_t  GPAACTDET1               :  1; /*!< Pin 1 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  GPADIR2                  :  1; /*!< Pin 2 output enable */
    uint8_t  GPAIE2                   :  1; /*!< Pin 2 interrupt enable */
    uint8_t  GPARE2                   :  1; /*!< Pin 2 rising edge enable */
    uint8_t  GPAFE2                   :  1; /*!< Pin 2 falling edge enable */
    uint8_t  GPACLR2                  :  1; /*!< Pin 2 interrupt clear */
    uint8_t  GPAACTDET2               :  1; /*!< Pin 2 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  GPADIR3                  :  1; /*!< Pin 3 output enable */
    uint8_t  GPAIE3                   :  1; /*!< Pin 3 interrupt enable */
    uint8_t  GPARE3                   :  1; /*!< Pin 3 rising edge enable */
    uint8_t  GPAFE3                   :  1; /*!< Pin 3 falling edge enable */
    uint8_t  GPACLR3                  :  1; /*!< Pin 3 interrupt clear */
    uint8_t  GPAACTDET3               :  1; /*!< Pin 3 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
  };
  uint32_t WORD;
}GPIO_GPAP03_t;


typedef union {
  struct {
    uint8_t  GPADIR4                  :  1; /*!< Pin 4 output enable */
    uint8_t  GPAIE4                   :  1; /*!< Pin 4 interrupt enable */
    uint8_t  GPARE4                   :  1; /*!< Pin 4 rising edge enable */
    uint8_t  GPAFE4                   :  1; /*!< Pin 4 falling edge enable */
    uint8_t  GPACLR4                  :  1; /*!< Pin 4 interrupt clear */
    uint8_t  GPAACTDET4               :  1; /*!< Pin 4 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  GPADIR5                  :  1; /*!< Pin 5 output enable */
    uint8_t  GPAIE5                   :  1; /*!< Pin 5 interrupt enable */
    uint8_t  GPARE5                   :  1; /*!< Pin 5 rising edge enable */
    uint8_t  GPAFE5                   :  1; /*!< Pin 5 falling edge enable */
    uint8_t  GPACLR5                  :  1; /*!< Pin 5 interrupt clear */
    uint8_t  GPAACTDET5               :  1; /*!< Pin 5 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  GPADIR6                  :  1; /*!< Pin 6 output enable */
    uint8_t  GPAIE6                   :  1; /*!< Pin 6 interrupt enable */
    uint8_t  GPARE6                   :  1; /*!< Pin 6 rising edge enable */
    uint8_t  GPAFE6                   :  1; /*!< Pin 6 falling edge enable */
    uint8_t  GPACLR6                  :  1; /*!< Pin 6 interrupt clear */
    uint8_t  GPAACTDET6               :  1; /*!< Pin 6 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  GPADIR7                  :  1; /*!< Pin 7 output enable */
    uint8_t  GPAIE7                   :  1; /*!< Pin 7 interrupt enable */
    uint8_t  GPARE7                   :  1; /*!< Pin 7 rising edge enable */
    uint8_t  GPAFE7                   :  1; /*!< Pin 7 falling edge enable */
    uint8_t  GPACLR7                  :  1; /*!< Pin 7 interrupt clear */
    uint8_t  GPAACTDET7               :  1; /*!< Pin 7 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
  };
  uint32_t WORD;
}GPIO_GPAP47_t;

typedef union {
  struct {
    uint8_t  GPBDIR0                  :  1; /*!< Pin 0 output enable */
    uint8_t  GPBIE0                   :  1; /*!< Pin 0 interrupt enable */
    uint8_t  GPBRE0                   :  1; /*!< Pin 0 rising edge enable */
    uint8_t  GPBFE0                   :  1; /*!< Pin 0 falling edge enable */
    uint8_t  GPBCLR0                  :  1; /*!< Pin 0 interrupt clear */
    uint8_t  GPBACTDET0               :  1; /*!< Pin 0 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  GPBDIR1                  :  1; /*!< Pin 1 output enable */
    uint8_t  GPBIE1                   :  1; /*!< Pin 1 interrupt enable */
    uint8_t  GPBRE1                   :  1; /*!< Pin 1 rising edge enable */
    uint8_t  GPBFE1                   :  1; /*!< Pin 1 falling edge enable */
    uint8_t  GPBCLR1                  :  1; /*!< Pin 1 interrupt clear */
    uint8_t  GPBACTDET1               :  1; /*!< Pin 1 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  GPBDIR2                  :  1; /*!< Pin 2 output enable */
    uint8_t  GPBIE2                   :  1; /*!< Pin 2 interrupt enable */
    uint8_t  GPBRE2                   :  1; /*!< Pin 2 rising edge enable */
    uint8_t  GPBFE2                   :  1; /*!< Pin 2 falling edge enable */
    uint8_t  GPBCLR2                  :  1; /*!< Pin 2 interrupt clear */
    uint8_t  GPBACTDET2               :  1; /*!< Pin 2 activity interrupt */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}GPIO_GPBP02_t;

/**
 * @brief A structure to represent Special Function Registers for GPIO.
 */
typedef struct {

  uint8_t       GPADATA[1024U];          /*<! Port A data +0x000 */
  uint8_t       GPBDATA[1024U];          /*<! Port B data +0x400 */
  GPIO_GPENA_t  GPENA;                
  GPIO_Cfg_t    GPIO_CFG[12U];           /* only 0-8 valid */
} GPIO_SFRS_t;

/**
 * @brief The starting address of GPIO SFRS.
 */
#define GPIO_REG_GPENA          (*(__IO  GPIO_GPENA_t *)(0x50014800))
#define GPIO_REG_GPIO_CFG(x)    (*(__IO    GPIO_Cfg_t *)(0x50014804 + x))

#define GPIO_SFRS ((__IO GPIO_SFRS_t *)0x50014000)

#endif /* end of __GPIO_SFR_H__ section */



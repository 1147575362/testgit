/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file pwm_sfr.h
 */

#ifndef PWM_SFR_H__
#define PWM_SFR_H__

#include <stdint.h>

typedef union{
  struct {
    uint8_t                           :  8; /*   (reserved) */
    uint8_t  PRESCALESEL              :  8; /*!< Prescaler select */
    uint16_t PERIOD                   : 16;
  };
  uint32_t WORD;
}PWM_BASE_t;

typedef union{
  struct {
    uint32_t ENAREQ                   : 24;
    uint8_t  ENAREQALL                :  1;
    uint8_t  CLRREQALL                :  1;
    uint8_t  FORCEINACTIVE            :  1;
    uint8_t                           :  5; /*   (reserved) */
  };
  uint32_t WORD;
}PWM_ENAREQ_t;

typedef union{
  struct {
    uint16_t PFALL                   : 16; /*!< Pulse Fall */
    uint16_t PRISE                   : 16; /*!< Pulse Rise */
  };
  uint32_t WORD;
}PWM_PULSE_t;

typedef union{
  struct {
    struct {
      uint8_t  PERIOD               :  1; /*!< Period Interrupt enable */
      uint8_t  UPD                  :  1; /*!< Updated Interrupt enable */
      uint8_t                       :  6; /*   (reserved) */
    }ENABLE;
    struct {
      uint8_t  PERIOD               :  1; /*!< Period Interrupt clear */
      uint8_t  UPD                  :  1; /*!< Updated Interrupt clear */
      uint8_t                       :  6; /*   (reserved) */
    }CLEAR;
    struct {
      uint8_t  PERIOD               :  1; /*!< Period Interrupt status */
      uint8_t  UPD                  :  1; /*!< Updated Interrupt status */
      uint8_t                       :  6; /*   (reserved) */
    }STATUS;
    struct {
      uint8_t  PERIOD               :  1; /*!< Period Interrupt active */
      uint8_t  UPD                  :  1; /*!< Updated Interrupt active */
      uint8_t                       :  6; /*   (reserved) */
    }IRQ;
  };
  uint32_t WORD;
}PWM_INTPWM_t;

/**
 * @brief A structure to represent Special Function Registers for PWM.
 */
typedef struct {

  PWM_BASE_t BASE;                            /* +0x000 */
  uint16_t PWMCNT;                            /* +0x004 */
  uint8_t  _RESERVED_06[2];                   /* +0x006 */
  PWM_ENAREQ_t ENAREQ;                        /* +0x008 */

  uint32_t ENASTS;                            /* +0x00C */
  uint32_t INIT;                              /* +0x010 */
  uint32_t INVERT;                            /* +0x014 */
  uint8_t  UPDATE;                            /* +0x018 */
  uint8_t  _RESERVED_19[3];                   /* +0x019 */

  PWM_PULSE_t PULSE[24];

  uint32_t INTPOSEDGENA;                      /*<! Interrupt enable +0x07C */
  uint32_t INTNEGEDGENA;                      /*<! Interrupt enable +0x080 */
  uint32_t INTPOSEDGCLR;                      /*<! Interrupt clear +0x084 */
  uint32_t INTNEGEDGCLR;                      /*<! Interrupt clear +0x088 */
  uint32_t INTPOSEDGSTS;                      /*<! Interrupt status +0x08C */
  uint32_t INTNEGEDGSTS;                      /*<! Interrupt status +0x090 */
  uint32_t INTPOSEDGIRQ;                      /*<! Interrupt active +0x094 */
  uint32_t INTNEGEDGIRQ;                      /*<! Interrupt active +0x098 */
  PWM_INTPWM_t INTPWM;                        /* +0x09C */

} PWM_SFRS_t;


/**
 * @brief The starting address of PWM SFRS.
 */

#define PWM_REG_BASE                    (*(__IO   PWM_BASE_t *)(0x50011000))
#define PWM_REG_PWMCNT                  (*(__IO     uint16_t *)(0x50011004))
#define PWM_REG_ENAREQ                  (*(__IO PWM_ENAREQ_t *)(0x50011008))
#define PWM_REG_ENASTS                  (*(__IO     uint32_t *)(0x5001100C))
#define PWM_REG_INIT                    (*(__IO     uint32_t *)(0x50011010))
#define PWM_REG_INVERT                  (*(__IO     uint32_t *)(0x50011014))
#define PWM_REG_UPDATE                  (*(__IO      uint8_t *)(0x50011018))
#define PWM_REG_PULSE(index)            (*(__IO  PWM_PULSE_t *)(0x5001101C + index*4)) /* index: 0-23 */
#define PWM_REG_INTPOSEDGENA            (*(__IO     uint32_t *)(0x5001107C))
#define PWM_REG_INTNEGEDGENA            (*(__IO     uint32_t *)(0x50011080))
#define PWM_REG_INTPOSEDGCLR            (*(__IO     uint32_t *)(0x50011084))
#define PWM_REG_INTNEGEDGCLR            (*(__IO     uint32_t *)(0x50011088))
#define PWM_REG_INTPOSEDGSTS            (*(__IO     uint32_t *)(0x5001108C))
#define PWM_REG_INTNEGEDGSTS            (*(__IO     uint32_t *)(0x50011090))
#define PWM_REG_INTPOSEDGIRQ            (*(__IO     uint32_t *)(0x50011094))
#define PWM_REG_INTNEGEDGIRQ            (*(__IO     uint32_t *)(0x50011098))
#define PWM_REG_INTPWM                  (*(__IO PWM_INTPWM_t *)(0x5001109C))

#define PWM_SFRS ((__IO PWM_SFRS_t *)0x50011000)

#endif /* end of __PWM_SFR_H__ section */



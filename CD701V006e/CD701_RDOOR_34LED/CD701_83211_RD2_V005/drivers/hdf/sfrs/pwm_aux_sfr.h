/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file pwm_aux_sfr.h
 */

#ifndef PWM_AUX_SFR_H__
#define PWM_AUX_SFR_H__

#include <stdint.h>

typedef union{
  struct {
    uint8_t                           :  8; /*   (reserved) */
    uint8_t  PRESCALESEL              :  8; /*!< Prescaler select */
    uint16_t  PERIOD                   : 16; /*!< Period */
  };
  uint32_t WORD;
}PWM_AUX_BASE_t;

typedef struct{
  uint8_t               :8;      /*   (reserved) */
  uint8_t  PRESCALESEL;          /*!< Prescaler select */
  uint16_t PERIOD;               /*!< Period */
  uint16_t PWMCNT;
  uint16_t              : 16;    /*!< reserved */
}PWM_AUX_Timer_t;


typedef union{
  struct {
    uint8_t  ENAREQ                   :  8;
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t  ENAREQALL                :  1;
    uint8_t  CLRREQALL                :  1;
    uint8_t  FORCEINACTIVE            :  1;
    uint8_t                           :  5; /*   (reserved) */
  };
  uint32_t WORD;
}PWM_AUX_ENAREQ_t;

typedef union {
  struct {
    uint16_t PFALL                   : 16; /*!< Pulse Fall */
    uint16_t PRISE                   : 16; /*!< Pulse Rise */
  };
  uint32_t WORD;
}PULSE_t; /* +0x028 */


typedef union{
  struct {
    struct {
      uint8_t  PERIOD; /*!< Period Interrupt enable */
    } ENABLE;
    struct {
      uint8_t  PERIOD; /*!< Period Interrupt clear */
    } CLEAR;
    struct {
      uint8_t  PERIOD; /*!< Period Interrupt status */
    } STATUS;
    struct {
      uint8_t  PERIOD; /*!< Period Interrupt active */
    } IRQ;
  };
  uint32_t WORD;
}PWM_AUX_INTPERIOD_t;

typedef union{
  struct {
    struct {
        uint8_t  UPD; /*!< Updated Interrupt enable */
    }ENABLE;
    struct {
        uint8_t  UPD; /*!< Updated Interrupt clear */
    }CLEAR;
    struct {
        uint8_t  UPD; /*!< Updated Interrupt status */
    } STATUS;
    struct {
        uint8_t  UPD; /*!< Updated Interrupt active */
    }IRQ;
  };
  uint32_t WORD;
}PWM_AUX_INTUPDATED_t;
/**
 * @brief A structure to represent Special Function Registers for PWM_AUX.
 */
typedef struct {

  PWM_AUX_Timer_t       TIMER[2];
  uint8_t               BASE_SEL;                          /* +0x010 */
  uint8_t               _RESERVED_11[3];                   /* +0x011 */

  PWM_AUX_ENAREQ_t      ENAREQ; /* +0x014 */
  uint8_t  ENASTS;                            /* +0x018 */
  uint8_t  _RESERVED_19[3];                   /* +0x019 */

  uint8_t  INIT;                              /* +0x01C */
  uint8_t  _RESERVED_1D[3];                   /* +0x01D */

  uint8_t  INVERT;                            /* +0x020 */
  uint8_t  _RESERVED_21[3];                   /* +0x021 */

  uint8_t  UPDATE;                            /* +0x024 */
  uint8_t  _RESERVED_25[3];                   /* +0x025 */

  PULSE_t  PULSE[5];

  uint8_t  INTPOSEDGENA;                      /*<! Interrupt enable +0x03C */
  uint8_t  _RESERVED_3D[3];                   /* +0x03D */

  uint8_t  INTNEGEDGENA;                      /*<! Interrupt enable +0x040 */
  uint8_t  _RESERVED_41[3];                   /* +0x041 */

  uint8_t  INTPOSEDGCLR;                      /*<! Interrupt clear +0x044 */
  uint8_t  _RESERVED_45[3];                   /* +0x045 */

  uint8_t  INTNEGEDGCLR;                      /*<! Interrupt clear +0x048 */
  uint8_t  _RESERVED_49[3];                   /* +0x049 */

  uint8_t  INTPOSEDGSTS;                      /*<! Interrupt status +0x04C */
  uint8_t  _RESERVED_4D[3];                   /* +0x04D */

  uint8_t  INTNEGEDGSTS;                      /*<! Interrupt status +0x050 */
  uint8_t  _RESERVED_51[3];                   /* +0x051 */

  uint8_t  INTPOSEDGIRQ;                      /*<! Interrupt active +0x054 */
  uint8_t  _RESERVED_55[3];                   /* +0x055 */

  uint8_t  INTNEGEDGIRQ;                      /*<! Interrupt active +0x058 */
  uint8_t  _RESERVED_59[3];                   /* +0x059 */

  PWM_AUX_INTPERIOD_t  INTPERIOD; /* +0x05C */
  PWM_AUX_INTUPDATED_t INTUPDATED; /* +0x060 */

} PWM_AUX_SFRS_t;

/**
 * @brief The starting address of PWM_AUX SFRS.
 */

#define PWM_AUX_REG_BASE0               (*(__IO       PWM_AUX_BASE_t *)(0x50010900))
#define PWM_AUX_REG_PWMCNT0             (*(__IO             uint16_t *)(0x50010904))
#define PWM_AUX_REG_BASE1               (*(__IO       PWM_AUX_BASE_t *)(0x50010908))
#define PWM_AUX_REG_PWMCNT1             (*(__IO             uint16_t *)(0x5001090C))
#define PWM_AUX_REG_BASE_SEL            (*(__IO              uint8_t *)(0x50010910))
#define PWM_AUX_REG_ENAREQ              (*(__IO     PWM_AUX_ENAREQ_t *)(0x50010914))
#define PWM_AUX_REG_ENASTS              (*(__IO              uint8_t *)(0x50010918))
#define PWM_AUX_REG_INIT                (*(__IO              uint8_t *)(0x5001091C))
#define PWM_AUX_REG_INVERT              (*(__IO              uint8_t *)(0x50010920))
#define PWM_AUX_REG_UPDATE              (*(__IO              uint8_t *)(0x50010924))
#define PWM_AUX_REG_PULSE(x)            (*(__IO              PULSE_t *)(0x50010928+x*4))
#define PWM_AUX_REG_INTPOSEDGENA        (*(__IO              uint8_t *)(0x5001093C))
#define PWM_AUX_REG_INTNEGEDGENA        (*(__IO              uint8_t *)(0x50010940))
#define PWM_AUX_REG_INTPOSEDGCLR        (*(__IO              uint8_t *)(0x50010944))
#define PWM_AUX_REG_INTNEGEDGCLR        (*(__IO              uint8_t *)(0x50010948))
#define PWM_AUX_REG_INTPOSEDGSTS        (*(__IO              uint8_t *)(0x5001094C))
#define PWM_AUX_REG_INTNEGEDGSTS        (*(__IO              uint8_t *)(0x50010950))
#define PWM_AUX_REG_INTPOSEDGIRQ        (*(__IO              uint8_t *)(0x50010954))
#define PWM_AUX_REG_INTNEGEDGIRQ        (*(__IO              uint8_t *)(0x50010958))
#define PWM_AUX_REG_INTPERIOD           (*(__IO  PWM_AUX_INTPERIOD_t *)(0x5001095C))
#define PWM_AUX_REG_INTUPDATED          (*(__IO PWM_AUX_INTUPDATED_t *)(0x50010960))

#define PWM_AUX_SFRS ((__IO PWM_AUX_SFRS_t *)0x50010900)

#endif /* end of __PWM_AUX_SFR_H__ section */



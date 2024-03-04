/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file timer0_sfr.h
 */

#ifndef TIMER0_SFR_H__
#define TIMER0_SFR_H__

#include <stdint.h>


/**
 * @brief A structure to represent Special Function Registers for TIMER0.
 */
typedef struct {
  uint32_t COUNT;                             /* +0x000 */
  uint8_t  ENA;                               /*<! Enable +0x004 */
  uint8_t  _RESERVED_05[3];                   /* +0x005 */
} TIMER0_SFRS_t;


/**
 * @brief The starting address of TIMER0 SFRS.
 */

#define TIMER0_REG_COUNT (*(__IO uint32_t *)(0x50020000))
#define TIMER0_REG_ENA   (*(__IO  uint8_t *)(0x50020004))


#define TIMER0_SFRS ((__IO TIMER0_SFRS_t *)0x50020000)

#endif /* end of __TIMER0_SFR_H__ section */



/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file timer2_sfr.h
 */

#ifndef TIMER2_SFR_H__
#define TIMER2_SFR_H__

#include <stdint.h>

/**
 * @brief A structure to represent Special Function Registers for TIMER2.
 */
typedef struct {
  uint32_t COUNT;                             /* +0x000 */
  uint8_t  ENA;                               /*<! Enable +0x004 */
  uint8_t  _RESERVED_05[3];                   /* +0x005 */
} TIMER2_SFRS_t;


/**
 * @brief The starting address of TIMER2 SFRS.
 */
#define TIMER2_REG_COUNT        (*(__IO uint32_t *)(0x50020010))
#define TIMER2_REG_ENA          (*(__IO  uint8_t *)(0x50020014))


#define TIMER2_SFRS ((__IO TIMER2_SFRS_t *)0x50020010)

#endif /* end of __TIMER2_SFR_H__ section */



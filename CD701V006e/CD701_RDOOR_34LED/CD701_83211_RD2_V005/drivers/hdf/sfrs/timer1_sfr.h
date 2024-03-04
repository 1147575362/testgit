/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file timer1_sfr.h
 */

#ifndef TIMER1_SFR_H__
#define TIMER1_SFR_H__

#include <stdint.h>


/**
 * @brief A structure to represent Special Function Registers for TIMER1.
 */
typedef struct {
  uint32_t COUNT;                             /* +0x000 */
  uint8_t  ENA;                               /*<! Enable +0x004 */
  uint8_t  _RESERVED_05[3];                   /* +0x005 */
} TIMER1_SFRS_t;

/**
 * @brief The starting address of TIMER1 SFRS.
 */
#define TIMER1_REG_COUNT        (*(__IO uint32_t *)(0x50020008))
#define TIMER1_REG_ENA          (*(__IO  uint8_t *)(0x5002000C))

#define TIMER1_SFRS ((__IO TIMER1_SFRS_t *)0x50020008)

#endif /* end of __TIMER1_SFR_H__ section */



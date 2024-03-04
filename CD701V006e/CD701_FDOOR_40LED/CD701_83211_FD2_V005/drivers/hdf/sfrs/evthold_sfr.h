/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file evthold_sfr.h
 */

#ifndef EVTHOLD_SFR_H__
#define EVTHOLD_SFR_H__

#include <stdint.h>

/**
 * @brief A structure to represent Special Function Registers for EVTHOLD.
 */
typedef struct {
  uint8_t  HOLD;                              /* +0x000 */
  uint8_t  _RESERVED_01[3];                   /* +0x001 */
} EVTHOLD_SFRS_t;

/**
 * @brief The starting address of EVTHOLD SFRS.
 */
#define EVTHOLD_REG_HOLD  (*(__IO uint8_t *)(0x50000060))


#define EVTHOLD_SFRS ((__IO EVTHOLD_SFRS_t *)0x50000060)

#endif /* end of __EVTHOLD_SFR_H__ section */



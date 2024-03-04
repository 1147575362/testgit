/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file wdt1_sfr.h
 */

#ifndef WDT1_SFR_H__
#define WDT1_SFR_H__

#include <stdint.h>


typedef union{
  struct {
    uint8_t  ENA                      :  1; /*!< WDT Enable */
    uint8_t  RSTEN                    :  1; /*!< Reset enable */
    uint8_t  RSTFLAG                  :  1; /*!< Reset flag */
    uint8_t  PRESET                   :  2;
    uint8_t                           :  3; /*   (reserved) */
    uint32_t                           : 24; /*   (reserved) */
  };
  uint32_t WORD;
}WDT1_CFG_t;
/**
 * @brief A structure to represent Special Function Registers for WDT1.
 */
typedef struct {
  WDT1_CFG_t CFG; /* +0x000 */
  uint32_t   KEY;                               /* +0x004 */
} WDT1_SFRS_t;

/**
 * @brief The starting address of WDT1 SFRS.
 */

#define WDT1_REG_CFG (*(__IO WDT1_CFG_t *)(0x50020018))
#define WDT1_REG_KEY (*(__IO   uint32_t *)(0x5002001C))



#define WDT1_SFRS ((__IO WDT1_SFRS_t *)0x50020018)

#endif /* end of __WDT1_SFR_H__ section */



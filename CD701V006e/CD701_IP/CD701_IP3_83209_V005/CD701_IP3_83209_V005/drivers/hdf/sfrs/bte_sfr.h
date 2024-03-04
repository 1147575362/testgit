/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file bte_sfr.h
 */

#ifndef BTE_SFR_H__
#define BTE_SFR_H__

#include <stdint.h>

typedef union{
  struct {
    uint16_t  BXADD                   : 16; /*!< Address of the ASIC die (LSB) */
    uint8_t  BXNUM                    :  8; /*!< Number of 32-bit words to transfer */
    uint8_t  INC_ADDR                 :  1;
    uint8_t  TX_DIR                   :  1;
    uint8_t  BLOCKING                 :  1;
    uint8_t  START                    :  1;
    uint8_t                           :  4; /*   (reserved) */
  };
  uint32_t WORD;
}BTE_CTRL_t;


/**
 * @brief A structure to represent Special Function Registers for BTE.
 */
typedef struct {
  BTE_CTRL_t  CTRL;
  uint16_t BXSRAMADDR;                        /*<! Address of the SRAM (LSB) +0x004 */
  uint8_t  _RESERVED_06[2];                   /* +0x006 */
} BTE_SFRS_t;

/**
 * @brief The starting address of BTE SFRS.
 */
#define BTE_REG_CTRL            (*(__IO BTE_CTRL_t *)(0x50000080))
#define BTE_REG_BXSRAMADDR      (*(__IO   uint16_t *)(0x50000084))

#define BTE_SFRS ((__IO BTE_SFRS_t *)0x50000080)

#endif /* end of __BTE_SFR_H__ section */



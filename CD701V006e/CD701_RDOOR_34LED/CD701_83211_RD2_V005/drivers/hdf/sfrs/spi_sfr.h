/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file spi_sfr.h
 */

#ifndef SPI_SFR_H__
#define SPI_SFR_H__

#include <stdint.h>

typedef union{
  struct {
    uint8_t  ENA_REQ                  :  1; /*!< SPI enable request */
    uint8_t  ENA_STS                  :  1; /*!< SPI enable status */
    uint8_t  SPR                      :  2; /*!< Standard clock divider selection */
    uint8_t  CPHA                     :  1; /*!< SPI clock phase */
    uint8_t  CPOL                     :  1; /*!< SPI clock polarity (Motorola SPI Frame Format) */
    uint8_t  SCKEXT                   :  2; /*!< Extended clock divider */
    uint8_t  ICNT                     :  8; /*!< Interrupt counter bits, keep being default 0 */
    uint8_t  LPBK                     :  8; /*!< Port wired-OR mode bit */
    uint8_t  FIFOSOFTRESET            :  8; /*!< FIFO Soft Reset */
  };
  uint32_t WORD;
}SPI_SPICTRL_t;

typedef union{
  struct {
    uint8_t  XFERCNT                  :  1; /*!< Completed Transfer Count */
    uint8_t  RXFIFOUF                 :  1; /*!< Reception FIFO underflow */
    uint8_t  TXFIFOOF                 :  1; /*!< Transmit FIFO overflow */
    uint8_t  RXFIFOEMPTY              :  1; /*!< Reception FIFO empty */
    uint8_t  RXFIFOFULL               :  1; /*!< Reception FIFO full */
    uint8_t  TXFIFOEMPTY              :  1; /*!< Transmit FIFO empty */
    uint8_t  TXFIFOFULL               :  1; /*!< Transmit FIFO full */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  RXFIFOCOUNT              :  8; /*!< Reception FIFO Count */
    uint8_t  TXFIFOCOUNT              :  8; /*!< Transmit FIFO Count */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}SPI_SPISTATUS_t;

typedef union{
  struct {
    uint8_t  INTXFERCNT               :  1; /*!< SPI Transfer Count Interrupt */
    uint8_t  INTRXFIFOUF              :  1; /*!< Reception FIFO Underflow Interrupt */
    uint8_t  INTTXFIFOOF              :  1; /*!< Transmit FIFO Overflow Interrupt */
    uint8_t                           :  5; /*   (reserved) */
    uint32_t                          :  24; /*   (reserved) */
  };
  uint32_t WORD;
}SPI_SPIINTSTATUS_t;

typedef union{
  struct {
    uint8_t  INTXFERCNTENA            :  1; /*!< SPI Transfer Count Interrupt Enable */
    uint8_t  INTRXFIFOUFENA           :  1; /*!< Reception FIFO Underflow Interrupt Enable */
    uint8_t  INTTXFIFOOFENA           :  1; /*!< Transmit FIFO Overflow Interrupt Enable */
    uint8_t                           :  5; /*   (reserved) */
    uint32_t                          : 24; /*   (reserved) */
  };
  uint32_t WORD;
}SPI_SPIINTENABLE_t;

typedef union{
  struct {
    uint8_t  INTXFERCNTCLR            :  1; /*!< SPI Transfer Count Interrupt */
    uint8_t  INTRXFIFOUFCLR           :  1; /*!< Reception FIFO Underflow Interrupt */
    uint8_t  INTTXFIFOOFCLR           :  1; /*!< Transmit FIFO Overflow Interrupt */
    uint8_t                           :  5; /*   (reserved) */
    uint32_t                          : 24; /*   (reserved) */
  };
  uint32_t WORD;
}SPI_SPIINTCLEAR_t;
/**
 * @brief A structure to represent Special Function Registers for SPI.
 */
typedef struct {
  uint8_t  RXDATA;                              /*<! Rx Data buffer +0x000 */
  uint8_t  _RESERVED_01[3];                     /* +0x001 */

  uint8_t  TXDATA;                              /*<! Tx Data buffer +0x004 */
  uint8_t  _RESERVED_05[3];                     /* +0x005 */

  SPI_SPICTRL_t         CTRL;                   /* +0x008 */
  SPI_SPISTATUS_t       STATUS;                 /* +0x00C */
  SPI_SPIINTSTATUS_t    INTSTATUS;              /* +0x010 */
  SPI_SPIINTENABLE_t    INTENABLE;              /* +0x014 */
  SPI_SPIINTCLEAR_t     INTCLEAR;               /* +0x018 */

} SPI_SFRS_t;

/**
 * @brief The starting address of SPI SFRS.
 */

#define SPI_REG_RXDATA                  (*(__IO            uint8_t *)(0x50010700))
#define SPI_REG_TXDATA                  (*(__IO            uint8_t *)(0x50010704))
#define SPI_REG_CTRL                    (*(__IO      SPI_SPICTRL_t *)(0x50010708))
#define SPI_REG_STATUS                  (*(__IO    SPI_SPISTATUS_t *)(0x5001070C))
#define SPI_REG_INTSTATUS               (*(__IO SPI_SPIINTSTATUS_t *)(0x50010710))
#define SPI_REG_INTENABLE               (*(__IO SPI_SPIINTENABLE_t *)(0x50010714))
#define SPI_REG_INTCLEAR                (*(__IO  SPI_SPIINTCLEAR_t *)(0x50010718))


#define SPI_SFRS ((__IO SPI_SFRS_t *)0x50010700)

#endif /* end of __SPI_SFR_H__ section */



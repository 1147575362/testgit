/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file uart_sfr.h
 */

#ifndef UART_SFR_H__
#define UART_SFR_H__

#include <stdint.h>


typedef union {
  struct {
    uint8_t BYTE                               :  8; /*!< Data */
    uint8_t BYTE_BREAK                         :  1; /*!< it is only available for Transmitter, and it indicates this data used for sending BREAK or Not */
    uint8_t DATARECEIVESTATUS_FRAMEERROR       :  1; /*!< Data Receive Status */
    uint8_t DATARECEIVESTATUS_PARITYERROR      :  1; /*!< Data Receive Status */
    uint8_t DATARECEIVESTATUS_BREAKERROR       :  1; /*!< Data Receive Status */
    uint8_t                                    :  4;
  };
  uint16_t HALFWORD;
}UART_DATA_t; /* +0x000 */



typedef union{
    struct {
      uint8_t  FRAMEERROR               :  1; /*!< Framing Error */
      uint8_t  PARITYERROR              :  1; /*!< Parity Error */
      uint8_t  BREAK                    :  1; /*!< is Break signal */
      uint8_t                           :  5; /*   (reserved) */
      uint32_t                           : 24; /*   (reserved) */
    };
    uint32_t WORD;
}UART_RECEIVESTATUS_t;

/*
    uint8_t  PARENA                   :  1; Parity enable 
    uint8_t  PARODD                   :  1; 
*/

typedef union{
  struct {
    uint8_t  ENABLE                   :  1;
    uint8_t  ENABLE_STS               :  1; /*!< Enable status */
    uint8_t  UFIFOSOFTRESET           :  1; /*!< FIFO SOFT RESET */
    uint8_t  RXXFERCNTCLR             :  1; /*!< RX Transfer Counter Clear */
    uint8_t  TXXFERCNTCLR             :  1; /*!< TX Transfer Counter Clear */
    uint8_t  ENA_ADDR_MATCH           :  1;
    uint8_t  MAX_BYTES_RXD            :  2;
    uint8_t  SIZE                     :  2; /*!< Transmission word size */
    uint8_t  STOP                     :  1; /*!< Stop bit control */
    uint8_t  PARITY                   :  2; /*!< Parity: 00: NONE, 01: EVEN, 11: ODD */
    uint8_t  STICKENA                 :  1; /*!< Sticky parity enable */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t                           :  7; /*   (reserved) */
    uint8_t  LOOPENA                  :  1; /*!< Loopback enable */
    uint8_t  ADDR_MATCH               :  8; /*!< Match Address */
  };
  uint32_t WORD;
}UART_MSGCTRL_t;




typedef union{
  struct {
    union {
      struct {
        uint8_t  RXDONE               :  1; /*!< Rx Data ready Interrupt Enable */
        uint8_t  OVRUNERR             :  1; /*!< Overrun error Interrupt Enable */
        uint8_t  FRMERR               :  1; /*!< Framing error Interrupt Enable */
        uint8_t  PRTYERR              :  1; /*!< Parity Error Interrupt Enable */
        uint8_t  BREAKKERR            :  1; /*!< Break Error Interrupt Enable */
        uint8_t  TXDONE               :  1; /*!< Transmission done Interrupt Enable */
        uint8_t  RXMULTDONE           :  1; /*!< Multiple Receive Transactions Done Interrupt Enable */
        uint8_t  TXMULTDONE           :  1; /*!< Multiple Transmit Transactions Done Interrupt Enable */
      };
      uint8_t BYTE;
    }ENABLE;
    union {
      struct {
        uint8_t  RXDONE               :  1; /*!< Rx Data ready Interrupt Clear */
        uint8_t  OVRUNERR             :  1; /*!< Overrun error Interrupt Clear */
        uint8_t  FRMERR               :  1; /*!< Framing error Interrupt Clear */
        uint8_t  PRTYERR              :  1; /*!< Parity Error Interrupt Clear */
        uint8_t  BREAKKERR            :  1; /*!< Break Error Interrupt  Clear */
        uint8_t  TXDONE               :  1; /*!< Transmission done Interrupt Clear */
        uint8_t  RXMULTDONE           :  1; /*!< Multiple Receive Transactions Done Interrupt Clear */
        uint8_t  TXMULTDONE           :  1; /*!< Multiple Transmit Transactions Done Interrupt Clear */
      };
      uint8_t BYTE;
    }CLEAR;
    union {
      struct {
        uint8_t  RXDONE               :  1; /*!< Rx Data ready */
        uint8_t  OVRUNERR             :  1; /*!< Overrun error */
        uint8_t  FRMERR               :  1; /*!< Framing error */
        uint8_t  PRTYERR              :  1; /*!< Parity Error */
        uint8_t  BREAKKERR            :  1; /*!< Break IRQ */
        uint8_t  TXDONE               :  1; /*!< Transmission is done */
        uint8_t  RXMULTDONE           :  1; /*!< Multiple Receive Transactions Done */
        uint8_t  TXMULTDONE           :  1; /*!< Multiple Transmit Transactions Done */
      };
      uint8_t BYTE;
    }STATUS;
    union {
      struct {
        uint8_t  RXDONE               :  1; /*!< Rx Data ready Interrupt */
        uint8_t  OVRUNERR             :  1; /*!< Overrun error Interrupt */
        uint8_t  FRMERR               :  1; /*!< Framing error Interrupt */
        uint8_t  PRTYERR              :  1; /*!< Parity Error Interrupt */
        uint8_t  BREAKKERR            :  1; /*!< Break Error Interrupt */
        uint8_t  TXDONE               :  1; /*!< Transmission done Interrupt */
        uint8_t  RXMULTDONE           :  1; /*!< Multiple Receive Transactions Done Interrupt */
        uint8_t  TXMULTDONE           :  1; /*!< Multiple Transmit Transactions Done Interrupt */
      };
      uint8_t BYTE;
    }IRQ;
  };
  uint32_t WORD;
}UART_UARTINT_t;

typedef union{
  struct {
    struct {
      uint8_t  RXTOUT; /*!< Rx Time-out Interrupt Enable */
    }ENABLE;
    struct {
      uint8_t  RXTOUT; /*!< Rx Time-out Interrupt Clear */
    }CLEAR;
    struct {
      uint8_t  RXTOUT; /*!< Rx Time-out */
    }STATUS;
    struct {
      uint8_t  RXTOUT; /*!< Rx Time-out Interrupt */
    }IRQ;
  };
  uint32_t WORD;
}UART_UARTINT2_t;



typedef union{
  struct {
    uint16_t BAUDDIV                  : 16; /*!< Baud rate divider */
    uint8_t  OSR                      :  8; /*!< Over-sampling ratio */
    uint8_t  FDIV                     :  3; /*!< Fractional divider */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  UADVANCE                 :  1; /*!< Advance Register */
    uint8_t  URETARD                  :  1; /*!< Retard Register */
    uint8_t                           :  2; /*   (reserved) */
  };
  uint32_t WORD;
}UART_UARTBAUD_t;

typedef union{
  struct {
    uint8_t  RXUF                     :  1; /*!< Reception FIFO underflow */
    uint8_t  RXEMPTY                  :  1; /*!< Reception FIFO empty */
    uint8_t  RXFULL                   :  1; /*!< Reception FIFO empty */
    uint8_t                           :  5; /*   (reserved) */
    uint8_t  RXCOUNT                  :  8; /*!< Reception FIFO current count */
    uint8_t  TXOF                     :  1; /*!< Transmit FIFO overflow */
    uint8_t  TXEMPTY                  :  1; /*!< Transmit FIFO empty */
    uint8_t  TXFULL                   :  1; /*!< Transmit FIFO full */
    uint8_t                           :  5; /*   (reserved) */
    uint8_t  TXCOUNT                  :  8; /*!< Transmit FIFO current count */
  };
  uint32_t WORD;
}UART_UARTFIFOSTATUS_t;

typedef union{
  struct {
    uint8_t  RXMULTIPLEXFERDONECNT    :  8; /*!< Receive Data Count Interrupt */
    uint8_t  TXMULTIPLEXFERDONECNT    :  8; /*!< Transmit Data Count Interrupt */
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}UART_FIFOLEVELCTL_t;
/**
 * @brief A structure to represent Special Function Registers for UART.
 */
typedef struct {
  UART_DATA_t           DATA;           /*<! Data +0x000 */
  uint16_t              RESVED0;
  UART_RECEIVESTATUS_t  RECEIVESTATUS;  /* +0x004 */
  UART_MSGCTRL_t        MSGCTRL;        /* +0x008 */
  UART_UARTINT_t        INT;            /* +0x00C */
  UART_UARTINT2_t       INT2;           /* +0x010 */
  UART_UARTBAUD_t       BAUD;           /* +0x014 */
  UART_UARTFIFOSTATUS_t FIFOSTATUS;     /* +0x018 */
  UART_FIFOLEVELCTL_t   FIFOLEVELCTL;   /* +0x01C */
} UART_SFRS_t;

/**
 * @brief The starting address of UART SFRS.
 */
#define UART_REG_BREAK_DATA     (*(__IO              uint16_t *)(0x50010800))
#define UART_REG_DATA           (*(__IO               uint8_t *)(0x50010800))
#define UART_REG_RECEIVESTATUS  (*(__IO  UART_RECEIVESTATUS_t *)(0x50010804))
#define UART_REG_MSGCTRL        (*(__IO        UART_MSGCTRL_t *)(0x50010808))
#define UART_REG_INT            (*(__IO        UART_UARTINT_t *)(0x5001080C))
#define UART_REG_INT2           (*(__IO       UART_UARTINT2_t *)(0x50010810))
#define UART_REG_BAUD           (*(__IO       UART_UARTBAUD_t *)(0x50010814))
#define UART_REG_FIFOSTATUS     (*(__IO UART_UARTFIFOSTATUS_t *)(0x50010818))
#define UART_REG_FIFOLEVELCTL   (*(__IO   UART_FIFOLEVELCTL_t *)(0x5001081C))

#define UART_SFRS ((__IO UART_SFRS_t *)0x50010800)

#endif /* end of __UART_SFR_H__ section */



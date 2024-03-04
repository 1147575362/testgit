/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file ioctrla_sfr.h
 */

#ifndef IOCTRLA_SFR_H__
#define IOCTRLA_SFR_H__

#include <stdint.h>
/*
    uint8_t  PUENA                    :  1; pullup enable (active-low)
    uint8_t  PDENA                    :  1; pulldown enable
*/
typedef union{
  struct {
    uint8_t  HWMODE                   :  3; /*!< hardware mode */
    uint8_t  PULLMODE                 :  2; /* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t  MUXSEL                   :  8;
  };
  uint32_t WORD;
}IOCTRLA_GPIO_t;


typedef union {
  struct {
    uint8_t  HWMODE                   :  3; /*!< hardware mode */
    uint8_t  PUENA                    :  1; /*!< pullup enable (active-low) */
    uint8_t  PDENA                    :  1; /*!< pulldown enable */
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  UART_TXD_IDLE_DIR        :  8; /*!< Control the direction of PAD for UART TXD Idle */
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}IOCTRLA_GPIO5_t; /* +0x010 */


typedef union {
  struct {
    uint8_t  HWMODE                   :  3; /*!< hardware mode */
    uint8_t  PUENA                    :  1; /*!< pullup enable (active-low) */
    uint8_t  PDENA                    :  1; /*!< pulldown enable */
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t  UART_RXO_SW              :  1; /*!< UART RX output Switch */
    uint8_t                           :  1; /*   (reserved) */
    uint32_t                          : 24; /*   (reserved) */
  };
  uint32_t WORD;
}IOCTRLA_GPIO6_t; /* +0x014 */


typedef union{
  struct {
    uint8_t  LINS_HWMODE              :  1; /*!< LIN Slave hardware mode */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  LINS_PU30K_ENA           :  1; /*!< LIN 30K pullup enable */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  LINS_TXENA               :  1; /*!< LIN transmit enable */
    uint8_t  LINS_RXENA               :  1; /*!< LIN receive  enable */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  LINM_HWMODE              :  1; /*!< LIN Master hardware mode */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  LINM_PU30K_ENA           :  1; /*!< LIN 30K  pullup enable */
    uint8_t  LINM_PU1K_ENA            :  1; /*!< LIN 1K  pullup enable */
    uint8_t  LINM_TXENA               :  1; /*!< LIN transmit enable */
    uint8_t  LINM_RXENA               :  1; /*!< LIN receive  enable */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  SWON                     :  1; /*!< LIN Switch On */
    uint8_t  SWOFF_TIMEOUT            :  1; /*!< Switch off in dominant TimeOut condition */
    uint8_t  LINS_PUOFF_TIMEOUT       :  1; /*!< LINS Pullup Disable in dominant TimeOut condition */
    uint8_t  LINM_PUOFF_TIMEOUT       :  1; /*!< LINM 1K Pullup disable in dominant TimeOut condition */
    uint8_t                           :  4; /*   (reserved) */
    uint8_t  SWON_LOCK                :  1; /*!< SWON Lock Bit */
    uint8_t                           :  6; /*   (reserved) */
    uint8_t  PMODE                    :  1; /*!< LIN Power Mode */
  };
  uint32_t WORD;
}IOCTRLA_LIN_t;


typedef union {
  struct {
    uint8_t  LINSRXGF1ST_ENA          :  1; /*!< LINS RXD Glitch Filter enables */
    uint8_t  LINSRXGF2ND_ENA          :  1; /*!< LINS RXD Glitch Filter enables */
    uint8_t  LINSRXGF3RD_ENA          :  2; /*!< LINS RXD Glitch Filter enables */
    uint8_t                           :  4;
    uint8_t  LINS_DBNC_THRES0_1ST       :  8; /*!< 1st Stage LINS Debounce Threshold for 1 to 0 */
    uint8_t  LINS_DBNC_THRES1_1ST       :  8; /*!< 1st Stage LINS Debounce Threshold for 0 to 1 */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}LINSGFCONF_t; /* +0x028 */


typedef union {
  struct {
    uint8_t  LINS_DBNC_THRES0_2ND     :  8; /*!< 2nd Stage LINS Debounce Threshold for 1 to 0 */
    uint8_t  LINS_DBNC_THRES1_2ND     :  8; /*!< 2nd Stage LINS Debounce Threshold for 0 to 1 */
    uint8_t  LINS_DBNC_THRES0_3RD     :  8; /*!< 3rd Stage LINS Debounce Threshold for 1 to 0 */
    uint8_t  LINS_DBNC_THRES1_3RD     :  8; /*!< 3rd Stage LINS Debounce Threshold for 0 to 1 */
  };
  uint32_t WORD;
}LINSGFCONF1_t; /* +0x02C */


typedef union {
  struct {
    uint8_t  LINMRXGF1ST_ENA          :  1; /*!< LINS RXD Glitch Filter enables */
    uint8_t  LINMRXGF2ND_ENA          :  1; /*!< LINS RXD Glitch Filter enables */
    uint8_t  LINMRXGF3RD_ENA          :  2; /*!< LINS RXD Glitch Filter enables */
    uint8_t                           :  4;
    uint8_t  LINM1ST_DBNCTHRES0       :  8; /*!< LINM Debounce Threshold for 1 to 0 */
    uint8_t  LINM1ST_DBNCTHRES1       :  8; /*!< LINM Debounce Threshold for 0 to 1 */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}LINMGFCONF_t; /* +0x030 */


typedef union {
  struct {
    uint8_t  LINM_DBNC_THRES0_2ND     :  8; /*!< 2nd Stage LINM Debounce Threshold for 1 to 0 */
    uint8_t  LINM_DBNC_THRES1_2ND     :  8; /*!< 2nd Stage LINM Debounce Threshold for 0 to 1 */
    uint8_t  LINM_DBNC_THRES0_3RD     :  8; /*!< 3rd Stage LINM Debounce Threshold for 1 to 0 */
    uint8_t  LINM_DBNC_THRES1_3RD     :  8; /*!< 3rd Stage LINM Debounce Threshold for 0 to 1 */
  };
  uint32_t WORD;
}LINMGFCONF1_t; /* +0x034 */


typedef union {
  struct {
    uint8_t  LINSTXDMONITORENA        :  1; /*!< LINS TxD Monitor enable */
    uint8_t  LINMTXDMONITORENA        :  1; /*!< LINM TxD Monitor enable */
    uint8_t                           :  6; /*   (reserved) */
    uint8_t  LINSTXDTIMEOUTDOMINANT   :  1; /*!< Tx Dominant Timeout */
    uint8_t  LINMTXDTIMEOUTDOMINANT   :  1; /*!< Tx Dominant Timeout */
    uint8_t                           :  6; /*   (reserved) */
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}LINTXDMONITOR_t; /* +0x038 */


typedef union{
  struct {
    uint8_t  HWMODE                   :  8; /*!< LED hardware mode */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t  SENSE_ENA                :  1; /*!< LED Forward Voltage Sense Enable */
    uint8_t  SENSE_CTRL               :  5; /*!< LED */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  VFW_ENA                  :  1; /*!< LED Forward Voltage Current Enable */
  };
  uint32_t WORD;
}IOCTRLA_LEDPIN_t;


typedef union{
  struct {
    uint8_t  ADCCONSEL                :  1; /*!< Hardware/Firmware Select */
    uint8_t  GPIOCONSEL               :  1; /*!< Hardware/Firmware Select */
    uint8_t  ADCSELSEL                :  1; /*!< Hardware/Firmware Select */
    uint8_t                           :  5; /*   (reserved) */
    uint8_t  ADCCONREG                :  8; /*!< Firmware Debug Value */
    uint16_t                           : 16; /*   (reserved) */
  };
  uint32_t WORD;
}IOCTRLA_ANALOGTESTMUXOVERRIDE1_t;

typedef union{
  struct {
    uint16_t GPIOCONREG               : 16; /*!< Firmware Debug Value */
    uint16_t ADCSELREG                : 16; /*!< Firmware Debug Value */
  };
  uint32_t WORD;
}IOCTRLA_ANALOGTESTMUXOVERRIDE2_t;


typedef union {
  struct {
    struct {
      uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt enable */
      uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt enable */
      uint8_t                       :  6; /*   (reserved) */
    }ENABLE;
    struct {
      uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt clear */
      uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt clear */
      uint8_t                       :  6; /*   (reserved) */
    }CLEAR;
    struct {
      uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt status */
      uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt status */
      uint8_t                       :  6; /*   (reserved) */
    }STATUS;
    struct {
      uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt active */
      uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt active */
      uint8_t                       :  6; /*   (reserved) */
    }IRQ;
  };
  uint32_t WORD;
}IOCTRLA_IRQLINTXDOM_t; /* +0x050 */



/**
 * @brief A structure to represent Special Function Registers for IOCTRLA.
 */
typedef struct {

  IOCTRLA_GPIO_t        GPIO[9]; /* +0x000 */
  IOCTRLA_LIN_t         LIN; /* +0x024 */
  LINSGFCONF_t          LINSGFCONF;
  LINSGFCONF1_t         LINSGFCONF1;
  LINMGFCONF_t          LINMGFCONF;
  LINMGFCONF1_t         LINMGFCONF1;
  LINTXDMONITOR_t       LINTXDMONITOR;
  IOCTRLA_LEDPIN_t      LEDPIN; /* +0x028 */
  uint32_t              LEDDATA;                           /*<! LED Data Out +0x02C */
  uint32_t              LEDPUENA;                          /*<! LED Pullup Enable +0x030 */
  IOCTRLA_ANALOGTESTMUXOVERRIDE1_t ANALOGTESTMUXOVERRIDE1; /* +0x034 */
  IOCTRLA_ANALOGTESTMUXOVERRIDE2_t ANALOGTESTMUXOVERRIDE2; /* +0x038 */
  IOCTRLA_IRQLINTXDOM_t IRQLINTXDOM;        
} IOCTRLA_SFRS_t;

/**
 * @brief The starting address of IOCTRLA SFRS.
 */
#define IOCTRLA_REG_GPIO(index)                 (*(__IO                   IOCTRLA_GPIO_t *)(0x50012000+ index*4)) /* index = 0-8 */
#define IOCTRLA_REG_LIN                         (*(__IO                    IOCTRLA_LIN_t *)(0x50012024))
#define IOCTRLA_REG_LINSGFCONF                  (*(__IO                     LINSGFCONF_t *)(0x50012028))
#define IOCTRLA_REG_LINSGFCONF1                 (*(__IO                    LINSGFCONF1_t *)(0x5001202C))
#define IOCTRLA_REG_LINMGFCONF                  (*(__IO                     LINMGFCONF_t *)(0x50012030))
#define IOCTRLA_REG_LINMGFCONF1                 (*(__IO                    LINMGFCONF1_t *)(0x50012034))
#define IOCTRLA_REG_LINTXDMONITOR               (*(__IO                  LINTXDMONITOR_t *)(0x50012038))

#define IOCTRLA_REG_LEDPIN                      (*(__IO                 IOCTRLA_LEDPIN_t *)(0x5001203C)) 
#define IOCTRLA_REG_LEDDATA                     (*(__IO                         uint32_t *)(0x50012040)) 
#define IOCTRLA_REG_LEDPUENA                    (*(__IO                         uint32_t *)(0x50012044))
#define IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1      (*(__IO IOCTRLA_ANALOGTESTMUXOVERRIDE1_t *)(0x50012048))
#define IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2      (*(__IO IOCTRLA_ANALOGTESTMUXOVERRIDE2_t *)(0x5001204C))
#define IOCTRLA_REG_IRQLINTXDOM                 (*(__IO            IOCTRLA_IRQLINTXDOM_t *)(0x50012050))

#define IOCTRLA_SFRS ((__IO IOCTRLA_SFRS_t *)0x50012000)

#endif /* end of __IOCTRLA_SFR_H__ section */



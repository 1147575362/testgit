/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file linm_sfr.h
 */

#ifndef LINM_SFR_H__
#define LINM_SFR_H__

#include <stdint.h>


typedef union{
  struct{
    uint8_t  DATA;                           /*<! Data Buffer 1 +0x000 */
    uint8_t  _RESERVED[3];                   /* +0x001 */
  };
  uint32_t WORD;
}LINM_DATABUF_t;


/*
    uint8_t  RSTERR                   :  1; Reset Error
    uint8_t  RSTINT                   :  1; Reset interrupt
*/

typedef union{
  struct {
    uint8_t  STARTREQ                 :  1; /*!< Start Request */
    uint8_t  WAKEUPREQ                :  1; /*!< WakeUp Request */
    uint8_t  RST_ERR_INT              :  2; /*!< Reset Error,Reset interrupt  */
    uint8_t  DATAACK                  :  1; /*!< Data Acknowledgement */
    uint8_t  TRANSMIT                 :  1; /*!< Transmit Operation */
    uint8_t  SLEEP                    :  1; /*!< Sleep Request */
    uint8_t  STOP                     :  1; /*!< Stop Register */
  };
  uint8_t BYTE;
}LINM_CTRL_t;

typedef union{
  struct {
    uint8_t  COMPLETE                 :  1;
    uint8_t  WAKEUP                   :  1;
    uint8_t  ERROR                    :  1; /*!< Lin Error */
    uint8_t  INTR                     :  1; /*!< Interupt Request */
    uint8_t  DATAREQ                  :  1; /*!< Data Request */
    uint8_t  ABORTED                  :  1;
    uint8_t  BUSIDLETIMEOUT           :  1; /*!< BUS Idle Timeout */
    uint8_t  ACTIVE                   :  1; /*!< Lin Bus Active */
  };
  uint8_t BYTE;
}LINM_STATUS_t;

typedef union{
  struct {
    uint8_t  BITERR                   :  1; /*!< Bit Error in Byte field */
    uint8_t  CHK                      :  1; /*!< Checksum Error */
    uint8_t  TIMEOUT                  :  1; /*!< Timeout Error */
    uint8_t  PARITY                   :  1; /*!< Parity Error */
    uint8_t  BITMON                   :  1; /*!< Bit Monitor Error */
    uint8_t  SBITERR                  :  1; /*!< Start Bit Error in Byte field */
    uint8_t  FRAMEERR                 :  1; /*!< Byte Field Framing Error */
    uint8_t                           :  1; /*   (reserved) */
  };
  uint8_t BYTE;
}LINM_ERROR_t;

typedef union{
  struct {
    uint8_t  LENGTH                   :  4; /*!< Data Length */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  DISAUTOSLEEP             :  1; /*!< Disable Auto Sleep */
    uint8_t  DISBITMON                :  1; /*!< Disable Bit Monitor */
    uint8_t  ENHCHK                   :  1; /*!< Enhancement Check */
  };
  uint8_t BYTE;
}LINM_DL_t;


typedef union{
  struct {
    uint8_t  BTDIV8                   :  1; /*!< Bt Div Most Significant bit */
    uint8_t  BTMULT                   :  5; /*!< Bt Div Most Significant bit */
    uint8_t  PRESCL                   :  2; /*!< Prescaler Register */
  };
  uint8_t BYTE;
}LINM_BITTIME_t;


typedef union{
  struct {
    uint8_t  WUPREPEAT                :  2; /*!< wakeup repeat time */
    uint8_t  BUSINACTIVE              :  2; /*!< Bus Inactivity Time */
    uint8_t  BUSDOMINANTRELEASEWUPENA :  1; /*!< Bus Dominant Release Wakeup Enable */
    uint8_t                           :  3; /*(reserved) */
  };
  uint8_t BYTE;
}LINM_BUSTIME_t;


typedef struct {
    uint8_t  COMPLETE                   :  1; /*!<Complete TX */
    uint8_t  BUSIDLETIMEOUTDOMINANT     :  1; /*!< Dominant Bus Idle Timeout */
    uint8_t  BUSIDLEMONITOR             :  2; /*!< Bus Idle Monitor Status */
    uint8_t                             :  4; /*!< (reserved) */
}LINM_TX_STATUS_t;
/**
 * @brief A structure to represent Special Function Registers for LINM.
 */
typedef struct {

  LINM_DATABUF_t DATABUFF[8];
  LINM_CTRL_t    CTRL;                  /* +0x020 */
  uint8_t        _RESERVED_20[3]; 
  LINM_STATUS_t  STATUS;                /* +0x024 */
  uint8_t        _RESERVED_24[3]; 
  LINM_ERROR_t   ERROR;                 /* +0x028 */
  uint8_t        _RESERVED_28[3]; 
  LINM_DL_t      DL;                    /* +0x02C */
  uint8_t        _RESERVED_2C[3]; 
  uint8_t        BTDIV07;               /*<! Bt Div LSBs +0x030 */
  uint8_t        _RESERVED_30[3];       
  LINM_BITTIME_t BITTIME;               /* +0x034 */
  uint8_t        _RESERVED_34[3]; 
  uint8_t        ID;                    /* +0x038 */
  uint8_t       _RESERVED_38[3];        /* +0x039 */
  LINM_BUSTIME_t BUSTIME;               /* +0x03C */
  uint8_t        _RESERVED_3C[3];
  LINM_TX_STATUS_t TX_STATUS;          /*<! Complete TX +0x040 */
  uint8_t          _RESERVED_40[3];    /* +0x041 */
  uint8_t       WUPDETECTTHRES;        /*<! Wakeup Detection Threshold +0x044 */
  uint8_t       _RESERVED_45[3];       /* +0x045 */
} LINM_SFRS_t;

/**
 * @brief The starting address of LINM SFRS.
 */
#define LINM_REG_DATA_LSB_WORD          (*(__IO        uint32_t *)(0x50010400))
#define LINM_REG_DATA_MSB_WORD          (*(__IO        uint32_t *)(0x50010404))

#define LINM_REG_DATA(index)            (*(__IO         uint8_t *)(0x50010400 + index *4U))
#define LINM_REG_CTRL                   (*(__IO     LINM_CTRL_t *)(0x50010420))
#define LINM_REG_STATUS                 (*(__IO   LINM_STATUS_t *)(0x50010424))
#define LINM_REG_ERROR                  (*(__IO    LINM_ERROR_t *)(0x50010428))
#define LINM_REG_DL                     (*(__IO       LINM_DL_t *)(0x5001042C))
#define LINM_REG_BTDIV07                (*(__IO         uint8_t *)(0x50010430))
#define LINM_REG_BITTIME                (*(__IO  LINM_BITTIME_t *)(0x50010434))
#define LINM_REG_ID                     (*(__IO         uint8_t *)(0x50010438))
#define LINM_REG_BUSTIME                (*(__IO  LINM_BUSTIME_t *)(0x5001043C))
#define LINM_REG_TX_STATUS              (*(__IO LINM_TX_STATUS_t *)(0x50010440))
#define LINM_REG_TX_WUPDETECTTHRES      (*(__IO         uint8_t *)(0x50010444))

#define LINM_SFRS ((__IO LINM_SFRS_t *)0x50010400)

#endif /* end of __LINM_SFR_H__ section */



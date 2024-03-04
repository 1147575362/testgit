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

/*  Only for Realpum C1 and later */
typedef union{
  struct {
    uint8_t  HWMODE                   :  2; /*!< hardware mode */
    uint8_t  LINM_SEL                  :  1; /* GPIO1: for LINM Connection Select;  GPIO3,GPIO5 for LINS Connection Select, others for reserved  */
    uint8_t  PULLMODE                 :  2; /* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t  PWM_SEL                  :  2; /*!< PWM output selection for PWM hardware mode */
    uint8_t  GPIO1_MUXSEL             :  8; /*   (reserved) */
    uint8_t                           :  7; /*   (reserved) */
    uint8_t  GPIO2_MUXSEL             :  1; /*  Selects debug signal to be output on gpio2. */
    uint8_t  MUXSEL                   :  8;
  };
  uint32_t WORD;
}IOCTRLA_GPIO1_t;

typedef union{
  struct {
    uint8_t  HWMODE_LSB               :  2; /*!< hardware mode */
    uint8_t  HWMODE_MSB               :  1; /* MSB of GPIO2_HW_MODE. */
    uint8_t  PULLMODE                 :  2; /* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t  PWM_SEL                  :  2; /*!< PWM output selection for PWM hardware mode */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8;
  };
  uint32_t WORD;
}IOCTRLA_GPIO2_t;

typedef union{
  struct {
    uint8_t  HWMODE_LSB               :  2; /*!< hardware mode */
    uint8_t  LINS_SEL                 :  1; /* MSB of GPIO3_HW_MODE. */
    uint8_t  PULLMODE                 :  2; /* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t  PWM_SEL                  :  2; /*!< PWM output selection for PWM hardware mode */
    uint8_t  HWMODE_MSB               :  1; /* MSB of GPIO3_HW_MODE.*/
    uint8_t                           :  7; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8;
  };
  uint32_t WORD;
}IOCTRLA_GPIO3_t;

typedef union{
  struct {
    uint8_t  HWMODE_LSB               :  2; /*!< hardware mode */
    uint8_t  HWMODE_MSB               :  1; /* MSB of GPIO4_HW_MODE. */
    uint8_t  PULLMODE                 :  2; /* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t  PWM_SEL                  :  2; /*!< PWM output selection for PWM hardware mode */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8;
  };
  uint32_t WORD;
}IOCTRLA_GPIO4_t;


typedef union{
  struct {
    uint8_t  HWMODE_LSB               :  2; /*!< hardware mode */
    uint8_t  LINS_SEL                 :  1; /* MSB of GPIO3_HW_MODE. */
    uint8_t  PULLMODE                 :  2; /* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t  PWM_SEL                  :  2; /*!< PWM output selection for PWM hardware mode */
    uint8_t  HWMODE_MSB               :  1; /* MSB of GPIO3_HW_MODE.*/
    uint8_t                           :  7; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8;
  };
  uint32_t WORD;
}IOCTRLA_GPIO5_t;


typedef union{
  struct {
    uint8_t  HWMODE_LSB               :  2; /*!< hardware mode */
    uint8_t  HWMODE_MSB               :  1; /* MSB of GPIO4_HW_MODE. */
    uint8_t  PULLMODE                 :  2; /* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t  PWM_SEL                  :  2; /*!< PWM output selection for PWM hardware mode */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8;
  };
  uint32_t WORD;
}IOCTRLA_GPIO6_t;

/*  end of nly for Realpum C1 and later */

/*
    uint8_t  PUENA                    :  1; pullup enable (active-low)
    uint8_t  PDENA                    :  1; pulldown enable
*/
typedef union{
  struct {
    uint8_t  HWMODE                   :  2; /*!< hardware mode */
    uint8_t  LIN_SEL                  :  1; /* GPIO1: for LINM Connection Select;  GPIO3,GPIO5 for LINS Connection Select, others for reserved  */
    uint8_t  PULLMODE                 :  2; /* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t  PWM_SEL                  :  2; /*!< PWM output selection for PWM hardware mode */
    uint8_t  RESV                     :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t  MUXSEL                   :  8;
  };
  uint32_t WORD;
}IOCTRLA_GPIO_t;

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
    uint8_t  SWON                     :  1; /*!< LIN Dual Mode Switch On */
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


typedef union{
  struct {
    uint8_t  LINSRXGF3RD_ENA          :  2; /*!< LINS RXD Glitch Filter 3rd enables */
    uint8_t  LINSRXGF1ST_ENA          :  1; /*!< LINS RXD Glitch Filter 1st enables  only available for Version C1 and later*/
    uint8_t  LINSRXGF2ND_ENA          :  1; /*!< LINS RXD Glitch Filter 2nd enables  only available for Version C1 and later*/
    uint8_t                           :  4; /*   (reserved) */
    uint8_t  LINS3RD_DBNCTHRES0       :  8; /*!< LINS Debounce Threshold for 1 to 0 */
    uint8_t  LINS3RD_DBNCTHRES1       :  8; /*!< LINS Debounce Threshold for 0 to 1 */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}IOCTRLA_LINSGFCONF_t;

typedef union{
  struct {
    uint8_t  LINMRXGF3RD_ENA          :  2; /*!< LINS RXD Glitch Filter 3rd enables */
    uint8_t  LINMRXGF1ST_ENA          :  1; /*!< LINS RXD Glitch Filter 1st enables  only available for Version C1 and later*/
    uint8_t  LINMRXGF2ND_ENA          :  1; /*!< LINS RXD Glitch Filter 2nd enables  only available for Version C1 and later*/
    uint8_t                           :  4; /*   (reserved) */
    uint8_t  LINM3RD_DBNCTHRES0       :  8; /*!< LINS Debounce Threshold for 0 to 1 */
    uint8_t  LINM3RD_DBNCTHRES1       :  8; /*!< LINS Debounce Threshold for 1 to 0 */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}IOCTRLA_LINMGFCONF_t;


typedef union{
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
}IOCTRLA_LINTXDMONITOR_t;



typedef union{
  struct {
    uint8_t  HWMODE                   :  3; /*!< LED hardware mode */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  DATA                     :  3; /*!< LED Data Out */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  SENSE_ENA                :  1; /*!< LED Forward Voltage Sense Enable */
    uint8_t  SENSE_CTRL               :  5; /*!< LED */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  VFW_ENA                  :  1; /*!< LED Forward Voltage Current Enable */
    uint8_t  GAIN_SEL                 :  8; /*!< LED Sense AFE gain select */
    uint8_t                           :  8; /*   (reserved) */
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
      uint8_t  GPIOCONREG               :  8; /*!< Firmware Debug Value */
      uint8_t  ADCSELREG                :  8; /*!< Firmware Debug Value */
    };
    uint32_t WORD;
}IOCTRLA_ANALOGTESTMUXOVERRIDE_t;



typedef union{
  struct {
    union {
      struct {
        uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt enable */
        uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt enable */
        uint8_t                       :  6; /*   (reserved) */
      };
      uint8_t BYTE;
    } ENABLE;
    union {
      struct {
        uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt clear */
        uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt clear */
        uint8_t                       :  6; /*   (reserved) */
      };
      uint8_t BYTE;
    } CLEAR;
    union {
      struct {
        uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt status */
        uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt status */
        uint8_t                       :  6; /*   (reserved) */
      };
      uint8_t BYTE;
    } STATUS;
    union {
      struct {
        uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt active */
        uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt active */
        uint8_t                       :  6; /*   (reserved) */
      };
      uint8_t BYTE;
    } IRQ;
  };
  uint32_t WORD;
}IOCTRLA_LINDMIRQ_t;


typedef union{
  struct {
    uint8_t  LINS1ST_DBNCTHRES0       :  8; /*!< LINS Debounce Threshold for 1 to 0 only available for Version C1 and later*/
    uint8_t  LINS1ST_DBNCTHRES1       :  8; /*!< LINS Debounce Threshold for 0 to 1 only available for Version C1 and later*/
    uint8_t  LINS2ND_DBNCTHRES0       :  8; /*!< LINS Debounce Threshold for 1 to 0 only available for Version C1 and later*/
    uint8_t  LINS2ND_DBNCTHRES1       :  8; /*!< LINS Debounce Threshold for 0 to 1 only available for Version C1 and later*/
  };
  uint32_t WORD;
}IOCTRLA_LINSGFCONF1_t;


typedef union{
  struct {
    uint8_t  LINM1ST_DBNCTHRES0       :  8; /*!< LINM Debounce Threshold for 1 to 0 only available for Version C1 and later*/
    uint8_t  LINM1ST_DBNCTHRES1       :  8; /*!< LINM Debounce Threshold for 0 to 1 only available for Version C1 and later*/
    uint8_t  LINM2ND_DBNCTHRES0       :  8; /*!< LINM Debounce Threshold for 1 to 0 only available for Version C1 and later*/
    uint8_t  LINM2ND_DBNCTHRES1       :  8; /*!< LINM Debounce Threshold for 0 to 1 only available for Version C1 and later*/
  };
  uint32_t WORD;
}IOCTRLA_LINMGFCONF1_t;

typedef union{
  struct {
    uint8_t  resvd[3];
    uint8_t                  : 7;
    uint8_t  FILTER_UNLOCK   : 1; /*!< Set Only bit. Write 1 to this bit to unlock FILT_CODE bits. only available for Version C1 and later*/
  };
  uint32_t WORD;
}IOCTRLA_FILT_ACCESS_t;
/**
 * @brief A structure to represent Special Function Registers for IOCTRLA.
 */
typedef struct {
  IOCTRLA_GPIO_t                  GPIO[6]; /* +0x000 */
  IOCTRLA_LIN_t                   LIN; /* +0x024 */
  IOCTRLA_LINSGFCONF_t            LINSGFCONF;
  IOCTRLA_LINMGFCONF_t            LINMGFCONF;
  IOCTRLA_LINTXDMONITOR_t         LINTXDMONITOR;
  IOCTRLA_LEDPIN_t                LEDPIN; /* +0x028 */
  IOCTRLA_ANALOGTESTMUXOVERRIDE_t ANALOGTESTMUXOVERRIDE; /* +0x034 */
  IOCTRLA_LINDMIRQ_t              LINDMIRQ;
  IOCTRLA_LINSGFCONF1_t           LINSGFCONF1; /*!< only available for Version C1 and later*/
  IOCTRLA_LINMGFCONF1_t           LINMGFCONF1; /*!< only available for Version C1 and later*/
  IOCTRLA_FILT_ACCESS_t           FILT_ACCESS; /*!< only available for Version C1 and later*/
}IOCTRLA_SFRS_t;


/*  Only for Realpum C1 and later */
#define IOCTRLA_REG_GPIO1                 (*(__IO                  IOCTRLA_GPIO1_t *)(0x50011000))
#define IOCTRLA_REG_GPIO2                 (*(__IO                  IOCTRLA_GPIO2_t *)(0x50011004))
#define IOCTRLA_REG_GPIO3                 (*(__IO                  IOCTRLA_GPIO3_t *)(0x50011008))
#define IOCTRLA_REG_GPIO4                 (*(__IO                  IOCTRLA_GPIO4_t *)(0x5001100C))
#define IOCTRLA_REG_GPIO5                 (*(__IO                  IOCTRLA_GPIO5_t *)(0x50011010))
#define IOCTRLA_REG_GPIO6                 (*(__IO                  IOCTRLA_GPIO6_t *)(0x50011014))
#define IOCTRLA_REG_LINSGFCONF1           (*(__IO                  IOCTRLA_LINSGFCONF1_t *)(0x50011034))
#define IOCTRLA_REG_LINMGFCONF1           (*(__IO                  IOCTRLA_LINMGFCONF1_t *)(0x50011038))
#define IOCTRLA_REG_FILT_ACCESS           (*(__IO                  IOCTRLA_FILT_ACCESS_t *)(0x5001103C))
/*  End of Only for Realpum C1 and later */


/**
 * @brief The starting address of IOCTRLA SFRS.
 */
#define IOCTRLA_REG_GPIO(index)                 (*(__IO                  IOCTRLA_GPIO_t *)(0x50011000+ index*4)) /* index = 0-8 */
#define IOCTRLA_REG_LIN                         (*(__IO                   IOCTRLA_LIN_t *)(0x50011018))
#define IOCTRLA_REG_LINSGFCONF                  (*(__IO            IOCTRLA_LINSGFCONF_t *)(0x5001101C))
#define IOCTRLA_REG_LINMGFCONF                  (*(__IO            IOCTRLA_LINMGFCONF_t *)(0x50011020))
#define IOCTRLA_REG_LINTXDMONITOR               (*(__IO         IOCTRLA_LINTXDMONITOR_t *)(0x50011024))
#define IOCTRLA_REG_LEDPIN                      (*(__IO                IOCTRLA_LEDPIN_t *)(0x50011028)) 
#define IOCTRLA_REG_ANALOGTESTMUXOVERRIDE       (*(__IO IOCTRLA_ANALOGTESTMUXOVERRIDE_t *)(0x5001102C))
#define IOCTRLA_REG_LINDMIRQ                    (*(__IO              IOCTRLA_LINDMIRQ_t *)(0x50011030))

#define IOCTRLA_SFRS ((__IO IOCTRLA_SFRS_t *)0x50011000)

#endif /* end of __IOCTRLA_SFR_H__ section */



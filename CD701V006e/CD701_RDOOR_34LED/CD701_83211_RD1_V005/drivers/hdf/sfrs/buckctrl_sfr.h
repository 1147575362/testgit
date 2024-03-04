/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file buckctrl_sfr.h
 */

#ifndef BUCKCTRL_SFR_H__
#define BUCKCTRL_SFR_H__

#include <stdint.h>

#define BUCKCTRL_STATUS_VBUCK_LOW_NOT_DETECTED          (1 << 0)
#define BUCKCTRL_STATUS_VBUCK_UV_DETECTED               (1 << 1)
#define BUCKCTRL_STATUS_VBUCK_OK_DETECTED               (1 << 2)

typedef union{
  struct {
    uint8_t  ENAREQ                   :  1; /*!< Request to Enable the Buck */
    uint8_t  HICCUPENA                :  1; /*!< Request to Enable Buck HICCUP */
    uint8_t  ENBYPASSBT               :  1; /*!< Enable CBT Charge */
    uint8_t                           :  5; /*   (reserved) */
    uint8_t  ENIBOS2P6U               :  8; /*!< Power Sense Bias Select */
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}BUCKCTRL_CTRL_t;

typedef union{
  struct {
    uint8_t  PERIOD                   :  7; /*!< Buck Clock Period */
    uint8_t  SPREAD                   :  1; /*!< Clock Spreading Enable */
    uint8_t  SSCDEEP                  :  3; /*!< Buck Clock Period Spread Depth Configuration */
    uint8_t                           :  5; /*   (reserved) */
    uint8_t  SSCDIV                   :  8; /*!< Buck SSC Clock Divider */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}BUCKCTRL_CLOCK_t;

typedef union{
  struct {
    uint8_t  BYPASSH                  :  8; /*!< Buck Bypass Signal High time */
    uint8_t  BYPASSL                  :  2; /*!< Buck Bypass signal Low time */
    uint8_t  STARTH                   :  1; /*!< Buck Start signal High time */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  ENAL                     :  2; /*!< time after ENAREQ goes low */
    uint8_t  DEBOUNCEOVERCURSTRBSEL   :  2; /*!< strobing point for debouncing the buck overcur signal */
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}BUCKCTRL_TDELAY_t;


typedef union{
  struct {
    uint8_t  VDACTRIM                 :  8; /*!< TRIM code for VDAC */
    uint8_t                           :  5; /*   (reserved) */
    uint8_t  RAMPRATE                 :  1; /*!< Ramp Rate of vdac_3v3 to reach to set vdac_trim */
    uint8_t                           :  2; /*   (reserved) */
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}BUCKCTRL_VDACCODE_t;


typedef union{
  struct {
    union{
      struct {
      uint8_t  READY                :  1; /*!< Buck Ready interrupt enable */
      uint8_t  DISABLED             :  1; /*!< Buck Disable Complete interrupt enable */
      uint8_t  OVERCUR              :  1; /*!< Buck Overcurrent detected interrupt enable */
      uint8_t  BUCK_UV              :  1; /*!< under voltage interrupt enable */
      uint8_t  BUCK_OV              :  1; /*!< over voltage interrupt enable */
      uint8_t  BUCK_OK              :  1; /*!< voltage ok interrupt enable */
      uint8_t  START_ERROR          :  1; /*!< Buck Startup Error interrupt enable */
      uint8_t                       :  1; /*   (reserved) */
      };
      uint8_t BYTE;
    }ENABLE;
    union{
      struct {
      uint8_t  READY                :  1; /*!< Buck Ready interrupt clear */
      uint8_t  DISABLED             :  1; /*!< Buck Disable Complete interrupt clear */
      uint8_t  OVERCUR              :  1; /*!< Buck Overcurrent detected interrupt clear */
      uint8_t  BUCK_UV              :  1; /*!< under voltage interrupt clear */
      uint8_t  BUCK_OV              :  1; /*!< over voltage interrupt clear */
      uint8_t  BUCK_OK              :  1; /*!< voltage ok interrupt clear */
      uint8_t  START_ERROR          :  1; /*!< Buck startup error interrupt/flags clear */
      uint8_t                       :  1; /*   (reserved) */
      };
      uint8_t BYTE;
    }CLEAR;
    union{
      struct {
      uint8_t  READY                :  1; /*!< Buck Ready interrupt status */
      uint8_t  DISABLED             :  1; /*!< Buck Disable Complete interrupt status */
      uint8_t  OVERCUR              :  1; /*!< Buck Overcurrent detected interrupt status */
      uint8_t  BUCK_UV              :  1; /*!< under voltage interrupt status */
      uint8_t  BUCK_OV              :  1; /*!< over voltage interrupt status */
      uint8_t  BUCK_OK              :  1; /*!< voltage OK interrupt status */
      uint8_t  START_ERROR          :  1; /*!< Startup Error interrupt status */
      uint8_t                       :  1; /*   (reserved) */
      };
      uint8_t BYTE;
    }STATUS;
    union{
      struct {
      uint8_t  READY                :  1; /*!< Buck Ready interrupt active */
      uint8_t  DISABLED             :  1; /*!< Buck Disable Complete interrupt active */
      uint8_t  OVERCUR              :  1; /*!< Buck Overcurrent detected interrupt active */
      uint8_t  BUCK_UV              :  1; /*!< under voltage interrupt active */
      uint8_t  BUCK_OV              :  1; /*!< over voltage interrupt active */
      uint8_t  BUCK_OK              :  1; /*!< voltage ok interrupt active */
      uint8_t  START_ERROR          :  1; /*!< buck startup error errorinterrupt active */
      uint8_t                       :  1; /*   (reserved) */
      };
      uint8_t BYTE;
    }IRQ;
  };
  uint32_t WORD;
}BUCKCTRL_IRQ_t;

typedef union{
  struct {
    uint8_t  BUCKOFF                  :  1; /*!< BUCK off status Register */
    uint8_t  BUCK_ERROR               :  3; /*!< BUCK Startup Error Bits */
    uint8_t                           :  4; /*   (reserved) */
    uint8_t  STATE                    :  8; /*!< Buck Controller State */
    uint16_t                           : 16; /*   (reserved) */
  };
  uint32_t WORD;
}BUCKCTRL_STATUS_t; 

typedef union{
  struct {
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  OV_MONITOR_ENA           :  1; /*!< Buck Over Voltage Monitor Enable */
    uint8_t  OK_MONITOR_ENA           :  1; /*!< Buck OK Voltage Monitor Enable */
    uint8_t                           :  5; /*   (reserved) */
    uint8_t  UVLEVEL_SEL              :  8; /*!< Buck Voltage Monitor Under Voltage Select */
    uint8_t  OVLEVEL_SEL              :  8; /*!< Buck Voltage Monitor Over Voltage Select */
    uint8_t  OKLEVEL_SEL              :  5; /*!< Buck Voltage Monitor Target Voltage Select */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  LOW                      :  1; /*!< Buck Voltage UV Status */
    uint8_t  HIGH                     :  1; /*!< Buck Voltage OV Status */
  };
  uint32_t WORD;
}BUCKCTRL_VBUCKCTRL_t; 

typedef union{
  struct {
    uint8_t  BUCKDBNCENA              :  4; /*!< voltage signal debounce enable */
    uint8_t  LFENA                    :  4; /*!< Low frequency strobing enable for debouncing */
    uint8_t  UVTHRES1                 :  3; /*!< Under Voltage debouncing threshold for 0to1 Transition */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  OVTHRES1                 :  3; /*!< Over Voltage debouncing threshold for 0 to 1 Transition */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  EXTRADBNC                :  8; /*!< Extra debounce enables */
    uint8_t  BUCKSTRBDBNCSEL          :  8; /*!< Strobing select for debouncing */
  };
  uint32_t WORD;
}BUCKCTRL_VBUCKDBNC_t; 

typedef union{
  struct {
    uint8_t  HICCUPDWELL              :  4; /*!< Hiccup Timeout Setting */
    uint8_t  OVDBNCDWELL              :  4; /*!< OV Debounce Timeout Setting */
    uint8_t  UVDBNCDWELL              :  4; /*!< UV Debounce Timeout Setting */
    uint8_t  STARTDBNCDWELL           :  4; /*!< START_L Debounce Timeout Setting */
    uint8_t  STARTLDWELL              :  4; /*!< START_L Timeout Setting */
    uint8_t  DETLOWDWELL              :  4; /*!< DET_LOW Timeout Setting */
    uint8_t  PREENADWELL              :  8; /*!< PRE_ENA Timeout Setting */
  };
  uint32_t WORD;
}BUCKCTRL_VBUCKTIMEOUT_t; 
/**
 * @brief A structure to represent Special Function Registers for BUCKCTRL.
 */
typedef struct {

  BUCKCTRL_CTRL_t               CTRL;           /* +0x000 */
  BUCKCTRL_CLOCK_t              CLOCK;          /* +0x004 */
  BUCKCTRL_TDELAY_t             TDELAY;         /* +0x00C */
  BUCKCTRL_VDACCODE_t           VDACCODE;       /* +0x010 */
  BUCKCTRL_IRQ_t                BUCKIRQ;        /* +0x014 */
  BUCKCTRL_STATUS_t             STATUS;         /* +0x018 */
  BUCKCTRL_VBUCKCTRL_t          VBUCKCTRL;      /* +0x01C */
  BUCKCTRL_VBUCKDBNC_t          VBUCKDBNC;      /* +0x020 */
  BUCKCTRL_VBUCKTIMEOUT_t       VBUCKTIMEOUT;   /* +0x024 */
  uint8_t                       BUCKIGNORE;     /*<! Ignore the control signals for debugging purpose +0x028 */
  uint8_t                       _RESERVED_29[3];/* +0x029 */
} BUCKCTRL_SFRS_t;


/**
 * @brief The starting address of BUCKCTRL SFRS.
 */

#define BUCKCTRL_REG_CTRL          (*(__IO         BUCKCTRL_CTRL_t *)(0x50010500))
#define BUCKCTRL_REG_CLOCK         (*(__IO        BUCKCTRL_CLOCK_t *)(0x50010504))
#define BUCKCTRL_REG_TDELAY        (*(__IO       BUCKCTRL_TDELAY_t *)(0x50010508))
#define BUCKCTRL_REG_VDACCODE      (*(__IO     BUCKCTRL_VDACCODE_t *)(0x5001050C))
#define BUCKCTRL_REG_BUCKIRQ       (*(__IO          BUCKCTRL_IRQ_t *)(0x50010510))
#define BUCKCTRL_REG_STATUS        (*(__IO       BUCKCTRL_STATUS_t *)(0x50010514))
#define BUCKCTRL_REG_VBUCKCTRL     (*(__IO    BUCKCTRL_VBUCKCTRL_t *)(0x50010518))
#define BUCKCTRL_REG_VBUCKDBNC     (*(__IO   BUCKCTRL_VBUCKDBNC_t * )(0x5001051C))
#define BUCKCTRL_REG_VBUCKTIMEOUT  (*(__IO BUCKCTRL_VBUCKTIMEOUT_t *)(0x50010520))
#define BUCKCTRL_REG_BUCKIGNORE    (*(__IO                 uint8_t *)(0x50010524))

#define BUCKCTRL_SFRS ((__IO BUCKCTRL_SFRS_t *)0x50010500)

#endif /* end of __BUCKCTRL_SFR_H__ section */



/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file pmua_sfr.h
 */

#ifndef PMUA_SFR_H__
#define PMUA_SFR_H__

#include <stdint.h>

typedef union{
  struct {
    uint8_t  HIBERNATE                :  1;
    uint8_t  FASTBOOT                 :  1; /*!< Fast boot */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  FASTMCUON                :  1; /*!< Fast MCU Power On */
    uint8_t                           :  4; /*   (reserved) */
    uint32_t                          : 24; /*   (reserved) */
  };
  uint32_t WORD;
}PMUA_CTRL_t;

typedef union{
  struct {
    uint8_t  ENABLE_BIAS              :  4; /*!< Enable bias dwell time */
    uint8_t  ENABLE_1V5               :  4; /*!< Enable 1.5V dwell time */
    uint8_t  ATTACH_1V5               :  4; /*!< Attach 1.5V dwell time */
    uint8_t  ATTACH_3V3               :  4; /*!< Attach 3.3V dwell time */
    uint8_t  POWER_DOWN_MCU           :  4; /*!< Power down MCU dwell time */
    uint8_t  ENABLE_MAIN_REG          :  4; /*!< Enable main regulator dwell time */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}PMUA_DWELL_t;


typedef union{
  struct {
    uint8_t  LOW_MONITOR_ENA          :  1; /*!< Battery Under Voltage Monitor Enable */
    uint8_t  OV_MONITOR_ENA           :  1; /*!< Battery Over Voltage Monitor Enable */
    uint8_t  UV_POL                   :  1; /*!< Battery Voltage Monitor Under Voltage Interrupt Event Polarity */
    uint8_t  OV_POL                   :  1; /*!< Battery Voltage Monitor Over Voltage Interrupt Event Polarity */
    uint8_t                           :  4; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t  LOW                      :  1; /*!< Battery Voltage Low Status */
    uint8_t  HIGH                     :  1; /*!< Battery Voltage High Status */
/*    uint8_t  LOW_DBNC                 :  1; !< Battery Voltage High Status after debouncing */
/*    uint8_t  HIGH_DBNC                :  1; !< Battery Voltage Low Status after debouncing */
    uint8_t  BATT_STATE               :  2;
    uint8_t                           :  4; /*   (reserved) */
  };
  uint32_t WORD;
}PMUA_VBATCTRL_t;


typedef union{
  struct {
    uint8_t  UVLEVEL                  :  8; /*!< Battery Voltage Monitor Under Voltage Select */
    uint8_t  UVHYS                    :  8; /*!< Battery Voltage Monitor Under Voltage Hysterisis Select */
    uint8_t  OVLEVEL                  :  8; /*!< Battery Voltage Monitor Over Voltage Select */
    uint8_t  OVHYS                    :  8; /*!< Battery Voltage Monitor Over Voltage Hysterisis Select */
  };
  uint32_t WORD;
}PMUA_VBATTRIM_t;

typedef union{
  struct {
    uint8_t  UV                       :  1; /*!< under voltage signal debounce enable */
    uint8_t  OV                       :  1; /*!< over voltage signal debounce enable */
    uint8_t  UVSTRB0SEL               :  1; /*!< Low frequency strobing select for debouncing */
    uint8_t  UVSTRB1SEL               :  1; /*!< Low frequency strobing select for debouncing */
    uint8_t  OVSTRB0SEL               :  1; /*!< Low frequency strobing select for debouncing */
    uint8_t  OVSTRB1SEL               :  1; /*!< Low frequency strobing select for debouncing */
    uint8_t                           :  2; /*   (reserved) */
    uint32_t                          : 24; /*   (reserved) */
  };
  uint32_t WORD;
}PMUA_VBATDBNC_t;

typedef union{
  struct {
    uint8_t  UVTHRES1                 :  8; /*!< Under Voltage debouncing threshold for 0to1 Transition */
    uint8_t  OVTHRES1                 :  8; /*!< Over Voltage debouncing threshold for 0to1 Transition */
    uint8_t  UVTHRES0                 :  8; /*!< Under Voltage debouncing threshold for 1to0 Transition */
    uint8_t  OVTHRES0                 :  8; /*!< Over Voltage debouncing threshold for 1to0 Transition */
  };
  uint32_t WORD;
}PMUA_VBATDBNCTHRES_t;




typedef union{
  struct {
    union {
      struct {
        uint8_t  UV                   :  1; /*!< under voltage interrupt enable */
        uint8_t  OV                   :  1; /*!< over voltage interrupt enable */
        uint8_t                       :  6; /*   (reserved) */
      };
      uint8_t BYTE;
    } ENABLE;
    union {
      struct {
        uint8_t  UV                   :  1; /*!< under voltage interrupt clear */
        uint8_t  OV                   :  1; /*!< over voltage interrupt clear */
        uint8_t                       :  6; /*   (reserved) */
      };
      uint8_t BYTE;
    } CLEAR;
    union {
      struct {
        uint8_t  UV                   :  1; /*!< under voltage interrupt status */
        uint8_t  OV                   :  1; /*!< over voltage interrupt status */
        uint8_t                       :  6; /*   (reserved) */
      };
      uint8_t BYTE;
    } STATUS;
    union {
      struct {
        uint8_t  UV                   :  1; /*!< under voltage interrupt active */
        uint8_t  OV                   :  1; /*!< over voltage interrupt active */
        uint8_t                       :  6; /*   (reserved) */
      };
      uint8_t BYTE;
    } IRQ;
  };
  uint32_t WORD;
}PMUA_PMUIRQ_t;
/**
 * @brief A structure to represent Special Function Registers for PMUA.
 */
typedef struct {
  PMUA_CTRL_t           CTRL;                   /* +0x000 */
  uint8_t               IGNORE_CIFS;            /*<! Ignore QACKs +0x004 */
  uint8_t               _RESERVED_05[3];        /* +0x005 */
  PMUA_DWELL_t          DWELL;                  /* +0x008 */
  PMUA_VBATCTRL_t       VBATCTRL;               /* +0x00C */
  PMUA_VBATTRIM_t       VBATTRIM;               /* +0x010 */
  PMUA_VBATDBNC_t       VBATDBNC;               /* +0x014 */
  PMUA_VBATDBNCTHRES_t  VBATDBNCTHRES;
  PMUA_PMUIRQ_t         PMUIRQ;                 /* +0x018 */
} PMUA_SFRS_t;

/**
 * @brief The starting address of PMUA SFRS.
 */
#define PMUA_REG_CTRL                   (*(__IO          PMUA_CTRL_t *)(0x50000040))
#define PMUA_REG_IGNORE_CIFS            (*(__IO              uint8_t *)(0x50000044))
#define PMUA_REG_DWELL                  (*(__IO         PMUA_DWELL_t *)(0x50000048))
#define PMUA_REG_VBATCTRL               (*(__IO      PMUA_VBATCTRL_t *)(0x5000004C))
#define PMUA_REG_VBATTRIM               (*(__IO      PMUA_VBATTRIM_t *)(0x50000050))
#define PMUA_REG_VBATDBNC               (*(__IO      PMUA_VBATDBNC_t *)(0x50000054))
#define PMUA_REG_VBATDBNCTHRES          (*(__IO PMUA_VBATDBNCTHRES_t *)(0x50000058))
#define PMUA_REG_PMUIRQ                 (*(__IO        PMUA_PMUIRQ_t *)(0x5000005C))


#define PMUA_SFRS ((__IO PMUA_SFRS_t *)0x50000040)

#endif /* end of __PMUA_SFR_H__ section */



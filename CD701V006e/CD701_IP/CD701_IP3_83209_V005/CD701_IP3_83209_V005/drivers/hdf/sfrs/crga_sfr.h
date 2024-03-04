/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file crga_sfr.h
 */

#ifndef CRGA_SFR_H__
#define CRGA_SFR_H__

#include <stdint.h>

#define MODUE_SOFT_RESET_ADC            (0x01U)
#define MODUE_SOFT_RESET_LINM           (0x02U)
#define MODUE_SOFT_RESET_LINS           (0x04U)
#define MODUE_SOFT_RESET_PWM            (0x08U)


typedef union{
  struct {
    uint8_t  LFRCSTS                  :  1; /*!< Slow oscillator status */
    uint8_t                           :  7;
    uint8_t  CLKLFSEL                 :  1; /*!< LF Clock Source select */
    uint8_t                           :  7;
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}CRGA_LFCLKCTRL_t;


typedef union{
  struct {
    uint8_t  HFRCENA                  :  1; /*!< Fast oscillator enable */
    uint8_t  HFRCSTS                  :  1; /*!< Fast oscillator status */
    uint8_t                           :  6; /*   (reserved) */
    uint8_t  SYSCLKSEL                :  8; /*!< Clock select */
    uint8_t  DIVSYSCLK                :  8; /*!< Clock div select */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}CRGA_SYSCLKCTRL_t;

typedef union{
  struct {
    uint8_t  PORFLAG                  :  1; /*!< Power on reset flag */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  BOR3V3FLAG               :  1; /*!< BOR 3v3 flag */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  BOR1V5FLAG               :  1; /*!< BOR 1v5 flag */
    uint8_t  WDTFLAG                  :  1; /*!< Watchdog bark flag */
    uint8_t  OVTEMPFLAG               :  1; /*!< Over Temp Violation flag */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  PORFLAGCLR               :  1; /*!< POR flag clear */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  BOR3V3FLAGCLR            :  1; /*!< BOR 3v3 clear */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  BOR1V5FLAGCLR            :  1; /*!< BOR 1v5 clear */
    uint8_t  WDTFLAGCLR               :  1; /*!< WDT flag clear */
    uint8_t  OVTEMPFLAGCLR            :  1; /*!< OVTEMP flag clear */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  HARDRSTREQ               :  1; /*!< Hard reset request */
    uint8_t  SAFEMODEREQ              :  1; /*!< Safe Mode request */
    uint8_t                           :  6; /*   (reserved) */
    uint8_t  SOFTRSTREQ               :  8; /*!< Soft reset request */
  };
  uint32_t WORD;
}CRGA_RESETCTRL_t;


typedef union{
  struct {
    uint8_t  VDD3V3                   :  2; /*!< BOR 3v3 action */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  VDD1V5                   :  2; /*!< BOR 1v5 action */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  7; /*   (reserved) */
    uint8_t  BOR_3V3_LOCK             :  1;
    uint8_t                           :  7; /*   (reserved) */
    uint8_t  BOR_1V5_LOCK             :  1;
  };
  uint32_t WORD;
}CRGA_BORACTION_t;


typedef union{
  struct {
    uint8_t  BOR3V3THRESH             :  8; /*!< BOR 3v3 threshold */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t  BOR1V5THRESH             :  8; /*!< BOR 1v5 threshold */
    uint8_t  BORBIASOVERRIDEENA       :  1; /*!< BOR bias override bit */
    uint8_t                           :  3; /*   (reserved) */
    uint8_t  BORBIASOVERRIDESEL       :  1; /*!< BOR bias override select */
    uint8_t                           :  3; /*   (reserved) */
  };
  uint32_t WORD;
}CRGA_BORCONFIG_t;

typedef union{
  struct {
    uint8_t  WDTACTION                :  1; /*!< Watchdog action only valid for Realplum C0, not valid for C1*/
    uint8_t  WDTBARKTHRESH            :  4; /*!< WatchDog Bark Threshold */
    uint8_t                           :  3; /*   (reserved) */
    uint8_t  WDTBARKCNT               :  8; /*!< WatchDog Bark Counter */
    uint8_t  WDTBARKCNTCLR            :  8; /*!< WatchDog Bark Counter Clear */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}CRGA_WDTACTION_t;


typedef union{
  struct {
    uint8_t  PMUCPSEL                 :  1; /*!< PMU charge pump select */
    uint8_t  PMUCPREG                 :  1; /*!< PMU charge pump override register */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  PMUCPDIVSEL              :  3; /*!< PMU charge pump divider select */
    uint8_t                           :  1; /*   (reserved) */
    uint32_t                          : 24; /*   (reserved) */
  };
  uint32_t WORD;
}CRGA_CPCLKCTRL_t;

typedef union{
  struct {
    uint8_t  OVTEMP                   :  8; /*!< Over Temperature action */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  7; /*   (reserved) */
    uint8_t  OVTEMP_LOCK              :  1;
  };
  uint32_t WORD;
}CRGA_OVTEMPACTION_t;

typedef union{
  struct {
    uint8_t  OVTEMPLEVEL              :  8; /*!< Over Temp threshold */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t  OVTEMPENA                :  1; /*!< OverTemp Monitor Enable bit */
    uint8_t  DISTEMPSENS              :  1; /*!< Disable Temp Sensor Analog Part */
    uint8_t                           :  6; /*   (reserved) */
  };
  uint32_t WORD;
}CRGA_OVTEMPCONFIG_t;
/**
 * @brief A structure to represent Special Function Registers for CRGA.
 */
typedef struct {
  CRGA_LFCLKCTRL_t      LFCLKCTRL;                         /*<! Slow oscillator status +0x000 */
  CRGA_SYSCLKCTRL_t     SYSCLKCTRL;           /* +0x004 */
  CRGA_RESETCTRL_t      RESETCTRL;            /* +0x008 */
  uint8_t  MODULERSTREQ;                      /*<! Module Soft reset request +0x00C */
  uint8_t  _RESERVED_0D[3];                   /* +0x00D */
  CRGA_BORACTION_t      BORACTION;            /* +0x010 */
  CRGA_BORCONFIG_t      BORCONFIG;            /* +0x014 */
  CRGA_WDTACTION_t      WDTACTION;            /* +0x018 */
  uint8_t  KILLLFRC;                          /*<! Kill slow RC oscillator +0x01C */
  uint8_t  _RESERVED_1D[3];                   /* +0x01D */
  CRGA_CPCLKCTRL_t      CPCLKCTRL;            /* +0x020 */
  CRGA_OVTEMPACTION_t   OVTEMPACTION;         /* +0x024 */
  CRGA_OVTEMPCONFIG_t   OVTEMPCONFIG;         /* +0x028 */
} CRGA_SFRS_t;

/**
 * @brief The starting address of CRGA SFRS.
 */
#define CRGA_REG_LFCLKCTRL              (*(__IO    CRGA_LFCLKCTRL_t *)(0x50000000))
#define CRGA_REG_SYSCLKCTRL             (*(__IO   CRGA_SYSCLKCTRL_t *)(0x50000004))
#define CRGA_REG_RESETCTRL              (*(__IO    CRGA_RESETCTRL_t *)(0x50000008))
#define CRGA_REG_MODULERSTREQ           (*(__IO             uint8_t *)(0x5000000C))
#define CRGA_REG_BORACTION              (*(__IO    CRGA_BORACTION_t *)(0x50000010))
#define CRGA_REG_BORCONFIG              (*(__IO    CRGA_BORCONFIG_t *)(0x50000014))
#define CRGA_REG_WDTACTION              (*(__IO    CRGA_WDTACTION_t *)(0x50000018))
#define CRGA_REG_KILLLFRC               (*(__IO             uint8_t *)(0x5000001C))
#define CRGA_REG_CPCLKCTRL              (*(__IO    CRGA_CPCLKCTRL_t *)(0x50000020))
#define CRGA_REG_OVTEMPACTION           (*(__IO CRGA_OVTEMPACTION_t *)(0x50000024))
#define CRGA_REG_OVTEMPCONFIG           (*(__IO CRGA_OVTEMPCONFIG_t *)(0x50000028))

#define CRGA_SFRS ((__IO CRGA_SFRS_t *)0x50000000)

#endif /* end of __CRGA_SFR_H__ section */



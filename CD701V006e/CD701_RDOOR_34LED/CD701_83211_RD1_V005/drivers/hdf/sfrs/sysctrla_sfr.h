/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file sysctrla_sfr.h
 */

#ifndef SYSCTRLA_SFR_H__
#define SYSCTRLA_SFR_H__

#include <stdint.h>

typedef union{
  struct {
    uint8_t  DEBUG_ACCESS_KEY         :  8;
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  7; /*   (reserved) */
    uint8_t  DEBUG_LOCK               :  1;
  };
  uint32_t WORD;
}SYSCTRLA_DEBUG_ACCESS_KEY_t;


typedef union{
  struct {
    uint8_t  KEY                      :  8;
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  7; /*   (reserved) */
    uint8_t  LOCK                     :  1;
  };
  uint32_t WORD;
}SYSCTRLA_TRIM_ACCESS_KEY_t;


typedef union{
  struct {
    uint8_t                           :  4; /*   (reserved) */
    uint8_t  TRIM                     :  3; /*!< Band Gap Trim */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  RESISTOR_TRIM            :  8; /*!< V2I Resistor Trim */
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}SYSCTRLA_PMU_TRIM_t;


typedef union{
  struct {
    uint8_t  TRIM_HF_RC               :  8; /*!< High Frequency RC Oscillator trim */
    uint8_t  SSCENA                   :  1; /*!< SSC Enable */
    uint8_t  SSCDEEP                  :  3; /*!< SSC Depth Configuration */
    uint8_t                           :  4; /*   (reserved) */
    uint8_t  SSCDIV                   :  8; /*!< SSC Clock Divider */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}SYSCTRLA_HF_OSC_TRIM_t;

typedef union{
  struct {
    uint8_t  LEDBIASSEL               :  1; /*!< High Voltage LED bias select */
    uint8_t  LEDBIASREG               :  1; /*!< High Voltage LED bias select register */
    uint8_t                           :  6; /*   (reserved) */
    uint32_t                          : 24; /*   (reserved) */
  };
  uint32_t WORD;
}SYSCTRLA_BIAS_t;

typedef union{
  struct {
    uint16_t RESERVED;                      /* +0x028 */
    uint16_t TRIM;                          /*<! Unified LED trim (100uA step) +0x02A */
  };
  uint32_t WORD;
}SYSCTRLA_LEDTRIM_t;


typedef union{
  struct {
    uint8_t  TXLINS_DR_SLOPE          :  2; /*!< LIN Slave IO Drive Slope select */
    uint8_t  TXLINM_DR_SLOPE          :  2; /*!< LIN Master IO Drive Slope select */
    uint8_t  LINSTX_BIAS_BOOST        :  2; /*!< LIN Slave IO TX Bias select */
    uint8_t  LINMTX_BIAS_BOOST        :  2; /*!< LIN Master IO TX Bias select */
    uint8_t  TXLINSRISESLOPE          :  1; /*!< LIN Slave IO Rise Slope select */
    uint8_t  TXLINMRISESLOPE          :  1; /*!< LIN Slave IO Rise Slope select */
    uint8_t                           :  6; /*   (reserved) */
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}SYSCTRLA_LIN_t;


typedef union{
  struct {
    uint8_t  DFTCODE                  :  8;
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t                           :  7; /*   (reserved) */
    uint8_t  DFT_LOCK                 :  1;
  };
  uint32_t WORD;
}SYSCTRLA_DFTCODE_t;
/**
 * @brief A structure to represent Special Function Registers for SYSCTRLA.
 */
typedef struct {

  uint8_t  RETAIN0;                           /*<! Firmware scratch register 0 +0x000 ,please don't use for it's reserved for system use */
  uint8_t  _RESERVED_01[3];                   /* +0x001 */

  uint8_t  RETAIN1;                           /*<! Firmware scratch register 1 (0x1) +0x004 ,please don't use for it's reserved for system use */
  uint8_t  _RESERVED_05[3];                   /* +0x005 */

  SYSCTRLA_DEBUG_ACCESS_KEY_t  DEBUG_ACCESS_KEY; /* +0x008 */

  uint8_t  DEBUG_ACCESS_ENABLED;              /* +0x00C */
  uint8_t  _RESERVED_0D[3];                   /* +0x00D */

  SYSCTRLA_TRIM_ACCESS_KEY_t TRIM_ACCESS_KEY; /* +0x010 */

  uint8_t  TRIM_ACCESS_ENABLED;               /* +0x014 */
  uint8_t  _RESERVED_15[3];                   /* +0x015 */

  SYSCTRLA_PMU_TRIM_t PMU_TRIM; /* +0x018 */

  uint8_t  LF_OSC_TRIM;                        /*<! 16KHz RC oscillator trim +0x01C */
  uint8_t  _RESERVED_1D[3];                   /* +0x01D */

  SYSCTRLA_HF_OSC_TRIM_t  HF_OSC_TRIM; /* +0x020 */
  SYSCTRLA_BIAS_t BIAS; /* +0x024 */

  SYSCTRLA_LEDTRIM_t    TRIMLED[24];

  uint8_t  TRIMVFW;                           /*<! PN Forward Voltage Current trim (10uA step) +0x02C */
  uint8_t  _RESERVED_2D[3];                   /* +0x02D */

  SYSCTRLA_LIN_t LIN; /* +0x030 */
  SYSCTRLA_DFTCODE_t DFTCODE; /* +0x034 */

  uint8_t  DFT_ACCESS_ENABLED;                /* +0x038 */
  uint8_t  _RESERVED_39[3];                   /* +0x039 */

  uint8_t  DFTTESTMODESTART;                  /* +0x03C */
  uint8_t  _RESERVED_3D[3];                   /* +0x03D */

  uint32_t NAME;                              /*<! ASIC name +0x040 */

  uint16_t REV;                               /*<! Silicon Revision +0x044 */
  uint8_t  _RESERVED_46[2];                   /* +0x046 */

  uint8_t  ENABORTESTMODE;                    /*<! BOR Testmode Enable +0x048 */
  uint8_t  _RESERVED_49[3];                   /* +0x049 */

} SYSCTRLA_SFRS_t;


/**
 * @brief The starting address of SYSCTRLA SFRS.
 */

#define SYSCTRLA_REG_RETAIN0                            (*(__IO                     uint8_t *)(0x50013000))/*  please don't use for it's reserved for system use   */
#define SYSCTRLA_REG_RETAIN1                            (*(__IO                     uint8_t *)(0x50013004))/*  please don't use for it's reserved for system use   */
#define SYSCTRLA_REG_DEBUG_ACCESS_KEY                   (*(__IO SYSCTRLA_DEBUG_ACCESS_KEY_t *)(0x50013008))
#define SYSCTRLA_REG_DEBUG_ACCESS_ENABLED               (*(__IO                     uint8_t *)(0x5001300C))
#define SYSCTRLA_REG_TRIM_ACCESS_KEY                    (*(__IO  SYSCTRLA_TRIM_ACCESS_KEY_t *)(0x50013010))
#define SYSCTRLA_REG_TRIM_ACCESS_ENABLED                (*(__IO                     uint8_t *)(0x50013014))
#define SYSCTRLA_REG_PMU_TRIM                           (*(__IO         SYSCTRLA_PMU_TRIM_t *)(0x50013018))
#define SYSCTRLA_REG_TRIM_LF_RC                         (*(__IO                     uint8_t *)(0x5001301C))
#define SYSCTRLA_REG_HF_OSC_TRIM                        (*(__IO                     uint8_t *)(0x50013020))
#define SYSCTRLA_REG_BIAS                               (*(__IO             SYSCTRLA_BIAS_t *)(0x50013024))
#define SYSCTRLA_REG_TRIMLED(x)                         (*(__IO                    uint16_t *)(0x50013028+ x*4))
#define SYSCTRLA_REG_TRIMVFW                            (*(__IO                     uint8_t *)(0x50013088))
#define SYSCTRLA_REG_LIN                                (*(__IO              SYSCTRLA_LIN_t *)(0x5001308C))
#define SYSCTRLA_REG_DFTCODE                            (*(__IO          SYSCTRLA_DFTCODE_t *)(0x50013090))
#define SYSCTRLA_REG_DFT_ACCESS_ENABLED                 (*(__IO                     uint8_t *)(0x50013094))
#define SYSCTRLA_REG_DFTTESTMODESTART                   (*(__IO                     uint8_t *)(0x50013098))
#define SYSCTRLA_REG_NAME                               (*(__IO                    uint32_t *)(0x5001309C))
#define SYSCTRLA_REG_REV                                (*(__IO                    uint16_t *)(0x500130A0))
#define SYSCTRLA_REG_ENABORTESTMODE                     (*(__IO                     uint8_t *)(0x500130A4))


#define SYSCTRLA_SFRS ((__IO SYSCTRLA_SFRS_t *)0x50013000)

#endif /* end of __SYSCTRLA_SFR_H__ section */



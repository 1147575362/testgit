/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file adc_sfr.h
 */

#ifndef ADC_SFR_H__
#define ADC_SFR_H__

#include <stdint.h>


typedef union {
  struct {
    uint8_t  MODE                     :  1; /*!< ADC mode select */
    uint8_t  AUTOEN                   :  1; /*!< Bias Enable mode */
    uint8_t  ATTEN                    :  1; /*!< ADC input Attenuation setting */
    uint8_t  BIASENA                  :  1; /*!< ADC Bias Enable */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  SWSYNCIN                 :  1; /*!< Soft Sync Input */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  SYNCENA                  :  8; /*!< Sync Enable */
    uint8_t  SAMPCYC                  :  8; /*!< Sample cycle */
    uint8_t                           :  8; /*   (reserved) */
  };
  uint32_t WORD;
}ADC_CONF_t;

typedef union {
  struct {
    uint8_t  CONVERT                  :  1; /*!< ADC START/STATUS Register */
    uint8_t  CONT                     :  1; /*!< Continuous Convesion Enable */
    uint8_t                           :  2; /*   (reserved) */
    uint8_t  STUPDLY                  :  2; /*!< Startup Delay */
    uint8_t  IRQENA                   :  1; /*!< IRQ Enable */
    uint8_t  IRQCLR                   :  1; /*!< IRQ Clear */
    uint8_t  CHSEQ                    :  8; /*!< Channel Sequence */
    uint8_t  CH1SEL                   :  2; /*!< Channel1 Selection */
    uint8_t  CH2SEL                   :  5; /*!< Channel2 Selection */
    uint8_t                           :  1; /*   (reserved) */
    uint8_t  CH3SEL                   :  2; /*!< Channel3 Selection */
    uint8_t  CH4SEL                   :  4; /*!< Channel4 Selection */
    uint8_t                           :  2; /*   (reserved) */
  };
  uint32_t WORD;
}ADC_CNTRL_t;

typedef union {
  struct {
    uint8_t  TCURR                    :  4; /*!< TCURR setting */
    uint8_t  TCHNL                    :  4; /*!< TCHNL setting */
    uint8_t  TGUARD                   :  8; /*!< TGUARD setting */
    uint16_t                          : 16; /*   (reserved) */
  };
  uint32_t WORD;
}ADC_TSET_t;

typedef union {
  struct {
    uint8_t  CONVDONE                 :  1; /*!< The set Sequence of Conversions is Done */
    uint8_t  FSM                      :  6; /*!< current state of the ADC Sequencer */
    uint8_t                           :  1; /*   (reserved) */
    uint32_t                          : 24; /*   (reserved) */
  };
  uint32_t WORD;
}ADC_STATUS_t;


/**
 * @brief A structure to represent Special Function Registers for ADC.
 */
typedef struct {
  ADC_CONF_t    CONF;
  ADC_CNTRL_t   CNTRL;
  ADC_TSET_t    TSET;
  uint16_t      DATA1;                       
  uint8_t       _RESERVED_0E1[2];            
  uint16_t      DATA2;                      
  uint8_t       _RESERVED_0E2[2];            
  uint16_t      DATA0345;                      
  uint8_t       _RESERVED_0E0345[2];            
  ADC_STATUS_t  STATUS;
} ADC_SFRS_t;

/**
 * @brief The starting address of ADC SFRS.
 */
#define ADC_REG_CNTRL_CONVERT   (*(__IO  uint8_t *)(0x50010604))
#define ADC_REG_CNTRL_CH1_CH2   (*(__IO  uint8_t *)(0x50010606))

#define ADC_REG_CONF            (*(__IO   ADC_CONF_t *)(0x50010600))
#define ADC_REG_CNTRL           (*(__IO  ADC_CNTRL_t *)(0x50010604))
#define ADC_REG_TSET            (*(__IO   ADC_TSET_t *)(0x50010608))
#define ADC_REG_DATA1           (*(__IO     uint16_t *)(0x5001060C))
#define ADC_REG_DATA2           (*(__IO     uint16_t *)(0x50010610))
#define ADC_REG_DATA0345        (*(__IO     uint16_t *)(0x50010614))
#define ADC_REG_STATUS          (*(__IO ADC_STATUS_t *)(0x50010618))

#define ADC_SFRS ((__IO ADC_SFRS_t *)0x50010600)

#endif /* end of __ADC_SFR_H__ section */



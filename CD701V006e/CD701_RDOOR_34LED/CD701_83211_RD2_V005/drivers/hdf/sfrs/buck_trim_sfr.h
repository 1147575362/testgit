/**
 * @copyright 2019 indie Semiconductor
 *
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 *
 * @file buck_trim_sfr.h
 */

#ifndef BUCK_TRIM_SFR_H__
#define BUCK_TRIM_SFR_H__

#include <stdint.h>

typedef union{
  struct {
    uint8_t  CRDMGMASK                :  1; /*!< db_carou_demag_masked */
    uint8_t  CRFLGHSOFFDNTWT          :  1; /*!< db_carou_flag_hs_off_dont_wait */
    uint8_t  CRFLGHSONDNTWT           :  1; /*!< db_carou_flag_hs_on_dont_wait */
    uint8_t  CRFLGLSOFFDNTWT          :  1; /*!< db_carou_flag_ls_off_dont_wait */
    uint8_t  CRFLGLSONDNTWT           :  1; /*!< db_carou_flag_ls_on_dont_wait */
    uint8_t  CRHSONFRCD               :  1; /*!< db_carou_hs_on_forced */
    uint8_t  CRHSONMSK                :  1; /*!< db_carou_hs_on_mask */
    uint8_t  CRLSONFRCD               :  1; /*!< db_carou_ls_on_forced */
    uint8_t  CRMAGMASK                :  1; /*!< db_carou_mag_masked */
    uint8_t  CRLSONMSK                :  1; /*!< db_carou_ls_on_mask */
    uint8_t  CRPAUSE                  :  4; /*!< db_carou_pause */
    uint8_t  FDMGRQST                 :  1; /*!< db_force_demag_rqst */
    uint8_t  FMAGRQST                 :  1; /*!< db_force_mag_rqst */
    uint8_t  CRDLYTMOUTIGN            :  8; /*!< db_carou_dly_timeout_ignored */
    uint8_t  CLKOVERRIGHT             :  1; /*!< db_clk_overright */
    uint8_t  MNBYPFSTOP               :  1; /*!< db_main_bypass_force_stop */
    uint8_t  MNGATECLKHF              :  1; /*!< db_main_gate_clk_hf */
    uint8_t  MNGO2PFMFRCD             :  1; /*!< db_main_go2pfm_forced */
    uint8_t  MNGO2PFMMASK             :  1; /*!< db_main_go2pfm_masked */
    uint8_t  MNGO2PWMFRCD             :  1; /*!< db_main_go2pwm_forced */
    uint8_t  MNGO2PWMMASK             :  1; /*!< db_main_go2pwm_masked */
    uint8_t  FRCDPWM                  :  1; /*!< forced_pwm */
  };
  uint32_t WORD;
}BUCK_TRIM_DATA0_t;


typedef union{
  struct {
    uint8_t  MNDLYTMOUTIGN            :  4; /*!< db_main_dly_timeout_ignored */
    uint8_t  FRCDPFM                  :  1; /*!< forced_pfm */
    uint8_t  SPAREU0W4B5              :  1; /*!< spareu0_w4_b5 */
    uint8_t  SPAREU0W4B6              :  1; /*!< spareu0_w4_b6 */
    uint8_t  SPAREU0W4B7              :  1; /*!< spareu0_w4_b7 */
    uint8_t  DTB01SEL                 :  5; /*!< dtb01_sel */
    uint8_t  DTBON                    :  1; /*!< dtb_on */
    uint8_t  ATBON01BUF               :  1; /*!< atb_on_01_buf */
    uint8_t  ATBON01DIRECT            :  1; /*!< atb_on_01_direct */
    uint8_t  DTB02SEL                 :  5; /*!< dtb02_sel */
    uint8_t  ATBON02BUF               :  1; /*!< atb_on_02_buf */
    uint8_t  ATBON02DIRECT            :  1; /*!< atb_on_02_direct */
    uint8_t  SPAREU0W6B7              :  1; /*!< spareu0_w6_b7 */
    uint8_t  DTB03SEL                 :  5; /*!< dtb03_sel */
    uint8_t  SPAREU0W7B5              :  1; /*!< spareu0_w7_b5 */
    uint8_t  SPAREU0W7B6              :  1; /*!< spareu0_w7_b6 */
    uint8_t  SPAREU0W7B7              :  1; /*!< spareu0_w7_b7 */
  };
  uint32_t WORD;
}BUCK_TRIM_DATA1_t;


typedef union{
  struct {
    uint8_t  PFMSTMAGEVTAIU1          :  1; /*!< db_pfm_start_mag_event_alwaysinuse_01 */
    uint8_t  PFMSTMAGEVTAIU2          :  1; /*!< db_pfm_start_mag_event_alwaysinuse_02 */
    uint8_t  PFMSTMAGEVTIGN1          :  1; /*!< db_pfm_start_mag_event_ignore_01 */
    uint8_t  PFMSTMAGEVTIGN2          :  1; /*!< db_pfm_start_mag_event_ignore_02 */
    uint8_t  PFMSTDMGEVTAIU1          :  1; /*!< db_pfm_start_demag_event_alwaysinuse_01 */
    uint8_t  PFMSTDMGEVTIGN1          :  1; /*!< db_pfm_start_demag_event_ignore_01 */
    uint8_t  PFMSPDMGEVTAIU1          :  1; /*!< db_pfm_stop_demag_event_alwaysinuse_01 */
    uint8_t  PFMSPDMGEVTIGN1          :  1; /*!< db_pfm_stop_demag_event_ignore_01 */
    uint8_t  PWMSTMAGEVTAIU1          :  1; /*!< db_pwm_start_mag_event_alwaysinuse_01 */
    uint8_t  PWMSTMAGEVTAIU2          :  1; /*!< db_pwm_start_mag_event_alwaysinuse_02 */
    uint8_t  PWMSTMAGEVTIGN1          :  1; /*!< db_pwm_start_mag_event_ignore_01 */
    uint8_t  PWMSTMAGEVTIGN2          :  1; /*!< db_pwm_start_mag_event_ignore_02 */
    uint8_t  PWMSTDMGEVTIGN1          :  1; /*!< db_pwm_start_demag_event_ignore_01 */
    uint8_t  PWMSTDMGEVTIGN2          :  1; /*!< db_pwm_start_demag_event_ignore_02 */
    uint8_t  PWMSTDMGEVTAIU1          :  1; /*!< db_pwm_start_demag_event_alwaysinuse_01 */
    uint8_t  SPAREU1W1B7              :  1; /*!< spareU1_W1_b7 */
    uint8_t  ENATB1IBIAS              :  1; /*!< en_atb_01_ibias */
    uint8_t  ENATB2IBIAS              :  1; /*!< en_atb_02_ibias */
    uint8_t  ENATB1V2I                :  1; /*!< en_atb_01_v2i */
    uint8_t  ENATB2V2I                :  1; /*!< en_atb_02_v2i */
    uint8_t  PHCOMP1                  :  1; /*!< placeholder_comp01 */
    uint8_t  PHCOMP2                  :  1; /*!< placeholder_comp02 */
    uint8_t  PHCOMP3                  :  1; /*!< placeholder_comp03 */
    uint8_t  PHCOMP4                  :  1; /*!< placeholder_comp04 */
    uint8_t  ENATB1ILOOP              :  1; /*!< en_atb_01_current_loop */
    uint8_t  ENATB2ILOOP              :  1; /*!< en_atb_02_current_loop */
    uint8_t  ENATB1ILIM               :  1; /*!< en_atb_01_ilim */
    uint8_t  ENATB2ILIM               :  1; /*!< en_atb_02_ilim */
    uint8_t  FOENOC                   :  1; /*!< db_force_off_en_overcurrent */
    uint8_t  ENATB1IRAMPCOMP          :  1; /*!< en_atb_01_iramp_comp */
    uint8_t  ENATB2IRAMPCOMP          :  1; /*!< en_atb_02_iramp_comp */
    uint8_t  FOENIRAMPCOMP            :  1; /*!< db_force_off_en_iramp_compensation */
  };
  uint32_t WORD;
}BUCK_TRIM_DATA2_t;

typedef union{
  struct {
    uint8_t  ENATBISENS               :  3; /*!< en_atb_isense */
    uint8_t  ENATB1VLOOP              :  1; /*!< en_atb_01_voltage_loop */
    uint8_t  ENATB2VLOOP              :  1; /*!< en_atb_02_voltage_loop */
    uint8_t  ENATB1VDAC               :  1; /*!< en_atb_01_vdac */
    uint8_t  ENATB2VDAC               :  1; /*!< en_atb_02_vdac */
    uint8_t  FOVDAC                   :  1; /*!< db_force_off_vdac */
    uint8_t  FORBIDRLDRPC             :  1; /*!< db_forbid_rladder_precharge */
    uint8_t  FOEA                     :  1; /*!< db_force_off_ea */
    uint8_t  FENVEACLMPDN             :  1; /*!< db_force_en_vea_clamp_down */
    uint8_t  FENVEACLMPUP             :  1; /*!< db_force_en_vea_clamp_up */
    uint8_t  ENATB1ANALV              :  1; /*!< en_atb_01_analog_lv */
    uint8_t  ENATB2ANALV              :  1; /*!< en_atb_02_analog_lv */
    uint8_t  RELPDFRMLDO5V            :  1; /*!< db_release_pd_from_ldo5V */
    uint8_t  SPAREU1W5B7              :  1; /*!< spareU1_W5_b7 */
    uint8_t  ENATB1FGND               :  1; /*!< en_atb_01_fgnd */
    uint8_t  ENATB2FGND               :  1; /*!< en_atb_02_fgnd */
    uint8_t  SELILOADOSISENSE         :  2; /*!< sel_iload_os_isense */
    uint8_t  FOENISENSE               :  1; /*!< db_force_off_en_isense */
    uint8_t  FOIOSISENSE              :  1; /*!< db_force_off_ioffset_isense */
    uint8_t  FOENPFM                  :  1; /*!< db_force_off_en_pfm */
    uint8_t  SPAREU1W6B7              :  1; /*!< spareU1_W6_b7 */
    uint8_t  DBENDEMAGCOMP01          :  1; /*!< db_en_demag_comp01 */
    uint8_t  DBENDEMAGCOMP02          :  1; /*!< db_en_demag_comp02 */
    uint8_t  SPAREU1W7                :  6; /*!< spareU1_W7 */
  };
  uint32_t WORD;
}BUCK_TRIM_DATA3_t;

typedef union{
  struct {
    uint8_t  SETVEACLMPDN             :  2; /*!< set_vea_clamp_down */
    uint8_t  SETVEACLMPUP             :  2; /*!< set_vea_clamp_up */
    uint8_t  SPAREU2W0B4              :  1; /*!< spareU2_W0_b4 */
    uint8_t  SPAREU2W0B5              :  1; /*!< spareU2_W0_b5 */
    uint8_t  SPAREU2W0B6              :  1; /*!< spareU2_W0_b6 */
    uint8_t  SPAREU2W0B7              :  1; /*!< spareU2_W0_b7 */
  /*  Ilimit = code*0.2-0.6, code = [0-4]=  SETILIM[0-3], TRIMILIM[0]*/
    uint8_t  TRIMILIM                 :  4; /*!< trim_ilim */
    uint8_t  SETILIM                  :  4; /*!< set_ilim */
    uint8_t                           :  8; /*   (reserved) */
    uint8_t  SETIOS                   :  2; /*!< set_ioffset */
    uint8_t  SETIRAMPCOMP             :  3; /*!< set_iramp_compensation */
    uint8_t  SETCOMP                  :  2; /*!< set_compensation */
    uint8_t  SPAREU2W3B7              :  1; /*!< spareU2_W3_b7 */
  };
  uint32_t WORD;
}BUCK_TRIM_DATA4_t;


typedef union{
  struct {
    uint8_t  SETRI2V                  :  2; /*!< set_ri2v */
    uint8_t  SELISENSEOSISENSE        :  2; /*!< sel_isense_os_isense */
    uint8_t  TRIMISENSEIOS            :  1; /*!< trim_isense_ioffset */
    uint8_t  SETFGNDIDAC              :  2; /*!< set_fgnd_idac */
    uint8_t  FOENIOS                  :  1; /*!< db_force_off_en_ioffset */
    uint8_t  ENPUGCOMP                :  1; /*!< en_pgoodup_comparator */
    uint8_t  ENPDGCOMP                :  1; /*!< en_pgooddown_comparator */
    uint8_t  PFMIPEAK                 :  2; /*!< pfm_ipeak */
    uint8_t  SPAREU2W5                :  4; /*!< spareU2_W5 */
    uint8_t  CRDLYSET1                :  2; /*!< carou_dly_set_01 */
    uint8_t  CRDLYSET2                :  2; /*!< carou_dly_set_02 */
    uint8_t  CRDLYSET3                :  2; /*!< carou_dly_set_03 */
    uint8_t  CRDLYSET4                :  2; /*!< carou_dly_set_04 */
    uint8_t  CRDLYSET5                :  2; /*!< carou_dly_set_05 */
    uint8_t  CRDLYSET6                :  2; /*!< carou_dly_set_06 */
    uint8_t  CRDLYSET7                :  2; /*!< carou_dly_set_07 */
    uint8_t  CRDLYSET8                :  2; /*!< carou_dly_set_08 */
  };
  uint32_t WORD;
}BUCK_TRIM_DATA5_t;
/**
 * @brief A structure to represent Special Function Registers for BUCK_TRIM.
 */
typedef struct {
  BUCK_TRIM_DATA0_t DATA0;
  BUCK_TRIM_DATA1_t DATA1;
  BUCK_TRIM_DATA2_t DATA2;
  BUCK_TRIM_DATA3_t DATA3;
  BUCK_TRIM_DATA4_t DATA4;
  BUCK_TRIM_DATA5_t DATA5;
} BUCK_TRIM_SFRS_t;

/**
 * @brief The starting address of BUCK_TRIM SFRS.
 */
#define BUCK_TRIM_REG_DATA0 (*(__IO BUCK_TRIM_DATA0_t *)(0x50018000))
#define BUCK_TRIM_REG_DATA1 (*(__IO BUCK_TRIM_DATA1_t *)(0x50018004))
#define BUCK_TRIM_REG_DATA2 (*(__IO BUCK_TRIM_DATA2_t *)(0x50018008))
#define BUCK_TRIM_REG_DATA3 (*(__IO BUCK_TRIM_DATA3_t *)(0x5001800C))
#define BUCK_TRIM_REG_DATA4 (*(__IO BUCK_TRIM_DATA4_t *)(0x50018010))
#define BUCK_TRIM_REG_DATA5 (*(__IO BUCK_TRIM_DATA5_t *)(0x50018014))

#define BUCK_TRIM_SFRS ((__IO BUCK_TRIM_SFRS_t *)0x50018000)

#endif /* end of __BUCK_TRIM_SFR_H__ section */



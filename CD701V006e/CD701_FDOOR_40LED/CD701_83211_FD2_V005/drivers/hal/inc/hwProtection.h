#ifndef __HW_PROTECTION_H__
#define __HW_PROTECTION_H__
#include <rugbyPro.h>
#include <core_cm0.h>


typedef enum{
  OVT_PROT_90C = 0,
  OVT_PROT_95C,
  OVT_PROT_100C,
  OVT_PROT_105C,
  OVT_PROT_110C,
  OVT_PROT_115C,
  OVT_PROT_120C,
  OVT_PROT_125C,
  OVT_PROT_130C,
  OVT_PROT_135C,
  OVT_PROT_140C,
  OVT_PROT_145C,
  OVT_PROT_150C,
  OVT_PROT_155C,
  OVT_PROT_160C,
}OverTempThres_t;

typedef enum{
  OVT_ACTION_HW_RESET = 0,
  OVT_ACTION_IRQ,
  OVT_ACTION_NONE,
}OverTempAction_t;

typedef enum{
  OV_VOLT_14_72V = 0U,
  OV_VOLT_15_21V,
  OV_VOLT_15_74V,
  OV_VOLT_16_30V,
  OV_VOLT_16_90V,
  OV_VOLT_17_55V,
  OV_VOLT_18_24V,
  OV_VOLT_19_00V,
  OV_VOLT_19_82V,
  OV_VOLT_20_72V,
}OverVoltThres_t;

typedef enum{
  OV_VOLT_HYS_440mV = 0U,
  OV_VOLT_HYS_1130mV,
  OV_VOLT_HYS_1770mV,
  OV_VOLT_HYS_2360mV,
}OverVoltHysteresis_t;


typedef enum{
  UV_VOLT_4_768V = 0,
  UV_VOLT_4_838V, 
  UV_VOLT_4_910V,
  UV_VOLT_4_984V, 
  UV_VOLT_5_061V, 
  UV_VOLT_5_140V,
  UV_VOLT_5_221V, 
  UV_VOLT_5_305V,
  UV_VOLT_5_392V, 
  UV_VOLT_5_482V, 
  UV_VOLT_5_574V,
  UV_VOLT_5_670V, 
  UV_VOLT_5_770V,
  UV_VOLT_5_872V, 
  UV_VOLT_5_979V, 
  UV_VOLT_6_089V,
  UV_VOLT_6_204V, 
  UV_VOLT_6_323V,
  UV_VOLT_6_447V, 
  UV_VOLT_6_575V, 
  UV_VOLT_6_709V,
  UV_VOLT_6_849V, 
  UV_VOLT_6_994V,
  UV_VOLT_7_146V, 
  UV_VOLT_7_304V, 
  UV_VOLT_7_470V,
  UV_VOLT_7_643V,
  UV_VOLT_7_825V, 
  UV_VOLT_8_015V,
  UV_VOLT_8_215V, 
  UV_VOLT_8_425V, 
  UV_VOLT_8_646V,
  UV_VOLT_8_879V, 
  UV_VOLT_9_125V,
  UV_VOLT_9_385V, 
  UV_VOLT_9_660V, 
  UV_VOLT_9_952V,
  UV_VOLT_10_26V,
  UV_VOLT_10_59V, 
  UV_VOLT_10_94V,
}UnderVoltThres_t;

typedef enum{
  UNDER_VOLT_HYS_127mV = 0U,
  UNDER_VOLT_HYS_199mV,
  UNDER_VOLT_HYS_273mV,
  UNDER_VOLT_HYS_351mV,
}UnderVoltHysteresis_t;


typedef enum{
  VOLT_DEBOUNCE_TIME_0ms  = 0U,
  VOLT_DEBOUNCE_TIME_1ms  = 16U,
  VOLT_DEBOUNCE_TIME_2ms  = 32U,
  VOLT_DEBOUNCE_TIME_3ms  = 48U,
  VOLT_DEBOUNCE_TIME_4ms  = 64U,
  VOLT_DEBOUNCE_TIME_5ms  = 80U,
  VOLT_DEBOUNCE_TIME_6ms  = 96U,
  VOLT_DEBOUNCE_TIME_7ms  = 112U,
  VOLT_DEBOUNCE_TIME_8ms  = 128U,
  VOLT_DEBOUNCE_TIME_9ms  = 144U,
  VOLT_DEBOUNCE_TIME_10ms = 160U,
  VOLT_DEBOUNCE_TIME_11ms = 176U,
  VOLT_DEBOUNCE_TIME_12ms = 192U,
  VOLT_DEBOUNCE_TIME_13ms = 208U,
  VOLT_DEBOUNCE_TIME_14ms = 224U,
  VOLT_DEBOUNCE_TIME_15ms = 240U,
}VoltDebouceTime_t;

typedef void (*hwProtIsrCallback_t)(void);


void HW_PROT_SetOverTmperature(OverTempThres_t threshold, OverTempAction_t action);
void HW_PROT_RegisterOVTemperatureIRQ(hwProtIsrCallback_t callback);
void HW_PROT_UnRegisterOVTemperatureIRQ(void);

void HW_PROT_SetOverVoltage(OverVoltThres_t threshold,OverVoltHysteresis_t hys, VoltDebouceTime_t triggerTime, VoltDebouceTime_t releaseTime);
void HW_PROT_RegisterOverVoltageIRQ(hwProtIsrCallback_t callback);
void HW_PROT_UnRegisterOverVoltageIRQ(void);

void HW_PROT_SetUnderVoltage(UnderVoltThres_t threshold,UnderVoltHysteresis_t hys, VoltDebouceTime_t triggerTime, VoltDebouceTime_t releaseTime);
void HW_PROT_RegisterUnderVoltageIRQ(hwProtIsrCallback_t callback);
void HW_PROT_UnRegisterUnderVoltageIRQ(void);

/* return 0: normal , 1: low, 2:high */
uint8_t HW_PROT_GetBattVoltState(void);
/* return 0: normal , others : over heat */
uint8_t HW_PROT_ChipIsOverHeat(void);




#endif
/**
 * @copyright 2015 IndieMicro.
 *
 * This file is proprietary to IndieMicro.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of IndieMicro.
 *
 * @file measureTask.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/09/10
 */
#include <crc32.h>
#include <measureTask.h>

#define AVERAGE_MEASURE_POS     (4U)
#define AVERAGE_MEASURE_GAIN    (1U << AVERAGE_MEASURE_POS)

#define MEASURE_GAIN_POS         12U
#define MEASURE_GAIN             4096 /*(1UL << MEASURE_GAIN_POS)*/

#define MEASURE_TEMP_GAIN_POS    14U
#define MEASURE_TEMP_GAIN        7447 /*16384*0.4546*/




typedef struct{
  int32_t coefficient;
  int32_t offset;
}coeffParam_t;

typedef struct{
  uint32_t currCode;
  uint16_t buff[AVERAGE_MEASURE_GAIN];
  uint16_t count;
  coeffParam_t coeff;
  int16_t target;
}AdcDatabuff_t;

typedef struct{
  AdcDatabuff_t vBatt;
  AdcDatabuff_t vLedPN[LED_NUM][3];
  AdcDatabuff_t vChipTemp;
  coeffParam_t  tChip;
  int16_t       chipTemperature;
  int16_t       ledPNTemperature[LED_NUM][3];
}AdcResult_t;


static uint8_t dataIntegrityError = FALSE;

void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff);
static TaskState_t adcTaskState = TASK_STATE_INIT;
static AdcResult_t adcResult;

static uint8_t measLedColorNo = 0U;
static AdcSyncMode_t measSyncMode = SYNC_MODE_NONE;
static AdcMeasureItem_t  measItem = ADC_MEASURE_ITEM_NONE;
static volatile uint32_t measStart = 0U;
static volatile uint16_t measGeneralAdcCode[3];
void measureDoneISR(ADCMeasureParam_t item, uint16_t *const result);
void measureParamStart(void);
void measureParamInit(void);

static uint32_t updateParamIndex = 0U;
static uint32_t updateParamStart = 0U;
void updateTimerExpired(SoftTimer_t *timer);

int16_t get_volt(AdcDatabuff_t *const param);
int16_t get_chip_temperature(int16_t vTemp);
int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt);
void updateSystemInfo(void);
void updateLedTemperature(LedNum_t index);
void updateParams(void);

static volatile uint32_t adcConvertDone = 0U;

static SoftTimer_t updateTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 50U,
    .handler  = updateTimerExpired
};


void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff)
{
    if (*original == INVALID_PARAM){
        *original = newData;
        *bufferIndex = 0;
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            buff[i] = newData;
        }
    }else{
        buff[*bufferIndex] = newData;
    }
    (*bufferIndex) ++;
    if ((*bufferIndex) >= AVERAGE_MEASURE_GAIN){
        (*bufferIndex) = 0U;
    }
}


void updateTimerExpired(SoftTimer_t *timer)
{
    static uint8_t measWaitCount = 3U;
    /*  update then start next measurement  */
    if (++measWaitCount >= 3U){
        measWaitCount = 0U;
        measStart = 1U; 
    }
    updateParamStart = 1U;
    TM_PostTask(TASK_ID_ADC_MEASURE);
}


void measureDoneISR(ADCMeasureParam_t param, uint16_t *const result)
{
    measGeneralAdcCode[0] = result[0];
    measGeneralAdcCode[1] = result[1];
    measGeneralAdcCode[2] = result[2];
    measStart = 1U;
    adcConvertDone = 1U;
    TM_PostTask(TASK_ID_ADC_MEASURE);
}

void measureParamStart(void)
{
    switch(measItem){
    case ADC_MEASURE_ITEM_NONE:
      measSyncMode = SYNC_MODE_NONE;
      measItem = ADC_MEASURE_ITEM_VBAT_VTEMP;
      break;
    case ADC_MEASURE_ITEM_VBAT_VTEMP:
      add_data_to_buff(&adcResult.vChipTemp.currCode, measGeneralAdcCode[0], &adcResult.vChipTemp.count,adcResult.vChipTemp.buff);
      add_data_to_buff(&adcResult.vBatt.currCode,     measGeneralAdcCode[1], &adcResult.vBatt.count,adcResult.vBatt.buff);
      if (PWM_LedIsOn(0U) != 0U){
          measLedColorNo = 0U;
          measSyncMode = SYNC_MODE_PWM_PERIOD;
          measItem = ADC_MEASURE_ITEM_LED_VBAT_VTEMP;
      }else{
          adcResult.vLedPN[0][0].currCode = INVALID_PARAM;
          adcResult.vLedPN[0][1].currCode = INVALID_PARAM;
          adcResult.vLedPN[0][2].currCode = INVALID_PARAM;
          measSyncMode = SYNC_MODE_NONE;
          measItem  = ADC_MEASURE_ITEM_NONE;
      }
      break;
    case ADC_MEASURE_ITEM_LED_VBAT_VTEMP:
      
//      add_data_to_buff(&adcResult.vChipTemp.currCode, measGeneralAdcCode[0], &adcResult.vChipTemp.count,adcResult.vChipTemp.buff);
      add_data_to_buff(&adcResult.vBatt.currCode,     measGeneralAdcCode[1], &adcResult.vBatt.count,adcResult.vBatt.buff);
      if (PWM_LedIsOn(0U) != 0U){
          add_data_to_buff(&adcResult.vLedPN[0][measLedColorNo].currCode, measGeneralAdcCode[2], &adcResult.vLedPN[0][measLedColorNo].count,adcResult.vLedPN[0][measLedColorNo].buff);
      }else{
          adcResult.vLedPN[0][0].currCode = INVALID_PARAM;
          adcResult.vLedPN[0][1].currCode = INVALID_PARAM;
          adcResult.vLedPN[0][2].currCode = INVALID_PARAM;
      }
      if (++measLedColorNo >= 3U){
          measSyncMode = SYNC_MODE_NONE;
          measItem  = ADC_MEASURE_ITEM_NONE;
      }else{
          measSyncMode = SYNC_MODE_PWM_PERIOD;
          measItem = ADC_MEASURE_ITEM_LED_VBAT_VTEMP;
      }
      
      break;
    default:
      break;
    }
    if (measItem != ADC_MEASURE_ITEM_NONE){
        ADC_Init(measItem,0U,(LedType_t)measLedColorNo,measSyncMode);
        ADC_Start();
    }
}


int16_t get_volt(AdcDatabuff_t *const param)
{
    int16_t volt = (int16_t)INVALID_VOLT;
    int32_t sVolt;
    uint32_t uVolt;
    uint32_t sum = 0;
    if (param->currCode != INVALID_PARAM){
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            sum += param->buff[i];
        }
        sum  = sum >> AVERAGE_MEASURE_POS;
        sVolt = param->coeff.coefficient * (int32_t)sum + param->coeff.offset;
        uVolt = (uint32_t)sVolt;
        uVolt = uVolt >> MEASURE_GAIN_POS;
        volt = (int16_t)uVolt;
    }
    return volt;
}


int16_t get_chip_temperature(int16_t vTemp)
{
    int32_t  stemp;
    uint32_t uTemp;
    uint8_t  negative = 0U;
    stemp = adcResult.tChip.coefficient*vTemp + adcResult.tChip.offset;
    if (stemp < 0){
        stemp = -stemp;
        negative = 1U;
    }
    uTemp = (uint32_t)stemp;
    uTemp = uTemp >> MEASURE_TEMP_GAIN_POS;
    stemp = (int32_t)uTemp;
    if (negative != 0U){
        stemp = -stemp;
    }
    return (int16_t)stemp;
}


void updateSystemInfo(void)
{
    /* calculate chip temperature */
    adcResult.vChipTemp.target = get_volt(&adcResult.vChipTemp);
    adcResult.chipTemperature = get_chip_temperature(adcResult.vChipTemp.target);
    adcResult.vBatt.target = get_volt(&adcResult.vBatt);
}


int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt)
{
    int8_t temperature;
    uint8_t minPos = 0U;
    uint8_t maxPos = TEMP_CMP_NUM_MAX-1U;
    uint8_t tempPos = ((TEMP_CMP_NUM_MAX-1U)>> 1U);
    for(;;){
        if (deltaPNVolt > table[tempPos].deltaVoltPN){
            maxPos = tempPos;
        }else if (deltaPNVolt < table[tempPos].deltaVoltPN){
            minPos = tempPos;
        }else{
            break;
        }
        tempPos = (minPos + maxPos) >> 1U;
        if ((minPos + 1U)>= maxPos){
            break;
        }
    }
    if (deltaPNVolt <= table[TEMP_CMP_NUM_MAX-1U].deltaVoltPN){
        tempPos = TEMP_CMP_NUM_MAX-1U;
    }
    tempPos = tempPos << DELTA_TEMP_POS;
    temperature = (int8_t)tempPos;
    temperature = temperature + MINIMUM_TEMP;
    return temperature;
}

void updateLedTemperature(LedNum_t ledIndex)
{
    int16_t typicalVolt[3];
    ColorTemperature_t temperature;
    temperature.value[0] = ROOM_TEMPERATURE;
    temperature.value[1] = ROOM_TEMPERATURE;
    temperature.value[2] = ROOM_TEMPERATURE;
    
    (void)CLM_GetLedPNVolts(LED0,temperature, &typicalVolt[0], &typicalVolt[1], &typicalVolt[2]);
    for (uint8_t i = 0U; i < 3U; i++){
        adcResult.vLedPN[0][i].target = get_volt(&adcResult.vLedPN[0][i]);
        if (adcResult.vLedPN[0][i].target != (int16_t)INVALID_VOLT){
            if (i == (uint8_t)LED_RED){
                adcResult.ledPNTemperature[0][i] = get_led_temperature(LP_GetTempTableRed(LED0),  (adcResult.vLedPN[0][i].target - typicalVolt[i]) );
            }else if (i == (uint8_t)LED_GREEN){
                adcResult.ledPNTemperature[0][i] = get_led_temperature(LP_GetTempTableGreen(LED0),(adcResult.vLedPN[0][i].target - typicalVolt[i]) );
            }else{
                adcResult.ledPNTemperature[0][i] = get_led_temperature(LP_GetTempTableBlue(LED0), (adcResult.vLedPN[0][i].target - typicalVolt[i]) );
            }
        }else{
            adcResult.ledPNTemperature[0][i] = ROOM_TEMPERATURE;
        }
    }
}


void updateParams(void)
{
    if( updateParamIndex == 0U){
        /* update system information */
        updateSystemInfo();
    }else{
        /* update led temperature information LED0-LED3*/
        for (uint8_t i = 0U; i < LED_NUM; i++){
            updateLedTemperature((LedNum_t)i);
        }
    }
    if (++updateParamIndex > 2U){
        updateParamIndex = 0U;
    }
}


void MES_TaskHandler(void)
{
    switch(adcTaskState){
    case TASK_STATE_INIT:
      measureParamInit();
      ADC_GeneralInit();/*  init ADC */
      ADC_RegisterIRQ(measureDoneISR);
      /* start updating timer */
      SoftTimer_Start(&updateTimer);
      adcTaskState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      /* start parameters updating  */
      if (updateParamStart != 0U){
          updateParamStart = 0U;
          updateParams();
      }
      /*  start parameters measuring */
      if (measStart != 0U){
          measStart = 0U;
  //        measureParamStart();        //暂时屏蔽，开最高优化会出现 hardfault
      }
      break;
    default:
      break;
    }
}

/*static HWCFG_SFRS_t *cfg;*/
void measureParamInit(void)
{
    uint32_t crc = CRC32_GetRunTimeCRC32((uint8_t *)HWCFG_SFRS->payload+4U ,508U);
    if (crc != HWCFG_SFRS->crc32){
        dataIntegrityError = TRUE;
    }
    /*cfg = (HWCFG_SFRS_t *)HWCFG_SFRS;*/
    
    /*  Vbap:1.188V  */
    uint16_t adc0V5 = HWCFG_SFRS->ADC_0V5_CODE;
    uint16_t adc1V0 = HWCFG_SFRS->ADC_1V0_CODE;
    adcResult.vChipTemp.coeff.coefficient = (1000 -500)*MEASURE_GAIN/((int32_t)adc1V0 - (int32_t)adc0V5);
    adcResult.vChipTemp.coeff.offset =  500*MEASURE_GAIN - adcResult.vChipTemp.coeff.coefficient*(int32_t)adc0V5;
    adcResult.vChipTemp.currCode = INVALID_PARAM;
    /* v = a*t + b;  a = 2.5162,  t = v/a - b/a;   gain = 1/a, offset = - b/a*/
    adcResult.tChip.coefficient = MEASURE_TEMP_GAIN;
    adcResult.tChip.offset      = 25*(int32_t)(1UL << MEASURE_TEMP_GAIN_POS) - MEASURE_TEMP_GAIN*(int32_t)HWCFG_SFRS->TSENSOR_mV_25C;
    
    /* battery volt*/
    uint16_t batt13V5 = HWCFG_SFRS->VBAT_CODE_13V5;
    uint16_t batt8V0 = HWCFG_SFRS->VBAT_CODE_8V0;
    adcResult.vBatt.coeff.coefficient = (int32_t)(13500 - 8000)*MEASURE_GAIN/((int32_t)batt13V5 - (int32_t)batt8V0);
    adcResult.vBatt.coeff.offset      = (int32_t)(8000*MEASURE_GAIN) - adcResult.vBatt.coeff.coefficient*(int32_t)batt8V0;
    adcResult.vBatt.currCode    = INVALID_PARAM;
    
    /* led parameters init */
    uint16_t codeH,codeL;
    uint16_t codeHVolt,codeLVolt;
    for (uint8_t i = 0; i < LED_NUM; i++){
        codeH = HWCFG_SFRS->LED[PHY_CHANNEL_R].LED_PN4P0V_CODE;
        codeL = HWCFG_SFRS->LED[PHY_CHANNEL_R].LED_PN1P5V_CODE;
        codeHVolt = HWCFG_SFRS->LED[PHY_CHANNEL_R].LED_PN4P0V_VOLT;
        codeLVolt = HWCFG_SFRS->LED[PHY_CHANNEL_R].LED_PN1P5V_VOLT;
        adcResult.vLedPN[0][0].coeff.coefficient = ((int32_t)codeHVolt - (int32_t)codeLVolt)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[0][0].coeff.offset      = (int32_t)codeLVolt*MEASURE_GAIN - adcResult.vLedPN[i][0].coeff.coefficient*(int32_t)codeL;
        adcResult.vLedPN[0][0].currCode = INVALID_PARAM;
        adcResult.vLedPN[0][0].target = (int16_t)INVALID_VOLT;
        
        codeH = HWCFG_SFRS->LED[PHY_CHANNEL_G].LED_PN4P0V_CODE;
        codeL = HWCFG_SFRS->LED[PHY_CHANNEL_G].LED_PN1P5V_CODE;
        
        codeHVolt = HWCFG_SFRS->LED[PHY_CHANNEL_G].LED_PN4P0V_VOLT;
        codeLVolt = HWCFG_SFRS->LED[PHY_CHANNEL_G].LED_PN1P5V_VOLT;
        
        adcResult.vLedPN[0][1].coeff.coefficient = ((int32_t)codeHVolt - (int32_t)codeLVolt)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[0][1].coeff.offset      = (int32_t)codeLVolt*MEASURE_GAIN - adcResult.vLedPN[i][1].coeff.coefficient*(int32_t)codeL;
        adcResult.vLedPN[0][1].currCode = INVALID_PARAM;
        adcResult.vLedPN[0][1].target   = (int16_t)INVALID_VOLT;
        
        codeH = HWCFG_SFRS->LED[PHY_CHANNEL_B].LED_PN4P0V_CODE;
        codeL = HWCFG_SFRS->LED[PHY_CHANNEL_B].LED_PN1P5V_CODE;
        
        codeHVolt = HWCFG_SFRS->LED[PHY_CHANNEL_B].LED_PN4P0V_VOLT;
        codeLVolt = HWCFG_SFRS->LED[PHY_CHANNEL_B].LED_PN1P5V_VOLT;
        
        adcResult.vLedPN[0][2].coeff.coefficient = ((int32_t)codeHVolt - (int32_t)codeLVolt)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[0][2].coeff.offset      = (int32_t)codeLVolt*MEASURE_GAIN - adcResult.vLedPN[i][2].coeff.coefficient*(int32_t)codeL;
        adcResult.vLedPN[0][2].currCode = INVALID_PARAM;
        adcResult.vLedPN[0][2].target   = (int16_t)INVALID_VOLT;
        
    }
}

/*
 *  Get factory calibration data state
 *  @return 0: calibration data OK, 1 : calibration data error
 */
uint8_t MES_DataIntegrityError(void)
{
    return dataIntegrityError;
  
}

/*
 *  Get battery voltage in mV
 *  @param [out] battVolt: 0-65535
 *  @return 0: get voltage success, other : failed
 */
int8_t MES_GetBatteryVolt(uint16_t *battVolt)
{
    int8_t result = -1;
    if (adcResult.vBatt.currCode != INVALID_PARAM){
        *battVolt = (uint16_t)adcResult.vBatt.target;
        result = 0;
    }
    return result;
}


int16_t MES_GetChipTemperature(int16_t *chipTemperature)
{
    int16_t result = -1;
    if (adcResult.vChipTemp.currCode != INVALID_PARAM){
        *chipTemperature = adcResult.chipTemperature;
        result = 0;
    }
    return result;
}


/*  */
ColorTemperature_t MES_GetLedTemperature(LedNum_t ledNo)
{
    ColorTemperature_t temperature;
    temperature.value[0] = (int8_t)adcResult.ledPNTemperature[ledNo][0];
    temperature.value[1] = (int8_t)adcResult.ledPNTemperature[ledNo][1];
    temperature.value[2] = (int8_t)adcResult.ledPNTemperature[ledNo][2];
    return temperature;
}

 int8_t MES_GetLedRGBTemperature(LedNum_t index, int8_t *rTemp,int8_t *gTemp,int8_t *bTemp)
 {
    *rTemp = (int8_t)adcResult.ledPNTemperature[index][0];
    *gTemp = (int8_t)adcResult.ledPNTemperature[index][1];
    *bTemp = (int8_t)adcResult.ledPNTemperature[index][2];
    return 0;
 }

#define OVERSAMPLING_CYCLES_POS     (6U) // 设成0,4,5,6 记录10分钟数据
#define OVERSAMPLING_CYCLES         (1U << OVERSAMPLING_CYCLES_POS)
void MES_MnftGetLedInfo(LedNum_t ledNo, uint16_t *pnRVolt,uint16_t *pnGVolt,uint16_t *pnBVolt)
{
    int32_t sLedCode = 0;
    uint32_t uLedCode[3];
    for(uint8_t i = 0U; i < LED_NUM; i++){
        (void)CLM_SetPWMs((LedNum_t)i,0U, 0U, 0U, 0U);  /* turn off all of channels */
    }
    while(PWM_UpdateFinished() != TRUE){}
    
    for (uint8_t led = 0U; led < 3U; led++){
        uLedCode[led] = 0U;
        ADC_Init(ADC_MEASURE_ITEM_LED,(uint8_t)0,(LedType_t)led,SYNC_MODE_SOFT_INPUT);
        for (uint16_t i = 0U; i < OVERSAMPLING_CYCLES; i++){
            ADC_Start();
            adcConvertDone = 0U;
            ADC_SFRS->CONF.SWSYNCIN = 1; /* soft trigger for adc conversion   */
            ADC_SFRS->CONF.SWSYNCIN = 0;
            while(adcConvertDone == 0U){}
            uLedCode[led] += measGeneralAdcCode[2];
        }
        uLedCode[led] = (uLedCode[led] >> OVERSAMPLING_CYCLES_POS);
        sLedCode = (int32_t)uLedCode[led];
        sLedCode = sLedCode* adcResult.vLedPN[0][led].coeff.coefficient + adcResult.vLedPN[0][led].coeff.offset;
        uLedCode[led] = (uint32_t)sLedCode;
    }
    *pnRVolt = (uint16_t)(uLedCode[0] >> MEASURE_GAIN_POS);
    *pnGVolt = (uint16_t)(uLedCode[1] >> MEASURE_GAIN_POS);
    *pnBVolt = (uint16_t)(uLedCode[2] >> MEASURE_GAIN_POS);
    (void)sLedCode;
}


/*
 *  Get Runtime Led PN voltage in mV
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return :none
 */
void MES_GetRunTimeLedPNVolt(LedNum_t ledIndex,uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB)
{     
    *pnVoltR = (uint16_t)adcResult.vLedPN[0][0].target;
    *pnVoltG = (uint16_t)adcResult.vLedPN[0][1].target;
    *pnVoltB = (uint16_t)adcResult.vLedPN[0][2].target;
}













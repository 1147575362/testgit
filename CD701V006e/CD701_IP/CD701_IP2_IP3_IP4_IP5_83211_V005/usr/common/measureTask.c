/**
 * @copyright 2020 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 * @file measureTask.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/07/18
 */

#include <crc32.h>
#include <measureTask.h>
#include <gpio_device.h>

#define AVERAGE_MEASURE_POS (4U)
#define AVERAGE_MEASURE_GAIN (1U << AVERAGE_MEASURE_POS)

#define MEASURE_GAIN_POS 12U
#define MEASURE_GAIN 4096 /*(1UL << MEASURE_GAIN_POS)*/

#define MEASURE_TEMP_GAIN_POS 14U
#define MEASURE_TEMP_GAIN 7447 /*16384*0.4546*/

#define UPDATE_TIEMS_LEDS (1U << 0)
#define UPDATE_TIEMS_BATT_BUCK_TEMP (1U << 1)

typedef struct
{
    int32_t coefficient;
    int32_t offset;
} coeffParam_t;

typedef struct
{
    uint32_t currCode;
    uint16_t buff[AVERAGE_MEASURE_GAIN];
    uint16_t count;
    coeffParam_t coeff;
    int16_t target;
} AdcDatabuff_t;

typedef struct
{
    AdcDatabuff_t vBatt;
    AdcDatabuff_t vBuck;
    AdcDatabuff_t vLedPN[LED_NUM][3];
    AdcDatabuff_t vChipTemp;
    coeffParam_t tChip;
    int16_t chipTemperature;
    int16_t ledPNTemperature[LED_NUM][3];
} AdcResult_t;

static uint8_t dataIntegrityError = FALSE;

void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex, uint16_t *buff);
static TaskState_t adcTaskState = TASK_STATE_INIT;
static AdcResult_t adcResult;
void measureParamInit(void);
void adcCodeUpdate(void);

static uint8_t updateItems = UPDATE_TIEMS_BATT_BUCK_TEMP;
void updateTimerExpired(SoftTimer_t *timer);

int16_t get_volt(AdcDatabuff_t *const param);
int16_t get_chip_temperature(int16_t vTemp);
int8_t get_led_temperature(TempParams_t const table[], int16_t deltaPNVolt);
void updateLedsTemperature(void);
void updateParams(void);

void updateSystemInfo(void);

static volatile uint32_t adcConvertDone = 0U;

static SoftTimer_t updateTimer = {
    .mode = TIMER_PERIODIC_MODE,
    .interval = 50U,
    .handler = updateTimerExpired};

void updateTimerExpired(SoftTimer_t *timer)
{
    static uint8_t measWaitCount = 0U;
    measWaitCount++;
    if (measWaitCount <= LED_NUM)
    {
        updateItems |= UPDATE_TIEMS_LEDS;
    }
    else
    {
        updateItems |= UPDATE_TIEMS_BATT_BUCK_TEMP;
        measWaitCount = 0;
    }
    TM_PostTask(TASK_ID_ADC_MEASURE);
}

void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex, uint16_t *buff)
{
    if (*original == INVALID_PARAM)
    {
        *original = newData;
        *bufferIndex = 0;
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++)
        {
            buff[i] = newData;
        }
    }
    else
    {
        buff[*bufferIndex] = newData;
    }
    (*bufferIndex)++;
    if ((*bufferIndex) >= AVERAGE_MEASURE_GAIN)
    {
        (*bufferIndex) = 0U;
    }
}

int16_t get_volt(AdcDatabuff_t *const param)
{
    int16_t volt = (int16_t)INVALID_VOLT;
    int32_t sVolt;
    uint32_t uVolt;
    uint32_t sum = 0;
    if (param->currCode != INVALID_PARAM)
    {
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++)
        {
            sum += param->buff[i];
        }
        sum = sum >> AVERAGE_MEASURE_POS;
        sVolt = param->coeff.coefficient * (int32_t)sum + param->coeff.offset;
        uVolt = (uint32_t)sVolt;
        uVolt = uVolt >> MEASURE_GAIN_POS;
        volt = (int16_t)uVolt;
    }
    return volt;
}

void bubbleSort(uint16_t arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
uint16_t sort_buff[AVERAGE_MEASURE_GAIN];
int16_t get_led_volt(AdcDatabuff_t *const param)//2024.02.28 modify
{
    int16_t volt = (int16_t)INVALID_VOLT;
    int32_t sVolt;
    uint32_t uVolt;
    uint32_t sum = 0;
    if (param->currCode != INVALID_PARAM)
    {
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++)
        {
            sort_buff[i] = param->buff[i];
        }
        bubbleSort(sort_buff, AVERAGE_MEASURE_GAIN);
        sum = sum >> AVERAGE_MEASURE_POS;
        for (uint8_t i = 4U; i < AVERAGE_MEASURE_GAIN - 4U; i++)
        {
            sum += sort_buff[i];
        }
        sum = sum / (AVERAGE_MEASURE_GAIN - 8U);

        sVolt = param->coeff.coefficient * (int32_t)sum + param->coeff.offset;
        uVolt = (uint32_t)sVolt;
        uVolt = uVolt >> MEASURE_GAIN_POS;
        volt = (int16_t)uVolt;
    }
    return volt;
}
int16_t get_chip_temperature(int16_t vTemp)
{
    int32_t stemp;
    uint32_t uTemp;
    uint8_t negative = 0U;
    stemp = adcResult.tChip.coefficient * vTemp + adcResult.tChip.offset;
    if (stemp < 0)
    {
        stemp = -stemp;
        negative = 1U;
    }
    uTemp = (uint32_t)stemp;
    uTemp = uTemp >> MEASURE_TEMP_GAIN_POS;
    stemp = (int32_t)uTemp;
    if (negative != 0U)
    {
        stemp = -stemp;
    }
    return (int16_t)stemp;
}

void updateSystemInfo(void)
{
    /* calculate chip temperature */
    adcResult.vChipTemp.target = get_led_volt(&adcResult.vChipTemp);
    adcResult.chipTemperature = get_chip_temperature(adcResult.vChipTemp.target);
    /* calculate battery volt */
    adcResult.vBatt.target = get_led_volt(&adcResult.vBatt);
    /* calculate buck volt */
    adcResult.vBuck.target = get_led_volt(&adcResult.vBuck);
}

int8_t get_led_temperature(TempParams_t const table[], int16_t deltaPNVolt)
{
    int8_t temperature;
    uint8_t minPos = 0U;
    uint8_t maxPos = TEMP_CMP_NUM_MAX - 1U;
    uint8_t tempPos = ((TEMP_CMP_NUM_MAX - 1U) >> 1U);
    for (;;)
    {
        if (deltaPNVolt > table[tempPos].deltaVoltPN)
        {
            maxPos = tempPos;
        }
        else if (deltaPNVolt < table[tempPos].deltaVoltPN)
        {
            minPos = tempPos;
        }
        else
        {
            break;
        }
        tempPos = (minPos + maxPos) >> 1U;
        if ((minPos + 1U) >= maxPos)
        {
            break;
        }
    }
    if (deltaPNVolt <= table[TEMP_CMP_NUM_MAX - 1U].deltaVoltPN)
    {
        tempPos = TEMP_CMP_NUM_MAX - 1U;
    }
    tempPos = tempPos << DELTA_TEMP_POS;
    temperature = (int8_t)tempPos;
    temperature = temperature + MINIMUM_TEMP;
    return temperature;
}
/*

*/
static int r_iir_filter(int data)//2024.02.28 modify
{
#define filter_len  10
    static unsigned char state = 0;
    static long int total = (filter_len >> 1);
    int per;
    if (state)
    {
        total += data;
    }
    else
    {
        total += data * filter_len;
        state = 1;
    }

    per = total / filter_len;
    total -= per;

    return per;
}

static int g_iir_filter(int data)//2024.02.28 modify
{
#define filter_len  10
    static unsigned char state = 0;
    static long int total = (filter_len >> 1);
    int per;
    if (state)
    {
        total += data;
    }
    else
    {
        total += data * filter_len;
        state = 1;
    }

    per = total / filter_len;
    total -= per;
    
    return per;
}

static int b_iir_filter(int data)//2024.02.28 modify
{
#define filter_len  10
    static unsigned char state = 0;
    static long int total = (filter_len >> 1);
    int per;
    if (state)
    {
        total += data;
    }
    else
    {
        total += data * filter_len;
        state = 1;
    }

    per = total / filter_len;
    total -= per;
    
    return per;
}
void updateLedsTemperature(void)
{
    static uint8_t ledIndex = 0U;
    int16_t typicalVolt[3];
    ColorTemperature_t temperature;
    temperature.value[0] = ROOM_TEMPERATURE;
    temperature.value[1] = ROOM_TEMPERATURE;
    temperature.value[2] = ROOM_TEMPERATURE;

    (void)CLM_GetLedPNVolts((LedNum_t)ledIndex, temperature, &typicalVolt[0], &typicalVolt[1], &typicalVolt[2]);
    for (uint8_t i = 0U; i < 3U; i++)
    {
        adcResult.vLedPN[ledIndex][i].target = get_led_volt(&adcResult.vLedPN[ledIndex][i]);
        if (adcResult.vLedPN[ledIndex][i].target != (int16_t)INVALID_VOLT)
        {
            if (i == (uint8_t)LED_RED)
            {
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableRed(), (adcResult.vLedPN[ledIndex][i].target - typicalVolt[0]));
                adcResult.ledPNTemperature[ledIndex][i] = r_iir_filter(adcResult.ledPNTemperature[ledIndex][i]);//2024.02.28 modify
            }
            else if (i == (uint8_t)LED_GREEN)
            {
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableGreen(), (adcResult.vLedPN[ledIndex][i].target - typicalVolt[1]));
                adcResult.ledPNTemperature[ledIndex][i] = g_iir_filter(adcResult.ledPNTemperature[ledIndex][i]);//2024.02.28 modify
            }
            else
            {
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableBlue(), (adcResult.vLedPN[ledIndex][i].target - typicalVolt[2]));
                adcResult.ledPNTemperature[ledIndex][i] = b_iir_filter(adcResult.ledPNTemperature[ledIndex][i]);//2024.02.28 modify
            }
        }
        else
        {
            adcResult.ledPNTemperature[ledIndex][i] = ROOM_TEMPERATURE;
        }
    }
    if (++ledIndex >= LED_NUM)
    {
        ledIndex = 0U;
    }
}

void adcCodeUpdate(void)
{
    uint16_t pnCodeR, pnCodeG, pnCodeB;
    uint16_t code = ADC_GetVTempCode();
    if (code != INVALID_ADC_CODE)
    {
        add_data_to_buff(&adcResult.vChipTemp.currCode, code, &adcResult.vChipTemp.count, adcResult.vChipTemp.buff);
    }
    code = ADC_GetVBattCode();
    if (code != INVALID_ADC_CODE)
    {
        add_data_to_buff(&adcResult.vBatt.currCode, code, &adcResult.vBatt.count, adcResult.vBatt.buff);
    }

    code = ADC_GetVBuckCode();
    if (code != INVALID_ADC_CODE)
    {
        add_data_to_buff(&adcResult.vBuck.currCode, code, &adcResult.vBuck.count, adcResult.vBuck.buff);
    }

    for (uint8_t i = 0; i < LED_NUM; i++)
    {
        (void)ADC_GetLedPNCode(i, &pnCodeR, &pnCodeG, &pnCodeB);
        if ((pnCodeR != INVALID_ADC_CODE) && (pnCodeG != INVALID_ADC_CODE) && (pnCodeB != INVALID_ADC_CODE))
        {
            add_data_to_buff(&adcResult.vLedPN[i][LED_RED].currCode, pnCodeR, &adcResult.vLedPN[i][LED_RED].count, adcResult.vLedPN[i][LED_RED].buff);
            add_data_to_buff(&adcResult.vLedPN[i][LED_GREEN].currCode, pnCodeG, &adcResult.vLedPN[i][LED_GREEN].count, adcResult.vLedPN[i][LED_GREEN].buff);
            add_data_to_buff(&adcResult.vLedPN[i][LED_BLUE].currCode, pnCodeB, &adcResult.vLedPN[i][LED_BLUE].count, adcResult.vLedPN[i][LED_BLUE].buff);
        }
        else
        {
            adcResult.vLedPN[i][LED_RED].currCode = INVALID_PARAM;
            adcResult.vLedPN[i][LED_GREEN].currCode = INVALID_PARAM;
            adcResult.vLedPN[i][LED_BLUE].currCode = INVALID_PARAM;
        }
    }
}

void MES_TaskHandler(void)
{
    switch (adcTaskState)
    {
    case TASK_STATE_INIT:
        measureParamInit();
        SoftTimer_Start(&updateTimer);
        adcTaskState = TASK_STATE_ACTIVE;
        break;
    case TASK_STATE_ACTIVE:
        if ((updateItems & UPDATE_TIEMS_LEDS) == UPDATE_TIEMS_LEDS)
        {
            updateItems &= (~UPDATE_TIEMS_LEDS);
            updateLedsTemperature();
        }
        else if ((updateItems & UPDATE_TIEMS_BATT_BUCK_TEMP) == UPDATE_TIEMS_BATT_BUCK_TEMP)
        {
            updateItems &= (~UPDATE_TIEMS_BATT_BUCK_TEMP);
            updateSystemInfo();
        }
        else
        {
            /* purpose to empty */
        }
        adcCodeUpdate();
        break;
    default:
        break;
    }
}

void measureParamInit(void)
{
    uint32_t crc = CRC32_GetRunTimeCRC32((uint8_t *)HWCFG_SFRS->payload + 4U, 508U);
    if (crc != HWCFG_SFRS->crc32)
    {
        dataIntegrityError = TRUE;
    }
    /*  Vbap:1.188V  */
    uint16_t adc0V5 = HWCFG_SFRS->ADC_0V5;
    uint16_t adc1V0 = HWCFG_SFRS->ADC_1V0;
    adcResult.vChipTemp.coeff.coefficient = (1000 - 500) * MEASURE_GAIN / ((int32_t)adc1V0 - (int32_t)adc0V5);
    adcResult.vChipTemp.coeff.offset = 500 * MEASURE_GAIN - adcResult.vChipTemp.coeff.coefficient * (int32_t)adc0V5;
    adcResult.vChipTemp.currCode = INVALID_PARAM;
    /* v = a*t + b;  a = 2.5162,  t = v/a - b/a;   gain = 1/a, offset = - b/a*/
    adcResult.tChip.coefficient = MEASURE_TEMP_GAIN;
    adcResult.tChip.offset = 25 * (int32_t)(1UL << MEASURE_TEMP_GAIN_POS) - MEASURE_TEMP_GAIN * (int32_t)HWCFG_SFRS->TSENSOR_mV_25C;

    /* battery volt*/
    uint16_t batt13V5 = HWCFG_SFRS->VBAT_CODE_13V5;
    uint16_t batt8V0 = HWCFG_SFRS->VBAT_CODE_8V0;

    if (batt8V0 == 0xFFFFU)
    {
        adcResult.vBatt.coeff.coefficient = (13500 - 0) * MEASURE_GAIN / ((int32_t)batt13V5 - 0);
        adcResult.vBatt.coeff.offset = 0;
    }
    else
    {
        adcResult.vBatt.coeff.coefficient = (13500 - 8000) * MEASURE_GAIN / ((int32_t)batt13V5 - (int32_t)batt8V0);
        adcResult.vBatt.coeff.offset = 8000 * MEASURE_GAIN - adcResult.vBatt.coeff.coefficient * (int32_t)batt8V0;
    }

    adcResult.vBatt.currCode = INVALID_PARAM;
    /* buck volt*/
    uint16_t buck5V0 = HWCFG_SFRS->BUCK_5V_ADC_CODE;
    adcResult.vBuck.coeff.coefficient = (5000 - 0) * MEASURE_GAIN / ((int32_t)buck5V0 - 0);
    adcResult.vBuck.coeff.offset = 0;
    adcResult.vBuck.currCode = INVALID_PARAM;

    /* led parameters init */
    uint16_t codeH, codeL;
    uint8_t index;
    for (uint8_t i = 0; i < LED_NUM; i++)
    {
        index = i / 3U;
        codeH = HWCFG_SFRS->LED[(uint8_t)PHY_CHANNEL_R + 3U * index].PN_CODE_VOLT_4000mV;
        codeL = HWCFG_SFRS->LED[(uint8_t)PHY_CHANNEL_R + 3U * index].PN_CODE_VOLT_1500mV;
        adcResult.vLedPN[i][0].coeff.coefficient = (4000 - 1500) * MEASURE_GAIN / ((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][0].coeff.offset = 1500 * MEASURE_GAIN - adcResult.vLedPN[i][0].coeff.coefficient * (int32_t)codeL;
        adcResult.vLedPN[i][0].currCode = INVALID_PARAM;

        codeH = HWCFG_SFRS->LED[(uint8_t)PHY_CHANNEL_G + 3U * index].PN_CODE_VOLT_4000mV;
        codeL = HWCFG_SFRS->LED[(uint8_t)PHY_CHANNEL_G + 3U * index].PN_CODE_VOLT_1500mV;
        adcResult.vLedPN[i][1].coeff.coefficient = (4000 - 1500) * MEASURE_GAIN / ((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][1].coeff.offset = 1500 * MEASURE_GAIN - adcResult.vLedPN[i][1].coeff.coefficient * (int32_t)codeL;
        adcResult.vLedPN[i][1].currCode = INVALID_PARAM;

        codeH = HWCFG_SFRS->LED[(uint8_t)PHY_CHANNEL_B + 3U * index].PN_CODE_VOLT_4000mV;
        codeL = HWCFG_SFRS->LED[(uint8_t)PHY_CHANNEL_B + 3U * index].PN_CODE_VOLT_1500mV;
        adcResult.vLedPN[i][2].coeff.coefficient = (4000 - 1500) * MEASURE_GAIN / ((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][2].coeff.offset = 1500 * MEASURE_GAIN - adcResult.vLedPN[i][2].coeff.coefficient * (int32_t)codeL;
        adcResult.vLedPN[i][2].currCode = INVALID_PARAM;
    }

    for (uint8_t led = 0U; led < LED_NUM; led++)
    {
        for (uint8_t channel = 0U; channel < 3U; channel++)
        {
            adcResult.vLedPN[led][channel].target = (int16_t)INVALID_VOLT;
            adcResult.vLedPN[led][channel].currCode = INVALID_PARAM;
            adcResult.ledPNTemperature[led][channel] = ROOM_TEMPERATURE;
        }
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
    if (adcResult.vBatt.currCode != INVALID_PARAM)
    {
        *battVolt = (uint16_t)adcResult.vBatt.target;
        result = 0;
    }
    return result;
}

/*
 *  Get buck voltage in mV
 *  @param [out] buckVolt: 0-65535
 *  @return 0: get voltage success, other : failed
 */
int8_t MES_GetBuckVolt(uint16_t *buckVolt)
{
    int8_t result = -1;
    if (adcResult.vBuck.currCode != INVALID_PARAM)
    {
        *buckVolt = (uint16_t)adcResult.vBuck.target;
        result = 0;
    }
    return result;
}

int16_t MES_GetChipTemperature(int16_t *chipTemperature)
{
    int8_t result = -1;
    if (adcResult.vChipTemp.currCode != INVALID_PARAM)
    {
        *chipTemperature = adcResult.chipTemperature;
        result = 0;
    }
    return result;
}

/*
 *  Get Runtime Led PN voltage in mV
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return :none
 */
ColorTemperature_t MES_GetLedTemperature(LedNum_t ledNo)
{
    ColorTemperature_t temperature;
    temperature.value[0] = (int8_t)adcResult.ledPNTemperature[ledNo][0];
    temperature.value[1] = (int8_t)adcResult.ledPNTemperature[ledNo][1];
    temperature.value[2] = (int8_t)adcResult.ledPNTemperature[ledNo][2];
    return temperature;
}
/*
 *  Get Runtime Led temperature in C
 *  @param [out]
 *  @return :0
 */
int8_t MES_GetLedRGBTemperature(LedNum_t index, int8_t *rTemp, int8_t *gTemp, int8_t *bTemp)
{
    *rTemp = (int8_t)adcResult.ledPNTemperature[index][0];
    *gTemp = (int8_t)adcResult.ledPNTemperature[index][1];
    *bTemp = (int8_t)adcResult.ledPNTemperature[index][2];
    return 0;
}

/*
 *  Get Runtime Led PN voltage in mV
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return :none
 */
void MES_GetRunTimeLedPNVolt(LedNum_t ledIndex, uint16_t *pnVoltR, uint16_t *pnVoltG, uint16_t *pnVoltB)
{
    *pnVoltR = (uint16_t)adcResult.vLedPN[ledIndex][0].target;
    *pnVoltG = (uint16_t)adcResult.vLedPN[ledIndex][1].target;
    *pnVoltB = (uint16_t)adcResult.vLedPN[ledIndex][2].target;
}

void MES_MnftGetLedInfo(LedNum_t ledNo, uint16_t *pnRVolt, uint16_t *pnGVolt, uint16_t *pnBVolt)
{
    int32_t sLedCode = 0;
    uint32_t uLedCode[3];
    uint8_t led_no = (uint8_t)ledNo;
    uint8_t lastCHSEQ;
    uint8_t lastCH2;
    uint8_t no = 0U;
    if (led_no < LED_NUM)
    {
        ADC_UnregisterIRQ();
        lastCHSEQ = ADC_SFRS->CNTRL.CHSEQ;
        lastCH2 = ADC_REG_CNTRL.CH2SEL;
        no = led_no % 3U;
        if (no == 0U)
        {
            GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6), (1U << GPIO_PORT_2)); // 20240119
        }
        else if (no == 1U)
        {
            GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6), (1U << GPIO_PORT_3)); // 20240119
        }
        else
        {
            GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6), (1U << GPIO_PORT_6));
        }
        PWM_Reset();
        while (PWM_UpdateFinished() == TRUE)
        {
        }
        ADC_SFRS->CONF.SYNCENA = (uint8_t)SYNC_MODE_SOFT_INPUT;
        ADC_SFRS->CNTRL.CHSEQ = (uint8_t)ADC_CH2_THEN_CH1_THEN_CH3;
        for (uint8_t channel = 0U; channel < 3U; channel++)
        {
            uLedCode[channel] = 0U;
            ADC_REG_CNTRL.CH2SEL = (led_no / 3U) * 3U + channel;
            for (uint16_t i = 0U; i < 16U; i++)
            {
                ADC_SFRS->CNTRL.IRQCLR = 1U;
                ADC_SFRS->CNTRL.CONVERT = 1U;
                ADC_SFRS->CONF.SWSYNCIN = 1; /* soft trigger for adc conversion   */
                ADC_SFRS->CONF.SWSYNCIN = 0;
                while (ADC_REG_STATUS.CONVDONE == 0U)
                {
                }
                uLedCode[channel] += ADC_SFRS->DATA2;
            }
            uLedCode[channel] = (uLedCode[channel] >> 4);
            sLedCode = (int32_t)uLedCode[channel];
            sLedCode = sLedCode * adcResult.vLedPN[led_no / 3U][channel].coeff.coefficient + adcResult.vLedPN[led_no / 3U][channel].coeff.offset;
            uLedCode[channel] = (uint32_t)sLedCode;
        }
        *pnRVolt = (uint16_t)(uLedCode[PHY_CHANNEL_R] >> MEASURE_GAIN_POS);
        *pnGVolt = (uint16_t)(uLedCode[PHY_CHANNEL_G] >> MEASURE_GAIN_POS);
        *pnBVolt = (uint16_t)(uLedCode[PHY_CHANNEL_B] >> MEASURE_GAIN_POS);
        (void)sLedCode;
        ADC_SFRS->CONF.SYNCENA = (uint8_t)SYNC_MODE_PWM_PERIOD;
        ADC_SFRS->CNTRL.CHSEQ = lastCHSEQ;
        ADC_REG_CNTRL.CH2SEL = lastCH2;
        ADC_RegisterIRQ();
        ADC_SFRS->CNTRL.CONVERT = 1U; // 2024.01.19 modify 颜色校准参数
    }
}

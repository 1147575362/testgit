/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pwm_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */


#include <stdint.h>
#include <string.h>
#include <rugbyPro.h>
#include <errno.h>
#include <pwm_device.h>
#include <appConfig.h>
#include <isrfuncs.h>
#include <adc_device.h>
#include <gpio_device.h>

#if defined PWM_SFRS

/* 128us for PN voltage measurement*/
#define MAX_MEASURE_TIME_TICKS          (MAIN_CPU_CLOCK * 128U)   /* us  */
#define MAX_DEBUNCE_TIME_TICKS          (MAIN_CPU_CLOCK * 32U) /* us  */

#define MAX_PWM_DUTY                    (PWM_VALUE_MAX - MAX_MEASURE_TIME_TICKS - MAX_DEBUNCE_TIME_TICKS - 1U)

#define LED_CONVERT_TYPE_VBUCK_LED_VBAT         (0U)
#define LED_CONVERT_TYPE_VBUCK_VBAT             (1U)
#define LED_CONVERT_TYPE_VTEMP_VBAT             (2U)

static volatile uint16_t currPWMMatchValue[LED_NUM][3U];
static uint32_t LedPowerSwitch = 2U;

static volatile uint8_t ledIsActive[LED_NUM];
static volatile uint16_t vLedPNCode[LED_NUM][3];
static volatile uint16_t vBuckCode;
static volatile uint16_t vBattCode;
static volatile uint16_t vTempCode;
static volatile uint8_t currLedNo  = 0U;
static volatile uint8_t ledConvertType = LED_CONVERT_TYPE_VBUCK_VBAT;

void ADC_Handler(void)
{
    static uint8_t ledChannel = 0U;
    static uint8_t ledNo = 0U;
    static uint16_t ledConvertCount = 0U;
    static uint8_t ledPhyChannel = 0U;
  
#if (LED_NUM < 17U) || (LED_NUM > 24U)
#error LED_NUM must be in the range [17,24]
#else
    if (LedPowerSwitch == 0U){
        LedPowerSwitch = 1U;
  #if LED_BOARD_TYPE == BOARD_TYPE_EVB_24LED
        GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6), (1U << GPIO_PORT_6)); /* 3*n*/
  #endif
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*0U].PRISE  = currPWMMatchValue[0][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*0U].PRISE  = currPWMMatchValue[0][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*0U].PRISE  = currPWMMatchValue[0][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*1U].PFALL  = currPWMMatchValue[3][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*1U].PFALL  = currPWMMatchValue[3][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*1U].PFALL  = currPWMMatchValue[3][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*2U].PRISE  = currPWMMatchValue[6][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*2U].PRISE  = currPWMMatchValue[6][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*2U].PRISE  = currPWMMatchValue[6][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*3U].PFALL  = currPWMMatchValue[9][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*3U].PFALL  = currPWMMatchValue[9][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*3U].PFALL  = currPWMMatchValue[9][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*4U].PRISE  = currPWMMatchValue[12][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*4U].PRISE  = currPWMMatchValue[12][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*4U].PRISE  = currPWMMatchValue[12][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*5U].PFALL  = currPWMMatchValue[15][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*5U].PFALL  = currPWMMatchValue[15][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*5U].PFALL  = currPWMMatchValue[15][PHY_CHANNEL_B];

  #if LED_NUM > 18U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*6U].PRISE  = currPWMMatchValue[18][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*6U].PRISE  = currPWMMatchValue[18][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*6U].PRISE  = currPWMMatchValue[18][PHY_CHANNEL_B];
  #endif
  #if LED_NUM > 21U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*7U].PFALL  = currPWMMatchValue[21][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*7U].PFALL  = currPWMMatchValue[21][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*7U].PFALL  = currPWMMatchValue[21][PHY_CHANNEL_B];
  #endif
    }else if (LedPowerSwitch == 1U){
        LedPowerSwitch = 2U;
  #if  LED_BOARD_TYPE == BOARD_TYPE_EVB_24LED
        GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6), (1U << GPIO_PORT_2));  /* 3*n+1 */
  #endif
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*0U].PRISE  = currPWMMatchValue[1][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*0U].PRISE  = currPWMMatchValue[1][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*0U].PRISE  = currPWMMatchValue[1][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*1U].PFALL  = currPWMMatchValue[4][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*1U].PFALL  = currPWMMatchValue[4][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*1U].PFALL  = currPWMMatchValue[4][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*2U].PRISE  = currPWMMatchValue[7][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*2U].PRISE  = currPWMMatchValue[7][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*2U].PRISE  = currPWMMatchValue[7][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*3U].PFALL  = currPWMMatchValue[10][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*3U].PFALL  = currPWMMatchValue[10][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*3U].PFALL  = currPWMMatchValue[10][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*4U].PRISE  = currPWMMatchValue[13][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*4U].PRISE  = currPWMMatchValue[13][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*4U].PRISE  = currPWMMatchValue[13][PHY_CHANNEL_B];

  #if LED_NUM > 16U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*5U].PFALL  = currPWMMatchValue[16][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*5U].PFALL  = currPWMMatchValue[16][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*5U].PFALL  = currPWMMatchValue[16][PHY_CHANNEL_B];
  #endif

  #if LED_NUM > 19U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*6U].PRISE  = currPWMMatchValue[19][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*6U].PRISE  = currPWMMatchValue[19][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*6U].PRISE  = currPWMMatchValue[19][PHY_CHANNEL_B];
  #endif

  #if LED_NUM > 21U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*7U].PFALL  = currPWMMatchValue[22][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*7U].PFALL  = currPWMMatchValue[22][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*7U].PFALL  = currPWMMatchValue[22][PHY_CHANNEL_B];
  #endif
    }else{
        LedPowerSwitch = 0U;
  #if  LED_BOARD_TYPE == BOARD_TYPE_EVB_24LED
        GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6), (1U << GPIO_PORT_3));  /* 3*n+2 */
  #endif
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*0U].PRISE  = currPWMMatchValue[2][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*0U].PRISE  = currPWMMatchValue[2][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*0U].PRISE  = currPWMMatchValue[2][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*1U].PFALL  = currPWMMatchValue[5][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*1U].PFALL  = currPWMMatchValue[5][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*1U].PFALL  = currPWMMatchValue[5][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*2U].PRISE  = currPWMMatchValue[8][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*2U].PRISE  = currPWMMatchValue[8][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*2U].PRISE  = currPWMMatchValue[8][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*3U].PFALL  = currPWMMatchValue[11][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*3U].PFALL  = currPWMMatchValue[11][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*3U].PFALL  = currPWMMatchValue[11][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*4U].PRISE  = currPWMMatchValue[14][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*4U].PRISE  = currPWMMatchValue[14][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*4U].PRISE  = currPWMMatchValue[14][PHY_CHANNEL_B];

  #if LED_NUM > 17U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*5U].PFALL  = currPWMMatchValue[17][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*5U].PFALL  = currPWMMatchValue[17][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*5U].PFALL  = currPWMMatchValue[17][PHY_CHANNEL_B];
  #endif

  #if LED_NUM > 20U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*6U].PRISE  = currPWMMatchValue[20][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*6U].PRISE  = currPWMMatchValue[20][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*6U].PRISE  = currPWMMatchValue[20][PHY_CHANNEL_B];
  #endif

  #if LED_NUM > 23U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*7U].PFALL  = currPWMMatchValue[23][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*7U].PFALL  = currPWMMatchValue[23][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*7U].PFALL  = currPWMMatchValue[23][PHY_CHANNEL_B];
  #endif
    }
#endif
    PWM_SFRS->UPDATE = 1U;
    
    if(ledConvertType == LED_CONVERT_TYPE_VBUCK_LED_VBAT){
        vBuckCode = ADC_SFRS->DATA1;
        vLedPNCode[ledNo][ledChannel]    = ADC_SFRS->DATA2;
        vBattCode  = ADC_SFRS->DATA0345;
    }else if (ledConvertType == LED_CONVERT_TYPE_VBUCK_VBAT){
        vBuckCode = ADC_SFRS->DATA1;
        vBattCode = ADC_SFRS->DATA0345;
    }else{
        vTempCode = ADC_SFRS->DATA1;
        vBattCode = ADC_SFRS->DATA0345;
        ADC_SFRS->CONF.SAMPCYC = 10U;
    }
    if (++ledNo >= LED_NUM){
        ledNo = 0U;
        if (++ledChannel >= 3U){
            ledChannel = 0U;
        }
    }
    ledPhyChannel = (ledNo/3U) * 3U +  ledChannel;
    
    if (++ledConvertCount <= 90U){
        if (ledIsActive[ledNo] == TRUE){
            ADC_REG_CNTRL_CH1_CH2 = (uint8_t)ADC_CH1_VH3_SEL_VBUCK + ( ledPhyChannel << 2);
            ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH2_THEN_CH1_THEN_CH3;
            ledConvertType = LED_CONVERT_TYPE_VBUCK_LED_VBAT;
        }else{
            ADC_REG_CNTRL_CH1_CH2 = (uint8_t)ADC_CH1_VH3_SEL_VBUCK + ( ledPhyChannel << 2);
            ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH3_THEN_CH1;
            ledConvertType = LED_CONVERT_TYPE_VBUCK_VBAT;
        }
    }else{
        ledConvertCount = 0U;
        ADC_REG_CNTRL_CH1_CH2 = (uint8_t)ADC_CH1_CH3_SEL_TEMP_SENSOR + ( ledPhyChannel << 2);
        ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH3_THEN_CH1;
        ADC_SFRS->CONF.SAMPCYC = 40U;
        ledConvertType = LED_CONVERT_TYPE_VTEMP_VBAT;
    }
    ADC_REG_CNTRL_CONVERT = 0xC1U;
    ADC_SFRS->CNTRL.IRQCLR = 1U;
}

void PWM_Handler(void)
{

}

/**
 * @brief The instance of PWM_SetRGBValue
 */
void PWM_SetRGBValue(uint8_t ledNo, uint16_t red, uint16_t green, uint16_t blue)
{
    uint8_t no;
    uint16_t r,g,b;
    r = (uint16_t)((red  *MAX_PWM_DUTY) >> 16);
    g = (uint16_t)((green*MAX_PWM_DUTY) >> 16);
    b = (uint16_t)((blue *MAX_PWM_DUTY) >> 16);
    
    if (red >= (65535U/256U) || green >= (65535U/256U) || blue >= (65535U/256U)){
        ledIsActive[ledNo] = TRUE;
    }else{
        ledIsActive[ledNo] = FALSE;
        vLedPNCode[ledNo][0] = INVALID_ADC_CODE;
        vLedPNCode[ledNo][1] = INVALID_ADC_CODE;
        vLedPNCode[ledNo][2] = INVALID_ADC_CODE;
    }
    no = ledNo/3U;
    if ((no - ((no >> 1) << 1U) ) == 0U){
        currPWMMatchValue[ledNo][PHY_CHANNEL_R] = (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - r;
        currPWMMatchValue[ledNo][PHY_CHANNEL_G] = (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - g;
        currPWMMatchValue[ledNo][PHY_CHANNEL_B] = (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - b;
    }else{
        currPWMMatchValue[ledNo][PHY_CHANNEL_R] = r + MAX_MEASURE_TIME_TICKS;
        currPWMMatchValue[ledNo][PHY_CHANNEL_G] = g + MAX_MEASURE_TIME_TICKS;
        currPWMMatchValue[ledNo][PHY_CHANNEL_B] = b + MAX_MEASURE_TIME_TICKS;
    }
}


/**
 * @brief The instance of PMW device
 */
void PWM_Init(void)
{
    for (uint8_t i = 0; i < LED_NUM; i++){
        vLedPNCode[i][0] = INVALID_ADC_CODE;
        vLedPNCode[i][1] = INVALID_ADC_CODE;
        vLedPNCode[i][2] = INVALID_ADC_CODE;
        ledIsActive[i] = FALSE;
    }
    vBuckCode = INVALID_ADC_CODE;
    vBattCode = INVALID_ADC_CODE;
    vTempCode = INVALID_ADC_CODE;
  
    IOCTRLA_SFRS->LEDPIN.HWMODE = 1U;
    PWM_SFRS->BASE.PERIOD = PWM_VALUE_MAX;
    PWM_SFRS->ENAREQ.ENAREQALL = 1U;
    /* set duty cycle to 0*/
    PWM_Reset();
    
    ADC_REG_CNTRL_CH1_CH2 = (uint8_t)ADC_CH1_VH3_SEL_VBUCK + ( 0U << 2);
    ADC_SFRS->CNTRL.CHSEQ  = (uint8_t)ADC_CH3_THEN_CH1;
    ledConvertType = LED_CONVERT_TYPE_VBUCK_VBAT;
    ADC_REG_CNTRL_CONVERT = 0xC1U;
}

void PWM_Reset(void)
{
    uint8_t no;
    for (uint8_t i = 0U; i < LED_NUM; i++){
        no = i / 3U;
        if( (no - ((no >> 1U)*2U)) ==  0U ){
            for (uint8_t j  = 0U; j < 3U; j ++){
                PWM_SFRS->PULSE[no*3U + j].PFALL = PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS;
                PWM_SFRS->PULSE[no*3U + j].PRISE = PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS; 
                currPWMMatchValue[i][j]          = PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS;
            }
        }else{
            for (uint8_t j  = 0U; j < 3U; j ++){
                PWM_SFRS->PULSE[no*3U + j].PFALL = MAX_MEASURE_TIME_TICKS;
                PWM_SFRS->PULSE[no*3U + j].PRISE = MAX_MEASURE_TIME_TICKS;
                currPWMMatchValue[i][j]          = MAX_MEASURE_TIME_TICKS;
            }
        }
        ledIsActive[i] = FALSE;
    }
    PWM_SFRS->UPDATE = 1U;
}



uint16_t ADC_GetLedPNCode(uint8_t ledNo, uint16_t *pnCodeR,uint16_t *pnCodeG,uint16_t *pnCodeB)
{   
__atomic_enter()
    *pnCodeR = vLedPNCode[ledNo][PHY_CHANNEL_R];
    *pnCodeG = vLedPNCode[ledNo][PHY_CHANNEL_G];
    *pnCodeB = vLedPNCode[ledNo][PHY_CHANNEL_B];
__atomic_exit()
    return 0U;
}

uint16_t ADC_GetVBuckCode(void)
{   
    uint16_t code = vBuckCode;
    return code;
}

uint16_t ADC_GetVBattCode(void)
{   
    uint16_t code = vBattCode;
    return code;
}

uint16_t ADC_GetVTempCode(void)
{   
    uint16_t code = vTempCode;
    return code;
}


uint8_t PWM_UpdateFinished(void)
{
    uint8_t result = TRUE;
    uint8_t update = PWM_SFRS->UPDATE;
    if (update == 0U){
        result = FALSE;
    }
    return result;
}


uint8_t PWM_LedIsOn(uint8_t ledNo)
{
    return ledIsActive[ledNo];
}




/**
 * @brief The instance of PMW device
 */
void PWM_RegisterPWMReloadIRQ(void)
{
    PWM_SFRS->INTPWM.CLEAR.PERIOD  = 1U;
    PWM_SFRS->INTPWM.ENABLE.PERIOD = 1U;
    NVIC_EnableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_UnRegisterPWMReloadIRQ(void)
{
    PWM_SFRS->INTPWM.CLEAR.PERIOD  = 1U;
    PWM_SFRS->INTPWM.ENABLE.PERIOD = 0U;
    NVIC_DisableIRQ(PWM_IRQn);
}
/**
 * @brief The instance of PMW device
 */
int8_t PWM_SetMaxChannelCurrent(void)
{
    /* Enable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x0EU;
    for (uint8_t i = 0U; i < 24U; i++){
        SYSCTRLA_SFRS->TRIMLED[i].TRIM = HWCFG_GetLEDTrimValue(i);
    }
    SYSCTRLA_SFRS->TRIMVFW = HWCFG_GetOffLEDTrimValue();
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x00U; /* Disable trim access write enable */
    return 0;
}

void PWM_TurnOffChannelCurrent(void)
{
    /* Enable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY  = 0x0EU;
    for (uint8_t i = 0U; i < 24U; i++){
        SYSCTRLA_SFRS->TRIMLED[i].TRIM  = 0U;
    }
    SYSCTRLA_SFRS->TRIMVFW              = 0U;
    /* Disable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x00U; 

}



#endif
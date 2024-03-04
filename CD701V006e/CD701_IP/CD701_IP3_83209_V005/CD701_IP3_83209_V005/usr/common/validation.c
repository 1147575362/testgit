#include <appConfig.h>
#if ENABLE_FUNCTION_VALIDATION == 1


#include <realplumPro.h>
#include <validation.h>
#include <gpio_device.h>
#include <adc_device.h>
#include <clock_device.h>
#include <lin_device.h>
#include <hwProtection.h>

SYSCTRLA_SFRS_t sysCtrl;

void  LED_Current(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    IOCTRLA_SFRS->LEDPIN.HWMODE = 0U;
    IOCTRLA_SFRS->LEDPIN.DATA =0x07U;
    sysCtrl = *SYSCTRLA_SFRS;
    SYSCTRLA_SFRS->PMU_TRIM.RESISTOR_TRIM = 0x1EU;
    SYSCTRLA_SFRS->LED[0].TRIM = 0xFAU;
    SYSCTRLA_SFRS->LED[1].TRIM = 0xFAU;
    SYSCTRLA_SFRS->LED[2].TRIM = 0xFAU;
    while(1);
}


void  Major_BandgapOUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.ADCCONSEL  = 1U;
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.GPIOCONSEL = 1U;
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.ADCCONREG  = 0x01U;/*GPIO1 */
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.GPIOCONREG = 0x08U; /* Select Bandgap supply to GPIO1 */
    while(1);
}

void  Tempsensor_OUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.ADCCONSEL  = 1U;
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.GPIOCONSEL = 1U;
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.ADCCONREG  = 0x02U;/*GPIO2 */
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.GPIOCONREG = 0x40U; /* Select tempsensor to GPIO2 */
    
    while(1);
}

void  VDD1V5_OUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.ADCCONSEL  = 1U;
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.GPIOCONSEL = 1U;
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.ADCCONREG  = 0x01U;/*GPIO1 */
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.GPIOCONREG = 0x04U; /* Select 1.5v MCU supply to GPIO1 */
    
    while(1);
}

void  VDD3V3_Analog_OUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.ADCCONSEL  = 1U;
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.GPIOCONSEL = 1U;
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.ADCCONREG  = 0x01U;/*GPIO1 */
    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.GPIOCONREG = 0x02U; /* Select 3.3v analog supply to GPIO1 */
    while(1);
}

void LF_32KHz_OUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    IOCTRLA_REG_GPIO(0).MUXSEL = 0x14U;/* 0x14: CRGA (Clock Reset Generation Asic) (scan_test_mode | lf_rc_clk) at GPIO1  */
    IOCTRLA_REG_GPIO(0).HWMODE = 2;
    while(1);
}


void  SSC_TEST(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    
    CRGA_SFRS->SYSCLKCTRL.HFRCENA = 1U;
    while(CRGA_SFRS->SYSCLKCTRL.HFRCSTS == 0U){}
    SYSCTRLA_SFRS->HF_OSC_TRIM.TRIM_HF_RC = 0x42U;
    CRGA_SFRS->SYSCLKCTRL.SYSCLKSEL = 1;
    CRGA_SFRS->SYSCLKCTRL.DIVSYSCLK = 0;
    /*  frequency spread    */
    SYSCTRLA_SFRS->HF_OSC_TRIM.SSCDEEP  = 7U;
    SYSCTRLA_SFRS->HF_OSC_TRIM.SSCDIV   = 255U;
    SYSCTRLA_SFRS->HF_OSC_TRIM.SSCENA   = 0U;
    
    IOCTRLA_SFRS->GPIO[0].HWMODE = 2;
    IOCTRLA_SFRS->GPIO[0].MUXSEL = 0x15;/*0x15: CRGA (Clock Reset Generation Asic) (scan_test_mode | hf_rc_clk) at GPIO1*/
//    IOCTRLA_SFRS->ANALOGTESTMUXOVERRIDE.ADCSELREG = 1;
    
    while(1);
}


void  SOFT_CTRL_LIN(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    sysCtrl = *SYSCTRLA_SFRS;
    
    IOCTRLA_SFRS->LIN.LINS_HWMODE       = 0U;     /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
    IOCTRLA_SFRS->LIN.LINS_PU30K_ENA    = 1U;     /* LIN 30K pullup enable.*/
    IOCTRLA_SFRS->LIN.LINS_TXENA        = 1U;     /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINS_RXENA        = 1U;     /* LIN receive enable. */
    IOCTRLA_SFRS->LIN.SWON              = 0U;     /* 1: LIN slave auto addressing On, 0: enable Master function on LINOUT pin */
    
    while(1){
        GPIO_Set(GPIO_PORT_LINS, GPIO_LOW);
        GPIO_Set(GPIO_PORT_LINS, GPIO_HIGH);  
    }
}

void  ADC_MEASURE_VBAT(void)
{
    uint32_t ADC_Code;
    ADC_SFRS->CONF.AUTOEN       = 0U; 
    ADC_SFRS->CONF.SAMPCYC      = 8U;
    ADC_SFRS->CONF.MODE         = (uint8_t)ADC_MODE_SINGLE_END;
    ADC_SFRS->CNTRL.STUPDLY     = (uint8_t)ADC_STARTUP_DELAY_1US;
    ADC_SFRS->CONF.SYNCENA      = (uint8_t)SYNC_MODE_NONE; /* disable sync convert mode */
    ADC_SFRS->CNTRL.CONT        = 0U; /* singel convert mode */
    ADC_REG_TSET.WORD           = (uint32_t)(((uint32_t)ADC_WAIT_TIME_1000NS << TSET_TCURR_POS) | ((uint32_t)ADC_WAIT_TIME_1000NS << TSET_TCHNL_POS) | ((uint32_t)ADC_WAIT_TIME_250NS << TSET_TGUARD_POS));
  
    ADC_SFRS->CNTRL.CH1SEL = (uint8_t)ADC_CH1_CH3_SEL_VBAT;
    ADC_SFRS->CNTRL.CHSEQ = (uint8_t)ADC_CH1;
    ADC_SFRS->CONF.SYNCENA = (uint8_t)SYNC_MODE_NONE;
    
    for (uint32_t i = 0; i <10000; i++){
        ADC_SFRS->CNTRL.CONVERT = 1U;
        while(ADC_SFRS->STATUS.CONVDONE == 0U);
        ADC_Code = ADC_SFRS->DATA1;
        ADC_SFRS->CNTRL.IRQCLR = 1;
#if CODE_DEBUG_EN == 1
        //DEBUG_OUT("VBAT_mV:%d\n",ADC_Code*1118*32/4096);
#endif
        for (uint32_t j = 0; j <100000; j++);
    }
  
}

void  ADC_MEASURE_VLED(uint8_t ledChannelNo)
{
    uint16_t ADC_Code;
    ADC_SFRS->CONF.AUTOEN       = 0U; 
    ADC_SFRS->CONF.SAMPCYC      = 8U;
    ADC_SFRS->CONF.MODE         = (uint8_t)ADC_MODE_SINGLE_END;
    ADC_SFRS->CNTRL.STUPDLY     = (uint8_t)ADC_STARTUP_DELAY_1US;
    ADC_SFRS->CONF.SYNCENA      = (uint8_t)SYNC_MODE_NONE; /* disable sync convert mode */
    ADC_SFRS->CNTRL.CONT        = 0U; /* singel convert mode */
    ADC_REG_TSET.WORD           = (uint32_t)(((uint32_t)ADC_WAIT_TIME_1000NS << TSET_TCURR_POS) | ((uint32_t)ADC_WAIT_TIME_1000NS << TSET_TCHNL_POS) | ((uint32_t)ADC_WAIT_TIME_250NS << TSET_TGUARD_POS));
  
//    IOCTRLA_SFRS->LEDPIN.VFW_ENA = 1U;        /* Enable 2mA led current source */
    IOCTRLA_SFRS->LEDPIN.SENSE_ENA = 1U;      /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
    
    ADC_SFRS->CNTRL.CH2SEL = ledChannelNo;
    ADC_SFRS->CNTRL.CHSEQ = (uint8_t)ADC_CH2;
    for (uint32_t i = 0; i <10000; i++){
        ADC_SFRS->CNTRL.CONVERT = 1U;
        while(ADC_SFRS->STATUS.CONVDONE == 0U);
        ADC_Code = ADC_SFRS->DATA2;
        ADC_SFRS->CNTRL.IRQCLR = 1;
#if CODE_DEBUG_EN == 1
        //DEBUG_OUT("LED_CODE[%d]:%d\n",ledChannelNo,ADC_Code);
#endif
        for (uint32_t j = 0; j <100000; j++);
    }
    while(1);
}


void SET_VLED_PNVoltToGPIO1(uint8_t ledChannel)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0EU;
    IOCTRLA_SFRS->LEDPIN.SENSE_ENA = 1U;      /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
    ADC_SFRS->CNTRL.CH2SEL = ledChannel;        /*  measure green PN volt */
    *(uint32_t *)0x50012034=0x1405U;
    *(uint32_t *)0x50012038=0x00010000U;
    
    while(1);
}

void delay(uint32_t delay)
{
  for (uint32_t i = 0; i < delay; i++){
      for (uint32_t j = 0; j < 10000; j++){
        ;
      }
  }
}


void LIN_PL_Test(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY        = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0EU;
    IOCTRLA_REG_FILT_ACCESS.FILTER_UNLOCK = 1U;
  
    GPIO_Init(GPIO_INIT_PORT_1, GPIO_DIR_INPUT,GPIO_PULL_UP);
    GPIO_Init(GPIO_INIT_PORT_2, GPIO_DIR_INPUT,GPIO_PULL_UP);
    IOCTRLA_SFRS->LIN.LINS_HWMODE = 0U;
    IOCTRLA_SFRS->LINSGFCONF.LINSRXGF1ST_ENA = 1U;
    IOCTRLA_SFRS->LINSGFCONF.LINSRXGF2ND_ENA = 1U;
    IOCTRLA_SFRS->LINSGFCONF.LINSRXGF3RD_ENA = 3U;
    IOCTRLA_SFRS->LINTXDMONITOR.LINSTXDMONITORENA = 1U;         /* LINS TxD Monitor enable   */
    
    IOCTRLA_SFRS->LINSGFCONF1.LINS1ST_DBNCTHRES0 = 0x08;        /* glitch filter settings */
    IOCTRLA_SFRS->LINSGFCONF1.LINS1ST_DBNCTHRES1 = 0x00;        /* value*62.5us   */
    
    IOCTRLA_SFRS->LINSGFCONF1.LINS2ND_DBNCTHRES0 = 0x10;        /* glitch filter settings */
    IOCTRLA_SFRS->LINSGFCONF1.LINS2ND_DBNCTHRES1 = 0x08;        /* value*62.5us   */
    
    IOCTRLA_REG_LINSGFCONF.LINS3RD_DBNCTHRES0 = 0x30;           /* glitch filter settings */
    IOCTRLA_REG_LINSGFCONF.LINS3RD_DBNCTHRES1 = 0x30;           /* value*62.5us   */
    
    
    SYSCTRLA_SFRS->LIN.TXLINSRISESLOPE = 1;
    SYSCTRLA_SFRS->LIN.TXLINS_DR_SLOPE = LIN_TX_SLEW_RATE_4_9V_PER_US;
    
    IOCTRLA_SFRS->LIN.SWON              = 0U;     /* 0: LIN slave auto addressing Off*/
    IOCTRLA_SFRS->LIN.LINS_PU30K_ENA    = 1U;     /* LIN 30K pullup enable.*/
    IOCTRLA_SFRS->LIN.LINS_TXENA        = 1U;     /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINS_RXENA        = 1U;     /* LIN receive enable. */
  
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_1].MUXSEL  = 0x2E;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_1].HWMODE  = 2U;       /* LINS RXD signal */
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_2].HWMODE  = 2U;       /* LINS TXD signal */
}


void LIN_GlitchFilter_Test(void)
{
  
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY        = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0EU;
    IOCTRLA_REG_FILT_ACCESS.FILTER_UNLOCK = 1U;
    
    LINS_REG_CTRL.SLEEP = 0U;                     /*  wake up lins when init if sleep  */
    /* Config GPIO to LIN mode, enable transmission */
    IOCTRLA_SFRS->LIN.LINS_HWMODE       = 1U;     /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
    IOCTRLA_SFRS->LIN.LINS_PU30K_ENA    = 1U;     /* LIN 30K pullup enable.*/
    IOCTRLA_SFRS->LIN.LINS_TXENA        = 1U;     /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINS_RXENA        = 1U;     /* LIN receive enable. */
    
    IOCTRLA_SFRS->LINSGFCONF.LINSRXGF1ST_ENA = 1U;
    IOCTRLA_SFRS->LINSGFCONF.LINSRXGF2ND_ENA = 1U;
    IOCTRLA_SFRS->LINSGFCONF.LINSRXGF3RD_ENA = 3U;
    IOCTRLA_SFRS->LINTXDMONITOR.LINSTXDMONITORENA = 1U;         /* LINS TxD Monitor enable   */
  
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_3].RESV = 1;     /* GPIO3: meta output of phy  */

    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_5].HWMODE = 2;   /* GPIO5: 1st out  */
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_5].RESV   = 1;
    IOCTRLA_SFRS->LINSGFCONF1.LINS1ST_DBNCTHRES0 = 0x08;        /* glitch filter settings */
    IOCTRLA_SFRS->LINSGFCONF1.LINS1ST_DBNCTHRES1 = 0x00;        /* value*62.5us   */
    
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_4].HWMODE = 2;   /* GPIO4: 2nd output */
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_4].LIN_SEL = 1;
    IOCTRLA_SFRS->LINSGFCONF1.LINS2ND_DBNCTHRES0 = 0x10;        /* glitch filter settings */
    IOCTRLA_SFRS->LINSGFCONF1.LINS2ND_DBNCTHRES1 = 0x08;        /* value*62.5us   */
    
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_1].MUXSEL  = 0x2E;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_1].HWMODE  = 2U; /* GPIO1:3rd output same as LINS RXD signal input of LIN controler*/
    IOCTRLA_REG_LINSGFCONF.LINS3RD_DBNCTHRES0 = 0x30;           /* glitch filter settings */
    IOCTRLA_REG_LINSGFCONF.LINS3RD_DBNCTHRES1 = 0x30;           /* value*62.5us   */
 
    
/* debug */
#if 0U
    
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY        = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0EU;
    
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_3].RESV = 1;     /* GPIO3: meta output of phy  */

    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_5].HWMODE = 2;   /* GPIO5: 1st out  */
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_5].RESV   = 1;
    
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_4].HWMODE = 2;   /* GPIO4: 2nd output */
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_4].LIN_SEL = 1;
    
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_1].MUXSEL  = 0x2E;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_1].HWMODE  = 2U; /* GPIO1:3rd output same as LINS RXD signal input of LIN controler*/
#endif  
    
    
}


void WDTA_WindowMode_Test(void)
{
    CRGA_SFRS->WDTACTION.WDTACTION = 0;
    WDTA_SFRS->INTERRUPT.TIMEOUT_EN = 0;//    
    WDTA_SFRS->INTERRUPT.WINDOW_OPEN_EN = 1;//
    WDTA_SFRS->CTRL.TIMEOUTSEL = 7;
    WDTA_SFRS->CTRL.WINOPENENA = 1;     //Window Mode Enable  
    WDTA_SFRS->CTRL.UPDATE = 1;         //Window Mode take effect 
    while(WDTA_SFRS->CTRL.UPDATE == 1);
    WDTA_SFRS->STOP.STOP = 0U;/* start wdt*/
    CRGA_SFRS->WDTACTION.WDTBARKCNT = 0;
    while (1){
        if (WDTA_SFRS->CTRL.WINOPENFLAG == 1){
            WDTA_SFRS->CLEAR = 0x3c574ad6U;
        }
    }
}

void UV_ISR(void)
{
    DEBUG_OUT("Battery LOW ISR!!\n");
}

void OV_ISR(void)
{
    DEBUG_OUT("Battery HIGH ISR!!\n");
}

void UV_Test(void)
{
    uint8_t ov = 0U,uv = 0;
    HW_PROT_SetUnderVoltage(UV_VOLT_5_482V,UNDER_VOLT_HYS_199mV, 160U, 160U);// 10ms
    HW_PROT_RegisterUnderVoltageIRQ(UV_ISR);
    
    HW_PROT_SetOverVoltage(OV_VOLT_19_00V,OV_VOLT_HYS_1130mV, 160U, 160U);
    HW_PROT_RegisterOverVoltageIRQ(OV_ISR);
    
    GPIO_Init(GPIO_INIT_PORT_1, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
    
    GPIO_Init(GPIO_INIT_PORT_2, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
    while(1){
        if (HW_PROT_GetBattVoltState() == 1){
            GPIO_Set(GPIO_PORT_1, GPIO_HIGH);
            if (uv == 0){
                uv = 1;
                DEBUG_OUT("Battery LOW triggerred!!\n");
            }
        }else{
            GPIO_Set(GPIO_PORT_1, GPIO_LOW);
            if (uv == 1){
                uv = 0;
                DEBUG_OUT("Battery LOW released!!\n");
            }
        }
        
        if (HW_PROT_GetBattVoltState() == 2){
            GPIO_Set(GPIO_PORT_2, GPIO_HIGH);
            if (ov == 0){
                ov = 1;
                DEBUG_OUT("Battery HIGH triggerred!!\n");
            }
        }else{
            GPIO_Set(GPIO_PORT_2, GPIO_LOW);
            if (ov == 1){
                ov = 0;
                DEBUG_OUT("Battery HIGH released!!\n");
            }
        }
    }
  
}



void GPIO_PWM_Test(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY        = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0EU;
  
    GPIO_Init(GPIO_INIT_PORT_3, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_3].HWMODE  = 1U;       /* PWM */
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_INIT_PORT_3].PWM_SEL = 0U;
    
    PWM_SFRS->BASE.WORD = ((uint32_t)PWM_PRESCALER_DIVIDE_1 << 8) + ((uint32_t)6400 << 16);//1600
    PWM_SFRS->PULSE[0].WORD = (0 << 16) + 800;
    PWM_SFRS->ENAREQ.ENAREQALL = 1U;
    PWM_SFRS->UPDATE = 1U;
}

void validation_main(void)
{
    Clock_SystemMainClockInit(SYS_MAIN_CLOCK_DIV);
   // SSC_TEST();
    LIN_PL_Test();
//    Major_BandgapOUT();
//    LED_Current();
//    GPIO_PWM_Test();
//    UV_Test();
    while(1);
}


#endif

#include <appConfig.h>
#if ENABLE_FUNCTION_VALIDATION == 1


#include <rugbyPro.h>
#include <validation.h>
#include <gpio_device.h>
#include <adc_device.h>
#include <uart_device.h>
#include <clock_device.h>
#include <buck_device.h>
#include <buckctrl_sfr.h>
#include <spi_device.h>

SYSCTRLA_SFRS_t sysCtrl;

void  LED_Current(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    IOCTRLA_SFRS->LEDPIN.HWMODE = 0U;
    IOCTRLA_SFRS->LEDDATA =0xFFFFFFU;
    sysCtrl = *SYSCTRLA_SFRS;
    SYSCTRLA_SFRS->PMU_TRIM.RESISTOR_TRIM_AUX = 0x20U;
    SYSCTRLA_SFRS->TRIMLED = 0x1FFU;
    
    while(1);
}

void  Aux_BandgapOUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1.ADCSELSEL = 0x01;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1.GPIOCONSEL = 1;
    
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2.ADCSELREG = 2;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2.GPIOCONREG = 0x80;
    while(1);
}


void  Major_BandgapOUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1.ADCSELSEL = 0x01;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1.GPIOCONSEL = 1;
    
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2.ADCSELREG = 1;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2.GPIOCONREG = 0x8;
    while(1);
}

void  Tempsensor_OUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1.ADCSELSEL = 0x01;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1.GPIOCONSEL = 1;
    
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2.ADCSELREG = 2;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2.GPIOCONREG = 0x40;
    while(1);
}

void  VDD1V5_OUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1.ADCSELSEL = 0x01;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1.GPIOCONSEL = 1;
    
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2.ADCSELREG = 1;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2.GPIOCONREG = 0x4;
    while(1);
}

void  VDD3V3_Analog_OUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;

    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1.ADCSELSEL = 0x01;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE1.GPIOCONSEL = 1;
    
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2.ADCSELREG = 1;
    IOCTRLA_REG_ANALOGTESTMUXOVERRIDE2.GPIOCONREG = 0x2;
    while(1);
}

void LF_32KHz_OUT(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    sysCtrl = *SYSCTRLA_SFRS;
    IOCTRLA_REG_GPIO(0).MUXSEL = 0x04U;
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
    SYSCTRLA_SFRS->HF_OSC_TRIM.SSCENA   = 1U;
    
    IOCTRLA_SFRS->GPIO[0].HWMODE = 2;
    IOCTRLA_SFRS->GPIO[0].MUXSEL = 0x5;
    
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

/* freqiency: KHz vBuckVolt: mV*/
void BUCK_VALDATION(uint16_t freqiency, uint16_t vBuckVolt,BuckOverOKVoltThres_t overVolt, BuckOverOKVoltThres_t okVolt,BuckUnderVoltThres_t underVolt)
{
    /* Init buck settings */
    BUCKCTRL_REG_CLOCK.PERIOD      = (uint8_t)(Clock_GetSystemClock()/freqiency -1U);
    BUCKCTRL_REG_CLOCK.SPREAD      = 0U; /* Enable frequency spread */
    
    /* Enable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0x0EU;
    BUCKCTRL_REG_VDACCODE.VDACTRIM = (uint8_t)(((uint32_t)vBuckVolt *256 *32)/(11*25000) -1U); /* vBuckVolt: 6.25*1.375*(VDACTRIM+1)/256 */
    /* Disable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.KEY = 0U;
    
    BUCKCTRL_REG_VBUCKCTRL.OVLEVEL_SEL  = (uint8_t)overVolt;     /* over volt monitor level */
    BUCKCTRL_REG_VBUCKCTRL.OKLEVEL_SEL  = (uint8_t)okVolt;       /* ok volt monitor level */
    BUCKCTRL_REG_VBUCKCTRL.UVLEVEL_SEL  = (uint8_t)underVolt;    /* under volt monitor level*/
    
    BUCKCTRL_REG_VBUCKCTRL.OV_MONITOR_ENA  = 1U;        /* enable over volt monitor */
    BUCKCTRL_REG_VBUCKCTRL.OK_MONITOR_ENA  = 1U;        /* enable OK volt monitor */
    BUCKCTRL_REG_VBUCKCTRL.UV_MONITOR_ENA  = 1U;        /* enable under volt monitor */
    
    BUCKCTRL_REG_VBUCKDBNC.BUCKSTRBDBNCSEL = 3U;        
    BUCKCTRL_REG_VBUCKDBNC.OVTHRES1 = 7U;               /* over volt debouncing settings  */
    BUCKCTRL_REG_VBUCKDBNC.UVTHRES1 = 7U;               /* under volt debouncing settings  */
    BUCKCTRL_REG_VBUCKDBNC.LFENA       = 0x00U;         /* Low frequency strobing enable for debouncing */
    BUCKCTRL_REG_VBUCKDBNC.BUCKDBNCENA = 0x0FU;         /* enable debouncing */
    BUCKCTRL_REG_VBUCKDBNC.EXTRADBNC   = 0x07U;
    
    BUCKCTRL_REG_VBUCKTIMEOUT.PREENADWELL       = 0x0FU;
    BUCKCTRL_REG_VBUCKTIMEOUT.DETLOWDWELL       = 0x08U;
    BUCKCTRL_REG_VBUCKTIMEOUT.STARTLDWELL       = 0x08U;
    BUCKCTRL_REG_VBUCKTIMEOUT.STARTDBNCDWELL    = 0x06U;
    BUCKCTRL_REG_VBUCKTIMEOUT.UVDBNCDWELL       = 0x01U;
    BUCKCTRL_REG_VBUCKTIMEOUT.OVDBNCDWELL       = 0x0FU;
    BUCKCTRL_REG_VBUCKTIMEOUT.HICCUPDWELL       = 0x0FU;
    BUCKCTRL_REG_CTRL.HICCUPENA  = 1U;
    BUCKCTRL_REG_CTRL.ENAREQ     = 1U;                  /* enable buck */
  
    while(1);
}

void SET_VbuckVoltToGPIO1(void)
{
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0E;
    *(uint32_t *)0x50012034=0x5005U;
    *(uint32_t *)0x50012038=0x00010000U;
    
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




void SPI_VALIDATION(void)
{
    uint8_t devider = SPI_CLK_DIV_2048;
    uint8_t rxData;
    CRGA_SFRS->MODULERSTREQ = MODUE_SOFT_RESET_SPI;
    
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_6].DIR     = (uint8_t)GPIO_DIR_INPUT;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_6].PULLMODE = (uint8_t)GPIO_PULL_DOWN;
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_7].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_8].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_6].HWMODE = 4U;/* GPIO_PORT_6: SPI Master MISO .*/
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_7].HWMODE = 5U;/* GPIO_PORT_7: SPI Master Clock Out*/
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_8].HWMODE = 4U;/* GPIO_PORT_8: SPI Master MOSI.*/
    
    SPI_REG_CTRL.FIFOSOFTRESET = 1U;
    SPI_REG_CTRL.LPBK = 0U;
    SPI_REG_CTRL.SCKEXT = ((uint8_t)devider >> 2);
    SPI_REG_CTRL.SPR    = (uint8_t)devider &0x03U;
    SPI_REG_CTRL.ENA_REQ = 1U;
    
    while(1){
        SPI_REG_CTRL.FIFOSOFTRESET = 1U;
        for (uint16_t i = 0; i < 200; i++){
            SPI_REG_TXDATA = 0x55;
            while(SPI_REG_STATUS.XFERCNT == 0){}
            rxData = SPI_REG_RXDATA;
#if CODE_DEBUG_EN == 1
            DEBUG_OUT("RX:%02x\r\n",rxData);
#endif
            delay(100);
        }
    }
}

void LIN_PL_Test(void)
{
    GPIO_Init(GPIO_PORT_2, GPIO_DIR_INPUT,GPIO_PULL_UP);
    GPIO_Init(GPIO_PORT_1, GPIO_DIR_INPUT,GPIO_PULL_UP);
    IOCTRLA_SFRS->LIN.LINS_HWMODE = 0U;
    IOCTRLA_SFRS->LIN.LINSRXGFENA = 1U;
    IOCTRLA_SFRS->LIN.SWON              = 0U;     /* 0: LIN slave auto addressing Off*/
    IOCTRLA_SFRS->LIN.LINS_PU30K_ENA    = 1U;     /* LIN 30K pullup enable.*/
    IOCTRLA_SFRS->LIN.LINS_TXENA        = 1U;     /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINS_RXENA        = 1U;     /* LIN receive enable. */
  
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_1].HWMODE = 2U;       /* LINS RXD signal */
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_1].MUXSEL = 0x11U;    
    SYSCTRLA_REG_DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;     
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_2].HWMODE = 2U;       /* LINS TXD signal */  
}



void validation_main(void)
{
    Clock_SystemMainClockInit(SYS_MAIN_CLOCK_DIV);
//    LIN_PL_Test();
//    Major_BandgapOUT();
    LED_Current();
    while(1);
}


#endif

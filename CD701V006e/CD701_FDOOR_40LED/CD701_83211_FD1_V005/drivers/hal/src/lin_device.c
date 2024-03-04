/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file lin_device.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <stdint.h>
#include <string.h>

#include <appConfig.h>
#include <rugbyPro.h>
#include "timer_device.h"
#include "errno.h"
#include <isrfuncs.h>
#include <lin_device.h>

#if LIN_STACK_TYPE == LIN_STACK_TYPE_PRIVATE
   static void LINS_SLAVE_ISR(void);
#else
  static lin_stack_cb_func_t lin_stack_callback = NULL;
  static lin_wakeup_cb_func_t lin_wakeup_callback = NULL;
#endif


static lin_cb_func_t linm_callback = NULL;

typedef struct {
    uint16_t divider;
    uint16_t multiplier;
    uint8_t prescale;
} lin_speed_setting_t;

/**********************************************************************  
 *    Local definitions
 *********************************************************************/   

/**
 * At 16MHz clock, the clock divider settings for each LIN speed.
 */

/**
 * At 16MHz clock, the clock divider settings for each LIN speed.
 */

#if SYS_MAIN_CLOCK_DIV != CLOCK_DIV_1
#error MAIN_CPU_CLOCK MUST BE at 16MHz system colck!
#endif

#if SYS_MAIN_CLOCK_DIV == CLOCK_DIV_1   /* 16MHz */
  static const lin_speed_setting_t lins_speed_map[] = {
      [E_LIN_BAUDRATE_10417] = {.divider = 200U, .multiplier = 0U, .prescale = 2U},
      [E_LIN_BAUDRATE_19200] = {.divider = 200U, .multiplier = 0U, .prescale = 2U},
      [E_LIN_BAUDRATE_20000] = {.divider = 200U, .multiplier = 0U, .prescale = 2U},
  };
  static const lin_speed_setting_t linm_speed_map[E_LIN_BAUDRATE_MAX] = {
	[E_LIN_BAUDRATE_10417] = {.divider = 192U, .multiplier = 0U, .prescale = 3U},
//	  [E_LIN_BAUDRATE_19200] = {.divider = 208U, .multiplier = 0U, .prescale = 2U},
	[E_LIN_BAUDRATE_19200] = {.divider = 150U, .multiplier = 0U, .prescale = 1U},
	[E_LIN_BAUDRATE_20000] = {.divider = 200U, .multiplier = 0U, .prescale = 2U},

  };
#elif SYS_MAIN_CLOCK_DIV == CLOCK_DIV_2 /* 8MHz */
  static const lin_speed_setting_t lins_speed_map[E_LIN_BAUDRATE_MAX] = {
      [E_LIN_BAUDRATE_10417] = {.divider = 200U, .multiplier = 0U, .prescale = 1U},
      [E_LIN_BAUDRATE_19200] = {.divider = 200U, .multiplier = 0U, .prescale = 1U},
      [E_LIN_BAUDRATE_20000] = {.divider = 200U, .multiplier = 0U, .prescale = 1U},
  };
  static const lin_speed_setting_t linm_speed_map[E_LIN_BAUDRATE_MAX] = {
      [E_LIN_BAUDRATE_10417] = {.divider = 192U, .multiplier = 0U, .prescale = 2U},
      [E_LIN_BAUDRATE_19200] = {.divider = 208U, .multiplier = 0U, .prescale = 1U},
      [E_LIN_BAUDRATE_20000] = {.divider = 200U, .multiplier = 0U, .prescale = 1U},
  };
#elif SYS_MAIN_CLOCK_DIV == CLOCK_DIV_4 /* 4MHz */
  static const lin_speed_setting_t lins_speed_map[E_LIN_BAUDRATE_MAX] = {
      [E_LIN_BAUDRATE_10417] = {.divider = 200U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_19200] = {.divider = 200U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_20000] = {.divider = 200U, .multiplier = 0U, .prescale = 0U},
  };
  static const lin_speed_setting_t linm_speed_map[E_LIN_BAUDRATE_MAX] = {
      [E_LIN_BAUDRATE_10417] = {.divider = 192U, .multiplier = 0U, .prescale = 1U},
      [E_LIN_BAUDRATE_19200] = {.divider = 208U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_20000] = {.divider = 200U, .multiplier = 0U, .prescale = 0U},
  };
#else
#error invalid SYS_MAIN_CLOCK_DIV Settings!
#endif

void lins_SetTiming(LIN_BaudRate_t BaudRate);


int8_t LINM_WriteBuffer(uint8_t buff[], uint32_t dataLength);
int8_t LINM_ReadBuffer(uint8_t buff[], uint32_t dataLength);
void linm_SetTiming(LIN_BaudRate_t BaudRate);

 void lins_SetTiming(LIN_BaudRate_t BaudRate)
{
    LINS_SFRS->BTDIV07          = (uint8_t)lins_speed_map[BaudRate].divider & 0xFFU;
    LINS_SFRS->BITTIME.BTDIV8   = (uint8_t)lins_speed_map[BaudRate].divider >> 8U;
    LINS_SFRS->BITTIME.PRESCL   = (uint8_t)lins_speed_map[BaudRate].prescale;
}


void LINS_Init(void)
{
    SYSCTRLA_REG_TRIM_ACCESS_KEY.KEY   = 0x0EU;
    LINS_REG_CTRL.SLEEP = 0U;                     /*  wake up lins when init if sleep  */
    /* Config GPIO to LIN mode, enable transmission */
    IOCTRLA_SFRS->LIN.LINS_HWMODE       = 1U;     /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
    IOCTRLA_SFRS->LIN.LINS_PU30K_ENA    = 1U;     /* LIN 30K pullup enable.*/
    IOCTRLA_SFRS->LIN.LINS_TXENA        = 1U;     /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINS_RXENA        = 1U;     /* LIN receive enable. */
  
#if LIN_STACK_TYPE == LIN_STACK_TYPE_SAEJ2602
    LINS_SFRS->BUSTIME.BUSINACTIVE      = E_LIN_TIME_INACTIVE_SEC_6;
#else 
    LINS_SFRS->BUSTIME.BUSINACTIVE      = E_LIN_TIME_INACTIVE_SEC_6;
#endif
    
    LINS_SFRS->BUSTIME.WUPREPEAT        = E_LIN_TIME_WAKEUP_REPEAT_MS_200;
    LINS_SFRS->CTRL.RST_INT_ERR         = 0x03U;    /* reset error,reset interrupt */
    
#if AUTO_ADDRESSING_EN == 0U
    IOCTRLA_SFRS->LIN.SWON             = 0U; /* 1: LIN slave switch On between LIN_IN and LIN_OUT pin*/
    SYSCTRLA_SFRS->LIN.TXLINSRISESLOPE = 0U;
    SYSCTRLA_SFRS->LIN.TXLINS_DR_SLOPE = LIN_TX_SLEW_RATE_5_1V_PER_US;
    SYSCTRLA_SFRS->LIN.TXLINMRISESLOPE = 0U;
    SYSCTRLA_SFRS->LIN.TXLINM_DR_SLOPE = LIN_TX_SLEW_RATE_5_1V_PER_US;
#else
    IOCTRLA_SFRS->LIN.SWON             = 0U; /* 1: LIN slave switch On between LIN_IN and LIN_OUT pin*/
    IOCTRLA_SFRS->LIN.LINM_PU1K_ENA    = 1U; /* LIN 1K pullup enable.*/
    SYSCTRLA_SFRS->LIN.TXLINSRISESLOPE = 1U;
    SYSCTRLA_SFRS->LIN.TXLINS_DR_SLOPE = LIN_TX_SLEW_RATE_5_1V_PER_US;
    SYSCTRLA_SFRS->LIN.TXLINMRISESLOPE = 1U;
    SYSCTRLA_SFRS->LIN.TXLINM_DR_SLOPE = LIN_TX_SLEW_RATE_5_1V_PER_US;
#endif

    lins_SetTiming(E_LIN_BAUDRATE_19200);       /* set the maximum supported baud rate */
    
    /* Enable wakeup detect and IRQ  when LINS is in sleep mode(LINS_REG_CTRL.SLEEP = 1U;)  */
    WICA_REG_CTRL.LINSIRQCLR = 1U;
    WICA_REG_CTRL.LINSIRQENA = 1U;
    WICA_REG_CTRL.LINSENA    = 1U;
    NVIC_EnableIRQ(WULIN_IRQn);
    
}


void LINS_RegisterLINStack_IRQ(lin_stack_cb_func_t callback,lin_wakeup_cb_func_t wakeupCallback)
{
#if LIN_STACK_TYPE != LIN_STACK_TYPE_PRIVATE
    lin_stack_callback = callback;
    lin_wakeup_callback = wakeupCallback;
    NVIC_EnableIRQ(LINS_IRQn);
#endif 
}

void LINS_SetSlaveModuleToSleep(void)
{
    LINS_REG_CTRL.SLEEP = 1U;
}


void LINS_AutoAddressingSwitch(AutoaddressingSwitchState_t state)
{
   IOCTRLA_SFRS->LIN.SWON = (uint8_t)state;
}


void LINS_Handler(void)
{
#if LIN_STACK_TYPE == LIN_STACK_TYPE_PRIVATE
    LINS_SLAVE_ISR();
#else
    if (lin_stack_callback != NULL){
        lin_stack_callback();
    }
#endif
}



static LIN_Device_Frame_t linmFrame;


int8_t LINM_WriteBuffer(uint8_t buff[], uint32_t dataLength)
{
    int8_t result;
    if (dataLength > 8U){
        result = -1;
    }else{
        result = 0;
        for (uint8_t i = 0U; i < dataLength; i++){
            LINM_SFRS->DATABUFF[i].DATA = (uint8_t)buff[i];
        }
    }
    return result;
}


int8_t LINM_ReadBuffer(uint8_t buff[], uint32_t dataLength)
{
    int8_t result;
    if (dataLength > 8U){
        result = -1;
    }else{
        result = 0;
        for (uint8_t i = 0U; i < dataLength; i++){
            buff[i] = (uint8_t)LINM_SFRS->DATABUFF[i].DATA;
        }
    }
    return result;
}


void linm_SetTiming(LIN_BaudRate_t BaudRate)
{
    LINM_SFRS->BTDIV07 = (uint8_t)linm_speed_map[BaudRate].divider & 0xFFU;
    LINM_SFRS->BITTIME.BTDIV8 = (uint8_t)linm_speed_map[BaudRate].divider >> 8U;
    LINM_SFRS->BITTIME.BTMULT = (uint8_t)linm_speed_map[BaudRate].multiplier;
    LINM_SFRS->BITTIME.PRESCL = (uint8_t)linm_speed_map[BaudRate].prescale;
}


void LINM_Init(void)
{
    IOCTRLA_SFRS->LIN.LINM_HWMODE       = 1U;    /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
    IOCTRLA_SFRS->LIN.LINM_RXENA        = 1U;    /* LIN receive enable. */
    IOCTRLA_SFRS->LIN.LINM_TXENA        = 1U;    /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINM_PU1K_ENA     = 1U;    /* LIN 1K pullup enable.*/
    IOCTRLA_SFRS->LIN.SWON              = 0U;    /* 0: disable Master function, 1: enable Master function on LINOUT pin */


    LINM_SFRS->BUSTIME.BUSINACTIVE      = E_LIN_TIME_INACTIVE_SEC_4;
    LINM_SFRS->BUSTIME.WUPREPEAT        = E_LIN_TIME_WAKEUP_REPEAT_MS_180;
    LINM_SFRS->CTRL.RST_ERR_INT         = 0x03U;    /* Reset Error, Reset interrupt*/
    LINM_SFRS->DL.DISAUTOSLEEP = 1;
	SYSCTRLA_SFRS->LIN.TXLINM_DR_SLOPE = LIN_TX_SLEW_RATE_5_1V_PER_US;
    linm_SetTiming(E_LIN_BAUDRATE_19200);
}

void LINM_RegisterLINM_IRQ(lin_cb_func_t cb)
{
    linm_callback = cb;
    NVIC_EnableIRQ(LINM_IRQn);
}


void LINM_UnRegisterLINM_IRQ(void)
{
    linm_callback = NULL;
    NVIC_DisableIRQ(LINM_IRQn);
}

int8_t LINM_SendFrame(LIN_Device_Frame_t *frame)
{
    int8_t result;
    linmFrame = *frame;
    LINM_SFRS->ID        = frame->frame_id;
    if (frame->msg_type == LIN_MSG_TYPE_TX){
        LINM_SFRS->DL.LENGTH = frame->length;
        LINM_SFRS->CTRL.TRANSMIT = 1U;
    }else{
        LINM_SFRS->DL.LENGTH = frame->length;
        LINM_SFRS->CTRL.TRANSMIT = 0U;
    }
    LINM_SFRS->DL.ENHCHK = frame->checksum;
    
    (void)LINM_WriteBuffer(frame->data, (uint32_t)frame->length);
    LINM_SFRS->CTRL.STARTREQ = 1U;
    
    return 0;
}

void LINM_Handler(void)
{
    uint8_t status   = LINM_REG_STATUS.BYTE;
    if( ((status & (uint8_t)E_LIN_STATUS_ERROR) == 0U) &&  ((status & (uint8_t)E_LIN_STATUS_WAKEUP) == 0U) ){
        if ( (status & (uint8_t)E_LIN_STATUS_COMPLETE) != 0U ){
            if (linmFrame.msg_type == LIN_MSG_TYPE_RX){
                linmFrame.length   = (uint8_t)LINM_SFRS->DL.LENGTH;
                (void)LINM_ReadBuffer(linmFrame.data, LIN_BUFF_SIZE);
                if (linm_callback != NULL){
                    linm_callback(&linmFrame);
                }
            }
        } 
    }
    LINM_SFRS->CTRL.RST_ERR_INT = 0x03U;    /* Reset Error, Reset interrupt*/
}


/* Please make sure here and don't delete these functions!!!! or it would cause severe error*/
void WULIN_Handler(void)
{
    if (WICA_REG_STATUS.LINS != 0U){
        LINS_REG_CTRL.SLEEP = 0U;                     /*  wake up lins occurs when wake up signal detected  */
        if (lin_wakeup_callback != NULL){
            lin_wakeup_callback();
        }
        WICA_REG_CTRL.LINSIRQCLR = 1U;
    }
}

/* Please make sure here and don't delete these functions!!!! or it would cause severe error*/
/* wakeup timer wake up interrupt ISR */
void WUTIMER_Handler(void)
{
    WICA_SFRS->CTRL.TIMERIRQCLR = 1U;
}





#if LIN_STACK_TYPE == LIN_STACK_TYPE_PRIVATE



static LIN_Device_Frame_t *privateFrameTable = NULL;
static uint8_t privateFrameTableCount = 0U;
static ls_DataReqCallback_t            dataReceivedCallback   = NULL;
static ls_DataRspCallback_t            dataRspCallback        = NULL;
static ls_SleepReqCallback_t           sleepReqCallback       = NULL;

void LINS_RegisterPrivateServices(LIN_Device_Frame_t *frameTable, uint8_t tableSize, const ls_LinsPrivateFramesCallback_t *pFunCallback)
{
    privateFrameTable           = frameTable;
    privateFrameTableCount      = tableSize;
    dataReceivedCallback        = pFunCallback->dataReqCallback;
    dataRspCallback             = pFunCallback->dataRspCallback;
    sleepReqCallback            = pFunCallback->sleepReqCallback;
    NVIC_EnableIRQ(LINS_IRQn);
}


static __INLINE int8_t isValidFrame(LIN_Device_Frame_t *frameTable, uint8_t frameTableLength,uint8_t id)
{
    int8_t index = -1;
    if (frameTable != NULL && id != 0xFFU){
        for (uint8_t i = 0U; i < frameTableLength; i++){
            if (frameTable[i].frame_id == id && frameTable[i].frameIsValid == TRUE){
                index = (int8_t)i;
                break;
            }
        }
    }
    return index;
}

static __INLINE void dataReceivedACK(uint8_t dataLength, ChecksumType_t checksumType)
{
    LINS_REG_DL = dataLength + ((uint8_t)checksumType << LINS_DL_ENHCHK_POS) + (0U << LINS_DL_DISBITMON_POS);
    LINS_REG_CTRL.DATAACK_TPYE = DATAACK_TPYE_RECEIVE;/* it's a data receive ack operation */
}

static __INLINE void dataResponseACK(uint8_t* payload, uint8_t dataLength,ChecksumType_t checksumType)
{
    LINS_REG_DL = dataLength + ((uint8_t)checksumType << LINS_DL_ENHCHK_POS) + (0U << LINS_DL_DISBITMON_POS);
    for (uint8_t i = 0U; i < dataLength; i++){
        LINS_REG_DATA(i) = payload[i];
    }
    LINS_REG_CTRL.DATAACK_TPYE = DATAACK_TPYE_TRANSMIT;/* it's a data transmit ack operation */
}


static void LINS_SLAVE_ISR(void)
{
    static int8_t index = -1;
    static uint8_t command = 0;
    uint8_t status = LINS_REG_STATUS;

    if ((status & E_LIN_STATUS_DATA_REQ) != 0U){
        command = LINS_REG_ID;
        index = isValidFrame(privateFrameTable, privateFrameTableCount,command);
        if (index >= 0){
            if (privateFrameTable[index].msg_type == LIN_MSG_TYPE_RX){
                dataReceivedACK(privateFrameTable[index].length, (ChecksumType_t)privateFrameTable[index].checksum);
            }else{
                if (dataRspCallback != NULL){
                    dataRspCallback(&privateFrameTable[index]);// feed data based on FID
                }
                dataResponseACK(privateFrameTable[index].data, privateFrameTable[index].length,(ChecksumType_t)privateFrameTable[index].checksum);
            }
        }else{
            LINS_SFRS->CTRL.STOP = 1U;
        }
    }else if ((status & E_LIN_STATUS_COMPLETE) != 0U){
        if (LINS_REG_TX_STATUS.COMPLETE == 0U){ /*receive finished */
            if (index >= 0){
                for (uint8_t i = 0U; i < LIN_BUFF_SIZE; i++){
                    privateFrameTable[index].data[i] = LINS_REG_DATA(i);
                }
                if (dataReceivedCallback != NULL){
                    dataReceivedCallback(&privateFrameTable[index]);
                }
            }
        }
    }else if ((status & E_LIN_STATUS_IDLE_TIMEOUT) != 0U){
        LINS_SFRS->CTRL.SLEEP = 0; /* workaround for LIN IP would set sleep   */
        if (sleepReqCallback != NULL){
            sleepReqCallback();
        }
    }else if ((status & E_LIN_STATUS_ERROR) != 0U){
        /*intent to empty*/
    }else{
        /*intent to empty*/
    }
    LINS_SFRS->CTRL.RST_INT_ERR  = 0x03U;    /*reset error,reset interrupt  */
}


#endif



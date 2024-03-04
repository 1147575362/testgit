/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file linSlaveTask.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/09/10
 */

#include <linSlaveTask.h>
#include <linStackTask.h>
#include <applicationTask.h>
#include <linsNodeCfgIdentify.h>
#include <softTimerTask.h>
#include <taskManager.h>
#include <ColorMixingTask.h>
#include <isrfuncs.h>
#include <measureTask.h>
#include <pdsTask.h>
#include <safetyMonitorTask.h>
#include <custom_proto.h>

#if LIN_STACK_TYPE == LIN_STACK_TYPE_LIN2_2A

#define FID_COLOR_CTRL DEFAULT_LINS_FID0
#define FID_STATUS_FRAME DEFAULT_LINS_FID1
#define USER_DATA_REPORT DEFAULT_LINS_FID2
#define EVENT_TRIGGER_DATA_REPORT DEFAULT_LINS_FID3
#define USER_COLOR_CTRL (0x24)

#define DIAG_DATA_BUFF_SIZE (128U)

#define FID_TABLE_INDEX_0 (0)
#define FID_TABLE_INDEX_1 (1)
#define FID_TABLE_INDEX_2 (2)
#define FID_TABLE_INDEX_3 (3)

#define FID_COLOR_CTRL_INDEX (0)
#define FID_STATUS_FRAME_INDEX (1)
#define USER_DATA_REPORT_INDEX (2)
#define EVENT_TRIGGERED_INDEX (3)
#define USER_COLOR_CTRL_INDEX (4)

/* ******************internal function declarations****************************/
void DiagnosticSubscribedCmdsHandle(const DiagReqInfo_t *const frameInfo);
void UnconditionalSubscribedCmdsHandle(LIN_Device_Frame_t const *frame);
void UnconditionalPublishedCmdsTxFinishedISR(uint8_t fid, uint8_t resvd);
void UnconditionalPublishedCmdsISR(LIN_Device_Frame_t *const frame);
void DiagnosticSleepRequestHandle(SleepRequestType_t type);
void BusWakeUpRequestHandle(BusWakeUpRequestResult_t result);
void DiagReqLogPrint(const LIN_Device_Frame_t *const frame);

/* *******************global variants declarations*****************************/
static TaskState_t linsTaskState = TASK_STATE_INIT;
static uint8_t diagDataRspBuff[DIAG_DATA_BUFF_SIZE];
static uint8_t diagDataRecvBuff[DIAG_DATA_BUFF_SIZE];
static uint8_t busWakeupRetryCount = 0U;
uint8_t appNoSleepFlag =0;
void busWakeupRetryTimerExpired(SoftTimer_t *timer);

static SoftTimer_t busWakeupRetryTimer = {
    .mode = TIMER_ONE_SHOT_MODE,
    .interval = 2250U,
    .handler = busWakeupRetryTimerExpired};

static uint8_t eventTriggeredLedOn = FALSE;

/*  LIN command handle callbacks declarations  */
static ls_LinsFramesCallback_t linsFramesCallback = {
    UnconditionalSubscribedCmdsHandle,       /* received data from master */
    UnconditionalPublishedCmdsISR,           /* send data to master, it's an interrupt function, please fill the data as fast as possible */
    UnconditionalPublishedCmdsTxFinishedISR, /* send data to master finished, it's an interrupt function, please fill the data as fast as possible  */
    DiagnosticSubscribedCmdsHandle,          /* Diagnostic sigle PDU and MultiPDU received data from master */
    NULL,                                    /* special functional NAD (0x7E) handler*/
    DiagnosticSleepRequestHandle,            /* Diagnostic sleep request from master */
    BusWakeUpRequestHandle,                  /* Get the bus wake up result this would take effects after call API: ls_send_wake_up_bus_signal()*/
    NULL,                                    /* SAE J2602 -1 Configuration Messages */
    NULL,                                    /*  5.7.2.5 DNN Based Broadcast Messages */
};

/* **FID table declarations****/
static LIN_Device_Frame_t UnconditionalCmdsTable[DEFAULT_LINS_FID_SIZE] = {
    [FID_COLOR_CTRL_INDEX] = {
        .frame_id = FID_COLOR_CTRL,
        .msg_type = LIN_MSG_TYPE_RX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length = LIN_BUFF_SIZE,
        .frameIsValid = TRUE,
    },
    [FID_STATUS_FRAME_INDEX] = {
        .frame_id = FID_STATUS_FRAME, /* status management frame */
        .msg_type = LIN_MSG_TYPE_TX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length = LIN_BUFF_SIZE,
        .frameIsValid = TRUE,
    },
    [USER_DATA_REPORT_INDEX] = {
        .frame_id = USER_DATA_REPORT, /* user data report */
        .msg_type = LIN_MSG_TYPE_TX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length = LIN_BUFF_SIZE,
        .frameIsValid = TRUE,
        .linkedEventTriggerFidTableIndex = EVENT_TRIGGERED_INDEX,
    },
    [EVENT_TRIGGERED_INDEX] = {
        .frame_id = EVENT_TRIGGER_DATA_REPORT, /* event trigger data report */
        .msg_type = LIN_MSG_TYPE_TX_EVENT,     /*event trigger data request from sender for slave; */
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length = LIN_BUFF_SIZE,
        .frameIsValid = TRUE,
        .eventTriggered = FALSE,
        .linkedEventTriggerFidTableIndex = USER_DATA_REPORT_INDEX,
    },
    [USER_COLOR_CTRL_INDEX] = {
        .frame_id = USER_COLOR_CTRL,
        .msg_type = LIN_MSG_TYPE_RX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length = LIN_BUFF_SIZE,
        .frameIsValid = TRUE,
    },
};

/* please don't do any changes for it would be used by lin stack */
LIN_Device_Frame_t *LINS_GetUcndFramesTable(uint8_t *tableItemsCount)
{
    *tableItemsCount = (uint8_t)(sizeof(UnconditionalCmdsTable) / sizeof(LIN_Device_Frame_t));
    return UnconditionalCmdsTable;
}

/*
  Diagnostic subscribed frame received from LIN master
*/
void DiagnosticSubscribedCmdsHandle(const DiagReqInfo_t *const diagReq)
{
    DiagRspInfo_t diagRsp = {
        .sid = diagReq->sid,
        .type = diagReq->type,
        .payload = diagDataRspBuff,
    };
    if (diagReq->type == PDU_TYPE_SINGLE_PDU)
    {
        if (diagReq->sid >= LIN_SID_ASSIGN_NAD && diagReq->sid <= LIN_SID_ASSIGN_FRAME_ID_RANGE)
        {
            /* handle Node configuration and Identification commands*/
            diagRsp.type = PDU_TYPE_SINGLE_PDU;
            if (LNCI_HandleNodeCfgIdentifyRequest(diagReq, &diagRsp) == TRUE)
            {
                ls_handle_diagnostic_response(&diagRsp);
            }
        }
        else
        {
            if (diagReq->sid == LIN_SID_APP_DIAG_LED_GET_CONFIG)
            {
                /*  prepare diagnostic single pdu, user defined diagnostic frame, here is for LED param configuration*/
                for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
                {
                    diagDataRecvBuff[i + diagReq->packCurrIndex] = diagReq->payload[i];
                }
                if (APPL_PrepareLedParamRequest(diagReq->sid, diagDataRecvBuff, &diagRsp) == TRUE)
                {
                    ls_handle_diagnostic_response(&diagRsp);
                }
            }
            else if (diagReq->sid == LIN_SID_APP_DIAG_SINGLE_PDU_RECV_MULTI_RSP)
            {
                /*  handle diagnostic single pdu, multi response, user defined diagnostic frame here for example: */
                for (uint16_t i = 0U; i < 15U; i++)
                {
                    diagDataRspBuff[i] = (uint8_t)i;
                }
                diagRsp.sid = diagReq->sid;
                diagRsp.packLength = 15U;
                diagRsp.type = (diagRsp.packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
                ls_handle_diagnostic_response(&diagRsp);
            }
#ifdef BOOTLOADER_EN
            else if (diagReq->sid == LIN_SID_APP_DIAG_BOOT_MODE && diagReq->packLength == sizeof(EnterBootloaderFrame_t))
            {
                EnterBootloaderFrame_t *boot = (EnterBootloaderFrame_t *)((void *)diagReq->payload);
                if (boot->command == BOOT_MODE_HANDSHAKE && boot->magicKey == BOOT_MODE_MAGIC_KEY_HANDSHAKE)
                {
                    /*  Reset Chip   */
                    SYSCTRLA_REG_RETAIN0 = 0x05U;
                    CRGA_REG_RESETCTRL.HARDRSTREQ = 1U;
                }
            }
#endif
            else if (LIN_SID_CUSTOM_PROTO == diagReq->sid)
            {
                if (lnci_handle_custom_request(diagReq, &diagRsp) == TRUE)
                {
                    ls_handle_diagnostic_response(&diagRsp);
                }
            }
        }
    }
    else
    {
        /*  handle diagnostic multi pdu received sigle pdu response, user defined diagnostic frame here   */
        if (diagReq->sid == LIN_SID_APP_DIAG_LED_SET_CONFIG)
        {
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
            {
                diagDataRecvBuff[i + diagReq->packCurrIndex] = diagReq->payload[i];
            }
            /* Package received finished */
            if ((diagReq->frameDataLength + diagReq->packCurrIndex) >= diagReq->packLength)
            {
                /* handle finished data here  */
                if (APPL_HandleLedParamConfigRequest(diagReq->sid, diagDataRecvBuff, &diagRsp) == TRUE)
                {
                    ls_handle_diagnostic_response(&diagRsp);
                }
            }
        }
        else if (diagReq->sid == LIN_SID_APP_DIAG_LED_GET_CONFIG)
        {
            /*  handle diagnostic multi pdu received multi pdu response, user defined diagnostic frame here   */
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
            {
                diagDataRecvBuff[i + diagReq->packCurrIndex] = diagReq->payload[i];
            }
            if ((diagReq->frameDataLength + diagReq->packCurrIndex) >= diagReq->packLength)
            {
                /*  prepare diagnostic single pdu, user defined diagnostic frame, here is for LED param configuration*/
                if (APPL_PrepareLedParamRequest(diagReq->sid, diagDataRecvBuff, &diagRsp) == TRUE)
                {
                    ls_handle_diagnostic_response(&diagRsp);
                }
            }
        }
        else if (diagReq->sid == LIN_SID_APP_DIAG_MULTI_PDU_RECV_SINGLE_RSP)
        {
            /*  handle diagnostic multi pdu received single pdu response, user defined diagnostic frame here   */
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
            {
                diagDataRecvBuff[i + diagReq->packCurrIndex] = diagReq->payload[i];
            }
            /* Package received finished */
            if ((diagReq->frameDataLength + diagReq->packCurrIndex) >= diagReq->packLength)
            {
                /* handle finished data here  */
                for (uint16_t i = 0U; i < 6U; i++)
                {
                    diagDataRspBuff[i] = (uint8_t)i;
                }
                /* handle response data here  */
                diagRsp.sid = diagReq->sid;
                diagRsp.packLength = 5U; /* Valid data length not includes sid*/
                diagRsp.type = (diagRsp.packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
                ls_handle_diagnostic_response(&diagRsp);
            }
        }
        else if (diagReq->sid == LIN_SID_APP_DIAG_MULTI_PDU_RECV_MULTI_RSP)
        {
            /*  handle diagnostic multi pdu received multi pdu response, user defined diagnostic frame here   */
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
            {
                diagDataRecvBuff[i + diagReq->packCurrIndex] = diagReq->payload[i];
            }
            /* Package received finished */
            if ((diagReq->frameDataLength + diagReq->packCurrIndex) >= diagReq->packLength)
            {
                /* handle finished data here  */
                /* handle response data here  */
                for (uint16_t i = 0U; i < 15U; i++)
                {
                    diagDataRspBuff[i] = (uint8_t)(DIAG_DATA_BUFF_SIZE - i);
                }
                diagRsp.sid = diagReq->sid;
                diagRsp.packLength = 15U; /* Valid data length not includes sid*/
                diagRsp.type = (diagRsp.packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
                ls_handle_diagnostic_response(&diagRsp);
            }
        }
        else
        {
        }
    }
}

/*
  Received sleep command from lin master or bus idle timeout occurs
*/
void DiagnosticSleepRequestHandle(SleepRequestType_t type)
{
    if (type == SLEEP_REQ_TYPE_MASTER_REQ)
    {
        if (FALSE == appNoSleepFlag)
        {
            SYSCTRLA_REG_RETAIN0 = 0xE;
            ls_clr_go_to_sleep_flag();
            /* Set lin to sleep mode */
            LINS_SetSlaveModuleToSleep();
            /* Set system to hibranate mode*/
            PMU_EnterDeepSleepMode();
        }
    }
}

void busWakeupRetryTimerExpired(SoftTimer_t *timer)
{
    ls_send_wake_up_bus_signal();
}

/* Called timeout after bus wake up 3 tries, or called when bus recovery */
void BusWakeUpRequestHandle(BusWakeUpRequestResult_t result)
{
    if (result == BUS_WAKEUP_REQ_RESULT_REQ_TIMEOUT)
    {
        if (busWakeupRetryCount == 1U)
        {
            busWakeupRetryCount++;
            SoftTimer_Start(&busWakeupRetryTimer);
        }
        else
        {
            busWakeupRetryCount = 0U;
        }
    }
    else
    {
        busWakeupRetryCount = 0U;
        SoftTimer_Stop(&busWakeupRetryTimer);
    }
}

void UnconditionalPublishedCmdsTxFinishedISR(uint8_t fid, uint8_t resvd)
{
    if (fid == UnconditionalCmdsTable[FID_STATUS_FRAME_INDEX].frame_id)
    {
        ls_ifc_clear_error_status();
        ls_clr_error_code_log();
    }
}

/*
  User defined published frame callback function which from LINS ISR
  please fill frame data[0-7] only, please don't do any changes on the left info except data[x]!!!!
*/
void UnconditionalPublishedCmdsISR(LIN_Device_Frame_t *const frame)
{
    /*   feed buffer to frame   */
    if (frame->frame_id == UnconditionalCmdsTable[FID_STATUS_FRAME_INDEX].frame_id)
    {
        /* status management frame */
        lin_status_t status = ls_ifc_read_status();
        frame->data[0] = (uint8_t)status.payload;
        frame->data[1] = LINS_GetPIDFromFID(status.pid);
        frame->data[2] = ls_read_error_code_log();
        frame->data[3] = MES_DataIntegrityError() | ((uint8_t)SAFM_GetBatteryState() << 2) | ((uint8_t)SAFM_GetChipTemperatureState() << 4);

        /* MUST BE 4 bytes Aligned  */
        uint32_t *ledPNState = frame->data32 + 1U;
        *ledPNState = 0U;
        for (uint8_t i = 0U; i < LED_NUM; i++)
        {
            (*ledPNState) |= (uint32_t)SAFM_GetLedPNStateState((LedNum_t)i) << (i * 2U);
        }
    }
    else if (frame->frame_id == UnconditionalCmdsTable[USER_DATA_REPORT_INDEX].frame_id)
    {
        /* user defined data report */
        frame->data[0] = (APPL_GetLEDState()) | (eventTriggeredLedOn << 1U) | 0xF8U;
        for (uint8_t i = 1U; i < LIN_BUFF_SIZE; i++)
        {
            frame->data[i] = 0xFFU;
        }
    }
    else if (frame->frame_id == UnconditionalCmdsTable[EVENT_TRIGGERED_INDEX].frame_id)
    {
        /* event trigger data report */
        if (frame->eventTriggered == TRUE)
        {
            frame->data[0] = LINS_GetPIDFromFID(UnconditionalCmdsTable[UnconditionalCmdsTable[EVENT_TRIGGERED_INDEX].linkedEventTriggerFidTableIndex].frame_id); /* transfer FID to PID  */
            frame->data[1] = (APPL_GetLEDState()) | (eventTriggeredLedOn << 1U) | 0xF8U;
            for (uint8_t i = 2U; i < LIN_BUFF_SIZE; i++)
            {
                frame->data[i] = 0xFFU;
            }
        }
    }
    else
    {
        /* intent to empty   */
    }
}

void LINS_EventTriggered_ISR(void)
{
    if (eventTriggeredLedOn == TRUE)
    {
        eventTriggeredLedOn = FALSE;
    }
    else
    {
        eventTriggeredLedOn = TRUE;
    }
    UnconditionalCmdsTable[EVENT_TRIGGERED_INDEX].eventTriggered = TRUE;
}

uint8_t LINS_GetPIDFromFID(uint8_t frameId)
{
    uint8_t id = frameId;
    uint8_t P0, P1;
    P0 = (uint8_t)(((id >> 0) & 0x01U) ^ ((id >> 1) & 0x01U) ^ ((id >> 2) & 0x01U) ^ ((id >> 4) & 0x01U));
    P1 = (uint8_t)(~(((id >> 1) & 0x01U) ^ ((id >> 3) & 0x01U) ^ ((id >> 4) & 0x01U) ^ ((id >> 5) & 0x01U))) & 0x01U;
    id = frameId | (P0 << 6) | (P1 << 7);
    return id;
}

void LINS_SendWakeUpSignal_ISR(void)
{
    if (busWakeupRetryCount == 0U)
    {
        busWakeupRetryCount++;
        ls_send_wake_up_bus_signal();
    }
}

/*
  User defined subscribed frame received from LIN master
*/
void UnconditionalSubscribedCmdsHandle(LIN_Device_Frame_t const *frame)
{
    if (frame->frame_id == UnconditionalCmdsTable[FID_COLOR_CTRL_INDEX].frame_id)
    {
        APPL_HandleColorControlCommands(frame);
    }
    if (frame->frame_id == UnconditionalCmdsTable[USER_COLOR_CTRL_INDEX].frame_id)
    {
        APPL_HandleBcmControlCommands(frame);
    }
}

void DiagReqLogPrint(const LIN_Device_Frame_t *const frame)
{
}

void LINS_TaskHandler(void)
{
    switch (linsTaskState)
    {
    case TASK_STATE_ACTIVE:
        break;
    case TASK_STATE_INIT:
        ls_register_services(LIN_PROTOCOL_LIN2_2A, UnconditionalCmdsTable, (l_u8)(sizeof(UnconditionalCmdsTable) / sizeof(LIN_Device_Frame_t)), DIAG_DATA_BUFF_SIZE, &linsFramesCallback);
        (void)ls_set_tp_timeout(N_AS, N_CR);
        ls_registerDiagReqLogPrint(DiagReqLogPrint);
        /* Note that please don't change the following glitch configuration!!!!*/
        (void)ls_set_lins_rx_glitch_filter_1st(0x00U, 0x08U);
        (void)ls_set_lins_rx_glitch_filter_2nd(0x0AU, 0x10U);
        (void)ls_set_lins_rx_glitch_filter_3rd(0x30U, 0x30U);
        (void)l_sys_init();
        (void)ls_disable_lin_auto_sleep(TRUE);
        linsTaskState = TASK_STATE_ACTIVE;
        break;
    default:
        break;
    }
}

#endif
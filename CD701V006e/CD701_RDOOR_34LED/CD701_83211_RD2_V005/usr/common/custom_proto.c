#include <appConfig.h>
#include "custom_proto.h"
#include "linsNodeCfgIdentify.h"
#include <pdsTask.h>

uint8_t lnci_handle_custom_request(const DiagReqInfo_t *const diagReq, DiagRspInfo_t *const diagRsp)
{
    stCustomDiagCmd_t *pData = (stCustomDiagCmd_t *)diagReq->payload;
    uint8_t cmd = pData->command;
    uint8_t bresponse = FALSE;
    switch (cmd)
    {
    case CUSTOMCMD_READ_VERSION:
    {
        extern SystemParams_t systemParam;
        diagRsp->type = PDU_TYPE_SINGLE_PDU;
        diagRsp->sid = diagReq->sid + 0x40;
        diagRsp->packLength = 5;
        diagRsp->payload[0] = 2;                            // version
        diagRsp->payload[1] = 0;                            //
        diagRsp->payload[2] = 0xB0 + (ls_read_nad() & 0xF); // 0xA0=bl,  0xb0=app
        diagRsp->payload[3] = IOCTRLA_SFRS->LIN.SWON;
        diagRsp->payload[4] = 0x0;
        bresponse = TRUE;
        break;
    }
    case CUSTOMCMD_SWITCH_SWON:
    {
        uint8_t appNoSleepFlag;
        uint8_t onoff = pData->scmd.subcmd;
        DEBUG_OUT("onoff = %d   %02X,%02X,%02X\n", onoff, pData->data[0], pData->data[1], pData->data[2]);
        if (onoff)
        {
            // close
            IOCTRLA_SFRS->LIN.LINM_HWMODE = 0U;   /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
            IOCTRLA_SFRS->LIN.LINM_RXENA = 0U;    /* LIN receive enable. */
            IOCTRLA_SFRS->LIN.LINM_TXENA = 0U;    /* LIN transmit enable.*/
            IOCTRLA_SFRS->LIN.LINM_PU1K_ENA = 0U; /* LIN 1K pullup enable.*/
            IOCTRLA_SFRS->LIN.SWON = 1U;          /* 0: disable Master function, 1: enable Master function on LINOUT pin */
            appNoSleepFlag = TRUE;
        }
        else
        {
            // lin switch open
            IOCTRLA_SFRS->LIN.LINM_HWMODE = 1U;   /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
            IOCTRLA_SFRS->LIN.LINM_RXENA = 1U;    /* LIN receive enable. */
            IOCTRLA_SFRS->LIN.LINM_TXENA = 1U;    /* LIN transmit enable.*/
            IOCTRLA_SFRS->LIN.LINM_PU1K_ENA = 1U; /* LIN 1K pullup enable.*/
            IOCTRLA_SFRS->LIN.SWON = 0U;          /* 0: disable Master function, 1: enable Master function on LINOUT pin */
            appNoSleepFlag = FALSE;
        }
        break;
    }

    case CUSTOMCMD_JUMP_BLandSTAY:
    {
        DEBUG_OUT("hand shake reset in bootloader\n");
        SYSCTRLA_REG_RETAIN0 = 0x06U;
        CRGA_REG_RESETCTRL.HARDRSTREQ = 1U;
        break;
    }
    case CUSTOMCMD_JUMP_APP:
    {
        // app no jump
        break;
    }
#if 0
    case n:{
        // let a slave 02 to stay in bootloader

        LIN_Device_Frame_t sendCmd;
        sendCmd.frame_id = 0x3C;
        sendCmd.msg_type = LIN_MSG_TYPE_TX;
        sendCmd.length = LIN_BUFF_SIZE;
        sendCmd.checksum = LIN_CHECKSUM_CLASSIC;
        sendCmd.data[0] = 0x02; // nad ,hard code
        sendCmd.data[1] = 0x06; // PCI
        sendCmd.data[2] = 0x20; // sid
        sendCmd.data[3] = 0x00; // cmd
        sendCmd.data[4] = 0x12; //
        sendCmd.data[5] = 0x34; //
        sendCmd.data[6] = 0x04; // stay in bl
        sendCmd.data[7] = 0x02; // slave nad
        LINM_SendFrame(&sendCmd);
        break;
    }
#endif

    default:
        break;
    }

    return bresponse;
}
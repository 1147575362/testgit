#ifndef __CUSTOM_PROTO_H__
#define __CUSTOM_PROTO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <linStackTask.h>

#define LIN_SID_CUSTOM_PROTO (0x4E)
#define LIN_SID_CD701 (0x71)
#define CUSTOMCMD_READ_VERSION (0x1)
#define CUSTOMCMD_SWITCH_SWON (0x2)
#define CUSTOMCMD_JUMP_BLandSTAY (0x3)
#define CUSTOMCMD_JUMP_APP (0x4)

    // sid = 0x4E
    typedef struct
    {
        uint8_t command; // 0x01 ... n
        union
        {
            uint8_t data[4];
            struct
            {
                uint8_t subcmd;
                uint8_t target_nad;
                uint8_t payload[2];
            } scmd;
        };

    } stCustomDiagCmd_t;

    uint8_t lnci_handle_custom_request(const DiagReqInfo_t *const diagReq, DiagRspInfo_t *const diagRsp);

#ifdef __cplusplus
}
#endif

#endif

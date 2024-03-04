#ifndef PDS_H__
#define PDS_H__

#include <stdint.h>
#include <stdbool.h>
#include <taskManager.h>

/*  please don't do any change for it would be used in color mixing  */
#define CFL_LED_NUM (24U) /* Max support 16 rgb Leds */

#if CFL_LED_NUM != 24U
#error max supports 24 leds!*/
#endif

/* ************color mixing  configuration default value******************** */
#define DEFAULT_COORDINATE_R_X (45088U)
#define DEFAULT_COORDINATE_R_Y (20217U)
#define DEFAULT_COORDINATE_G_X (11016U)
#define DEFAULT_COORDINATE_G_Y (45468U)
#define DEFAULT_COORDINATE_B_X (9410U)
#define DEFAULT_COORDINATE_B_Y (3401U)

#define DEFAULT_COORDINATE_WHITE_POINT_X (21845U)
#define DEFAULT_COORDINATE_WHITE_POINT_Y (21845U)

#define DEFAULT_MAX_INTENSITY_R (851U)
#define DEFAULT_MAX_INTENSITY_G (1600U)
#define DEFAULT_MAX_INTENSITY_B (267U)

#define DEFAULT_MIN_INTENSITY_R (DEFAULT_MAX_INTENSITY_R * 65U / 100U)
#define DEFAULT_MIN_INTENSITY_G (DEFAULT_MAX_INTENSITY_G * 95U / 100U)
#define DEFAULT_MIN_INTENSITY_B (DEFAULT_MAX_INTENSITY_B * 95U / 100U)

#define DEFAULT_LED_PN_VOLT_R (1804U)
#define DEFAULT_LED_PN_VOLT_G (2838U)
#define DEFAULT_LED_PN_VOLT_B (2608U)
#define INIT_LIGHT_MAX_LV (0x0a)
/**************************************************************************************************/
/* !!!!!please don't do any changes for the defines below would be used in color mixing library !!!!!!*/
/*  led parameters */

typedef enum
{
    LED0 = 0U,
    LED1,
    LED2,
    LED3,
    LED4,
    LED5,
    LED6,
    LED7,
    LED8,
    LED9,
    LED10,
    LED11,
    LED12,
    LED13,
    LED14,
    LED15,
    LED16,
    LED17,
    LED18,
    LED19,
    LED20,
    LED21,
    LED22,
    LED23,
} LedNum_t;

typedef enum
{
    LED_RED = 0U,
    LED_GREEN,
    LED_BLUE
} LedType_t;

typedef struct
{
    uint16_t x; /*0-65535  based on color xy range xy(0.3,0.3) = (19660,19660)*/
    uint16_t y; /*0-65535  based on color xy range xy(0.3,0.3) = (19660,19660)*/
} Coordinate_t;

/* size : 3*4 + 6*2 =  24  */
typedef struct
{
    Coordinate_t color[3];
    uint16_t maxTypicalIntensity[3]; /*0-65535 @25C*/
    int16_t ledTypicalPNVolt[3];     /* @25C*/
} LedPhyParam_t;

typedef union
{
    struct
    {
        Coordinate_t whitePoint;        /* size: 4bytes  */
        LedPhyParam_t led[CFL_LED_NUM]; /* size: 24bytes * CFL_LED_NUM */
        uint16_t minIntensity[3];       /* size: 6bytes min rgb Intensity, red has more degration */
        uint8_t lightMaxLevel;          //
    };
    uint8_t payload[CFL_LED_NUM * 24U + 12U];
} LedClusterParams_t;

typedef struct
{
    uint8_t frameId;
    uint8_t frameIsValid;
} FrameID_t;

/**************************************************************************************************/
/*  lin parameters */
typedef union
{
    /* 16 + 32*2 = 80  */
    struct
    {
        uint16_t fwVersion;  /*  Reserved should be LINS_FW_VERSION    */
        uint16_t hwVersion;  /*  Reserved should be LINS_HW_VERSION    */
        uint32_t serialNo;   /*  Reserved should be LINS_SERIAL_NO     */
        uint16_t supplierID; /*  Reserved should be LINS_SUPPLIER_ID   */
        uint16_t functionID; /*  Reserved should be LINS_FUNCTION_ID   */
        uint8_t variant;     /*  Reserved should be LINS_VARIANT       */
        uint8_t nad;
        uint8_t fid;
        uint8_t frameIDSize;
        FrameID_t frameInfo[32];
        uint8_t light_max_lv;
    };
    uint8_t payload[96U];
} LINClusterParams_t;
/**************************************************************************************************/

typedef enum
{
    PDS_STATUS_USING_NORMAL,
    PDS_STATUS_USING_BACKUP,
    PDS_STATUS_USING_DEFAULT,
} PdsStatus_t;

typedef struct
{
    struct
    {
        uint32_t crc32;
        uint32_t index;
    } head; /*  size = 8 bytes  */
    struct
    {
        LINClusterParams_t linParams; /*  size = 96 bytes */
        LedClusterParams_t ledParams; /*  size = 394 bytes */
    } info;
} SystemParams_t;

extern SystemParams_t systemParam;

LedClusterParams_t *PDS_GetLedParam(void);
LINClusterParams_t *PDS_GetLINParam(void);
PdsStatus_t PDS_GetPdsStatus(void);
void PDS_Init(void);
void PDS_TaskHandler(void);
void changeMaxLight( uint8_t Light);
uint8_t GetFID();
uint8_t GetNAD();
uint8_t Getlight_max_lv();
#endif
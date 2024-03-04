#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <appConfig.h>
#include <softTimerTask.h>
#include <systick_device.h>
#include <taskManager.h>

#include <ColorMixingTask.h>
#include <lin_device.h>
#include <adc_device.h>
#include <gpio_device.h>
#include <linMasterTask.h>
#include <ledParamTables.h>
#include <timer_device.h>
#include <pmu_device.h>
#include <linStackTask.h>
#include <crc32.h>

typedef enum
{
  CMD_COLOR_CTRL_XYY = 0x00,
  CMD_COLOR_CTRL_HSL = 0x01,
  CMD_COLOR_CTRL_RGBL = 0x02,
  CMD_COLOR_CTRL_RGB = 0x03,
  CMD_COLOR_CTRL_PWM = 0x04,
  CMD_COLOR_CTRL_LUV = 0x05,
  CMD_COLOR_CTRL_XYY_EXT = 0x06
} ColorCtrlCommand_t;

typedef union
{
  struct
  {
    uint8_t nodeAddress;
    uint8_t channel : 4;
    uint8_t command : 4;
  };
  struct
  {
    uint8_t nodeAddress;
    uint8_t channel : 4;
    uint8_t command : 4;
    uint16_t x;
    uint16_t y;
    uint8_t Y;
    uint8_t transitionTime; /* 100ms/count */
  } ColorXY;
  struct
  {
    uint8_t nodeAddress;
    uint8_t channel : 4;
    uint8_t command : 4;
    uint16_t x;
    uint16_t y;
    uint16_t Y;
  } ColorXY_EXT;
  struct
  {
    uint8_t nodeAddress;
    uint8_t channel : 4;
    uint8_t command : 4;
    uint16_t hue;
    uint8_t saturation;
    uint8_t level;
    uint8_t transitionTime; /* 100ms/count */
  } ColorHSL;
  struct
  {
    uint8_t nodeAddress;
    uint8_t channel : 4;
    uint8_t command : 4;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t level;
    uint8_t transitionTime; /* 100ms/count */
  } ColorRGBL;
  struct
  {
    uint8_t nodeAddress;
    uint8_t channel : 4;
    uint8_t command : 4;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t transitionTime; /* 100ms/count */
  } ColorRGB;
  struct
  {
    uint8_t nodeAddress;
    uint8_t channel : 4;
    uint8_t command : 4;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t transitionTime; /* 100ms/count */
  } ColorPWM;
  struct
  {
    uint8_t nodeAddress;
    uint8_t channel : 4;
    uint8_t command : 4;
    uint16_t u;
    uint16_t v;
    uint8_t level;
    uint8_t transitionTime; /* 100ms/count */
  } ColorLUV;
  uint8_t data[LIN_BUFF_SIZE]; /*!< data buffer. */
} ColorCtrlFrame_t;

#define APP_DIAG_GET_LED_PHY_PARAM (0x00U)
#define APP_DIAG_SET_LED_PHY_PARAM (0x01U)
#define APP_DIAG_GET_LED_GENERAL_PARAM (0x02U)
#define APP_DIAG_SET_LED_GENERAL_PARAM (0x03U)
#define APP_DIAG_GET_LED_TYPICAL_PN_VOLT (0x04U)
#define APP_DIAG_SET_LED_TYPICAL_PN_VOLT (0x05U)
#define APP_DIAG_GET_LED_RUN_TIME_INFO (0x06U)
#define APP_DIAG_GET_LED_STATIC_PN_VOLT (0x07U)
#define APP_DIAG_GET_LED_RGB_RUN_TEMP (0x08U)
#define APP_DIAG_GET_LED_RGB_RUN_VOLT (0x09U)
#define APP_DIAG_GET_SDK_VERSION (0x0FU)

#define COLOR_TABLE_SIZE (257U)

// #define VE_2_AQ_4_TG_P		//蓝(鸿利)
#define VE_2_AR_5_TG_P // 橙(鸿利)

typedef struct
{
  float x;
  float y;
} xy_shift_t;

#ifdef VE_2_AR_5_TG_P // 橙(鸿利)
#if (PCBAType == CD701)
#define xy_shift_defined
static const xy_shift_t xy_shift[ColourContrUpperLimit] = {
    [0] = {0.005, 0.050},
    [1] = {0.015, 0.0039},
    [2] = {0.0157, 0.0037},
    [3] = {0.0162, 0.0034},
    [4] = {0.0168, 0.0036},
    [5] = {0.0171, 0.0036},
    [6] = {0.0174, 0.0034},
    [7] = {0.0177, 0.0039},
    [8] = {0.018, 0.0036},
    [9] = {0.0181, 0.0037},
    [10] = {0.0183, 0.004},
    [11] = {0.0182, 0.0039},
    [12] = {0.0184, 0.0041},
    [13] = {0.0182, 0.0043},
    [14] = {0.018, 0.0043},
    [15] = {0.0179, 0.0044},
    [16] = {0.0179, 0.0044},
    [17] = {0.0174, 0.0044},
    [18] = {0.0174, 0.0045},
    [19] = {0.0169, 0.0046},
    [20] = {0.0164, 0.0046},
    [21] = {0.016, 0.0047},
    [22] = {0.0155, 0.0046},
    [23] = {0.0149, 0.0045},
    [24] = {0.0145, 0.0044},
    [25] = {0.014, 0.0043},
    [26] = {0.0134, 0.0042},
    [27] = {0.0128, 0.0042},
    [28] = {0.0124, 0.004},
    [29] = {0.0118, 0.0039},
    [30] = {0.0115, 0.0038},
    [31] = {0.0111, 0.0038},
    [32] = {0.0108, 0.0037},
    [33] = {0.0107, 0.0037},
    [34] = {0.0106, 0.0035},
    [35] = {0.0106, 0.0035},
    [36] = {0.0102, 0.0029},
    [37] = {0.0101, 0.0028},
    [38] = {0.0097, 0.0027},
    [39] = {0.0097, 0.0025},
    [40] = {0.0097, 0.0026},
    [41] = {0.0096, 0.0022},
    [42] = {0.0095, 0.0019},
    [43] = {0.0094, 0.0019},
    [44] = {0.009, 0.0017},
    [45] = {0.0093, 0.0016},
    [46] = {0.0093, 0.0014},
    [47] = {0.009, 0.0012},
    [48] = {0.0092, 0.001},
    [49] = {0.0092, 0.0007},
    [50] = {0.0089, 0.0005},
    [51] = {0.0089, 0.0004},
    [52] = {0.0086, 0.0002},
    [53] = {0.0085, 0},
    [54] = {0.0085, -0.0003},
    [55] = {0.0081, -0.0005},
    [56] = {0.0083, -0.0006},
    [57] = {0.0082, -0.0009},
    [58] = {0.0083, -0.0012},
    [59] = {0.0081, -0.0015},
    [60] = {0.0081, -0.0016},
    [61] = {0.008, -0.0018},
    [62] = {0.008, -0.0021},
    [63] = {0.0078, -0.0022},
    [64] = {0.0079, -0.0024},
    [65] = {0.0078, -0.003},
    [66] = {0.008, -0.0031},
    [67] = {0.008, -0.003},
    [68] = {0.0081, -0.0032},
    [69] = {0.0081, -0.0032},
    [70] = {0.0081, -0.0036},
    [71] = {0.0083, -0.0036},
    [72] = {0.0085, -0.0038},
    [73] = {0.0085, -0.0039},
    [74] = {0.0087, -0.004},
    [75] = {0.0089, -0.0038},
    [76] = {0.0091, -0.0043},
    [77] = {0.0092, -0.0042},
    [78] = {0.0093, -0.0045},
    [79] = {0.0094, -0.0048},
    [80] = {0.0098, -0.0049},
    [81] = {0.0097, -0.0048},
    [82] = {0.01, -0.0052},
    [83] = {0.01, -0.0054},
    [84] = {0.0101, -0.0054},
    [85] = {0.0103, -0.0056},
    [86] = {0.0105, -0.0056},
    [87] = {0.0104, -0.0058},
    [88] = {0.0106, -0.006},
    [89] = {0.0106, -0.0061},
    [90] = {0.0109, -0.0063},
    [91] = {0.011, -0.0062},
    [92] = {0.011, -0.0063},
    [93] = {0.0112, -0.0065},
    [94] = {0.0114, -0.0066},
    [95] = {0.0118, -0.0052},
    [96] = {0.0127, -0.004},
    [97] = {0.0133, -0.0028},
    [98] = {0.014, -0.0015},
    [99] = {0.0144, -0.0004},
    [100] = {0.0149, 0.0003},
    [101] = {0.0154, 0.0011},
    [102] = {0.0128, -0.0079},
    [103] = {0.0127, -0.008},
    [104] = {0.0126, -0.0076},
    [105] = {0.0126, -0.0079},
    [106] = {0.0127, -0.0078},
    [107] = {0.0127, -0.0079},
    [108] = {0.0125, -0.0077},
    [109] = {0.0123, -0.0077},
    [110] = {0.012, -0.0075},
    [111] = {0.012, -0.0073},
    [112] = {0.0117, -0.0069},
    [113] = {0.0113, -0.0067},
    [114] = {0.0108, -0.0065},
    [115] = {0.0106, -0.0062},
    [116] = {0.0107, -0.0056},
    [117] = {0.0108, -0.0051},
    [118] = {0.011, -0.0042},
    [119] = {0.0109, -0.0038},
    [120] = {0.0111, -0.0031},
    [121] = {0.0114, -0.0024},
    [122] = {0.0115, -0.002},
    [123] = {0.0117, -0.0016},
    [124] = {0.0118, -0.0011},
    [125] = {0.0119, -0.0005},
    [126] = {0.0121, -0.0001},
    [127] = {0.0121, 0.0002},
    [128] = {0.0121, 0.0003},
    [129] = {0.0124, 0.001},
    [130] = {0.0124, 0.0011},
    [131] = {0.0124, 0.0016},
    [132] = {0.0125, 0.0021},
    [133] = {0.0125, 0.0019},
    [134] = {0.0126, 0.0024},
    [135] = {0.0128, 0.0023},
    [136] = {0.0128, 0.0027},
    [137] = {0.0128, 0.0029},
    [138] = {0.0129, 0.0031},
    [139] = {0.0129, 0.0032},
    [140] = {0.013, 0.0034},
    [141] = {0.0129, 0.0036},
    [142] = {0.0128, 0.0033},
    [143] = {0.0129, 0.0035},
    [144] = {0.0129, 0.0037},
    [145] = {0.0129, 0.0036},
    [146] = {0.0129, 0.004},
    [147] = {0.0129, 0.0038},
    [148] = {0.0129, 0.0039},
    [149] = {0.0129, 0.0039},
    [150] = {0.0128, 0.0039},
    [151] = {0.0128, 0.0039},
    [152] = {0.0126, 0.0039},
    [153] = {0.0128, 0.0039},
    [154] = {0.0127, 0.0038},
    [155] = {0.0128, 0.0039},
    [156] = {0.0128, 0.0039},
    [157] = {0.0127, 0.0038},
    [158] = {0.0128, 0.0039},
    [159] = {0.0128, 0.0036},
    [160] = {0.0128, 0.0036},
    [161] = {0.0128, 0.0036},
    [162] = {0.0126, 0.0035},
    [163] = {0.0126, 0.0033},
    [164] = {0.0125, 0.0036},
    [165] = {0.0125, 0.0036},
    [166] = {0.0122, 0.0036},
    [167] = {0.0121, 0.0035},
    [168] = {0.0119, 0.0037},
    [169] = {0.0118, 0.0037},
    [170] = {0.0115, 0.0037},
    [171] = {0.0114, 0.0039},
    [172] = {0.0111, 0.0039},
    [173] = {0.011, 0.0039},
    [174] = {0.0109, 0.004},
    [175] = {0.0106, 0.0041},
    [176] = {0.0103, 0.004},
    [177] = {0.0102, 0.0041},
    [178] = {0.0103, 0.0041},
    [179] = {0.0102, 0.0042},
    [180] = {0.0102, 0.0044},
    [181] = {0.0102, 0.0044},
    [182] = {0.0102, 0.0045},
    [183] = {0.0101, 0.0045},
    [184] = {0.0101, 0.0045},
    [185] = {0.01, 0.0045},
    [186] = {0.0101, 0.0046},
    [187] = {0.01, 0.0045},
    [188] = {0.0101, 0.0046},
    [189] = {0.01, 0.0046},
    [190] = {0.01, 0.0046},
    [191] = {0.0099, 0.0046},
    [192] = {0.0099, 0.0046},
    [193] = {0.01, 0.0048},
    [194] = {0.01, 0.0047},
    [195] = {0.0098, 0.0047},
    [196] = {0.0098, 0.0047},
    [197] = {0.0098, 0.0047},
    [198] = {0.0098, 0.0046},
    [199] = {0.0098, 0.0047},
    [200] = {0.0098, 0.0047},
    [201] = {0.0098, 0.0048},
    [202] = {0.0098, 0.0048},
    [203] = {0.0097, 0.0047},
    [204] = {0.0096, 0.0048},
    [205] = {0.0097, 0.0047},
    [206] = {0.0097, 0.0046},
    [207] = {0.0097, 0.0047},
    [208] = {0.0096, 0.0047},
    [209] = {0.0096, 0.0046},
    [210] = {0.0096, 0.0047},
    [211] = {0.0096, 0.0047},
    [212] = {0.0096, 0.0047},
    [213] = {0.0095, 0.0046},
    [214] = {0.0095, 0.0046},
    [215] = {0.0094, 0.0046},
    [216] = {0.0095, 0.0046},
    [217] = {0.0096, 0.0047},
    [218] = {0.0095, 0.0046},
    [219] = {0.0098, 0.0047},
    [220] = {0.01, 0.0048},
    [221] = {0.0104, 0.0049},
    [222] = {0.0107, 0.0051},
    [223] = {0.0111, 0.0053},
    [224] = {0.0113, 0.0053},
    [225] = {0.0117, 0.0054},
    [226] = {0.0122, 0.0055},
    [227] = {0.0125, 0.0057},
    [228] = {0.0131, 0.0059},
    [229] = {0.0135, 0.006},
    [230] = {0.014, 0.0061},
    [231] = {0.0145, 0.0064},
    [232] = {0.0154, 0.0067},
    [233] = {0.0158, 0.0067},
    [234] = {0.0161, 0.0067},
    [235] = {0.0164, 0.0067},
    [236] = {0.0169, 0.0068},
    [237] = {0.0171, 0.0067},
    [238] = {0.0174, 0.0067},
    [239] = {0.0176, 0.0066},
    [240] = {0.018, 0.0066},
    [241] = {0.0184, 0.0066},
    [242] = {0.0185, 0.0066},
    [243] = {0.0187, 0.0065},
    [244] = {0.019, 0.0065},
    [245] = {0.0192, 0.0065},
    [246] = {0.0193, 0.0062},
    [247] = {0.0194, 0.0063},
    [248] = {0.0194, 0.006},
    [249] = {0.0194, 0.0059},
    [250] = {0.0195, 0.0058},
    [251] = {0.0195, 0.0055},
    [252] = {0.0195, 0.0054},
    [253] = {0.0194, 0.005},
    [254] = {0.0069, -0.0014},
    [255] = {0.0179, -0.001},
};
#endif

#endif

#ifndef xy_shift_defined
static const xy_shift_t xy_shift[ColourContrUpperLimit];
#endif

BEGIN_PACK

typedef struct
{
  uint32_t RGB;
  float Cx;
  float Cy;
} RGB_xy_t;

#define COLOR_MAX (65535U)

//#define left_front_door
#define 	Right_front_door

// modify 2023.12.20
#ifdef left_front_door
#define INIT_LINS_NAD_ADDR (0x08U)
#else
#define INIT_LINS_NAD_ADDR (0x0AU)
#endif

#ifdef left_front_door // 左前门
#define FID_BCM_CTRL (0x01)
#define SlaveSide SideLeft
#else // 右前门
#define FID_BCM_CTRL (0x11)
#define SlaveSide SideRight
#endif

#define SLAVEADDRESS (1) // 固定2号，1号通过主机控制

#if SLAVEADDRESS == 1
#define SlaveLEDNum (20)
#endif
#if SLAVEADDRESS == 2
#define SlaveLEDNum (20)
#endif

typedef union
{
  struct
  {
    uint64_t controlFlag_1 : 2;
    uint64_t mode : 3;
    uint64_t reserved1 : 3;
    
    uint64_t Color1 : 16;
    uint64_t pending1 : 8;
    uint64_t brightness : 16;
    uint64_t time_ON : 7;
    uint64_t reserved2 : 1;

    uint64_t time_OFF : 7;
    uint64_t reserved3 : 1;
  } matrix;
  uint8_t data[8];
} FrameNum_1;

typedef union
{
  struct
  {
    uint64_t controlFlag_2 : 2;

    uint64_t Color2 : 16;
    uint64_t pending2 : 8;
    //		uint64_t slave_1 		: 18 ;
    //		uint64_t slave_2 		: 18 ;
  } matrix;
  uint8_t data[8];
} FrameNum_2;

typedef union
{
  struct
  {
    uint64_t controlFlag_3 : 2;

    uint64_t reserved : 22;
#ifdef left_front_door
    uint64_t slave_2 : 20;
    uint64_t slave_1 : 20;
#else
    uint64_t slave_1 : 20;
    uint64_t slave_2 : 20;
#endif
  } matrix;
  uint8_t data[8];
} FrameNum_3;

typedef struct
{
  uint32_t rgb;
  float Cx;
  float Cy;
} RGB_XYY_t;

typedef struct
{
  uint8_t sR;
  uint8_t sG;
  uint8_t sB;
} RGBParam;
typedef struct
{
  uint8_t Mode;
  RGBParam RGBActulParam1;
  RGBParam RGBActulParam2;
  uint16_t Brightness;
  uint8_t Period_ON;
  uint8_t Period_OFF;
  uint32_t LedFlag; // related to the number of LED on PCBA board
} LedParamTemp_t;

typedef struct
{
  LedNum_t ledIndex;
  int8_t temperature[3];
  LedcolorParam_t colorParam;
} CommLedPhysicalParam_t;

typedef struct
{
  LedNum_t ledIndex;
  int8_t temperature;
  int16_t ledTypicalPNVolt[3]; /* @25C*/
} CommLedTypicalPNVolt_t;

typedef struct
{
  Coordinate_t whitePoint;
  uint16_t minIntensity[3]; /* size: 6bytes min rgb Intensity, red has more degration */
} CommLedGeneralParam_t;

typedef struct
{
  uint16_t battVolt;
  uint16_t buckVolt;
  int16_t chipTemp;
  uint16_t ledNum;
  int16_t ledTemperature[LED_NUM];
} CommLedRunTimeParam_t;

typedef struct
{
  LedNum_t ledIndex;
  uint8_t reserved;
  uint16_t ledPNVolt[3];
} CommLedRunTimePNVolt_t;

typedef struct
{
  LedNum_t ledIndex;
  uint8_t reserved;
  int8_t ledTemp[3];
} CommLedRunTimePNTemp_t;

typedef struct
{
  uint16_t sdkApplication;
  lin_stack_version_t linStack;
  LibVersionInfo_t ledLibrary;
} CommVersion_t;

typedef struct
{
  uint16_t x;
  uint16_t y;
  uint8_t Y;
} appxyY_t;

typedef struct
{
  uint8_t address;
  appxyY_t xyY[16];
} APPXYY_t;

typedef struct
{
  uint16_t command;
  union
  {
    CommLedPhysicalParam_t ledPhysicalParam;
    CommLedTypicalPNVolt_t ledTypicalPNVolt;
    CommLedGeneralParam_t ledGeneralParam;
    CommLedRunTimeParam_t ledRunTimeParam;
    CommLedRunTimePNVolt_t ledRunTimePNVolt;
    CommLedRunTimePNTemp_t LedRunTimePNTemp;
    CommVersion_t Version;
  };
} CommLedParamInfo_t;

END_PACK

extern SoftTimer_t ApplTimer;
extern ColorTemperature_t temperature;

uint8_t APPL_GetLEDState(void);

void APPL_TaskHandler(void);
void APPL_HandleColorControlCommands(LIN_Device_Frame_t const *frame);
/* Response enable  */
uint8_t APPL_PrepareLedParamRequest(uint8_t reqSid, uint8_t *const dataBuff, DiagRspInfo_t *const diagRsp);
uint8_t APPL_HandleLedParamConfigRequest(uint8_t reqSid, uint8_t *const dataBuff, DiagRspInfo_t *const diagRsp);

void APPL_UART_ISR(uint8_t *buff, uint8_t length);

void APPL_HandleBcmControlCommands(uint8_t *data);
void APPL_DataHandle();
void Led_InitParam(void);

#endif

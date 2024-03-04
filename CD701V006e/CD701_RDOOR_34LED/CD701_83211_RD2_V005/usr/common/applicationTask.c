#include <applicationTask.h>
#include <measureTask.h>
#include <pdsTask.h>
#include <linStackTask.h>
#include <stdlib.h>

static const RGB_XYY_t rgb2xyyTable[COLOR_TABLE_SIZE] = {
    {0xFFFFFF, 0.3127, 0.3290}, // 1
    {0xFFF9FA, 0.3159, 0.3281}, // 2
    {0xFFF3F6, 0.3187, 0.3261}, // 3
    {0xFFEDF2, 0.3216, 0.3242}, // 4
    {0xFFE7EE, 0.3247, 0.3222}, // 5
    {0xFFE1EA, 0.3279, 0.3201}, // 6
    {0xFFDBE6, 0.3313, 0.3181}, // 7
    {0xFFD5E2, 0.3348, 0.3160}, // 8
    {0xFFCCDC, 0.3404, 0.3128}, // 9
    {0xFFC3D5, 0.3473, 0.3107}, // 10
    {0xFFBACF, 0.3538, 0.3074}, // 11
    {0xFFB1C8, 0.3617, 0.3052}, // 12
    {0xFFAAC4, 0.3668, 0.3020}, // 13
    {0xFFA4BF, 0.3730, 0.3009}, // 14
    {0xFF9EBB, 0.3785, 0.2988}, // 15
    {0xFF94B4, 0.3884, 0.2957}, // 16
    {0xFF8EB0, 0.3945, 0.2937}, // 17
    {0xFF88AB, 0.4020, 0.2928}, // 18
    {0xFF7EA4, 0.4135, 0.2901}, // 19
    {0xFF749D, 0.4258, 0.2876}, // 20
    {0xFF6D98, 0.4349, 0.2862}, // 21
    {0xFF6693, 0.4443, 0.2849}, // 22
    {0xFF6390, 0.4495, 0.2853}, // 23
    {0xFF5C8B, 0.4594, 0.2843}, // 24
    {0xFF5888, 0.4653, 0.2840}, // 25
    {0xFF5183, 0.4754, 0.2835}, // 26
    {0xFF4E80, 0.4811, 0.2841}, // 27
    {0xFF467A, 0.4934, 0.2842}, // 28
    {0xFF4378, 0.4976, 0.2842}, // 29
    {0xFE3B72, 0.5091, 0.2845}, // 30
    {0xFA376F, 0.5120, 0.2838}, // 31
    {0xF23069, 0.5175, 0.2832}, // 32
    {0xF22F68, 0.5195, 0.2836}, // 33
    {0xF23368, 0.5179, 0.2859}, // 34
    {0xF33768, 0.5171, 0.2886}, // 35
    {0xF43A67, 0.5181, 0.2918}, // 36
    {0xF64066, 0.5184, 0.2973}, // 37
    {0xF64365, 0.5183, 0.3004}, // 38
    {0xF74664, 0.5190, 0.3037}, // 39
    {0xF74963, 0.5187, 0.3069}, // 40
    {0xF84C63, 0.5178, 0.3094}, // 41
    {0xF94E62, 0.5187, 0.3121}, // 42
    {0xF95161, 0.5182, 0.3155}, // 43
    {0xF95460, 0.5176, 0.3189}, // 44
    {0xFA565E, 0.5197, 0.3225}, // 45
    {0xFA595D, 0.5189, 0.3260}, // 46
    {0xFA5B5C, 0.5187, 0.3287}, // 47
    {0xFB5E5B, 0.5185, 0.3323}, // 48
    {0xFB6059, 0.5194, 0.3359}, // 49
    {0xFB6358, 0.5183, 0.3395}, // 50
    {0xFB6556, 0.5190, 0.3431}, // 51
    {0xFB6755, 0.5185, 0.3458}, // 52
    {0xFC6A53, 0.5189, 0.3502}, // 53
    {0xFC6C51, 0.5193, 0.3538}, // 54
    {0xFC6E4F, 0.5196, 0.3573}, // 55
    {0xFC704D, 0.5198, 0.3608}, // 56
    {0xFC734B, 0.5189, 0.3653}, // 57
    {0xFC7548, 0.5198, 0.3694}, // 58
    {0xFB7746, 0.5189, 0.3731}, // 59
    {0xFB7943, 0.5195, 0.3772}, // 60
    {0xFB7B40, 0.5199, 0.3812}, // 61
    {0xFB7D3D, 0.5201, 0.3851}, // 62
    {0xFB7F3A, 0.5202, 0.3889}, // 63
    {0xFA8136, 0.5200, 0.3936}, // 64
    {0xFA8332, 0.5203, 0.3978}, // 65
    {0xFA8829, 0.5195, 0.4069}, // 66
    {0xFA8728, 0.5210, 0.4064}, // 67
    {0xFC8B29, 0.5176, 0.4089}, // 68
    {0xFF8F29, 0.5155, 0.4113}, // 69
    {0xFF9329, 0.5111, 0.4151}, // 70
    {0xFF9729, 0.5069, 0.4188}, // 71
    {0xFF9B29, 0.5026, 0.4226}, // 72
    {0xFF9E29, 0.4994, 0.4253}, // 73
    {0xFFA229, 0.4953, 0.4290}, // 74
    {0xFFA629, 0.4911, 0.4326}, // 75
    {0xFFA929, 0.4880, 0.4353}, // 76
    {0xFFAD28, 0.4843, 0.4392}, // 77
    {0xFFB128, 0.4803, 0.4427}, // 78
    {0xFFB428, 0.4774, 0.4453}, // 79
    {0xFFB827, 0.4737, 0.4491}, // 80
    {0xFFBB27, 0.4708, 0.4516}, // 81
    {0xFFBF26, 0.4673, 0.4552}, // 82
    {0xFFC225, 0.4648, 0.4580}, // 83
    {0xFFC625, 0.4611, 0.4612}, // 84
    {0xFFC924, 0.4586, 0.4638}, // 85
    {0xFFCD23, 0.4552, 0.4672}, // 86
    {0xFFD022, 0.4528, 0.4697}, // 87
    {0xFFD421, 0.4495, 0.4730}, // 88
    {0xFFD720, 0.4471, 0.4754}, // 89
    {0xFFDA1F, 0.4448, 0.4778}, // 90
    {0xFFDE1D, 0.4418, 0.4811}, // 91
    {0xFFE11C, 0.4395, 0.4833}, // 92
    {0xFFE41A, 0.4374, 0.4857}, // 93
    {0xFFE818, 0.4345, 0.4887}, // 94
    {0xFFEE13, 0.4304, 0.4934}, // 95
    {0xFEEF49, 0.4174, 0.4766}, // 96
    {0xFEF065, 0.4056, 0.4599}, // 97
    {0xFEF17A, 0.3948, 0.4444}, // 98
    {0xFDF38D, 0.3827, 0.4299}, // 99
    {0xFDF49E, 0.3723, 0.4148}, // 100
    {0xFCF5AD, 0.3622, 0.4013}, // 101
    {0xFCF7BC, 0.3523, 0.3881}, // 102
    {0xF8FD01, 0.4158, 0.5079}, // 103
    {0xF0FC02, 0.4108, 0.5118}, // 104
    {0xE9FA03, 0.4071, 0.5145}, // 105
    {0xE2F904, 0.4027, 0.5179}, // 106
    {0xDBF705, 0.3989, 0.5207}, // 107
    {0xD4F506, 0.3950, 0.5236}, // 108
    {0xCCF407, 0.3897, 0.5276}, // 109
    {0xC5F208, 0.3858, 0.5305}, // 110
    {0xBEF009, 0.3818, 0.5335}, // 111
    {0xB6EE09, 0.3771, 0.5372}, // 112
    {0xAEEC0A, 0.3722, 0.5408}, // 113
    {0xA6E90A, 0.3680, 0.5440}, // 114
    {0x97E50C, 0.3590, 0.5506}, // 115
    {0x97E50B, 0.3591, 0.5508}, // 116
    {0x96E429, 0.3547, 0.5412}, // 117
    {0x95E439, 0.3501, 0.5325}, // 118
    {0x94E445, 0.3456, 0.5240}, // 119
    {0x93E44F, 0.3413, 0.5158}, // 120
    {0x92E458, 0.3370, 0.5076}, // 121
    {0x91E460, 0.3328, 0.4996}, // 122
    {0x90E467, 0.3289, 0.4922}, // 123
    {0x90E36E, 0.3257, 0.4832}, // 124
    {0x8FE375, 0.3215, 0.4750}, // 125
    {0x8EE37B, 0.3177, 0.4677}, // 126
    {0x8DE381, 0.3139, 0.4602}, // 127
    {0x8CE386, 0.3105, 0.4538}, // 128
    {0x8BE38B, 0.3072, 0.4473}, // 129
    {0x8AE391, 0.3032, 0.4393}, // 130
    {0x89E295, 0.3004, 0.4329}, // 131
    {0x88E29A, 0.2970, 0.4261}, // 132
    {0x86E29F, 0.2930, 0.4194}, // 133
    {0x85E2A4, 0.2896, 0.4125}, // 134
    {0x84E2A8, 0.2868, 0.4070}, // 135
    {0x83E2AC, 0.2840, 0.4015}, // 136
    {0x82E1B1, 0.2806, 0.3934}, // 137
    {0x81E1B5, 0.2779, 0.3878}, // 138
    {0x7FE1B9, 0.2746, 0.3824}, // 139
    {0x7EE1BD, 0.2720, 0.3768}, // 140
    {0x7DE1C1, 0.2693, 0.3713}, // 141
    {0x7BE1C5, 0.2662, 0.3659}, // 142
    {0x7AE1C9, 0.2636, 0.3605}, // 143
    {0x78E0CC, 0.2609, 0.3552}, // 144
    {0x77E0D0, 0.2584, 0.3498}, // 145
    {0x75E0D4, 0.2555, 0.3445}, // 146
    {0x74E0D8, 0.2531, 0.3392}, // 147
    {0x72E0DB, 0.2508, 0.3353}, // 148
    {0x71DFDF, 0.2482, 0.3288}, // 149
    {0x6FDFE2, 0.2460, 0.3250}, // 150
    {0x6DDFE6, 0.2434, 0.3199}, // 151
    {0x6BDFEA, 0.2408, 0.3148}, // 152
    {0x68DFF1, 0.2367, 0.3062}, // 153
    {0x68DFF0, 0.2371, 0.3074}, // 154
    {0x65DCF2, 0.2343, 0.3010}, // 155
    {0x63DAF3, 0.2326, 0.2972}, // 156
    {0x61D8F4, 0.2309, 0.2933}, // 157
    {0x5FD6F5, 0.2292, 0.2894}, // 158
    {0x5DD3F6, 0.2272, 0.2841}, // 159
    {0x5BD1F7, 0.2255, 0.2802}, // 160
    {0x58CFF7, 0.2239, 0.2774}, // 161
    {0x56CCF8, 0.2220, 0.2721}, // 162
    {0x54CAF8, 0.2207, 0.2692}, // 163
    {0x51C7F9, 0.2184, 0.2639}, // 164
    {0x4FC5F9, 0.2171, 0.2610}, // 165
    {0x4DC2F9, 0.2155, 0.2567}, // 166
    {0x4AC0F9, 0.2139, 0.2538}, // 167
    {0x48BDF9, 0.2123, 0.2495}, // 168
    {0x45BBF9, 0.2108, 0.2465}, // 169
    {0x43B8F9, 0.2092, 0.2422}, // 170
    {0x40B5F8, 0.2077, 0.2388}, // 171
    {0x3DB3F8, 0.2062, 0.2358}, // 172
    {0x3AB0F7, 0.2047, 0.2324}, // 173
    {0x38ADF7, 0.2031, 0.2279}, // 174
    {0x35AAF6, 0.2016, 0.2245}, // 175
    {0x2FA5F3, 0.1994, 0.2199}, // 176
    {0x299FF1, 0.1964, 0.2128}, // 177
    {0x299FF0, 0.1968, 0.2138}, // 178
    {0x2C9DF1, 0.1962, 0.2100}, // 179
    {0x2F9BF2, 0.1958, 0.2062}, // 180
    {0x329AF3, 0.1957, 0.2039}, // 181
    {0x3498F4, 0.1951, 0.2001}, // 182
    {0x3796F5, 0.1948, 0.1965}, // 183
    {0x3995F5, 0.1950, 0.1952}, // 184
    {0x3B93F6, 0.1944, 0.1916}, // 185
    {0x3D91F7, 0.1940, 0.1881}, // 186
    {0x3F90F7, 0.1942, 0.1868}, // 187
    {0x418EF8, 0.1938, 0.1834}, // 188
    {0x438CF9, 0.1934, 0.1800}, // 189
    {0x4689FA, 0.1931, 0.1754}, // 190
    {0x4787FA, 0.1927, 0.1728}, // 191
    {0x4985FB, 0.1925, 0.1696}, // 192
    {0x4A83FB, 0.1922, 0.1671}, // 193
    {0x4B82FB, 0.1922, 0.1659}, // 194
    {0x4D80FC, 0.1920, 0.1629}, // 195
    {0x4E7EFC, 0.1918, 0.1604}, // 196
    {0x4F7CFC, 0.1916, 0.1581}, // 197
    {0x507AFC, 0.1913, 0.1557}, // 198
    {0x5178FD, 0.1908, 0.1527}, // 199
    {0x5276FD, 0.1907, 0.1504}, // 200
    {0x5374FD, 0.1905, 0.1482}, // 201
    {0x5472FD, 0.1904, 0.1460}, // 202
    {0x5570FD, 0.1902, 0.1438}, // 203
    {0x566EFD, 0.1901, 0.1416}, // 204
    {0x566CFD, 0.1896, 0.1393}, // 205
    {0x576AFC, 0.1898, 0.1379}, // 206
    {0x5966FC, 0.1897, 0.1338}, // 207
    {0x5963FC, 0.1889, 0.1306}, // 208
    {0x5A61FB, 0.1892, 0.1292}, // 209
    {0x5A5FFB, 0.1887, 0.1271}, // 210
    {0x5B5DFA, 0.1890, 0.1258}, // 211
    {0x5B5AFA, 0.1883, 0.1227}, // 212
    {0x5B58F9, 0.1882, 0.1212}, // 213
    {0x5C53F8, 0.1879, 0.1171}, // 214
    {0x5C51F7, 0.1877, 0.1157}, // 215
    {0x5C4EF6, 0.1874, 0.1133}, // 216
    {0x5C4CF5, 0.1873, 0.1120}, // 217
    {0x5D49F4, 0.1875, 0.1100}, // 218
    {0x5D44F2, 0.1872, 0.1066}, // 219
    {0x6444F2, 0.1915, 0.1087}, // 220
    {0x6B44F3, 0.1957, 0.1106}, // 221
    {0x7145F4, 0.1996, 0.1130}, // 222
    {0x7845F5, 0.2042, 0.1150}, // 223
    {0x7E46F5, 0.2088, 0.1180}, // 224
    {0x8346F5, 0.2127, 0.1199}, // 225
    {0x8946F6, 0.2170, 0.1218}, // 226
    {0x8F46F6, 0.2219, 0.1242}, // 227
    {0x9447F6, 0.2262, 0.1270}, // 228
    {0x9947F5, 0.2311, 0.1296}, // 229
    {0x9E47F5, 0.2355, 0.1318}, // 230
    {0xA347F5, 0.2400, 0.1340}, // 231
    {0xA847F5, 0.2446, 0.1362}, // 232
    {0xB248F3, 0.2555, 0.1426}, // 233
    {0xB44CF5, 0.2562, 0.1451}, // 234
    {0xB651F5, 0.2584, 0.1497}, // 235
    {0xB856F6, 0.2599, 0.1537}, // 236
    {0xBC5FF6, 0.2642, 0.1627}, // 237
    {0xBD63F7, 0.2647, 0.1658}, // 238
    {0xBF68F8, 0.2661, 0.1703}, // 239
    {0xC06DF8, 0.2674, 0.1751}, // 240
    {0xC272F9, 0.2688, 0.1798}, // 241
    {0xC376F9, 0.2699, 0.1839}, // 242
    {0xC47BF8, 0.2719, 0.1899}, // 243
    {0xC57FF8, 0.2730, 0.1942}, // 244
    {0xC787F6, 0.2767, 0.2046}, // 245
    {0xC88BF5, 0.2785, 0.2099}, // 246
    {0xC993F3, 0.2813, 0.2206}, // 247
    {0xC997F1, 0.2829, 0.2268}, // 248
    {0xCA9BEF, 0.2854, 0.2333}, // 249
    {0xCBA3EB, 0.2895, 0.2465}, // 250
    {0xD2ADF1, 0.2911, 0.2533}, // 251
    {0xDAB8F5, 0.2947, 0.2629}, // 252
    {0xDEBFF7, 0.2965, 0.2692}, // 253
    {0xE6CFF7, 0.3023, 0.2873}, // 254
    {0xFF0D0D, 0.6340, 0.3300}, // 255
    {0xAA6A4C, 0.4449, 0.3742}, // 256
    {0x000000, 0, 0},           // 257
};

/*static uint8_t ledNum = LED0;*/
static TaskState_t applState = TASK_STATE_INIT;
void ApplTimerExpired(SoftTimer_t *timer);
//ColorTemperature_t temperature;               //modify 2024.02.28
FrameNum_1 *BCM_FrameNum_1;
FrameNum_2 *BCM_FrameNum_2;
FrameNum_3 *BCM_FrameNum_3;

uint8_t BCM_FrameNum_1_Arr[8] = {0};
uint8_t BCM_FrameNum_2_Arr[8] = {0};
uint8_t BCM_FrameNum_3_Arr[8] = {0};

CPoint_t xyYParam1;
CPoint_t xyYParam2;

uint8_t CheckParamFlag;

uint8_t Mode;
static uint16_t Brightness;
uint16_t Period_ON, Period_OFF;
uint32_t SlaveLed; // related to the number of LED on PCBA board
uint32_t LedFlag;  // related to the number of LED on PCBA board
LedParamTemp_t LedParamTemp[SlaveLEDNum];
uint8_t LedIndexTemp;

static uint8_t LED_State = 0U;

void LedOnPorcess(uint8_t ledIndex, uint8_t ColorIndex, uint16_t Brightness, uint16_t transitionTime)
{
  float x = 0.00, y = 0.00;
  
  //modify 2024.02.28
  /*
  temperature.value[0] = 48;
  temperature.value[1] = 54;
  temperature.value[2] = 54;
  */

  switch (ColorIndex)
  {
  case -1:
    x = 0;
    y = 0;
    break;

  default:
    x = xy_shift[ColorIndex].x;
    y = xy_shift[ColorIndex].y;
    break;
  }
    //    modify 2024.02.28
  CLM_SetXYY(ledIndex,/* temperature */ MES_GetLedTemperature(ledIndex), (uint16_t)((rgb2xyyTable[ColorIndex].Cx + x) * COLOR_MAX), (uint16_t)((rgb2xyyTable[ColorIndex].Cy + y) * COLOR_MAX), Brightness, transitionTime);
}

uint8_t APPL_GetLEDState(void)
{
  return LED_State;
}

#define base_time 150U

static SoftTimer_t ApplTimer = {
    .mode = TIMER_PERIODIC_MODE,
    .interval = base_time,
    .handler = ApplTimerExpired};
void ApplTimerExpired(SoftTimer_t *timer)
{
  static uint8_t index = 0U;
  volatile static uint32_t ledIndexnum;
  static uint8_t ledno;
  static uint32_t pos = 0x01;

  switch (index)
  {
  case 0:
    index = 1;
    srand(SysTick->VAL);
    ledIndexnum = rand();

    for (ledno = 0; ledno < 24; ledno += 4)
    {
      if ((ledIndexnum & (pos << ledno)) != 0)
      {
        LedOnPorcess(ledno, BCM_FrameNum_1->matrix.Color1, Brightness, base_time * 2);
      }
    }

    for (ledno = 2; ledno < 24; ledno += 4)
    {
      LedOnPorcess(ledno, BCM_FrameNum_1->matrix.Color1, 0, base_time * 2);
    }

    break;

  case 1:
    index = 2;
    srand(SysTick->VAL);
    ledIndexnum = rand();

    for (ledno = 3; ledno < 24; ledno += 4)
    {
      if ((ledIndexnum & (pos << ledno)) != 0)
      {
        LedOnPorcess(ledno, BCM_FrameNum_1->matrix.Color1, Brightness, base_time * 2);
      }
    }

    for (ledno = 1; ledno < 24; ledno += 4)
    {
      LedOnPorcess(ledno, BCM_FrameNum_1->matrix.Color1, 0, base_time * 2);
    }

    break;

  case 2:
    index = 3;
    srand(SysTick->VAL);
    ledIndexnum = rand();

    for (ledno = 2; ledno < 24; ledno += 4)
    {
      if ((ledIndexnum & (pos << ledno)) != 0)
      {
        LedOnPorcess(ledno, BCM_FrameNum_1->matrix.Color1, Brightness, base_time * 2);
      }
    }

    for (ledno = 0; ledno < 24; ledno += 4)
    {
      LedOnPorcess(ledno, BCM_FrameNum_1->matrix.Color1, 0, base_time * 2);
    }

    break;

  case 3:
    index = 0;
    srand(SysTick->VAL);
    ledIndexnum = rand();

    for (ledno = 1; ledno < 24; ledno += 4)
    {
      if ((ledIndexnum & (pos << ledno)) != 0)
      {
        LedOnPorcess(ledno, BCM_FrameNum_1->matrix.Color1, Brightness, base_time * 2);
      }
    }

    for (ledno = 3; ledno < 24; ledno += 4)
    {
      LedOnPorcess(ledno, BCM_FrameNum_1->matrix.Color1, 0, base_time * 2);
    }
    break;
  }

  CheckParamFlag = 0;
}

void APPL_TaskHandler(void)
{
  switch (applState)
  {
  case TASK_STATE_INIT:
    Led_InitParam();
    applState = TASK_STATE_ACTIVE;
    break;
  case TASK_STATE_ACTIVE:
    APPL_DataHandle();
#if UART_MILKY_WAY_EN == 1U
    handleColorFromUart(rxBuff, rxBuffLength);
#endif
    break;
  default:
    break;
  }
}

void APPL_HandleBcmControlCommands(LIN_Device_Frame_t const *frame)
{
  uint8_t BCM_Frame_Byte1 = frame->data[0];
  uint8_t BCM_Frame_Byte1Temp = BCM_Frame_Byte1 & 0x03;

  static uint8_t receive_num = 0xFF;
  if (BCM_Frame_Byte1Temp == 0x00)
  {
    BCM_FrameNum_1 = (FrameNum_1 *)BCM_FrameNum_1_Arr;
    memcpy(BCM_FrameNum_1, frame->data, 8);
    receive_num = 0;
  }
  else if (BCM_Frame_Byte1Temp == 0x01 && receive_num == 0)
  {
    BCM_FrameNum_2 = (FrameNum_2 *)BCM_FrameNum_2_Arr;
    memcpy(BCM_FrameNum_2, frame->data, 8);
    receive_num = 1;
  }
  else if (BCM_Frame_Byte1Temp == 0x03 && receive_num == 1)
  {
    BCM_FrameNum_3 = (FrameNum_3 *)BCM_FrameNum_3_Arr;
    memcpy(BCM_FrameNum_3, frame->data, 8);
    receive_num = 3;
  }
  else
  {
  }

  static uint32_t old_crc1 = 0xffffffff;
  static uint32_t old_crc2 = 0xffffffff;
  static uint32_t old_crc3 = 0xffffffff;
  if (frame->data != NULL && receive_num == 3)
  {
    uint32_t temp1 = CRC32_GetRunTimeCRC32(BCM_FrameNum_1_Arr, LIN_BUFF_SIZE);
    uint32_t temp2 = CRC32_GetRunTimeCRC32(BCM_FrameNum_2_Arr, LIN_BUFF_SIZE);
    uint32_t temp3 = CRC32_GetRunTimeCRC32(BCM_FrameNum_3_Arr, LIN_BUFF_SIZE);
    if (temp1 != old_crc1 || temp2 != old_crc2 || temp3 != old_crc3)
    {
      old_crc1 = temp1;
      old_crc2 = temp2;
      old_crc3 = temp3;
    //  TM_PostTask(TASK_ID_APPL);
      APPL_DataHandle();
    }
    receive_num = 0xFF;
  }
}
void APPL_DataHandle()
{
  //modify 2024.02.28
  /*
  temperature.value[0] = 48;
  temperature.value[1] = 54;
  temperature.value[2] = 54;
  */

  if (BCM_FrameNum_1->matrix.mode == 0x01)
  {
    LedFlag = 1;
    SlaveLed = BCM_FrameNum_3->matrix.slave_2;
    for (uint8_t LedIndex = 0; LedIndex < SlaveLEDNum; LedIndex++, LedFlag *= 2)
    {

      LedIndexTemp = LedIndex;
      if (LedParamTemp[LedIndexTemp].Mode != BCM_FrameNum_1->matrix.mode ||
          LedParamTemp[LedIndexTemp].RGBActulParam1.sR != (uint8_t)(BCM_FrameNum_1->matrix.Color1 & 0x00ff) ||
          LedParamTemp[LedIndexTemp].RGBActulParam1.sG != (uint8_t)(BCM_FrameNum_1->matrix.Color1 >> 8) ||
          LedParamTemp[LedIndexTemp].RGBActulParam1.sB != BCM_FrameNum_1->matrix.pending1 ||
          LedParamTemp[LedIndexTemp].RGBActulParam2.sR != (uint8_t)(BCM_FrameNum_2->matrix.Color2& 0x00ff) ||
          LedParamTemp[LedIndexTemp].RGBActulParam2.sG != (uint8_t)(BCM_FrameNum_2->matrix.Color2 >> 8) ||
          LedParamTemp[LedIndexTemp].RGBActulParam2.sB != BCM_FrameNum_2->matrix.pending2 ||
          LedParamTemp[LedIndexTemp].Brightness != BCM_FrameNum_1->matrix.brightness ||
          LedParamTemp[LedIndexTemp].Period_ON != BCM_FrameNum_1->matrix.time_ON ||
          LedParamTemp[LedIndexTemp].Period_OFF != BCM_FrameNum_1->matrix.time_OFF ||
          LedParamTemp[LedIndexTemp].LedFlag != (SlaveLed & LedFlag)) // 判断参数是否变化
      {
        /*参数处理**********************************************************/

        Brightness = (uint16_t)(BCM_FrameNum_1->matrix.brightness);
        Period_ON = (uint16_t)(BCM_FrameNum_1->matrix.time_ON * 200);
        Period_OFF = (uint16_t)(BCM_FrameNum_1->matrix.time_OFF * 200);
        /*******************************************************************/

        /*参数保存**********************************************************/
        LedParamTemp[LedIndexTemp].Mode = BCM_FrameNum_1->matrix.mode;
        LedParamTemp[LedIndexTemp].RGBActulParam1.sR = (uint8_t)(BCM_FrameNum_1->matrix.Color1 & 0x00ff);
        LedParamTemp[LedIndexTemp].RGBActulParam1.sG = (uint8_t)(BCM_FrameNum_1->matrix.Color1 >> 8);
        LedParamTemp[LedIndexTemp].RGBActulParam1.sB = BCM_FrameNum_1->matrix.pending1;
        LedParamTemp[LedIndexTemp].RGBActulParam2.sR = (uint8_t)(BCM_FrameNum_2->matrix.Color2& 0x00ff);
        LedParamTemp[LedIndexTemp].RGBActulParam2.sG = (uint8_t)(BCM_FrameNum_2->matrix.Color2 >> 8) ;
        LedParamTemp[LedIndexTemp].RGBActulParam2.sB = BCM_FrameNum_2->matrix.pending2;
        LedParamTemp[LedIndexTemp].Brightness = BCM_FrameNum_1->matrix.brightness;
        LedParamTemp[LedIndexTemp].Period_ON = BCM_FrameNum_1->matrix.time_ON;
        LedParamTemp[LedIndexTemp].Period_OFF = BCM_FrameNum_1->matrix.time_OFF;
        LedParamTemp[LedIndexTemp].LedFlag = (SlaveLed & LedFlag);
        /*******************************************************************/
      }
    }
    SoftTimer_Start(&ApplTimer);
  }
  else if (BCM_FrameNum_1->matrix.mode == 0x00)
  {
    SoftTimer_Stop(&ApplTimer);
    LedFlag = 1;
    SlaveLed = BCM_FrameNum_3->matrix.slave_2;
    for (uint8_t LedIndex = 0; LedIndex < SlaveLEDNum; LedIndex++, LedFlag *= 2)
    {

      LedIndexTemp = LedIndex;
      if (LedParamTemp[LedIndexTemp].Mode != BCM_FrameNum_1->matrix.mode ||
          LedParamTemp[LedIndexTemp].RGBActulParam1.sR != (uint8_t)(BCM_FrameNum_1->matrix.Color1 & 0x00ff) ||
          LedParamTemp[LedIndexTemp].RGBActulParam1.sG != (uint8_t)(BCM_FrameNum_1->matrix.Color1 >> 8) ||
          LedParamTemp[LedIndexTemp].RGBActulParam1.sB != BCM_FrameNum_1->matrix.pending1 ||
          LedParamTemp[LedIndexTemp].RGBActulParam2.sR != (uint8_t)(BCM_FrameNum_2->matrix.Color2& 0x00ff) ||
          LedParamTemp[LedIndexTemp].RGBActulParam2.sG != (uint8_t)(BCM_FrameNum_2->matrix.Color2 >> 8) ||
          LedParamTemp[LedIndexTemp].RGBActulParam2.sB != BCM_FrameNum_2->matrix.pending2 ||
          LedParamTemp[LedIndexTemp].Brightness != BCM_FrameNum_1->matrix.brightness ||
          LedParamTemp[LedIndexTemp].Period_ON != BCM_FrameNum_1->matrix.time_ON ||
          LedParamTemp[LedIndexTemp].Period_OFF != BCM_FrameNum_1->matrix.time_OFF ||
          LedParamTemp[LedIndexTemp].LedFlag != (SlaveLed & LedFlag)) // 判断参数是否变化
      {
        /*参数处理**********************************************************/

        Brightness = (uint16_t)(BCM_FrameNum_1->matrix.brightness);
        Period_ON = (uint16_t)(BCM_FrameNum_1->matrix.time_ON * 200);
        Period_OFF = (uint16_t)(BCM_FrameNum_1->matrix.time_OFF * 200);
        /*******************************************************************/

        /*参数保存**********************************************************/
        LedParamTemp[LedIndexTemp].Mode = BCM_FrameNum_1->matrix.mode;
        LedParamTemp[LedIndexTemp].RGBActulParam1.sR = (uint8_t)(BCM_FrameNum_1->matrix.Color1 & 0x00ff);
        LedParamTemp[LedIndexTemp].RGBActulParam1.sG = (uint8_t)(BCM_FrameNum_1->matrix.Color1 >> 8);
        LedParamTemp[LedIndexTemp].RGBActulParam1.sB = BCM_FrameNum_1->matrix.pending1;
        LedParamTemp[LedIndexTemp].RGBActulParam2.sR = (uint8_t)(BCM_FrameNum_2->matrix.Color2& 0x00ff);
        LedParamTemp[LedIndexTemp].RGBActulParam2.sG = (uint8_t)(BCM_FrameNum_2->matrix.Color2 >> 8) ;
        LedParamTemp[LedIndexTemp].RGBActulParam2.sB = BCM_FrameNum_2->matrix.pending2;
        LedParamTemp[LedIndexTemp].Brightness = BCM_FrameNum_1->matrix.brightness;
        LedParamTemp[LedIndexTemp].Period_ON = BCM_FrameNum_1->matrix.time_ON;
        LedParamTemp[LedIndexTemp].Period_OFF = BCM_FrameNum_1->matrix.time_OFF;
        LedParamTemp[LedIndexTemp].LedFlag = (SlaveLed & LedFlag);
        /*******************************************************************/
        //        CheckParamFlag=1;

        if (SlaveLed & LedFlag) // 点灯
        {
          if (BCM_FrameNum_2->matrix.Color2 != MaxColor)
          {
            LedOnPorcess((LedNum_t)LedIndexTemp, BCM_FrameNum_2->matrix.Color2, Brightness, 0);
          }
          LedOnPorcess((LedNum_t)LedIndexTemp, BCM_FrameNum_1->matrix.Color1, Brightness, Period_ON);
        }
        else // 关灯
        {
          LedOnPorcess((LedNum_t)LedIndexTemp, BCM_FrameNum_1->matrix.Color1, 0, Period_OFF);
        }
      }
    }
  }
  else if (BCM_FrameNum_1->matrix.mode == 0x02)
  {
    SoftTimer_Stop(&ApplTimer);
    LedFlag = 1;
    SlaveLed = BCM_FrameNum_3->matrix.slave_2;
    /*参数处理**********************************************************/

    Brightness = (uint16_t)(BCM_FrameNum_1->matrix.brightness);
    Period_ON = (uint16_t)(BCM_FrameNum_1->matrix.time_ON * 200);
    Period_OFF = (uint16_t)(BCM_FrameNum_1->matrix.time_OFF * 200);
    /*******************************************************************/
    for (uint8_t LedIndex = 0; LedIndex < SlaveLEDNum; LedIndex++, LedFlag *= 2)
    {
#ifdef SideRight
      LedIndexTemp = SlaveLEDNum - LedIndex - 1;
#else
      LedIndexTemp = LedIndex;
#endif
      //       CheckParamFlag=1;

      if (SlaveLed & LedFlag) // 点灯
      {

        if (BCM_FrameNum_2->matrix.Color2 != MaxColor)
        {
          LedOnPorcess((LedNum_t)LedIndexTemp, BCM_FrameNum_2->matrix.Color2, Brightness, 0);
        }

        LedOnPorcess((LedNum_t)LedIndexTemp, BCM_FrameNum_1->matrix.Color1, Brightness, Period_ON);
      }
      else // 关灯
      {
        LedOnPorcess((LedNum_t)LedIndexTemp, BCM_FrameNum_1->matrix.Color1, 0, Period_OFF);
      }
    }
    for (uint8_t LedIndexTemp = 0; LedIndexTemp < SlaveLEDNum; LedIndexTemp++, LedFlag *= 2)
    {
      if (LedParamTemp[LedIndexTemp].Mode != BCM_FrameNum_1->matrix.mode ||
          LedParamTemp[LedIndexTemp].RGBActulParam1.sR != (uint8_t)(BCM_FrameNum_1->matrix.Color1 & 0x00ff) ||
          LedParamTemp[LedIndexTemp].RGBActulParam1.sG != (uint8_t)(BCM_FrameNum_1->matrix.Color1 >> 8) ||
          LedParamTemp[LedIndexTemp].RGBActulParam1.sB != BCM_FrameNum_1->matrix.pending1 ||
          LedParamTemp[LedIndexTemp].RGBActulParam2.sR != (uint8_t)(BCM_FrameNum_2->matrix.Color2& 0x00ff) ||
          LedParamTemp[LedIndexTemp].RGBActulParam2.sG != (uint8_t)(BCM_FrameNum_2->matrix.Color2 >> 8) ||
          LedParamTemp[LedIndexTemp].RGBActulParam2.sB != BCM_FrameNum_2->matrix.pending2 ||
          LedParamTemp[LedIndexTemp].Brightness != BCM_FrameNum_1->matrix.brightness ||
          LedParamTemp[LedIndexTemp].Period_ON != BCM_FrameNum_1->matrix.time_ON ||
          LedParamTemp[LedIndexTemp].Period_OFF != BCM_FrameNum_1->matrix.time_OFF ||
          LedParamTemp[LedIndexTemp].LedFlag != (SlaveLed & LedFlag)) // 判断参数是否变化
      {
        /*参数保存**********************************************************/
        LedParamTemp[LedIndexTemp].Mode = BCM_FrameNum_1->matrix.mode;
        LedParamTemp[LedIndexTemp].RGBActulParam1.sR = (uint8_t)(BCM_FrameNum_1->matrix.Color1 & 0x00ff);
        LedParamTemp[LedIndexTemp].RGBActulParam1.sG = (uint8_t)(BCM_FrameNum_1->matrix.Color1 >> 8);
        LedParamTemp[LedIndexTemp].RGBActulParam1.sB = BCM_FrameNum_1->matrix.pending1;
        LedParamTemp[LedIndexTemp].RGBActulParam2.sR = (uint8_t)(BCM_FrameNum_2->matrix.Color2& 0x00ff);
        LedParamTemp[LedIndexTemp].RGBActulParam2.sG = (uint8_t)(BCM_FrameNum_2->matrix.Color2 >> 8) ;
        LedParamTemp[LedIndexTemp].RGBActulParam2.sB = BCM_FrameNum_2->matrix.pending2;
        LedParamTemp[LedIndexTemp].Brightness = BCM_FrameNum_1->matrix.brightness;
        LedParamTemp[LedIndexTemp].Period_ON = BCM_FrameNum_1->matrix.time_ON;
        LedParamTemp[LedIndexTemp].Period_OFF = BCM_FrameNum_1->matrix.time_OFF;
        LedParamTemp[LedIndexTemp].LedFlag = (SlaveLed & LedFlag);
        /*******************************************************************/
      }
    }
  }
  else
  {
    SoftTimer_Stop(&ApplTimer);
  }
}

void APPL_HandleColorControlCommands(LIN_Device_Frame_t const *frame)
{
  ColorCtrlFrame_t const *color = (ColorCtrlFrame_t const *)((void const *)frame->data);
  if ((color->nodeAddress == ls_read_nad()) || (color->nodeAddress == LIN_NAD_WILDCARD))
  {
    uint8_t channel = color->channel + ((color->command & 0x08U) << 1U);
    if (channel < LED_NUM)
    {
      LedNum_t ledIndex = (LedNum_t)channel;
      switch (color->command & 0x07U)
      {
      case CMD_COLOR_CTRL_XYY:
        (void)CLM_SetXYY(ledIndex, MES_GetLedTemperature(ledIndex), color->ColorXY.x, color->ColorXY.y, (uint16_t)color->ColorXY.Y * 10U, (uint16_t)color->ColorXY.transitionTime * 100U);
        break;
      case CMD_COLOR_CTRL_XYY_EXT:
        (void)CLM_SetXYY(ledIndex, MES_GetLedTemperature(ledIndex), color->ColorXY_EXT.x, color->ColorXY_EXT.y, (uint16_t)color->ColorXY_EXT.Y, 0U);
        break;
      case CMD_COLOR_CTRL_HSL:
        (void)CLM_SetHSL(ledIndex, MES_GetLedTemperature(ledIndex), color->ColorHSL.hue, color->ColorHSL.saturation, (uint16_t)color->ColorHSL.level * 10U, (uint16_t)color->ColorHSL.transitionTime * 100U);
        break;
      case CMD_COLOR_CTRL_RGBL:
        (void)CLM_SetRGBL(ledIndex, MES_GetLedTemperature(ledIndex), color->ColorRGBL.red, color->ColorRGBL.green, color->ColorRGBL.blue, (uint16_t)color->ColorRGBL.level * 10U, (uint16_t)color->ColorRGBL.transitionTime * 100U);
        break;
      case CMD_COLOR_CTRL_RGB:
        (void)CLM_SetRGB(ledIndex, MES_GetLedTemperature(ledIndex), color->ColorRGB.red, color->ColorRGB.green, color->ColorRGB.blue, (uint16_t)color->ColorRGB.transitionTime * 100U);
        break;
      case CMD_COLOR_CTRL_PWM:
        if (color->ColorPWM.red == 255U && color->ColorPWM.green == 255U && color->ColorPWM.blue == 255U)
        {
          (void)CLM_SetPWMs(ledIndex, 65535U, 65535U, 65535U, (uint16_t)color->ColorPWM.transitionTime * 100U);
        }
        else
        {
          (void)CLM_SetPWMs(ledIndex, (uint16_t)color->ColorPWM.red * 256U, (uint16_t)color->ColorPWM.green * 256U, (uint16_t)color->ColorPWM.blue * 256U, (uint16_t)color->ColorPWM.transitionTime * 100U);
        }
        break;
      case CMD_COLOR_CTRL_LUV:
        (void)CLM_SetLUV(ledIndex, MES_GetLedTemperature(ledIndex), color->ColorLUV.u, color->ColorLUV.v, (uint16_t)color->ColorLUV.level * 10U, (uint16_t)color->ColorLUV.transitionTime * 100U);
        break;
      default:
        break;
      }
    }
  }
}

uint8_t APPL_HandleLedParamConfigRequest(uint8_t reqSid, uint8_t *const dataBuff, DiagRspInfo_t *const diagRsp)
{
  uint8_t response = FALSE;
  CommLedParamInfo_t *const info = (CommLedParamInfo_t *)((void *)dataBuff);
  CommLedParamInfo_t *const infoRsp = (CommLedParamInfo_t *)((void *)diagRsp->payload);

  /* handle response data here  */
  diagRsp->type = PDU_TYPE_SINGLE_PDU;
  diagRsp->sid = reqSid;
  diagRsp->packLength = SF_MAX_DATA_LENGTH; /* Valid data length not includes sid*/
  diagRsp->payload[0] = 0xFFU;              /* failed   */

  infoRsp->command = info->command;
  switch (info->command)
  {
  case APP_DIAG_SET_LED_PHY_PARAM:
  {
    LedcolorParam_t ledColorParam = info->ledPhysicalParam.colorParam;
    ColorTemperature_t temperature;
    temperature.value[0] = info->ledPhysicalParam.temperature[0];
    temperature.value[1] = info->ledPhysicalParam.temperature[1];
    temperature.value[2] = info->ledPhysicalParam.temperature[2];
    if (info->ledPhysicalParam.ledIndex <= (LedNum_t)(LED_NUM - 1U))
    {
      (void)CLM_SetLedPhyParams((LedNum_t)info->ledPhysicalParam.ledIndex, temperature, &ledColorParam);
      diagRsp->payload[0] = 0x00U;
    }
    response = TRUE;
    break;
  }
  case APP_DIAG_SET_LED_GENERAL_PARAM:
  {
    Coordinate_t whitePoint = info->ledGeneralParam.whitePoint;
    uint16_t minIntensity[3];
    minIntensity[0] = info->ledGeneralParam.minIntensity[0];
    minIntensity[1] = info->ledGeneralParam.minIntensity[1];
    minIntensity[2] = info->ledGeneralParam.minIntensity[2];
    (void)CLM_SetWhitePointParams(&whitePoint);
    (void)CLM_SetMinimumIntensity(minIntensity[0], minIntensity[1], minIntensity[2]);
    diagRsp->payload[0] = 0x00U;
    response = TRUE;
    break;
  }
  case APP_DIAG_SET_LED_TYPICAL_PN_VOLT:
  {
    int16_t volt_R, volt_G, volt_B;
    ColorTemperature_t temperature;
    volt_R = info->ledTypicalPNVolt.ledTypicalPNVolt[0];
    volt_G = info->ledTypicalPNVolt.ledTypicalPNVolt[1];
    volt_B = info->ledTypicalPNVolt.ledTypicalPNVolt[2];

    temperature.value[0] = info->ledTypicalPNVolt.temperature;
    temperature.value[1] = info->ledTypicalPNVolt.temperature;
    temperature.value[2] = info->ledTypicalPNVolt.temperature;

    (void)CLM_SetLedPNVolts((LedNum_t)info->ledTypicalPNVolt.ledIndex, temperature, volt_R, volt_G, volt_B);
    diagRsp->payload[0] = 0x00U;
    response = TRUE;
    break;
  }
  default:
    break;
  }
  return response;
}

uint8_t APPL_PrepareLedParamRequest(uint8_t reqSid, uint8_t *const dataBuff, DiagRspInfo_t *const diagRsp)
{
  uint8_t response = FALSE;
  CommLedParamInfo_t *const info = (CommLedParamInfo_t *)((void *)dataBuff);
  CommLedParamInfo_t *const infoRsp = (CommLedParamInfo_t *)((void *)diagRsp->payload);

  diagRsp->sid = reqSid;
  infoRsp->command = info->command;
  switch (info->command)
  {
  case APP_DIAG_GET_LED_PHY_PARAM:
  {
    LedcolorParam_t ledColorParam;
    ColorTemperature_t temperature;
    temperature.value[0] = info->ledPhysicalParam.temperature[0];
    temperature.value[1] = info->ledPhysicalParam.temperature[1];
    temperature.value[2] = info->ledPhysicalParam.temperature[2];
    if (info->ledPhysicalParam.ledIndex <= (LedNum_t)(LED_NUM - 1U))
    {
      (void)CLM_GetLedPhyParams((LedNum_t)info->ledPhysicalParam.ledIndex, temperature, &ledColorParam);
      infoRsp->ledPhysicalParam.colorParam = ledColorParam;
      infoRsp->ledPhysicalParam.ledIndex = info->ledPhysicalParam.ledIndex;
      infoRsp->ledPhysicalParam.temperature[0] = info->ledPhysicalParam.temperature[0];
      infoRsp->ledPhysicalParam.temperature[1] = info->ledPhysicalParam.temperature[1];
      infoRsp->ledPhysicalParam.temperature[2] = info->ledPhysicalParam.temperature[2];

      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedPhysicalParam_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
      response = TRUE;
    }
    break;
  }
  case APP_DIAG_GET_LED_GENERAL_PARAM:
  {
    Coordinate_t whitePoint;
    uint16_t minIntensity[3];
    (void)CLM_GetWhitePointParams(&whitePoint);
    (void)CLM_GetMinimumIntensity(&minIntensity[0], &minIntensity[1], &minIntensity[2]);
    infoRsp->ledGeneralParam.minIntensity[0] = minIntensity[0];
    infoRsp->ledGeneralParam.minIntensity[1] = minIntensity[1];
    infoRsp->ledGeneralParam.minIntensity[2] = minIntensity[2];
    infoRsp->ledGeneralParam.whitePoint = whitePoint;
    diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedGeneralParam_t);
    diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
    response = TRUE;
    break;
  }
  case APP_DIAG_GET_LED_TYPICAL_PN_VOLT:
  {
    int16_t volt_R, volt_G, volt_B;
    ColorTemperature_t temperature;
    temperature.value[0] = info->ledTypicalPNVolt.temperature;
    temperature.value[1] = info->ledTypicalPNVolt.temperature;
    temperature.value[2] = info->ledTypicalPNVolt.temperature;
    (void)CLM_GetLedPNVolts((LedNum_t)info->ledTypicalPNVolt.ledIndex, temperature, &volt_R, &volt_G, &volt_B);
    infoRsp->ledTypicalPNVolt.ledIndex = info->ledTypicalPNVolt.ledIndex;
    infoRsp->ledTypicalPNVolt.temperature = info->ledTypicalPNVolt.temperature;
    infoRsp->ledTypicalPNVolt.ledTypicalPNVolt[0] = volt_R;
    infoRsp->ledTypicalPNVolt.ledTypicalPNVolt[1] = volt_G;
    infoRsp->ledTypicalPNVolt.ledTypicalPNVolt[2] = volt_B;
    diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedTypicalPNVolt_t);
    diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
    response = TRUE;
    break;
  }
  case APP_DIAG_GET_LED_RUN_TIME_INFO:
  {
    uint8_t ledNo;
    uint16_t battVolt, buckVolt;
    int16_t chipTemperature;
    ColorTemperature_t temperature;
    int16_t sum;

    (void)MES_GetBatteryVolt(&battVolt);
    (void)MES_GetBuckVolt(&buckVolt);
    (void)MES_GetChipTemperature(&chipTemperature);

    infoRsp->ledRunTimeParam.battVolt = battVolt;
    infoRsp->ledRunTimeParam.buckVolt = buckVolt;
    infoRsp->ledRunTimeParam.chipTemp = chipTemperature;
    infoRsp->ledRunTimeParam.ledNum = LED_NUM;
    for (ledNo = 0U; ledNo < LED_NUM; ledNo++)
    {
      temperature = MES_GetLedTemperature((LedNum_t)ledNo);
      sum = (int16_t)temperature.value[0] + (int16_t)temperature.value[1] + (int16_t)temperature.value[1] + (int16_t)temperature.value[2];
      infoRsp->ledRunTimeParam.ledTemperature[ledNo] = (int16_t)(sum / 4);
    }
    diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedRunTimeParam_t);
    diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
    response = TRUE;
    break;
  }

  case APP_DIAG_GET_LED_RGB_RUN_TEMP:
  {
    if ((uint8_t)info->LedRunTimePNTemp.ledIndex < LED_NUM)
    {
      int8_t rTemp, gTemp, bTemp;
      (void)MES_GetLedRGBTemperature(info->LedRunTimePNTemp.ledIndex, &rTemp, &gTemp, &bTemp);
      infoRsp->LedRunTimePNTemp.ledIndex = info->LedRunTimePNTemp.ledIndex;
      infoRsp->LedRunTimePNTemp.ledTemp[0] = rTemp;
      infoRsp->LedRunTimePNTemp.ledTemp[1] = gTemp;
      infoRsp->LedRunTimePNTemp.ledTemp[2] = bTemp;
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedRunTimePNTemp_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
      response = TRUE;
    }
    break;
  }

  case APP_DIAG_GET_LED_STATIC_PN_VOLT:
  {
    /* force to measure PN volt when Led is off for getting initialized PN voltage */
    uint16_t volt_R, volt_G, volt_B;
    MES_MnftGetLedInfo(info->ledRunTimePNVolt.ledIndex, &volt_R, &volt_G, &volt_B);
    infoRsp->ledRunTimePNVolt.ledIndex = info->ledRunTimePNVolt.ledIndex;
    infoRsp->ledRunTimePNVolt.ledPNVolt[0] = volt_R;
    infoRsp->ledRunTimePNVolt.ledPNVolt[1] = volt_G;
    infoRsp->ledRunTimePNVolt.ledPNVolt[2] = volt_B;
    diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedRunTimePNVolt_t);
    diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
    response = TRUE;
    break;
  }

  case APP_DIAG_GET_LED_RGB_RUN_VOLT:
  {
    if ((uint8_t)info->LedRunTimePNTemp.ledIndex < LED_NUM)
    {
      uint16_t volt_R, volt_G, volt_B;
      MES_GetRunTimeLedPNVolt(info->ledRunTimePNVolt.ledIndex, &volt_R, &volt_G, &volt_B);
      infoRsp->ledRunTimePNVolt.ledIndex = info->ledRunTimePNVolt.ledIndex;
      infoRsp->ledRunTimePNVolt.ledPNVolt[0] = volt_R;
      infoRsp->ledRunTimePNVolt.ledPNVolt[1] = volt_G;
      infoRsp->ledRunTimePNVolt.ledPNVolt[2] = volt_B;
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedRunTimePNVolt_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
      response = TRUE;
    }
    break;
  }
  case APP_DIAG_GET_SDK_VERSION:
    infoRsp->Version.sdkApplication = SDK_APPL_VERSION;
    infoRsp->Version.linStack = ls_get_lin_version();
    infoRsp->Version.ledLibrary = CLM_GetColorLibraryVersion();
    diagRsp->packLength = 2U + (uint16_t)sizeof(CommVersion_t);
    diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH) ? PDU_TYPE_MULTI_PDU : PDU_TYPE_SINGLE_PDU;
    response = TRUE;
    break;
  default:
    break;
  }
  return response;
}

#if UART_MILKY_WAY_EN == 1U

void APPL_UART_ISR(uint8_t *buff, uint8_t length)
{
  rxBuffLength = length;
  memcpy(rxBuff, buff, length);
  TM_PostTask(TASK_ID_APPL);
}

void handleColorFromUart(uint8_t *buff, uint8_t length)
{
  APPXYY_t *color = (APPXYY_t *)buff;
  for (uint8_t i = 0; i < 16U; i++)
  {
    (void)CLM_SetXYY((LedNum_t)i, MES_GetLedTemperature((LedNum_t)i), color->xyY[i].x, color->xyY[i].y, (uint16_t)color->xyY[i].Y * 10U, 0);
  }
}
#endif
static void Led_InitParam(void)
{
  ColorTemperature_t temperature;       //modify 2024.02.28
  temperature.value[0] = ROOM_TEMPERATURE;
  temperature.value[1] = ROOM_TEMPERATURE;
  temperature.value[2] = ROOM_TEMPERATURE;
  CLM_SetFadingMode(FADING_MODE_DERIVATIVE);
  if (PDS_GetPdsStatus() == PDS_STATUS_USING_DEFAULT)
  {
    uint16_t MAX_Intensity_R = DEFAULT_MAX_INTENSITY_R;
    uint16_t MAX_Intensity_G = DEFAULT_MAX_INTENSITY_G;
    uint16_t MAX_Intensity_B = DEFAULT_MAX_INTENSITY_B;
    uint16_t r_pn_vol = DEFAULT_LED_PN_VOLT_R;
    uint16_t g_pn_vol = DEFAULT_LED_PN_VOLT_G;
    uint16_t b_pn_vol = DEFAULT_LED_PN_VOLT_B;
    Coordinate_t red = {
        .x = DEFAULT_COORDINATE_R_X,
        .y = DEFAULT_COORDINATE_R_Y};
    Coordinate_t green = {
        .x = DEFAULT_COORDINATE_G_X,
        .y = DEFAULT_COORDINATE_G_Y};
    Coordinate_t blue = {
        .x = DEFAULT_COORDINATE_B_X,
        .y = DEFAULT_COORDINATE_B_Y};
#ifdef VE_2_AR_5_TG_P // 橙
    red.x = 0xB1BC;
    red.y = 0x4E07;
    green.x = 0x2865;
    green.y = 0xB9A5;
    blue.x = 0x213A;
    blue.y = 0x11E4;
    MAX_Intensity_R = 0x0384;
    MAX_Intensity_G = 0x089D;
    MAX_Intensity_B = 0x0192;
    r_pn_vol = 0x08A8;
    g_pn_vol = 0x0AB9;
    b_pn_vol = 0x0AFD;
#endif
    Coordinate_t whitepoint = {0x5555, 0x5555};
    LedcolorParam_t ledColorParam;
    ledColorParam.color[0] = red;
    ledColorParam.color[1] = green;
    ledColorParam.color[2] = blue;
    ledColorParam.maxTypicalIntensity[0] = MAX_Intensity_R;
    ledColorParam.maxTypicalIntensity[1] = MAX_Intensity_G;
    ledColorParam.maxTypicalIntensity[2] = MAX_Intensity_B;
    for (uint8_t LEDIndex = 0; LEDIndex < LED_NUM; LEDIndex++)
    {
      CLM_SetLedPhyParams(LEDIndex, temperature, &ledColorParam);
      CLM_SetLedPNVolts(LEDIndex, temperature, r_pn_vol, g_pn_vol, b_pn_vol);
    }
    CLM_SetWhitePointParams(&whitepoint);
    CLM_SetMinimumIntensity((uint16_t)(MAX_Intensity_R * (float)65 / 100), (uint16_t)(MAX_Intensity_G * (float)95 / 100), (uint16_t)(MAX_Intensity_B * (float)95 / 100));
    TM_PostTask(TASK_ID_PDS);
  }
}

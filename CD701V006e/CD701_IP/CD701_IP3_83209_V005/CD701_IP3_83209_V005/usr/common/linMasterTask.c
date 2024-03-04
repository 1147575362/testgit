/**
 * @copyright 2015 IndieMicro.
 *
 * This file is proprietary to IndieMicro.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of IndieMicro.
 *
 * @file linMasterTask.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/09/10
 */

#include <linMasterTask.h>
#include <stdlib.h>
#include <applicationTask.h>
#include <linSlaveTask.h>
#include "My_simplerunning.h"
#include "My_DashedLine.h"
#include <pdsTask.h>
#include <math.h>

#if LIN_MASTER_EN == 1

TaskState_t linmTaskState = TASK_STATE_INIT;
LIN_Device_Frame_t activelinmFrame;

AppDataProtocol APPL_dataProtocol_callback = NULL;

void APPL_RegisterDataProtocol(AppDataProtocol dp)
{
	if (APPL_dataProtocol_callback == NULL)
		APPL_dataProtocol_callback = dp;
}
void clearTimerExpired(SoftTimer_t *timer);
void setRunParameter();

#define LEDNUM 100
#define meteorLength (10U)
#define moveLength (10U)

#define NODE_MODE_13_14_15_16 (5) // 虚线平移流水,虚线游动,虚线依次点亮,踏步平移 的段数
#define buf_size (13)
#define sendByte (5)
#define singleGroupTimer (12) // 一组报文时间 4ms *3帧
#define SENDFRAMETIME (10)	  // 表示长安报文发送一帧的时间

Node_Parameter ParameterBuff;
Send_Parameter send_Parameter;
Send_Slave_Frame1_t sendSlaveFrame1;
Send_Slave_Frame2_t sendSlaveFrame2;
Send_Slave_Frame3_t sendSlaveFrame3;

volatile static uint8_t alreadyRunNUM; // 运行次数
volatile static uint8_t alreadyframe;  // 发送帧数
volatile static uint8_t SentGroup;	   // 已发送组数
static int16_t running_times1;
static int16_t running_times2;
//
// static uint8_t alreadyRunNUM;	//
static bool modeChangeFlag = FALSE;
static bool syncChangeFlag = FALSE;
static bool lastNUMFlag = FALSE;

static bool upDateFlag;			  // 更新数据标志位
static bool Execute_EN;			  //
static uint64_t syncTimer;		  // 同步时间
static uint8_t syncTimerMAXcount; // 溢出计数
static uint32_t frameTimer;		  // 单帧报文发送时间
static float timer_tick;		  // 定时时间(ms)

static bool send_EN; // 发送使能
static bool oneTime;

syncMode_Parameter lastModeParameter;
syncMode_Parameter currentModeParameter;
static uint8_t mode234flag;
static uint8_t halfPeriod;

/////////////////////////////
uint8_t ColorIndex1;
uint8_t ColorIndex2;
static uint16_t Brightness;
uint16_t ModePeriod;
uint8_t ModeTime;

uint16_t CurrentRunTime = 0; //	当前灯效运行的时间
uint16_t WholeLampTime = 0;
bool OverflowFlag = false; // 若此值為真，則在下一個報文到達后將OverFlow++

static uint8_t nweNode_Mode;

uint8_t ALU_ModeCount;
uint8_t ModeTemp;
uint8_t ModeCache;
volatile uint8_t VIU_Sync;
uint8_t VIU_SyncInit;
uint16_t VIU_SyncOverflow;
float SyncLongGap;
BCM_ALU_CTRL_t const *ALU_DATA;
BCM_ALU_CTRL_t const *BCM_DATA;
BCM_ALU_CTRL_t const *SWAP_DATA;
///////////////////////////////

extern Led_Data_t LedData;
#define dataByetLen (13)
uint8_t data[dataByetLen] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define COLOR_TABLE_SIZE (257U)

/*同步计数值套圈后的补偿计数值*/
#define OVERFLOWCONTER	0X100		// modify 2023.12.29 解决每0~255一个周期多算一次的问题


static const RGB_XYY_t rgb2xyyTable[COLOR_TABLE_SIZE] = {
	{0xFFFFFF},
	{0xFFF9FA},
	{0xFFF3F6},
	{0xFFEDF2},
	{0xFFE7EE},
	{0xFFE1EA},
	{0xFFDBE6},
	{0xFFD5E2},
	{0xFFCCDC},
	{0xFFC3D5},
	{0xFFBACF},
	{0xFFB1C8},
	{0xFFAAC4},
	{0xFFA4BF},
	{0xFF9EBB},
	{0xFF94B4},
	{0xFF8EB0},
	{0xFF88AB},
	{0xFF7EA4},
	{0xFF749D},
	{0xFF6D98},
	{0xFF6693},
	{0xFF6390},
	{0xFF5C8B},
	{0xFF5888},
	{0xFF5183},
	{0xFF4E80},
	{0xFF467A},
	{0xFF4378},
	{0xFE3B72},
	{0xFA376F},
	{0xF23069},
	{0xF22F68},
	{0xF23368},
	{0xF33768},
	{0xF43A67},
	{0xF64066},
	{0xF64365},
	{0xF74664},
	{0xF74963},
	{0xF84C63},
	{0xF94E62},
	{0xF95161},
	{0xF95460},
	{0xFA565E},
	{0xFA595D},
	{0xFA5B5C},
	{0xFB5E5B},
	{0xFB6059},
	{0xFB6358},
	{0xFB6556},
	{0xFB6755},
	{0xFC6A53},
	{0xFC6C51},
	{0xFC6E4F},
	{0xFC704D},
	{0xFC734B},
	{0xFC7548},
	{0xFB7746},
	{0xFB7943},
	{0xFB7B40},
	{0xFB7D3D},
	{0xFB7F3A},
	{0xFA8136},
	{0xFA8332},
	{0xFA8829},
	{0xFA8728},
	{0xFC8B29},
	{0xFF8F29},
	{0xFF9329},
	{0xFF9729},
	{0xFF9B29},
	{0xFF9E29},
	{0xFFA229},
	{0xFFA629},
	{0xFFA929},
	{0xFFAD28},
	{0xFFB128},
	{0xFFB428},
	{0xFFB827},
	{0xFFBB27},
	{0xFFBF26},
	{0xFFC225},
	{0xFFC625},
	{0xFFC924},
	{0xFFCD23},
	{0xFFD022},
	{0xFFD421},
	{0xFFD720},
	{0xFFDA1F},
	{0xFFDE1D},
	{0xFFE11C},
	{0xFFE41A},
	{0xFFE818},
	{0xFFEE13},
	{0xFEEF49},
	{0xFEF065},
	{0xFEF17A},
	{0xFDF38D},
	{0xFDF49E},
	{0xFCF5AD},
	{0xFCF7BC},
	{0xF8FD01},
	{0xF0FC02},
	{0xE9FA03},
	{0xE2F904},
	{0xDBF705},
	{0xD4F506},
	{0xCCF407},
	{0xC5F208},
	{0xBEF009},
	{0xB6EE09},
	{0xAEEC0A},
	{0xA6E90A},
	{0x97E50C},
	{0x97E50B},
	{0x96E429},
	{0x95E439},
	{0x94E445},
	{0x93E44F},
	{0x92E458},
	{0x91E460},
	{0x90E467},
	{0x90E36E},
	{0x8FE375},
	{0x8EE37B},
	{0x8DE381},
	{0x8CE386},
	{0x8BE38B},
	{0x8AE391},
	{0x89E295},
	{0x88E29A},
	{0x86E29F},
	{0x85E2A4},
	{0x84E2A8},
	{0x83E2AC},
	{0x82E1B1},
	{0x81E1B5},
	{0x7FE1B9},
	{0x7EE1BD},
	{0x7DE1C1},
	{0x7BE1C5},
	{0x7AE1C9},
	{0x78E0CC},
	{0x77E0D0},
	{0x75E0D4},
	{0x74E0D8},
	{0x72E0DB},
	{0x71DFDF},
	{0x6FDFE2},
	{0x6DDFE6},
	{0x6BDFEA},
	{0x68DFF1},
	{0x68DFF0},
	{0x65DCF2},
	{0x63DAF3},
	{0x61D8F4},
	{0x5FD6F5},
	{0x5DD3F6},
	{0x5BD1F7},
	{0x58CFF7},
	{0x56CCF8},
	{0x54CAF8},
	{0x51C7F9},
	{0x4FC5F9},
	{0x4DC2F9},
	{0x4AC0F9},
	{0x48BDF9},
	{0x45BBF9},
	{0x43B8F9},
	{0x40B5F8},
	{0x3DB3F8},
	{0x3AB0F7},
	{0x38ADF7},
	{0x35AAF6},
	{0x2FA5F3},
	{0x299FF1},
	{0x299FF0},
	{0x2C9DF1},
	{0x2F9BF2},
	{0x329AF3},
	{0x3498F4},
	{0x3796F5},
	{0x3995F5},
	{0x3B93F6},
	{0x3D91F7},
	{0x3F90F7},
	{0x418EF8},
	{0x438CF9},
	{0x4689FA},
	{0x4787FA},
	{0x4985FB},
	{0x4A83FB},
	{0x4B82FB},
	{0x4D80FC},
	{0x4E7EFC},
	{0x4F7CFC},
	{0x507AFC},
	{0x5178FD},
	{0x5276FD},
	{0x5374FD},
	{0x5472FD},
	{0x5570FD},
	{0x566EFD},
	{0x566CFD},
	{0x576AFC},
	{0x5966FC},
	{0x5963FC},
	{0x5A61FB},
	{0x5A5FFB},
	{0x5B5DFA},
	{0x5B5AFA},
	{0x5B58F9},
	{0x5C53F8},
	{0x5C51F7},
	{0x5C4EF6},
	{0x5C4CF5},
	{0x5D49F4},
	{0x5D44F2},
	{0x6444F2},
	{0x6B44F3},
	{0x7145F4},
	{0x7845F5},
	{0x7E46F5},
	{0x8346F5},
	{0x8946F6},
	{0x8F46F6},
	{0x9447F6},
	{0x9947F5},
	{0x9E47F5},
	{0xA347F5},
	{0xA847F5},
	{0xB248F3},
	{0xB44CF5},
	{0xB651F5},
	{0xB856F6},
	{0xBC5FF6},
	{0xBD63F7},
	{0xBF68F8},
	{0xC06DF8},
	{0xC272F9},
	{0xC376F9},
	{0xC47BF8},
	{0xC57FF8},
	{0xC787F6},
	{0xC88BF5},
	{0xC993F3},
	{0xC997F1},
	{0xCA9BEF},
	{0xCBA3EB},
	{0xD2ADF1},
	{0xDAB8F5},
	{0xDEBFF7},
	{0xE6CFF7},
	{0xFF0D0D},
	{0xB99B7A},
	{0x000000},
};

LIN_Device_Frame_t linm_cmd_table[] = {
	[0] = {
		.frame_id = 0x24,
		.msg_type = LIN_MSG_TYPE_TX,
		.checksum = LIN_CHECKSUM_ENHANCED,
		.length = LIN_BUFF_SIZE,
	},
	[1] = {
		.frame_id = 0xFF,
		.msg_type = LIN_MSG_TYPE_TX,
		.checksum = LIN_CHECKSUM_ENHANCED,
		.length = LIN_BUFF_SIZE,
	},
	[2] = {
		.frame_id = 0x3C,
		.msg_type = LIN_MSG_TYPE_TX,
		.checksum = LIN_CHECKSUM_CLASSIC,
		.length = LIN_BUFF_SIZE,
	},
};
//#define off_set_Left (75U)  2024.01.24 modify
#define off_set_Left (79U)
//#define off_set_Right (25U)
#define off_set_Right (21U)
#define Left (5U)
#define Right (6U)
#define Some (7U)
uint16_t max_len_left = 8;
uint16_t max_len_right = 8;
uint8_t Voice_temp_x = 1;
uint8_t Voice_temp_y = 1;

//(voice lamp parament)
uint8_t GroupMAX;
static bool rise_flag = FALSE;
// 最小亮灯数 = temp_times - 1
uint8_t temp_times;
// 次数上限值

static bool periodComplete = FALSE;
static bool breathingFlag = 0;
static uint8_t multiColorTransitionCount = 0;					//11.20 modify 多色渐变专用
static bool runFlag = 0;

// 语音灯效拓宽的范围
static uint8_t arr_voice_mode8[] = {3, 4, 5, 4};
static uint8_t arr_voice_mode9[] = {5, 6, 7, 8, 9, 8, 7, 6, 5};
static uint8_t arr_voice_mode10[] = {3, 4, 5, 4};
// 数组下标
static uint8_t voice_index_mode8 = 0;
static uint8_t voice_index_mode9 = 0;
static uint8_t voice_index_mode10 = 0;

static uint8_t arr_loopend_mode8 = 4;
static uint8_t arr_loopend_mode9 = 9;

bool ParameterInFlag = FALSE;

// 渐亮、渐灭、流水熄灭沿用上个模式的亮度、颜色
static uint16_t mode1brightness;
static uint16_t mode1Color1;									// modify 11.22
static uint16_t mode1Color2;

// 保存LIN报文参数到BUFF

uint8_t ColorIndexCalculate(uint8_t ColourContr)
{
	return ColourContr; // 颜色的索引[0-255]
}
uint16_t BrightnessCalculate(uint8_t DimLevel)
{
	//	return (DimLevel - 1) * 1023 * Getlight_max_lv() / 15000; // 调光值
	return (DimLevel - 1) * 1023 * Getlight_max_lv() / 10000;
}
uint16_t ModePeriodCalculate(uint8_t TimeContr)
{
	if(TimeContr == 0)																							//11.15 modify 防止0-1 = 255
		return 0;
	else
		return (TimeContr - 1) * 200;
}
uint8_t ModeTimeCalculate(uint8_t FrecyContr)
{
	return FrecyContr;
}

void sendParameterinit(void)
{
	SentGroup = 0;
	send_EN = 0;
	alreadyframe = 0;
	ALU_ModeCount = 0;
	alreadyRunNUM = 0;
}
void sendParametermodeinit(void)
{
	send_EN = 0;
	ALU_ModeCount = 0;
	alreadyRunNUM = 0;
}
void changeColorIndex()
{
       // modify 11.24
	send_Parameter.mode = ALU_DATA->matrix.Node_ModeContr;
	// if(ALU_DATA->matrix.NodeLEDSwitch == 0 && (send_Parameter.mode == Mode1 || send_Parameter.mode == Mode5))	//进入渐灭、流水熄灭时，用上个模式的颜色

/* modify 2023.12.18 进入渐灭、流水熄灭时，用上个模式的颜色 (渐灭LED总开关为1且双色标志位为0且亮度较上次低，流水熄灭总开关为0) */
	if((ALU_DATA->matrix.NodeLEDSwitch == 1 && send_Parameter.mode == Mode1 && ALU_DATA->matrix.Node_ColourCtr1Flg == 0 && ALU_DATA->matrix.Node_ColourCtr2Flg == 0) || 
	   (ALU_DATA->matrix.NodeLEDSwitch == 0 && send_Parameter.mode == Mode5))
	{
		if(multiColorTransitionCount != 0)	//modify 2023.12.12  上个模式是多色渐变模式，按照目前的颜色来赋色
		{
			(multiColorTransitionCount == 2) ? (mode1Color1 = send_Parameter.color2) : (mode1Color1 = send_Parameter.color1);
		}
		send_Parameter.color1 = mode1Color1;
	}
	/*进入其他模式，正常上色，并存储此颜色*/
	else	// modify 2023.12.18
	{
		if (ALU_DATA->matrix.Node_ColourCtr1Flg == 1)           
		{
			send_Parameter.color1 = ColorIndexCalculate(ALU_DATA->matrix.Node8_Colour1);
			mode1Color1 = send_Parameter.color1;	
		}
		else
		{
			send_Parameter.color1 = mode1Color1;	// modify 2023.12.18   若颜色入参标志位为0，则用上一个颜色的入参 (适用渐暗效果需求)
		}
	}

	if (ALU_DATA->matrix.Node_ColourCtr2Flg == 1)
	{
		send_Parameter.color2 = ColorIndexCalculate(ALU_DATA->matrix.Node8_Colour2);
		if(send_Parameter.mode == Mode11)																		//炫彩流水模式使用第二色，颠倒赋值
			mode1Color1 = send_Parameter.color2;
	}
	else
	{
		send_Parameter.color2 = mode1Color1;		// modify 2023.12.18   若颜色入参标志位为0，则用上一个颜色的入参
	}
		
}
static uint8_t oldNode_Mode;
static uint8_t direction;



static uint8_t arr_loopend_mode10 = 4;

#define sendByte (13)

static int16_t running_times1;
static int16_t running_times2;

// 同步变量
static bool Sys_Flag = FALSE;

void TimerCallback(void)
{
	if (Sys_Flag == 1)
	{
		if (send_EN == 0)
		{
			if (SentGroup < send_Parameter.sendGroupNUM)
			{
				SentGroup++;
				if (ALU_DATA->matrix.NodeLEDSwitch == 0)
				{
					if (send_Parameter.mode == Mode5)
					{
						memset(data,0xff,sizeof(data));
						if (send_Parameter.direction == 0x03 || send_Parameter.direction == 0x04)
						{
							if (SentGroup <= LEDNUM / 2)
							{
								FlowingLamp(data, sizeof(data), LEDNUM,  (send_Parameter.direction == 0x03 ? 0x04 : 0x03), SentGroup, Low);		//11.14 modify
							}
						}
						else
						{
							if (SentGroup <= LEDNUM)
							{
								FlowingLamp(data, sizeof(data), LEDNUM, send_Parameter.direction, SentGroup, Low);
							}
						}

						sendSlaveFrame1.matrix.Time_OFF = 0;
						sendSlaveFrame1.matrix.Time_ON = 0;
						sendSlaveFrame1.matrix.IntensitySet = mode1brightness;																	// 11.20 modify 使用上个模式的亮度

						//sendSlaveFrame1.matrix.IntensitySet = send_Parameter.brightness;		
						//sendSlaveFrame1.matrix.IntensitySet = ALU_DATA->matrix.Node_DimLevel;													// 11.14 modify
						//sendSlaveFrame1.matrix.IntensitySet = 101;
					}
					else if (send_Parameter.mode == Mode1) // 渐灭
					{
						for (uint8_t i = 0; i < sendByte; i++)
						{
							data[i] = 0x00;
						}
						sendSlaveFrame1.matrix.Time_ON = 0;
						sendSlaveFrame1.matrix.Time_OFF = send_Parameter.displayTimer;
						sendSlaveFrame1.matrix.IntensitySet = mode1brightness;																	// 11.20 modify 使用上个模式的亮度
						//sendSlaveFrame1.matrix.IntensitySet = send_Parameter.brightness;		
						//sendSlaveFrame1.matrix.IntensitySet = ALU_DATA->matrix.Node_DimLevel;													// 11.14 modify

					}
					else
					{
						sendSlaveFrame1.matrix.Time_OFF = 0;
						sendSlaveFrame1.matrix.Time_ON = 0;
						sendSlaveFrame1.matrix.IntensitySet = 0;
						for (uint8_t i = 0; i < sendByte; i++)
						{
							data[i] = 0x0;
						}
						sendSlaveFrame1.matrix.Time_OFF = 0;
						sendSlaveFrame1.matrix.Time_ON = 0;
						sendSlaveFrame1.matrix.IntensitySet = 0;
					}
					sendSlaveFrame1.matrix.Start = 0x00; // 填充共用体数据1
					if (send_Parameter.mode == Mode12)
						sendSlaveFrame1.matrix.Mode = 0x01;
					else if ((send_Parameter.mode == Mode3) || (send_Parameter.mode == Mode1))
						sendSlaveFrame1.matrix.Mode = 0x02;
					else
						sendSlaveFrame1.matrix.Mode = 0x00;

					sendSlaveFrame1.matrix.Color1 = send_Parameter.color1;

					sendSlaveFrame2.matrix.Start = 0x01;								   // 填充共用体数据2
					if ((send_Parameter.mode == Mode11) || (send_Parameter.mode == Mode6)) // 炫彩流水 流光
					{
						sendSlaveFrame2.matrix.Color2 = send_Parameter.color1; // color2改成了color1
					}
					else
					{
						sendSlaveFrame2.matrix.Color2 = 0;
					}
					sendSlaveFrame2.matrix.ledDATA12_High4 = data[12] >> 4;
					sendSlaveFrame2.matrix.ledDATA11 = data[11];
					sendSlaveFrame2.matrix.ledDATA10 = data[10];
					sendSlaveFrame2.matrix.ledDATA9 = data[9];
					sendSlaveFrame2.matrix.ledDATA8 = data[8];
					sendSlaveFrame2.matrix.ledDATA7_Low2 = (data[7] & 0x03);
					sendSlaveFrame3.matrix.Start = 0x03; // 填充共用体数据3
					sendSlaveFrame3.matrix.ledDATA7_High6 = (data[7] >> 2);
					sendSlaveFrame3.matrix.ledDATA6 = data[6];
					sendSlaveFrame3.matrix.ledDATA5 = data[5];
					sendSlaveFrame3.matrix.ledDATA4 = data[4];
					sendSlaveFrame3.matrix.ledDATA3 = data[3];
					sendSlaveFrame3.matrix.ledDATA2 = data[2];
					sendSlaveFrame3.matrix.ledDATA1 = data[1];
					sendSlaveFrame3.matrix.ledDATA0 = data[0];
					send_EN = 1;
					alreadyframe = 0;
				}
				else  // when ALU_DATA->matrix.NodeLEDSwitch != 0
				{

					if (send_Parameter.mode == Mode6) // 跑马函数生成数据
					{
						running_times1++;
						simplerunning(data, sizeof(data), LEDNUM, send_Parameter.direction, running_times1 - 1, meteorLength, High);
						if (send_Parameter.direction == 3 || send_Parameter.direction == 4)
						{
							if (running_times1 - 1 >= LEDNUM / 2 + meteorLength)
							{
								memset(data, 0x00, sizeof(data));
								running_times1 = 0;
							}
						}
						else
						{
							if (running_times1 - 1 >= LEDNUM + meteorLength)
							{
								memset(data, 0x00, sizeof(data));
								running_times1 = 0;
							}
						}
					}
					else if (send_Parameter.mode == Mode7) // 跑马函数生成数据
					{

						running_times2++;
						simplerunning(data, sizeof(data), LEDNUM, send_Parameter.direction, running_times2 - 1, meteorLength, High);
						if (send_Parameter.direction == Mode3 || send_Parameter.direction == Mode4)
						{
							if (running_times2 - 1 >= LEDNUM / 2 + meteorLength)
							{
								memset(data, 0x00, sizeof(data));
								running_times2 = 0;
							}
						}
						else
						{
							if (running_times2 - 1 >= LEDNUM + meteorLength)
							{
								memset(data, 0x00, sizeof(data));
								running_times2 = 0;
							}
						}
					}
					else if ((send_Parameter.mode == Mode5) || (send_Parameter.mode == Mode11)) // 流水函生成数据
					{
						if (send_Parameter.direction == 0x03 || send_Parameter.direction == 0x04)
						{
							if (SentGroup <= LEDNUM / 2)
							{
								FlowingLamp(data, sizeof(data), LEDNUM, send_Parameter.direction, SentGroup, High);
							}
						}
						else
						{
							if (SentGroup <= LEDNUM)
							{
								FlowingLamp(data, sizeof(data), LEDNUM, send_Parameter.direction, SentGroup, High);
							}
						}
					}
					else if ((send_Parameter.mode == Mode2) || (send_Parameter.mode == Mode3))
					{
						// 闪烁亮灭切换
						if (breathingFlag == 1)
						{
							for (uint8_t i = 0; i < sendByte; i++)
							{
								data[i] = 0xff;
							}
						}
						else
						{

							for (uint8_t i = 0; i < sendByte; i++)
							{
								data[i] = 0;
							}
						}
					}
					else if (send_Parameter.mode == Mode1 || send_Parameter.mode == Mode4) // 常亮与多色渐变模式
					{
						for (uint8_t i = 0; i < sendByte; i++)
						{
							data[i] = 0xff;
						}
					}
					else if ((send_Parameter.mode == Mode8) || (send_Parameter.mode == Mode9) || (send_Parameter.mode == Mode10)) // 0x8=语音唤醒//0x9=语音识别	//0xA=TTS播报
					{
						switch (send_Parameter.mode)
						{
						case 8:
							voice_index_mode9 = 0;
							voice_index_mode10 = 0;
							if (send_Parameter.direction == 5)
								VoiceLamp(data, sizeof(data), off_set_Left, 0, arr_voice_mode8[voice_index_mode8], arr_voice_mode8[voice_index_mode8]);
							else if (send_Parameter.direction == 6)
								VoiceLamp(data, sizeof(data), 0, off_set_Right, arr_voice_mode8[voice_index_mode8], arr_voice_mode8[voice_index_mode8]);
							else
								VoiceLamp(data, sizeof(data), off_set_Left, off_set_Right, arr_voice_mode8[voice_index_mode8], arr_voice_mode8[voice_index_mode8]);
							voice_index_mode8++;
							if (alreadyRunNUM == send_Parameter.runNUM - 1)
								arr_voice_mode8[arr_loopend_mode8 - 1] = 0;
							else
								arr_voice_mode8[arr_loopend_mode8 - 1] = 4;
							if (voice_index_mode8 == arr_loopend_mode8)
								voice_index_mode8 = 0;

							break;
						case 9:
							voice_index_mode8 = 0;
							voice_index_mode10 = 0;
							if (send_Parameter.direction == 5)
								VoiceLamp(data, sizeof(data), off_set_Left, 0, arr_voice_mode9[voice_index_mode9], arr_voice_mode9[voice_index_mode9]);
							else if (send_Parameter.direction == 6)
								VoiceLamp(data, sizeof(data), 0, off_set_Right, arr_voice_mode9[voice_index_mode9], arr_voice_mode9[voice_index_mode9]);
							else
								VoiceLamp(data, sizeof(data), off_set_Left, off_set_Right, arr_voice_mode9[voice_index_mode9], arr_voice_mode9[voice_index_mode9]);
							voice_index_mode9++;

							if (alreadyRunNUM == send_Parameter.runNUM - 1)
								arr_voice_mode9[arr_loopend_mode9 - 1] = 0;
							else
								arr_voice_mode9[arr_loopend_mode9 - 1] = 5;
							if (voice_index_mode9 == arr_loopend_mode9)
								voice_index_mode9 = 0;

							break;
						case 10:
							voice_index_mode8 = 0;
							voice_index_mode9 = 0;
							if (send_Parameter.direction == 5)
								VoiceLamp(data, sizeof(data), off_set_Left, 0, arr_voice_mode10[voice_index_mode10], arr_voice_mode10[voice_index_mode10]);
							else if (send_Parameter.direction == 6)
								VoiceLamp(data, sizeof(data), 0, off_set_Right, arr_voice_mode10[voice_index_mode10], arr_voice_mode10[voice_index_mode10]);
							else
								VoiceLamp(data, sizeof(data), off_set_Left, off_set_Right, arr_voice_mode10[voice_index_mode10], arr_voice_mode10[voice_index_mode10]);
							voice_index_mode10++;

							if (alreadyRunNUM == send_Parameter.runNUM - 1)
								arr_voice_mode10[arr_loopend_mode10 - 1] = 0;
							else
								arr_voice_mode10[arr_loopend_mode10 - 1] = 4;
							if (voice_index_mode10 == arr_loopend_mode10)
								voice_index_mode10 = 0;

							break;
						}
					}
					else if (send_Parameter.mode == Mode13)
					{
						DashedLineMovement(data, sizeof(data), LEDNUM, send_Parameter.direction, SentGroup);
					}
					else if (send_Parameter.mode == Mode14)
					{
						DashedLineSwimming(data, sizeof(data), LEDNUM, send_Parameter.direction, SentGroup);
					}
					else if (send_Parameter.mode == Mode15)
					{
						DashedStep_lightup(data, sizeof(data), LEDNUM, send_Parameter.direction, SentGroup); // 虚线依次点亮，重写
					}
					else if (send_Parameter.mode == Mode16)
					{

						DashedStep(data, sizeof(data), LEDNUM, send_Parameter.direction, SentGroup);
					}

					sendSlaveFrame1.matrix.Start = 0x00; // 填充共用体数据1

					if (send_Parameter.mode == Mode12)
						sendSlaveFrame1.matrix.Mode = 0x01;
					else if ((send_Parameter.mode == Mode3) || (send_Parameter.mode == Mode1))
						sendSlaveFrame1.matrix.Mode = 0x02;
					else
						sendSlaveFrame1.matrix.Mode = 0x00;

					sendSlaveFrame1.matrix.IntensitySet = send_Parameter.brightness;
					sendSlaveFrame1.matrix.Color1 = send_Parameter.color1;

					if (send_Parameter.mode == Mode1 || send_Parameter.mode == Mode2 || send_Parameter.mode == Mode4 || send_Parameter.mode == Mode6 || send_Parameter.mode == Mode11)
					{
						if (send_Parameter.mode == Mode2)
						{

							if (breathingFlag == TRUE)
							{
								sendSlaveFrame1.matrix.Time_ON = (send_Parameter.displayTimer - 200) / 200 / 2;
								if(send_Parameter.displayTimer == 200||send_Parameter.displayTimer == 400)	// 11.15 modify 修复时间参数为0x01\0x02\0x03不呼吸的问题
									sendSlaveFrame1.matrix.Time_ON = 1;
								sendSlaveFrame1.matrix.Time_OFF = 0;
							}
							else
							{
								sendSlaveFrame1.matrix.Time_ON = 0;
							}
						}
						else if (send_Parameter.mode == Mode3)
						{
							if (breathingFlag == TRUE)
							{
								sendSlaveFrame1.matrix.Time_ON = 0;
								sendSlaveFrame1.matrix.Time_OFF = 0;
							}
						}
						else if (send_Parameter.mode == Mode6)
						{
							// 2024.01.16 modify	提高流光两个颜色的区分度（仅IP）
							if (send_Parameter.displayTimer / 200 < 7) 
								sendSlaveFrame1.matrix.Time_ON = 1;
							else if ((send_Parameter.displayTimer / 200 >= 7) && (send_Parameter.displayTimer / 200 < 15))
								sendSlaveFrame1.matrix.Time_ON = 1;
							else if ((send_Parameter.displayTimer / 200 >= 15) && (send_Parameter.displayTimer / 200 < 25))
								sendSlaveFrame1.matrix.Time_ON = 2;
							else if ((send_Parameter.displayTimer / 200 >= 25) && (send_Parameter.displayTimer / 200 < 35))
								sendSlaveFrame1.matrix.Time_ON = 4;
							else if ((send_Parameter.displayTimer / 200 >= 35) && (send_Parameter.displayTimer / 200 < 45))
								sendSlaveFrame1.matrix.Time_ON = 5;
							else
								sendSlaveFrame1.matrix.Time_ON = 5;

							sendSlaveFrame1.matrix.Time_OFF = 2;
						}
						else if (send_Parameter.mode == Mode11)
						{
							// 2024.01.16 modify 提高炫彩流水两个颜色的区分度（仅IP）
							// 2024.02.02 modify 延长第一色的长度
							if (send_Parameter.direction == 0x03 || send_Parameter.direction == 0x04)
							{
								// sendSlaveFrame1.matrix.Time_ON = send_Parameter.displayTimer / 200 / 3;
								sendSlaveFrame1.matrix.Time_ON = send_Parameter.displayTimer / 200 / 2;  
							}
							else
							{
								// sendSlaveFrame1.matrix.Time_ON = send_Parameter.displayTimer / 200 / 2;
								sendSlaveFrame1.matrix.Time_ON = send_Parameter.displayTimer / 200 / 1.3;	
							}
							
						// 2024.02.02 modify 增加炫彩流水第1个颜色的长度
							// if (send_Parameter.displayTimer / 200 < 7)
							// 	sendSlaveFrame1.matrix.Time_ON = 3;
							// else if ((send_Parameter.displayTimer / 200 >= 7) && (send_Parameter.displayTimer / 200 < 15))
							// 	sendSlaveFrame1.matrix.Time_ON = 3;
							// else if ((send_Parameter.displayTimer / 200 >= 15) && (send_Parameter.displayTimer / 200 < 25))
							// 	sendSlaveFrame1.matrix.Time_ON = 10;
							// else if ((send_Parameter.displayTimer / 200 >= 25) && (send_Parameter.displayTimer / 200 < 35))
							// 	sendSlaveFrame1.matrix.Time_ON = 15;
							// else if ((send_Parameter.displayTimer / 200 >= 35) && (send_Parameter.displayTimer / 200 < 45))
							// 	sendSlaveFrame1.matrix.Time_ON = 20;
							// else
							// 	sendSlaveFrame1.matrix.Time_ON = 25;
						}
						else if (send_Parameter.mode == Mode13)
						{
							if (send_Parameter.displayTimer / 200 < 7)
								sendSlaveFrame1.matrix.Time_ON = 1;
							else if ((send_Parameter.displayTimer / 200 >= 7) && (send_Parameter.displayTimer / 200 < 15))
								sendSlaveFrame1.matrix.Time_ON = 1;
							else if ((send_Parameter.displayTimer / 200 >= 15) && (send_Parameter.displayTimer / 200 < 25))
								sendSlaveFrame1.matrix.Time_ON = 1;
							else if ((send_Parameter.displayTimer / 200 >= 25) && (send_Parameter.displayTimer / 200 < 35))
								sendSlaveFrame1.matrix.Time_ON = 2;
							else if ((send_Parameter.displayTimer / 200 >= 35) && (send_Parameter.displayTimer / 200 < 45))
								sendSlaveFrame1.matrix.Time_ON = 3;
							else
								sendSlaveFrame1.matrix.Time_ON = 3;
						}
						else if (send_Parameter.mode == Mode4)
						{

							//sendSlaveFrame1.matrix.Time_ON = send_Parameter.displayTimer / 2 / 200; //
							sendSlaveFrame1.matrix.Time_ON = send_Parameter.displayTimer / 200; 							//  modify 11.18
						}
						else
						{
							sendSlaveFrame1.matrix.Time_ON = send_Parameter.displayTimer / 200; //	//渐亮 多色渐变时间要快，这里不需要乘200
						}
					}
					else
					{
						sendSlaveFrame1.matrix.Time_ON = 0; // 流水不渐变颜色
					}

					if ((send_Parameter.mode == Mode8) || (send_Parameter.mode == Mode9) || (send_Parameter.mode == Mode10)) // 0x8=语音唤醒//0x9=语音识别	//0xA=TTS播报
					{
						sendSlaveFrame1.matrix.Color1 = 0; // 索引号为0 颜色=0XFFFFFF
					}
					else if (send_Parameter.mode == Mode4) // 多色渐变
					{

						if (multiColorTransitionCount == 1) // 第一帧用颜色1 													// modify 11.18
						{
							sendSlaveFrame1.matrix.Color1 = send_Parameter.color1;
						}
						else if(multiColorTransitionCount == 2)// 第二帧用颜色2													// modify 11.18
						{
							sendSlaveFrame1.matrix.Color1 = send_Parameter.color2;
						}
					}
					else if ((send_Parameter.mode == Mode11) || (send_Parameter.mode == Mode6)) // 新增
					{
						sendSlaveFrame1.matrix.Color1 = send_Parameter.color2;
					}
					else
					{
						sendSlaveFrame1.matrix.Color1 = send_Parameter.color1;
					}

					if (send_Parameter.mode == Mode13)
					{
						if (send_Parameter.displayTimer / 200 < 7)
							sendSlaveFrame1.matrix.Time_OFF = 2;
						else if ((send_Parameter.displayTimer / 200 >= 7) && (send_Parameter.displayTimer / 200 < 15))
							sendSlaveFrame1.matrix.Time_OFF = 3;
						else if ((send_Parameter.displayTimer / 200 >= 15) && (send_Parameter.displayTimer / 200 < 25))
							sendSlaveFrame1.matrix.Time_OFF = 4;
						else if ((send_Parameter.displayTimer / 200 >= 25) && (send_Parameter.displayTimer / 200 < 35))
							sendSlaveFrame1.matrix.Time_OFF = 5;
						else if ((send_Parameter.displayTimer / 200 >= 35) && (send_Parameter.displayTimer / 200 < 45))
							sendSlaveFrame1.matrix.Time_OFF = 6;
						else
							sendSlaveFrame1.matrix.Time_OFF = 7;
					}
					else if (send_Parameter.mode == Mode2)
					{

						if (breathingFlag == FALSE)
						{
							sendSlaveFrame1.matrix.Time_OFF = (send_Parameter.displayTimer - 200) / 200 / 2;		
							if(send_Parameter.displayTimer == 200||send_Parameter.displayTimer == 400)		// 11.15 modify 修复时间参数为0x01\0x02\0x03不呼吸的问题
								sendSlaveFrame1.matrix.Time_OFF = 1;
							sendSlaveFrame1.matrix.Time_ON = 0;
						}
						else
						{
							sendSlaveFrame1.matrix.Time_OFF = 0;
						}
					}
					else if (send_Parameter.mode == Mode3)
					{
						if (breathingFlag == FALSE)
						{
							sendSlaveFrame1.matrix.Time_OFF = 0;
							sendSlaveFrame1.matrix.Time_ON = 0;
						}
					}
					else if (send_Parameter.mode == Mode16)
					{
						sendSlaveFrame1.matrix.Time_OFF = send_Parameter.displayTimer / 200 / 7;
					}
					else
					{
						sendSlaveFrame1.matrix.Time_OFF = 0;
					}

					sendSlaveFrame2.matrix.Start = 0x01;						   // 填充共用体数据2
					if ((send_Parameter.mode == 11) || (send_Parameter.mode == 6)) // 炫彩流水 流光
					{
						sendSlaveFrame2.matrix.Color2 = send_Parameter.color1;
					}
					else
					{
						// 需要保留
						sendSlaveFrame2.matrix.Color2 = MaxColor;
					}
					sendSlaveFrame2.matrix.ledDATA12_High4 = data[12] >> 4;
					sendSlaveFrame2.matrix.ledDATA11 = data[11];
					sendSlaveFrame2.matrix.ledDATA10 = data[10];
					sendSlaveFrame2.matrix.ledDATA9 = data[9];
					sendSlaveFrame2.matrix.ledDATA8 = data[8];
					sendSlaveFrame2.matrix.ledDATA7_Low2 = (data[7] & 0x03);

					sendSlaveFrame3.matrix.Start = 0x03; // 填充共用体数据3
					sendSlaveFrame3.matrix.ledDATA7_High6 = (data[7] >> 2);
					sendSlaveFrame3.matrix.ledDATA6 = data[6];
					sendSlaveFrame3.matrix.ledDATA5 = data[5];
					sendSlaveFrame3.matrix.ledDATA4 = data[4];
					sendSlaveFrame3.matrix.ledDATA3 = data[3];
					sendSlaveFrame3.matrix.ledDATA2 = data[2];
					sendSlaveFrame3.matrix.ledDATA1 = data[1];
					sendSlaveFrame3.matrix.ledDATA0 = data[0];

					send_EN = 1;
					alreadyframe = 0;
				}
			}
			else	// when SentGroup >= send_Parameter.sendGroupNUM
			{

				if (send_Parameter.mode > Mode4)
				{

					if (ALU_ModeCount < send_Parameter.runNUM || send_Parameter.runNUM == FrecyContrUpperLimit)
					{

						// if (send_Parameter.mode == Mode11)
						// 	SentGroup = -1;
						// else
							SentGroup = 0;
						send_EN = 0;
						alreadyframe = 0;
						running_times1 = 0;
						running_times2 = 0;
						TIMER_Disable(TIMER0);
						Sys_Flag = 0;
						for (uint8_t i = 0; i < sendByte; i++)
						{
							data[i] = 0;
						}
					}
					else
					{
						TIMER_Disable(TIMER0);
						Sys_Flag = 0;
					}
					if (nweNode_Mode == TRUE)
					{
						nweNode_Mode = FALSE;
						sendParameterinit();
						changeColorIndex();
						send_Parameter.mode = SWAP_DATA->matrix.Node_ModeContr;
						send_Parameter.direction = SWAP_DATA->matrix.Node_RegContr;
						send_Parameter.brightness = BrightnessCalculate(SWAP_DATA->matrix.Node_DimLevel);
						send_Parameter.displayTimer = ModePeriodCalculate(SWAP_DATA->matrix.Node_TimeContr);
						send_Parameter.runNUM = ModeTimeCalculate(SWAP_DATA->matrix.Node_FrecyContr);
						ALU_ModeCount = 0; // 入参更新后次数要重新

						if (send_Parameter.direction == 3)
							send_Parameter.direction = 4;
						else if (send_Parameter.direction == 4)
							send_Parameter.direction = 3;

						for (uint8_t i = 0; i < sendByte; i++)
						{
							data[i] = 0;
						}
						Sys_Flag = 1;

						setRunParameter();
						SentGroup = 0;
					}
				}
				else
				{
					SentGroup = 0;
				}
			}
		}		// if(send_EN == 0) end
		if (send_EN == 1)
		{
			if (alreadyframe <= 3)
			{
				// 发送时4ms发一帧
				TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, 4 * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
				if (alreadyframe == 1)
				{
					for (uint8_t i = 0; i < 8; i++)
					{
						linm_cmd_table[0].data[i] = sendSlaveFrame1.data[i];
					}
					LINM_SendFrame(&linm_cmd_table[0]); // 发送第1帧
				}

				else if (alreadyframe == 2)
				{
					for (uint8_t i = 0; i < 8; i++)
					{
						linm_cmd_table[0].data[i] = sendSlaveFrame2.data[i];
					}
					LINM_SendFrame(&linm_cmd_table[0]); // 发送第2帧
				}
				else if (alreadyframe == 3)
				{
					for (uint8_t i = 0; i < 8; i++)
					{
						linm_cmd_table[0].data[i] = sendSlaveFrame3.data[i];
					}
					LINM_SendFrame(&linm_cmd_table[0]); // 发送第3帧
				}
				alreadyframe++;
			}
			else
			{
				if (send_Parameter.mode < Mode3)
				{
					Sys_Flag = 0;
				}
				send_EN = 0;
				alreadyframe = 0;
				TIMER_Disable(TIMER0);
				if(timer_tick <= 4 + 2.293)		timer_tick = 4 + 2.293 + 0.01; 																	// 11.15 modify
				TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, (timer_tick - 4 - 2.293) * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
			}
		}
	}
}

void LIN_Master_ISR(LIN_Device_Frame_t const *frame)
{
	activelinmFrame = *frame;
	TM_PostTask(TASK_ID_LINM);
}

void hanldeCommands(LIN_Device_Frame_t *frame)
{
	// Handle received command
}

void BcmControlCommandsDataHandle() // 处理参数
{
	if (ALU_DATA->matrix.NodeLEDSwitch == 0) // 开关关闭
	{
		if (ALU_DATA->matrix.Node_ModeContr == Mode1) // 渐灭
		{
			if (ALU_DATA->matrix.Node_TimeContr > 1)
			{
				sendParameterinit();
				changeColorIndex();
				oldNode_Mode = ALU_DATA->matrix.Node_ModeContr; // 渐灭
				send_Parameter.mode = ALU_DATA->matrix.Node_ModeContr;
				send_Parameter.runNUM = ModeTimeCalculate(ALU_DATA->matrix.Node_FrecyContr);
				send_Parameter.direction = ALU_DATA->matrix.Node_RegContr;
				send_Parameter.brightness = 0;
				send_Parameter.displayTimer = ALU_DATA->matrix.Node_TimeContr;
				timer_tick = 10; // 4 --> 10
				send_Parameter.sendGroupNUM = 1;
				send_Parameter.sendFrameNUM = 3;
			}
			else
			{
				sendParameterinit();
				changeColorIndex();
				send_Parameter.mode = ALU_DATA->matrix.Node_ModeContr; // 立即熄灭
				send_Parameter.runNUM = 1;
				send_Parameter.direction = ALU_DATA->matrix.Node_RegContr;
				send_Parameter.brightness = 0;
				send_Parameter.displayTimer = 0;
				timer_tick = 10; // 4 --> 10
				send_Parameter.sendGroupNUM = 1;
				send_Parameter.sendFrameNUM = 3;
			}
		}
		else if (ALU_DATA->matrix.Node_ModeContr == Mode5) // 流水熄灭
		{
			if (ALU_DATA->matrix.Node_TimeContr != 0)
			{
				sendParameterinit();
				changeColorIndex();
				send_Parameter.mode = ALU_DATA->matrix.Node_ModeContr;
				send_Parameter.runNUM = ModeTimeCalculate(ALU_DATA->matrix.Node_FrecyContr);
				send_Parameter.direction = ALU_DATA->matrix.Node_RegContr;
				send_Parameter.brightness = mode1brightness;
				send_Parameter.displayTimer = ModePeriodCalculate(ALU_DATA->matrix.Node_TimeContr);
			}
			else
			{
				changeColorIndex();
				send_Parameter.runNUM = ModeTimeCalculate(ALU_DATA->matrix.Node_FrecyContr);
				send_Parameter.direction = ALU_DATA->matrix.Node_RegContr;
				send_Parameter.direction = ALU_DATA->matrix.Node_RegContr;
				send_Parameter.brightness = mode1brightness;
				send_Parameter.displayTimer = ModePeriodCalculate(ALU_DATA->matrix.Node_TimeContr);
			}
			send_Parameter.runNUM = 1;
		}
		else // 立即熄灭
		{
			sendParameterinit();
			changeColorIndex();
			send_Parameter.mode = ALU_DATA->matrix.Node_ModeContr;
			send_Parameter.runNUM = ModeTimeCalculate(ALU_DATA->matrix.Node_FrecyContr);
			send_Parameter.direction = ALU_DATA->matrix.Node_RegContr;
			send_Parameter.brightness = 0;
			send_Parameter.displayTimer = ModePeriodCalculate(ALU_DATA->matrix.Node_TimeContr);
			timer_tick = 10; // 4 --> 10
			send_Parameter.sendGroupNUM = 1;
			send_Parameter.sendFrameNUM = 3;
		}
		multiColorTransitionCount = 0;  //   2023.12.11  解决多色渐变熄灭后再次调用时，未将参数初始化的问题
	}
	else
	{

		if ((ALU_DATA->matrix.Node_ModeContr == Mode2) || (ALU_DATA->matrix.Node_ModeContr == Mode3) || (ALU_DATA->matrix.Node_ModeContr == Mode4)) // 呼吸 闪烁 多色渐变
		{
			if ((ALU_DATA->matrix.Node8_Colour1 >= ColourContrLowerLimit && ALU_DATA->matrix.Node8_Colour1 < ColourContrUpperLimit)	  /*颜色1*/
				&& (ALU_DATA->matrix.Node_DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.Node_DimLevel <= DimLevelUpperLimit)	  /*亮度*/
				&& (ALU_DATA->matrix.Node_TimeContr >= TimeContrLowerLimit && ALU_DATA->matrix.Node_TimeContr <= TimeContrUpperLimit) /*效果时间*/
				&& (ALU_DATA->matrix.Node_FrecyContr >= FrecyContrLowerLimit && ALU_DATA->matrix.Node_FrecyContr <= FrecyContrUpperLimit) /*次数*/)
			{

				sendParametermodeinit();
				changeColorIndex();
				send_Parameter.mode = ALU_DATA->matrix.Node_ModeContr;
				send_Parameter.runNUM = ModeTimeCalculate(ALU_DATA->matrix.Node_FrecyContr);
				send_Parameter.direction = ALU_DATA->matrix.Node_RegContr;
				send_Parameter.brightness = BrightnessCalculate(ALU_DATA->matrix.Node_DimLevel);
				mode1brightness = BrightnessCalculate(ALU_DATA->matrix.Node_DimLevel); // 	11.20 modify 保存亮度，在流水熄灭和渐灭时使用
				send_Parameter.displayTimer = ModePeriodCalculate(ALU_DATA->matrix.Node_TimeContr);
				if(send_Parameter.displayTimer == 0)												//11.15 modify  解决时间入参0x01卡死
					send_Parameter.displayTimer = 200;
				VIU_SyncOverflow = 0;
				ALU_ModeCount = 0; // 入参更新后次数要重新
			}
		}
		else if ((ALU_DATA->matrix.Node_ModeContr == Mode1) || (ALU_DATA->matrix.Node_ModeContr == Mode12))
		{
			if ((ALU_DATA->matrix.Node8_Colour1 >= ColourContrLowerLimit && ALU_DATA->matrix.Node8_Colour1 < ColourContrUpperLimit)	   /*颜色1*/
				&& (ALU_DATA->matrix.Node_DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.Node_DimLevel <= DimLevelUpperLimit))	   /*亮度*/
				//&& (ALU_DATA->matrix.Node_TimeContr >= TimeContrLowerLimit && ALU_DATA->matrix.Node_TimeContr <= TimeContrUpperLimit)) /*效果时间*/			//11.15 modify 常亮和无序没有效果时间判断
			{
				sendParameterinit();
				changeColorIndex();	
				send_Parameter.mode = ALU_DATA->matrix.Node_ModeContr;
				send_Parameter.runNUM = ModeTimeCalculate(ALU_DATA->matrix.Node_FrecyContr);
				send_Parameter.direction = ALU_DATA->matrix.Node_RegContr;
				send_Parameter.brightness = BrightnessCalculate(ALU_DATA->matrix.Node_DimLevel);
				send_Parameter.displayTimer = ModePeriodCalculate(ALU_DATA->matrix.Node_TimeContr);
				mode1brightness = BrightnessCalculate(ALU_DATA->matrix.Node_DimLevel); // 11.20 modify 保存亮度，在流水熄灭和渐灭时使用
			}
		}
		else if ((ALU_DATA->matrix.Node_ModeContr == Mode8) || (ALU_DATA->matrix.Node_ModeContr == Mode9) || (ALU_DATA->matrix.Node_ModeContr == Mode10))
		{
			if ((ALU_DATA->matrix.Node_DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.Node_DimLevel <= DimLevelUpperLimit) && 	/*亮度*/
			 	(ALU_DATA->matrix.Node_RegContr >= RegContrVoiceLowerLimit && ALU_DATA->matrix.Node_RegContr <= RegContrUpperLimit)) /*方向*/
			{
				sendParameterinit();
				changeColorIndex();
				send_Parameter.mode = ALU_DATA->matrix.Node_ModeContr;
				// send_Parameter.runNUM = ModeTimeCalculate(ALU_DATA->matrix.Node_FrecyContr);	
				send_Parameter.runNUM = 2;											   // 2023.12.19 modify 修复频率入参0或1时异常
				send_Parameter.direction = ALU_DATA->matrix.Node_RegContr;
				send_Parameter.brightness = BrightnessCalculate(ALU_DATA->matrix.Node_DimLevel);
				mode1brightness = BrightnessCalculate(ALU_DATA->matrix.Node_DimLevel); // 11.20 modify 保存亮度，在流水熄灭和渐灭时使用
				send_Parameter.displayTimer = ModePeriodCalculate(ALU_DATA->matrix.Node_TimeContr);
				setRunParameter();
			}
		}
		else
		{
			if ((ALU_DATA->matrix.Node8_Colour1 >= ColourContrLowerLimit && ALU_DATA->matrix.Node8_Colour1 < ColourContrUpperLimit)		  /*颜色1*/
				&& (ALU_DATA->matrix.Node_DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.Node_DimLevel <= DimLevelUpperLimit)		  /*亮度*/
				&& (ALU_DATA->matrix.Node_TimeContr >= TimeContrLowerLimit && ALU_DATA->matrix.Node_TimeContr <= TimeContrUpperLimit)	  /*效果时间*/
				&& (ALU_DATA->matrix.Node_FrecyContr >= FrecyContrLowerLimit && ALU_DATA->matrix.Node_FrecyContr <= FrecyContrUpperLimit) /*次数*/
				&& (ALU_DATA->matrix.Node_RegContr >= RegContrLowerLimit && ALU_DATA->matrix.Node_RegContr <= RegContrUpperLimit))		  /*方向*/
			{
				// static bool fristTimer = false;
				// if (fristTimer == false)			// modify 2023.12.11 解决流水流星等模式切换直接常亮的问题
				// {
				// 	fristTimer = true;
					// oldNode_Mode = ALU_DATA->matrix.Node_ModeContr;
				// }
				// if ((oldNode_Mode != ALU_DATA->matrix.Node_ModeContr) && (fristTimer))
				// {
				// 	oldNode_Mode = ALU_DATA->matrix.Node_ModeContr;
				// 	SWAP_DATA = ALU_DATA;
				// 	nweNode_Mode = TRUE;
				// }
				// else
				{
					
					send_Parameter.mode = ALU_DATA->matrix.Node_ModeContr;
					changeColorIndex();
					send_Parameter.runNUM = ModeTimeCalculate(ALU_DATA->matrix.Node_FrecyContr);
					// direction = ALU_DATA->matrix.Node_RegContr;		//保存方向，到下一效果周期再换方向
					send_Parameter.direction = ALU_DATA->matrix.Node_RegContr;
					send_Parameter.brightness = BrightnessCalculate(ALU_DATA->matrix.Node_DimLevel);
					mode1brightness = BrightnessCalculate(ALU_DATA->matrix.Node_DimLevel); // 11.20 modify 保存亮度，在流水熄灭和渐灭时使用
					send_Parameter.displayTimer = ModePeriodCalculate(ALU_DATA->matrix.Node_TimeContr);
					if (send_Parameter.direction == 3)
						send_Parameter.direction = 4;
					else if (send_Parameter.direction == 4)
						send_Parameter.direction = 3;
					ALU_ModeCount = 0; // 入参更新后次数要重新
				}
			}
		}
	}
}

bool Mode4IneffectiveFlag = true;		// 2024.01.24  modify 	多色渐变若第3字节为0x64，则此标志置true
void SyncJudge(void) // 按模式点灯
{
	if (OverflowFlag == true)
	{
		VIU_SyncOverflow++;
		if (VIU_SyncOverflow >= 0xff)
			VIU_SyncOverflow = 0;
		OverflowFlag = false;
	}
	if (VIU_Sync == 0xFF)
	{
		OverflowFlag = true;
	}

		// modify 2023.12.11  解决多色渐变熄灭后，再次调用多色渐变未初始化参数的缺陷
		// modify 2024.01.15  修改if判断，每次多色渐变入参有效均将此变量置0，进度从头开始
		// modify 2024.01.24  新增变量，以区分多次唤醒和电竞模式重复有效入参，解决多次唤醒错误的导致multiColorTransitionCount多次置零而出现001212此进程
	if((ALU_DATA->matrix.Node_ModeContr == Mode4 && ALU_DATA->matrix.Node_Flag == 0) || ALU_DATA->matrix.Node_ModeContr != Mode4 /*|| 以及在休眠唤醒的那一刻*/)
	{
		Mode4IneffectiveFlag = true;
	}
	if(ALU_DATA->matrix.Node_ModeContr == Mode4 && ALU_DATA->matrix.Node_Flag == 1)
	{
		if(Mode4IneffectiveFlag == true)
		{
			multiColorTransitionCount = 0;		
			Mode4IneffectiveFlag = false;
		}
	}


	// if(ALU_DATA->matrix.Node_ModeContr == Mode4 && ALU_DATA->matrix.Node_Flag == 1)
	// {
	// 	multiColorTransitionCount = 0;		
	// }
	if (ALU_DATA->matrix.NodeLEDSwitch == 0) // 开关关闭
	{
		if (ALU_DATA->matrix.Node_ModeContr == Mode1) // 渐灭
		{
			setRunParameter();
			Sys_Flag = 1;
			TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, 1 * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
		}
		else if (ALU_DATA->matrix.Node_ModeContr == Mode5)
		{
			if (ALU_ModeCount < send_Parameter.runNUM || send_Parameter.runNUM == FrecyContrUpperLimit)
			{
				//	第一个或：判断实际运行周期是否小于了预期运行周期，且是否是在重置同步的时刻
				//  第二个或：判断实际运行周期是否大于了预期运行周期
				CurrentRunTime = (VIU_Sync + VIU_SyncOverflow * OVERFLOWCONTER - VIU_SyncInit) * FramePeriod; //	当前灯效运行的时间
				WholeLampTime = (send_Parameter.sendGroupNUM * (SyncLongGap + SENDFRAMETIME));
				WholeLampTime /= 100;
				WholeLampTime *= 100;
				// if(CurrentRunTime > WholeLampTime)	 CurrentRunTime = 0;
				if ((VIU_SyncInit == VIU_Sync) || ((CurrentRunTime % WholeLampTime) == 0))
				{
					TIMER_Disable(TIMER0);
					CurrentRunTime = 0;
					VIU_SyncInit = VIU_Sync;
					VIU_SyncOverflow = 0;
					SentGroup = 0;
					send_EN = 0;
					alreadyframe = 0;
					running_times1 = 0;
					running_times2 = 0;
					timer_tick = 0;
					for (uint8_t i = 0; i < sendByte; i++)
					{
						data[i] = 0;
					}

					Sys_Flag = 1;
					setRunParameter();
					ALU_ModeCount++;
					TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, 1 * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
				}
			}
		}
	}
	else
	{
		if ((ALU_DATA->matrix.Node_ModeContr == Mode2) || (ALU_DATA->matrix.Node_ModeContr == Mode3)) // 11.20 modify 删除MODE4 单独放到下面 
		{
			if ((ALU_ModeCount < send_Parameter.runNUM || send_Parameter.runNUM == FrecyContrUpperLimit) && (send_Parameter.runNUM != 0)) // 判断循环次数
			{
				if ((((VIU_Sync + VIU_SyncOverflow * OVERFLOWCONTER - VIU_SyncInit) * FramePeriod) < (send_Parameter.displayTimer) && (VIU_SyncInit == VIU_Sync)) ||
					(((VIU_Sync + VIU_SyncOverflow * OVERFLOWCONTER - VIU_SyncInit) * FramePeriod) >= (send_Parameter.displayTimer) && 
					((VIU_Sync + VIU_SyncOverflow * OVERFLOWCONTER - VIU_SyncInit) * FramePeriod) % (send_Parameter.displayTimer) == 0)) // 起始第一次 或 同步计数时长大于效果周期
				{
					TIMER_Disable(TIMER0);
					timer_tick = send_Parameter.displayTimer / 2 - SENDFRAMETIME; // 计算定时器时间
					if(ALU_DATA->matrix.Node_TimeContr == 0x01)		timer_tick = 15;			//11.15 modify

					send_Parameter.sendGroupNUM = 1;
					send_Parameter.sendFrameNUM = 3;
					breathingFlag = 1;
					send_EN = 0;
					TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, 1 * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
					Sys_Flag = 1;
				}
				else if (((VIU_Sync + VIU_SyncOverflow * OVERFLOWCONTER - VIU_SyncInit) * FramePeriod) >= (send_Parameter.displayTimer / 2) &&
						((VIU_Sync + VIU_SyncOverflow * OVERFLOWCONTER - VIU_SyncInit) * FramePeriod) % (send_Parameter.displayTimer / 2) == 0)
				{
					TIMER_Disable(TIMER0);
					timer_tick = send_Parameter.displayTimer / 2 - SENDFRAMETIME; // 计算定时器时间
					if(ALU_DATA->matrix.Node_TimeContr == 0x01)		timer_tick = 15;			//11.15 modify

					send_Parameter.sendGroupNUM = 1;
					send_Parameter.sendFrameNUM = 3;
					breathingFlag = 0;
					send_EN = 0;
					CurrentRunTime = 0;
					TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, 1 * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
					Sys_Flag = 1;
					if (send_Parameter.runNUM != FrecyContrUpperLimit)
						ALU_ModeCount++;
				}
			}
		}
		else if(ALU_DATA->matrix.Node_ModeContr == Mode4)													// modify 11.20  解决多色渐变周期增加1倍的需求
		{
			if ((ALU_ModeCount < send_Parameter.runNUM + 1|| send_Parameter.runNUM == FrecyContrUpperLimit) && (send_Parameter.runNUM != 0)) // 判断循环次数  modify 11.20 解决多色渐变颜色逻辑问题
			{
				if ((((VIU_Sync + VIU_SyncOverflow * OVERFLOWCONTER - VIU_SyncInit) * FramePeriod) < (send_Parameter.displayTimer) && (VIU_SyncInit == VIU_Sync)) ||
					(((VIU_Sync + VIU_SyncOverflow * OVERFLOWCONTER - VIU_SyncInit) * FramePeriod) >= (send_Parameter.displayTimer) && 
					((VIU_Sync + VIU_SyncOverflow * OVERFLOWCONTER - VIU_SyncInit) * FramePeriod) % (send_Parameter.displayTimer) == 0)) // 起始第一次 或 同步计数时长大于效果周期
				{
					TIMER_Disable(TIMER0);
					timer_tick = send_Parameter.displayTimer / 2 - SENDFRAMETIME; // 计算定时器时间
					if(ALU_DATA->matrix.Node_TimeContr == 0x01)		timer_tick = 15;			//11.15 modify

					send_Parameter.sendGroupNUM = 1;
					send_Parameter.sendFrameNUM = 3;
					breathingFlag = 1;
					send_EN = 0;
					TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, 1 * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
					Sys_Flag = 1;
					multiColorTransitionCount ++;
					if(multiColorTransitionCount > 2)
						multiColorTransitionCount = 1;
					if(send_Parameter.runNUM != FrecyContrUpperLimit)
						ALU_ModeCount++;
				}
			}
		}
		
		// 流水跑马公用 MODE5流水 MODE11炫彩流水
		else if (((ALU_DATA->matrix.Node_ModeContr >= Mode5) && (ALU_DATA->matrix.Node_ModeContr <= Mode7)) || ((ALU_DATA->matrix.Node_ModeContr >= Mode13) && (ALU_DATA->matrix.Node_ModeContr <= Mode16)) || (ALU_DATA->matrix.Node_ModeContr == Mode11))
		{
			if (ALU_ModeCount < send_Parameter.runNUM || send_Parameter.runNUM == FrecyContrUpperLimit)
			{
				//	第一个或：判断实际运行周期是否小于了预期运行周期，且是否是在重置同步的时刻
				//  第二个或：判断实际运行周期是否大于了预期运行周期
				CurrentRunTime = ((VIU_Sync + (VIU_SyncOverflow << 8)) - VIU_SyncInit) * FramePeriod; //	当前灯效运行的时间
				// WholeLampTime = (send_Parameter.sendGroupNUM * (SyncLongGap + SENDFRAMETIME));
				// WholeLampTime /= 100;
				// WholeLampTime *= 100;
				WholeLampTime = send_Parameter.displayTimer;		// V006b版本待修改此处  2024.01.16 modify 修复跑马不同步

				if ((VIU_SyncInit == VIU_Sync) && (nweNode_Mode == FALSE) || ((CurrentRunTime >= WholeLampTime)))
				{
					TIMER_Disable(TIMER0);
					CurrentRunTime = 0;
					VIU_SyncInit = VIU_Sync;
					VIU_SyncOverflow = 0;
					SentGroup = 0;
					send_EN = 0;
					alreadyframe = 0;
					running_times1 = 0;
					running_times2 = 0;
					timer_tick = 0;
					for (uint8_t i = 0; i < sendByte; i++)
					{
						data[i] = 0;
					}
					Sys_Flag = 1;
					setRunParameter();
					ALU_ModeCount++;
					TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, 1 * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
				}
			}
		}
		else if (ALU_DATA->matrix.Node_ModeContr == Mode1)
		{
			Sys_Flag = 1;
			setRunParameter();
			TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, 1 * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
		}
		else if (ALU_DATA->matrix.Node_ModeContr == Mode12)
		{
			Sys_Flag = 1;
			setRunParameter();
			TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, 1 * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
		}
		else if ((ALU_DATA->matrix.Node_ModeContr == Mode8) || (ALU_DATA->matrix.Node_ModeContr == Mode9) || (ALU_DATA->matrix.Node_ModeContr == Mode10))
		{
			Sys_Flag = 1;
			TIMER_Init(TIMER0, HW_TIMER_PERIODIC_MODE, 1 * 1000 * MAIN_CPU_CLOCK, &TimerCallback);
		}
	}
}
void LINM_DataProtocol(BCM_ALU_CTRL_t const *BCM_CMD)
{

	static uint32_t old_crc0 = 0xFFFFFFFF;
	static uint32_t old_crc1 = 0xFFFFFFFF;
	static uint32_t old_crc2 = 0xFFFFFFFF;
	VIU_Sync = BCM_CMD->matrix.Node_Conter;

	CRC32_Init();
	uint32_t temp0 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[0])), LIN_BUFF_SIZE);
	if(ALU_DATA->matrix.Node_ModeContr != Mode3)		// 2024.01.16 modify 关闭闪烁模式的CRC，以迎合音乐律动模式
	{
		if (temp0 != old_crc0)
		{
			old_crc0 = temp0;
			if (BCM_CMD->matrix.Node_Flag == 1)
			{
				uint32_t temp1 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[0])), LIN_BUFF_SIZE - 4);
				uint32_t temp2 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[5])), LIN_BUFF_SIZE - 5);
				
				if (temp1 != old_crc1 || temp2 != old_crc2)
				{
					old_crc1 = temp1;
					old_crc2 = temp2;
					old_crc0 = 0xFFFFFFFF;
					ALU_DATA = BCM_CMD;
					VIU_SyncInit = VIU_Sync;
					VIU_SyncOverflow = 0;
					ALU_ModeCount = 0;
					BcmControlCommandsDataHandle();
				}
			}
		}
	}
	else
	{
		if (BCM_CMD->matrix.Node_Flag == 1)
		{
			//	2024.01.16 modify 即使不开启闪烁模式的CRC拦截，也要刷新CRC校验位
			old_crc0 = temp0;
			uint32_t temp1 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[0])), LIN_BUFF_SIZE - 4);
			uint32_t temp2 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[5])), LIN_BUFF_SIZE - 5);
			old_crc1 = temp1;
			old_crc2 = temp2;
			old_crc0 = 0xFFFFFFFF;

			ALU_DATA = BCM_CMD;
			VIU_SyncInit = VIU_Sync;
			VIU_SyncOverflow = 0;
			ALU_ModeCount = 0;
			BcmControlCommandsDataHandle();		
		}
	}
	SyncJudge();
}

void setRunParameter()
{

	// 0x4=多色渐变

	switch (send_Parameter.mode)
	{
	// 根据各模式得到:定时器值，组数，帧数
	// 0x1=常亮  	0xC=无序点光
	case 1:
	case 12:

		timer_tick = 10; // 5 --> 10
		send_Parameter.sendGroupNUM = 2;
		send_Parameter.sendFrameNUM = 3;

		// 这里定时1ms是为了尽快进入到中断

		break;

		// 0x5=流水
	case 5:

		send_Parameter.sendFrameNUM = 3;
		if (send_Parameter.direction == 3 || send_Parameter.direction == 4)
		{
			send_Parameter.sendGroupNUM = LEDNUM / 2;
			timer_tick = ceil((float)send_Parameter.displayTimer / (LEDNUM / 2)) - SENDFRAMETIME; // 计算定时器时间
		}
		else
		{
			timer_tick = ceil((float)send_Parameter.displayTimer / LEDNUM) - SENDFRAMETIME; // 计算定时器时间
			send_Parameter.sendGroupNUM = LEDNUM;
		}

		break;

	// 0x6=流光	0x7=流星
	case 6:
	case 7:

		if (send_Parameter.direction == 3 || send_Parameter.direction == 4)
		{
			send_Parameter.sendGroupNUM = (LEDNUM / 2) + meteorLength + 1;
			timer_tick = ceil((float)send_Parameter.displayTimer / ((LEDNUM / 2) + meteorLength)) - SENDFRAMETIME;		
			// timer_tick = ((float)send_Parameter.displayTimer / ((LEDNUM / 2) + meteorLength + 1)) - SENDFRAMETIME;
		}
		else
		{
			send_Parameter.sendGroupNUM = LEDNUM + meteorLength + 1;
			timer_tick = ceil((float)send_Parameter.displayTimer / (LEDNUM + meteorLength)) - SENDFRAMETIME;
			// timer_tick = ((float)send_Parameter.displayTimer / (LEDNUM + meteorLength + 1)) - SENDFRAMETIME;	
		}
		send_Parameter.sendFrameNUM = 3;

		break;

	// 0x8=语音唤醒			0x9=语音识别		0xA=TTS播报
	case 8:
	case 9:
	case 10:

		if (send_Parameter.mode == 10) // TTS播报 频率最快
		{
			timer_tick = 25;
			send_Parameter.sendGroupNUM = arr_loopend_mode10;
		}
		else if (send_Parameter.mode == 8) // 唤醒频率最慢
		{
			timer_tick = 80;
			send_Parameter.sendGroupNUM = arr_loopend_mode8;
		}
		else
		{
			timer_tick = 50; // 识别频率中等
			send_Parameter.sendGroupNUM = arr_loopend_mode9;
		}
		// send_Parameter.sendGroupNUM = 6; //?
		send_Parameter.sendFrameNUM = 3;
		if (send_Parameter.mode == 8)
			GroupMAX = 6;
		else if (send_Parameter.mode == 9)
			GroupMAX = 8;
		else
			GroupMAX = 6;
		temp_times = GroupMAX / 2;

		for (uint8_t i = 0; i < buf_size; i++)
		{
			data[i] = 0;
		}

		break;

		// 0xB=炫彩流水

	case 11:

		if (send_Parameter.direction == 3 || send_Parameter.direction == 4)
		{
			send_Parameter.sendGroupNUM = (LEDNUM / 2) + 20;																		// 11.14 modify
			timer_tick = ceil((float)send_Parameter.displayTimer / ((LEDNUM / 2) + 20)) - SENDFRAMETIME; // 3，4方向时速度要慢一半
		}
		else
		{
 			send_Parameter.sendGroupNUM = LEDNUM + 20;																				// 11.14 modify
			timer_tick = ceil((float)send_Parameter.displayTimer / (LEDNUM + 20)) - SENDFRAMETIME;
		}
		send_Parameter.sendFrameNUM = 3;

		break;

	// 0xD=虚线平移流水
	case 13:

		//timer_tick = ceil((float)send_Parameter.displayTimer / LEDNUM * 3 + singleGroupTimer);
		timer_tick = ceil((float)send_Parameter.displayTimer / 22 - SENDFRAMETIME); 												// 11.15 modify
		if (ALU_ModeCount == send_Parameter.runNUM - 1) // 实现不循环则熄灭功能
		{
			send_Parameter.sendGroupNUM = 23;
		}
		else
		{
			send_Parameter.sendGroupNUM = 22;
		}
		send_Parameter.sendFrameNUM = 3;

		break;

		//	0xE=虚线游动
	case 14:
		//timer_tick = ceil((float)send_Parameter.displayTimer / LEDNUM * 3 + singleGroupTimer); 
		timer_tick = ceil((float)send_Parameter.displayTimer / 22 - SENDFRAMETIME); 												// 11.15 modify
		if (ALU_ModeCount == send_Parameter.runNUM - 1)										   // 实现不循环则熄灭功能
		{
			send_Parameter.sendGroupNUM = 23;
		}
		else
		{
			send_Parameter.sendGroupNUM = 22;
		}

		send_Parameter.sendFrameNUM = 3;

		break;

	//	0xF=虚线依次点亮
	case 15:

		if (send_Parameter.direction == 3 || send_Parameter.direction == 4)
		{
			send_Parameter.sendGroupNUM = 4;
			timer_tick = ceil((float)send_Parameter.displayTimer / 4 - SENDFRAMETIME);
		}
		else
		{
			send_Parameter.sendGroupNUM = 6;
			timer_tick = ceil((float)send_Parameter.displayTimer / 6 - SENDFRAMETIME);
		}
		send_Parameter.sendFrameNUM = 3;
		// TIMER_Disable(TIMER0);

		break;
		// 0x10=踏步平移
	case 16:
		if (send_Parameter.direction == 3 || send_Parameter.direction == 4)
		{
			send_Parameter.sendGroupNUM = 6;
			timer_tick = ceil((float)send_Parameter.displayTimer / 6 - SENDFRAMETIME);
		}
		else
		{
			send_Parameter.sendGroupNUM = 10;
			timer_tick = ceil((float)send_Parameter.displayTimer / 10 - SENDFRAMETIME);
		}
		break;

	default:

		break;
	}
	SyncLongGap = timer_tick;
}

void LINM_TaskHandler(void)
{
	switch (linmTaskState)
	{
	case TASK_STATE_ACTIVE:
		setRunParameter();
		break;
	case TASK_STATE_INIT:
		LINM_Init();

		(void)ls_set_linm_rx_glitch_filter_1st(0x00U, 0x08U);
		(void)ls_set_linm_rx_glitch_filter_2nd(0x0AU, 0x10U);
		(void)ls_set_linm_rx_glitch_filter_3rd(0x30U, 0x30U);
		//     LINM_RegisterLINM_IRQ(LIN_Master_ISR);
		RegisterDataProtocol(LINM_DataProtocol);
		//	  CRC32_Init();
		linmTaskState = TASK_STATE_ACTIVE;
		break;
	}
}

#endif

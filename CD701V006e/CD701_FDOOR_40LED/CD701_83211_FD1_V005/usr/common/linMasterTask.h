#ifndef _LINM_TASK_H
#define _LINM_TASK_H

#include <appConfig.h>
#include <softTimerTask.h>
#include "systick_device.h"
#include <taskManager.h>
#include <lin_device.h>
#include <ColorMixingTask.h>

#define FramePeriod (50) // 帧周期
#define ColourContrLowerLimit (0x00)
#define ColourContrUpperLimit (257)
#define DimLevelLowerLimit (0x01)
#define DimLevelUpperLimit (0x65)
#define TimeContrLowerLimit (0x01)
#define TimeContrUpperLimit (0x33)
#define FrecyContrLowerLimit (0x01)
#define FrecyContrUpperLimit (0x3F)
#define RegContrLowerLimit (0x01)
#define RegContrUpperLimit (0x07)
#define MaxColor (257)

typedef union
{
	uint8_t command;
	struct
	{
		uint8_t command;
		uint8_t transitionTime; /* 100ms/count */
		uint16_t x;
		uint16_t y;
		uint16_t Y;
	} ColorXY;
	struct
	{
		uint8_t command;
		uint8_t transitionTime;
		uint16_t hue;
		uint16_t saturation;
		uint16_t level;
	} ColorHSL;
	struct
	{
		uint8_t command;
		uint8_t transitionTime;
		uint16_t level;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	} ColorRGB;
	struct
	{
		uint8_t command;
		uint8_t transitionTime;
		uint16_t PWM_Red;
		uint16_t PWM_Green;
		uint16_t PWM_Blue;
	} ColorPWM;
	uint8_t data[LIN_BUFF_SIZE]; /*!< data buffer. */
} Color_CMDM_Frame_t;

typedef enum
{
	COLOR_CMDM_XYY = 0x00,
	COLOR_CMDM_HSL = 0x01,
	COLOR_CMDM_RGB = 0x02,
	COLOR_CMDM_PWM = 0x03,
} ColorMCommand_t;



typedef union
{
	struct
	{

		uint64_t Start : 2; // 起始标志
		uint64_t Mode : 3;	// 模式
		uint64_t reserve1 : 3;
		uint64_t Color1 : 16;
		uint64_t pending1 : 8;
		uint64_t IntensitySet : 16; // 亮度控制
		uint64_t Time_ON : 7;
		uint64_t reserve2 : 1;
		uint64_t Time_OFF : 7;
		uint64_t reserve3 : 1;
	} matrix;
	uint8_t data[LIN_BUFF_SIZE];
} Send_Slave_Frame1_t;

/*大端
typedef union
{
  struct
  {

	uint64_t Start: 			2;			//起始标志
	uint64_t Red2: 				8;
	uint64_t Green2: 			8;
	uint64_t Blue2: 			8;
	uint64_t ledDATA0: 			8;
	uint64_t ledDATA1: 			8;
	uint64_t ledDATA2: 			8;
	uint64_t ledDATA3: 			8;
	uint64_t ledDATA4_Low6: 	6;
  } matrix;
  uint8_t data[LIN_BUFF_SIZE];
} Send_Slave_Frame2_t;

typedef union
{
  struct
  {

	uint64_t Start: 			2;			//起始标志
	uint64_t ledDATA4_High2: 	2;
	uint64_t ledDATA5: 			8;
	uint64_t ledDATA6: 			8;
	uint64_t ledDATA7: 			8;
	uint64_t ledDATA8: 			8;
	uint64_t ledDATA9: 			8;
	uint64_t ledDATA10: 		8;
	uint64_t ledDATA11: 		8;
	uint64_t ledDATA12_High4: 	4;

  } matrix;
  uint8_t data[LIN_BUFF_SIZE];
} Send_Slave_Frame3_t;
 */

typedef union
{
	struct
	{

		uint64_t Start : 2; // 起始标志
		uint64_t Color2 : 16;
		uint64_t pending2 : 8;
		uint64_t ledDATA12_High4 : 4;
		uint64_t ledDATA11 : 8;
		uint64_t ledDATA10 : 8;
		uint64_t ledDATA9 : 8;
		uint64_t ledDATA8 : 8;
		uint64_t ledDATA7_Low2 : 2;
	} matrix;
	uint8_t data[LIN_BUFF_SIZE];
} Send_Slave_Frame2_t;

typedef union
{
	struct
	{
		uint64_t Start : 2; // 起始标志
		uint64_t ledDATA7_High6 : 6;
		uint64_t ledDATA6 : 8;
		uint64_t ledDATA5 : 8;
		uint64_t ledDATA4 : 8;
		uint64_t ledDATA3 : 8;
		uint64_t ledDATA2 : 8;
		uint64_t ledDATA1 : 8;
		uint64_t ledDATA0 : 8;
	} matrix;
	uint8_t data[LIN_BUFF_SIZE];
} Send_Slave_Frame3_t;

typedef struct
{
	uint8_t Node_color1 : 8;		 // 颜色1
	uint8_t Node_color2 : 8;		 // 颜色2
	uint8_t Node_color1flag : 1;	 // 颜色1有效果标志位
	uint8_t Node_color2flag : 1;	 // 颜色2有效果标志位
	uint8_t Node_brightness : 7;	 // 亮度
	uint64_t Node_displayTimer : 64; // 效果时间
	uint8_t Node_runNumer : 6;		 // 运行次数
	uint8_t Node_mode : 5;			 // 模式
	uint8_t Node_syncTimer : 8;		 // 同步时间
	uint8_t Node_direction : 4;		 // 方向
} Node_Parameter;

typedef struct
{

	uint8_t mode;		  // 模式
	uint8_t GroupNUM;	  // 组数
	uint8_t SentGroupNUM; // 已发组数
	uint8_t runNUM;		  // 运行次数
	uint8_t color1;
	uint8_t color2;

} syncMode_Parameter;

typedef struct
{
	uint8_t sendFrameNUM; // 发送帧数
	uint8_t sendGroupNUM; // 发送组数
	uint8_t mode;		  // 模式
	uint8_t runNUM;		  // 运行次数
	uint8_t direction;	  // 运行方向
	uint8_t color1;
	uint8_t color2;
	uint8_t color1flag;
	uint8_t color2flag;
	uint16_t brightness;
	uint16_t displayTimer;
} Send_Parameter;

typedef void (*AppDataProtocol)(uint8_t *ctrlDATA);

void APPL_RegisterDataProtocol(AppDataProtocol dp);

typedef enum
{
	Mode1 = 0x01,  // 常亮
	Mode2 = 0x02,  // 呼吸
	Mode3 = 0x03,  // 闪烁
	Mode4 = 0x04,  // 多色渐变
	Mode5 = 0x05,  // 流水
	Mode6 = 0x06,  // 流光
	Mode7 = 0x07,  // 流星
	Mode8 = 0x08,  // 语音唤醒
	Mode9 = 0x09,  // 语音识别
	Mode10 = 0x0A, // TTS播报
	Mode11 = 0x0B, // 炫彩流水
	Mode12 = 0x0C, // 无序点光
	Mode13 = 0x0D, // 虚线平移流水
	Mode14 = 0x0E, // 虚线游动
	Mode15 = 0x0F, // 虚线依次点亮
	Mode16 = 0x10, // 踏步平移
} ModeControl_t;

void LINM_TaskHandler(void);

extern LIN_Device_Frame_t linm_cmd_table[];

#endif

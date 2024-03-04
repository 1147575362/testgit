/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file applicationTask.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/09/10
 */

#include <applicationTask.h>
#include <measureTask.h>
#include <pdsTask.h>
#include <linStackTask.h>
#include <crc32.h>

/*static uint8_t ledNum = LED0;*/
static TaskState_t applState = TASK_STATE_INIT;
void ApplTimerExpired(SoftTimer_t *timer);
ColorTemperature_t temperature;
BCM_ALU_CTRL *ALU_DATA;

uint8_t VIU_Sync;
uint8_t VIU_SyncInit;
uint64_t VIU_SyncOverflow;

uint8_t ModeTemp;
uint8_t ModeCache;

uint8_t ColorIndex1;
uint8_t ColorIndex2;
uint16_t Brightness;
uint16_t ModePeriod;
uint8_t ModeTime;

uint8_t ALU_ModeCount;
static uint8_t multiColorTransitionCount = 0;   // modify 11.23
bool Mode4IneffectiveFlag = true;		            // 2024.01.24  modify 	多色渐变若第3字节为0x64，则此标志置true
static uint8_t mode1Color1;                     // modify 2023.12.19

static uint8_t LED_State = 0U;

uint8_t APPL_GetLEDState(void)
{
    return LED_State;
}

static SoftTimer_t ApplTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 500U,
    .handler  = ApplTimerExpired
};


void ApplTimerExpired(SoftTimer_t *timer)
{

}

void APPL_TaskHandler(void)
{
    switch(applState){
    case TASK_STATE_INIT:
	  Led_InitParam();
      //SoftTimer_Start(&ApplTimer);
      applState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      break;
    default:
      break;
    }
}

void APPL_HandleBcmControlCommands(LIN_Device_Frame_t const *frame)
{
	static uint32_t old_crc0 = 0xFFFFFFFF;
    static uint32_t old_crc1 = 0xFFFFFFFF;
	static uint32_t old_crc2 = 0xFFFFFFFF;
	BCM_ALU_CTRL *BCM_CMD = (BCM_ALU_CTRL *)((void*)frame->data);
	
	VIU_Sync = BCM_CMD->matrix.Conter;
	if(VIU_Sync==0xFF) VIU_SyncOverflow++;	
	CRC32_Init();
	uint32_t temp0 = CRC32_GetRunTimeCRC32(((void*)(&BCM_CMD->data[0])),LIN_BUFF_SIZE);
  /*
  if (temp0 != old_crc0)
  {
    old_crc0 = temp0;
    if (BCM_CMD->matrix.Flag == 1)
    {
      uint32_t temp1 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[0])), LIN_BUFF_SIZE - 4);
      uint32_t temp2 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[5])), LIN_BUFF_SIZE - 5);
      if (temp1 != old_crc1 || temp2 != old_crc2)
      {
        old_crc1 = temp1;
        old_crc2 = temp2;
        ALU_DATA = BCM_CMD;
        if (ALU_DATA->matrix.Contr == 0)
          ModeTemp = ModeCache;
        ModeCache = ALU_DATA->matrix.ModeContr;
        VIU_SyncInit = VIU_Sync;
        VIU_SyncOverflow = 0;
        ALU_ModeCount = 0;
        BcmControlCommandsDataHandle();
      }
    }
    if (ALU_DATA->matrix.Node_ModeContr == Mode4 && ALU_DATA->matrix.Node_Flag == 1)
    {
      // modify 2023.12.11  解决多色渐变熄灭后，再次调用多色渐变未初始化参数的缺陷
      multiColorTransitionCount = 0;
    }
    if (ALU_DATA->matrix.Contr == 1)
    {
      if (ALU_DATA->matrix.ModeContr >= Mode2 && ALU_DATA->matrix.ModeContr <= Mode4)
      {
        SyncJudge();
      }
    }
  }
  */
  if(ALU_DATA->matrix.ModeContr != Mode3)		// 2024.01.16 modify 关闭闪烁模式的CRC，以迎合音乐律动模式
	{
    if (temp0 != old_crc0)
    {
      old_crc0 = temp0;
      if (BCM_CMD->matrix.Flag == 1)
      {
        uint32_t temp1 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[0])), LIN_BUFF_SIZE - 4);
        uint32_t temp2 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[5])), LIN_BUFF_SIZE - 5);
        if (temp1 != old_crc1 || temp2 != old_crc2)
        {
          old_crc1 = temp1;
          old_crc2 = temp2;
          ALU_DATA = BCM_CMD;
          if (ALU_DATA->matrix.Contr == 0)
            ModeTemp = ModeCache;
          ModeCache = ALU_DATA->matrix.ModeContr;
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
    if (BCM_CMD->matrix.Flag == 1)
    {
      //	2024.01.16 modify 即使不开启闪烁模式的CRC拦截，也要刷新CRC校验位
      old_crc0 = 0xFFFFFFFF;
      uint32_t temp1 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[0])), LIN_BUFF_SIZE - 4);
      uint32_t temp2 = CRC32_GetRunTimeCRC32(((void *)(&BCM_CMD->data[5])), LIN_BUFF_SIZE - 5);
      old_crc1 = temp1;
      old_crc2 = temp2;
      
      ALU_DATA = BCM_CMD;
      if (ALU_DATA->matrix.Contr == 0)
        ModeTemp = ModeCache;
      ModeCache = ALU_DATA->matrix.ModeContr;
      VIU_SyncInit = VIU_Sync;
      VIU_SyncOverflow = 0;
      ALU_ModeCount = 0;
      BcmControlCommandsDataHandle();
    }
  }

    // modify 2023.12.11  解决多色渐变熄灭后，再次调用多色渐变未初始化参数的缺陷
		// modify 2024.01.15  修改if判断，每次多色渐变入参有效均将此变量置0，进度从头开始
		// modify 2024.01.24  新增变量，以区分多次唤醒和电竞模式重复有效入参，解决多次唤醒错误的导致multiColorTransitionCount多次置零，造成001212进程
  if((ALU_DATA->matrix.ModeContr == Mode4 && BCM_CMD->matrix.Flag == 0) || ALU_DATA->matrix.ModeContr != Mode4 /*|| 以及在休眠唤醒的那一刻*/)
	{
		Mode4IneffectiveFlag = true;
	}
	if(ALU_DATA->matrix.ModeContr == Mode4 && BCM_CMD->matrix.Flag == 1)
	{
		if(Mode4IneffectiveFlag == true)
		{
			multiColorTransitionCount = 0;		
			Mode4IneffectiveFlag = false;
		}
	}
    // if (ALU_DATA->matrix.ModeContr == Mode4 && BCM_CMD->matrix.Flag == 1)
    // {
    //   // modify 2023.12.11  解决多色渐变熄灭后，再次调用多色渐变未初始化参数的缺陷
    //   // modify 2024.01.15  修改if判断，每次多色渐变入参有效均将此变量置0，进度从头开始
    //   multiColorTransitionCount = 0;
    // }
    if (ALU_DATA->matrix.Contr == 1)
    {
      if (ALU_DATA->matrix.ModeContr >= Mode2 && ALU_DATA->matrix.ModeContr <= Mode4)
      {
        SyncJudge();
      }
    }
}

void BcmControlCommandsDataHandle()	//处理参数
{
	ModeParamInit();
	if(ALU_DATA->matrix.Contr==0)//开关关闭
	{
		if(ModeTemp==Mode1 || ModeTemp==Mode5)//渐灭
		{
			if((ALU_DATA->matrix.ColourContr1 >= ColourContrLowerLimit && ALU_DATA->matrix.ColourContr1 < ColourContrUpperLimit)/*颜色1*/
				&& (ALU_DATA->matrix.TimeContr >= TimeContrLowerLimit && ALU_DATA->matrix.TimeContr <= TimeContrUpperLimit)/*效果时间*/)
			{
                if(ALU_DATA->matrix.ColourCtr1Flg==1)
                {
                    ColorIndex1 = ColorIndexCalculate(ALU_DATA->matrix.ColourContr1);
                }
        if(ALU_DATA->matrix.ModeContr==Mode1)   // modify 2023.12.19 流水熄灭不响应效果控制时间
        {
          ModePeriod = ModePeriodCalculate(ALU_DATA->matrix.TimeContr); 
        }
        else 
        {
          ModePeriod = 0;
        }
				LedOnPorcess(ColorIndex1, 0, ModePeriod);
			}
		}
		else//立即熄灭
		{
			if(ALU_DATA->matrix.ColourContr1 >= ColourContrLowerLimit && ALU_DATA->matrix.ColourContr1 < ColourContrUpperLimit)/*颜色1*/
			{
                if(ALU_DATA->matrix.ColourCtr1Flg==1)
                {
                  ColorIndex1 = ColorIndexCalculate(ALU_DATA->matrix.ColourContr1);
                }
				LedOnPorcess(ColorIndex1, 0, 0);
			}
		}
	}
	else
	{
    if(ALU_DATA->matrix.ModeContr==Mode1) // modify 2023.12.19 细分常亮、渐暗
    {
      if((ALU_DATA->matrix.ColourContr1 >= ColourContrLowerLimit && ALU_DATA->matrix.ColourContr1 < ColourContrUpperLimit)/*颜色1*/
			   && (ALU_DATA->matrix.DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.DimLevel <= DimLevelUpperLimit)/*亮度*/
			   && (ALU_DATA->matrix.TimeContr >= TimeContrLowerLimit && ALU_DATA->matrix.TimeContr <= TimeContrUpperLimit)/*效果时间*/)
      {
        if(ALU_DATA->matrix.ColourCtr1Flg==1) // 常亮
        {
          ColorIndex1 = ColorIndexCalculate(ALU_DATA->matrix.ColourContr1);
          mode1Color1 = ColorIndex1;  // modify 2023.12.19
        }
        else                                  // 渐暗
        {
          ColorIndex1 = mode1Color1;  // modify 2023.12.19
        }
        Brightness = BrightnessCalculate(ALU_DATA->matrix.DimLevel);
        ModePeriod = ModePeriodCalculate(ALU_DATA->matrix.TimeContr);
        LedOnPorcess(ColorIndex1, Brightness, ModePeriod);
      }
    }
		else if((ALU_DATA->matrix.ModeContr>=Mode5  && ALU_DATA->matrix.ModeContr<=Mode7) ||
            (ALU_DATA->matrix.ModeContr==Mode11)                                      || 
            (ALU_DATA->matrix.ModeContr>=Mode13 && ALU_DATA->matrix.ModeContr<=Mode16))
		{
			if((ALU_DATA->matrix.ColourContr1 >= ColourContrLowerLimit && ALU_DATA->matrix.ColourContr1 < ColourContrUpperLimit)/*颜色1*/
			   && (ALU_DATA->matrix.DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.DimLevel <= DimLevelUpperLimit)/*亮度*/
			   && (ALU_DATA->matrix.TimeContr >= TimeContrLowerLimit && ALU_DATA->matrix.TimeContr <= TimeContrUpperLimit)/*效果时间*/)
			{
        if(ALU_DATA->matrix.ColourCtr1Flg==1)
        {
            ColorIndex1 = ColorIndexCalculate(ALU_DATA->matrix.ColourContr1);
            mode1Color1 = ColorIndex1;  // modify 2023.12.19
        }
				Brightness = BrightnessCalculate(ALU_DATA->matrix.DimLevel);
				ModePeriod = ModePeriodCalculate(ALU_DATA->matrix.TimeContr);
				LedOnPorcess(ColorIndex1, Brightness, ModePeriod);
			}
		}
    else if(ALU_DATA->matrix.ModeContr>=Mode8 && ALU_DATA->matrix.ModeContr<=Mode10)  // 语音 不响应效果时间参数与颜色参数
    {
      if(ALU_DATA->matrix.DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.DimLevel <= DimLevelUpperLimit)/*亮度*/
      {
        ColorIndex1 = 0;
        Brightness = BrightnessCalculate(ALU_DATA->matrix.DimLevel);
        LedOnPorcess(ColorIndex1, Brightness, ModePeriod);     
      }
    }
		else if(ALU_DATA->matrix.ModeContr==Mode2)//呼吸
		{
			if((ALU_DATA->matrix.ColourContr1 >= ColourContrLowerLimit && ALU_DATA->matrix.ColourContr1 < ColourContrUpperLimit)/*颜色1*/
			   && (ALU_DATA->matrix.DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.DimLevel <= DimLevelUpperLimit)/*亮度*/
			   && (ALU_DATA->matrix.TimeContr >= TimeContrLowerLimit && ALU_DATA->matrix.TimeContr <= TimeContrUpperLimit)/*效果时间*/
			   && (ALU_DATA->matrix.FrecyContr >= FrecyContrLowerLimit && ALU_DATA->matrix.FrecyContr <= FrecyContrUpperLimit)/*次数*/)
			{
                if(ALU_DATA->matrix.ColourCtr1Flg==1)
                {
                    ColorIndex1 = ColorIndexCalculate(ALU_DATA->matrix.ColourContr1);
                    mode1Color1 = ColorIndex1;  // modify 2023.12.19
                }
				Brightness = BrightnessCalculate(ALU_DATA->matrix.DimLevel);
				ModePeriod = ModePeriodCalculate(ALU_DATA->matrix.TimeContr);
				ModeTime = ModeTimeCalculate(ALU_DATA->matrix.FrecyContr);
			}
		}
		else if(ALU_DATA->matrix.ModeContr==Mode3)//闪烁
		{
			if((ALU_DATA->matrix.ColourContr1 >= ColourContrLowerLimit && ALU_DATA->matrix.ColourContr1 < ColourContrUpperLimit)/*颜色1*/
			   && (ALU_DATA->matrix.DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.DimLevel <= DimLevelUpperLimit)/*亮度*/
			   && (ALU_DATA->matrix.TimeContr >= TimeContrLowerLimit && ALU_DATA->matrix.TimeContr <= TimeContrUpperLimit)/*效果时间*/
			   && (ALU_DATA->matrix.FrecyContr >= FrecyContrLowerLimit && ALU_DATA->matrix.FrecyContr <= FrecyContrUpperLimit)/*次数*/)
			{
                if(ALU_DATA->matrix.ColourCtr1Flg==1)
                {
                    ColorIndex1 = ColorIndexCalculate(ALU_DATA->matrix.ColourContr1);
                    mode1Color1 = ColorIndex1;  // modify 2023.12.19
                }
				Brightness = BrightnessCalculate(ALU_DATA->matrix.DimLevel);
				ModePeriod = ModePeriodCalculate(ALU_DATA->matrix.TimeContr);
				ModeTime = ModeTimeCalculate(ALU_DATA->matrix.FrecyContr);
			}
		}
		else if(ALU_DATA->matrix.ModeContr==Mode4)//多色渐变
		{
			if((ALU_DATA->matrix.ColourContr1 >= ColourContrLowerLimit && ALU_DATA->matrix.ColourContr1 < ColourContrUpperLimit)/*颜色1*/
			   && (ALU_DATA->matrix.ColourContr2 >= ColourContrLowerLimit && ALU_DATA->matrix.ColourContr2 < ColourContrUpperLimit)/*颜色2*/
			   && (ALU_DATA->matrix.DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.DimLevel <= DimLevelUpperLimit)/*亮度*/
			   && (ALU_DATA->matrix.TimeContr >= TimeContrLowerLimit && ALU_DATA->matrix.TimeContr <= TimeContrUpperLimit)/*效果时间*/
			   && (ALU_DATA->matrix.FrecyContr >= FrecyContrLowerLimit && ALU_DATA->matrix.FrecyContr <= FrecyContrUpperLimit)/*次数*/)
			{
                if(ALU_DATA->matrix.ColourCtr1Flg==1)
                {
                    ColorIndex1 = ColorIndexCalculate(ALU_DATA->matrix.ColourContr1);
                }
                if(ALU_DATA->matrix.ColourCtr2Flg==1)
                {
                    ColorIndex2 = ColorIndexCalculate(ALU_DATA->matrix.ColourContr2);
                }
				Brightness = BrightnessCalculate(ALU_DATA->matrix.DimLevel);
				ModePeriod = ModePeriodCalculate(ALU_DATA->matrix.TimeContr);
				ModeTime = ModeTimeCalculate(ALU_DATA->matrix.FrecyContr);
			}
		}
    else if(ALU_DATA->matrix.ModeContr==Mode12) // 无序点光 modify 2023.12.19 不响应效果时间入参
    {
      if((ALU_DATA->matrix.ColourContr1 >= ColourContrLowerLimit && ALU_DATA->matrix.ColourContr1 < ColourContrUpperLimit)/*颜色1*/
        && (ALU_DATA->matrix.DimLevel >= DimLevelLowerLimit && ALU_DATA->matrix.DimLevel <= DimLevelUpperLimit))/*亮度*/
      {
        if(ALU_DATA->matrix.ColourCtr1Flg==1)
        {
          ColorIndex1 = ColorIndexCalculate(ALU_DATA->matrix.ColourContr1);
          mode1Color1 = ColorIndex1;  // modify 2023.12.19
        }
        Brightness = BrightnessCalculate(ALU_DATA->matrix.DimLevel);
        LedOnPorcess(ColorIndex1, Brightness, ModePeriod);
      }
    }
	}
}

void SyncJudge()	//按模式点灯
{
	if(ALU_DATA->matrix.ModeContr==Mode2)//呼吸
	{
		if(ALU_ModeCount<ModeTime || ModeTime==FrecyContrUpperLimit)	//判断循环次数
		{
			if((((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)<(ModePeriod) && (VIU_SyncInit==VIU_Sync)) || 
			   (((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)>=(ModePeriod) && ((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)%(ModePeriod)==0))//起始第一次 或 同步计数时长大于效果周期
			{
				LedOnPorcess(ColorIndex1, Brightness, ModePeriod/2);
			}
			else if(((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)>=(ModePeriod/2) && ((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)%(ModePeriod/2)==0)
			{
				LedOnPorcess(ColorIndex1, 0, ModePeriod/2);
				ALU_ModeCount++;
			}
		}
	}
	else if(ALU_DATA->matrix.ModeContr==Mode3)//闪烁
	{
		if(ALU_ModeCount<ModeTime || ModeTime==FrecyContrUpperLimit)
		{
			if((((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)<(ModePeriod) && (VIU_SyncInit==VIU_Sync)) || 
			   (((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)>=(ModePeriod) && ((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)%(ModePeriod)==0))//起始第一次 或 同步计数时长大于效果周期
			{
				LedOnPorcess(ColorIndex1, Brightness, 0);
			}
			else if(((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)>=(ModePeriod/2) && ((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)%(ModePeriod/2)==0)
			{
				LedOnPorcess(ColorIndex1, 0, 0);
				ALU_ModeCount++;
			}
		}
	}
	else if(ALU_DATA->matrix.ModeContr==Mode4)//多色渐变
	{
    /*
		if(ALU_ModeCount<ModeTime || ModeTime==FrecyContrUpperLimit)
		{
			if((((VIU_Sync+VIU_SyncOverflow*0xFF-VIU_SyncInit)*FramePeriod)<(ModePeriod) && (VIU_SyncInit==VIU_Sync)) || 
			   (((VIU_Sync+VIU_SyncOverflow*0xFF-VIU_SyncInit)*FramePeriod)>=(ModePeriod) && ((VIU_Sync+VIU_SyncOverflow*0xFF-VIU_SyncInit)*FramePeriod)%(ModePeriod)==0))//起始第一次 或 同步计数时长大于效果周期
			{
				LedOnPorcess(ColorIndex1, Brightness, ModePeriod/2);
			}
			else if(((VIU_Sync+VIU_SyncOverflow*0xFF-VIU_SyncInit)*FramePeriod)>=(ModePeriod/2) && ((VIU_Sync+VIU_SyncOverflow*0xFF-VIU_SyncInit)*FramePeriod)%(ModePeriod/2)==0)
			{
				LedOnPorcess(ColorIndex2, Brightness, ModePeriod/2);
				ALU_ModeCount++;
			}
		}
    */

    //modify 11.23
    if(ALU_ModeCount<ModeTime+1 || ModeTime==FrecyContrUpperLimit)
    {
      if((((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)<(ModePeriod) && (VIU_SyncInit==VIU_Sync)) || 
      (((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)>=(ModePeriod) && ((VIU_Sync+VIU_SyncOverflow*0x100-VIU_SyncInit)*FramePeriod)%(ModePeriod)==0))//起始第一次 或 同步计数时长大于效果周期
      { 
        ALU_ModeCount++;
        multiColorTransitionCount ++;
        if(multiColorTransitionCount > 2)
						multiColorTransitionCount = 1;
        if(multiColorTransitionCount == 1)
        {
          mode1Color1 = ColorIndex1;  // modify 2023.12.19
          LedOnPorcess(ColorIndex1, Brightness, ModePeriod);
        }
        else if(multiColorTransitionCount == 2)
        {
          mode1Color1 = ColorIndex2;  // modify 2023.12.19
          LedOnPorcess(ColorIndex2, Brightness, ModePeriod);
        }
            
      }
    }
	}
}

void LedOnPorcess(uint8_t ColorIndex, uint16_t Brightness, uint16_t transitionTime)
{
	float x=0.00,y=0.00;
    temperature.value[0] = 48;
    temperature.value[1] = 54;
    temperature.value[2] = 54;
	
		switch(ColorIndex)
		{
			case -1:
				x =0;
				y =0;
			break;

			default:
				x = xy_shift[ColorIndex].x;
				y = xy_shift[ColorIndex].y;
			break;
		}
	CLM_SetXYY(LED0, MES_GetLedTemperature(LED0), (uint16_t)((rgbTable[ColorIndex].Cx+x)*COLOR_MAX), (uint16_t)((rgbTable[ColorIndex].Cy+y)*COLOR_MAX), Brightness, transitionTime);

}

void ModeParamInit()
{
	ColorIndex1 = 0;
	ColorIndex2 = 0;
	Brightness = 0;
	ModePeriod = 0;
	ModeTime = 0;
}

uint8_t ColorIndexCalculate(uint8_t ColourContr)
{
	return ColourContr;		//颜色的索引[0-255]
}
uint16_t BrightnessCalculate(uint8_t DimLevel)
{
	return (DimLevel-1)*1023*Getlight_max_lv()/10000;
  // return (DimLevel-1)*1023*Getlight_max_lv()/2000;
}
uint16_t ModePeriodCalculate(uint8_t TimeContr)
{
	return (TimeContr-1)*200;
}
uint8_t ModeTimeCalculate(uint8_t FrecyContr)
{
	return FrecyContr;
}

void APPL_HandleColorControlCommands(LIN_Device_Frame_t const *frame)
{
    ColorCtrlFrame_t const *color = (ColorCtrlFrame_t const*)((void const*)frame->data);
    if ( (color->nodeAddress == ls_read_nad()) || (color->nodeAddress == LIN_NAD_WILDCARD)){
        if (color->channel < LED_NUM){
            LedNum_t ledIndex = (LedNum_t)color->channel;
            switch(color->command){
            case CMD_COLOR_CTRL_XYY:
              (void)CLM_SetXYY(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorXY.x, color->ColorXY.y, (uint16_t)color->ColorXY.Y*10U, (uint16_t)color->ColorXY.transitionTime*100U);
              break;
            case CMD_COLOR_CTRL_XYY_EXT:
              (void)CLM_SetXYY(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorXY_EXT.x, color->ColorXY_EXT.y, (uint16_t)color->ColorXY_EXT.Y, 0U);
              break;
            case CMD_COLOR_CTRL_HSL:
              (void)CLM_SetHSL(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorHSL.hue, color->ColorHSL.saturation, (uint16_t)color->ColorHSL.level*10U, (uint16_t)color->ColorHSL.transitionTime*100U);
              break;
            case CMD_COLOR_CTRL_RGBL:
              if (color->ColorRGBL.level > 0U){
                  LED_State = 1U;
              }else{
                  LED_State = 0U;
              }
              (void)CLM_SetRGBL(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorRGBL.red, color->ColorRGBL.green, color->ColorRGBL.blue,(uint16_t)color->ColorRGBL.level*10U, (uint16_t)color->ColorRGBL.transitionTime*100U);
              break;
            case CMD_COLOR_CTRL_RGB:
              (void)CLM_SetRGB(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorRGB.red, color->ColorRGB.green, color->ColorRGB.blue, (uint16_t)color->ColorRGB.transitionTime*100U);
              break;
            case CMD_COLOR_CTRL_PWM:
              if (color->ColorPWM.red == 255U && color->ColorPWM.green == 255U && color->ColorPWM.blue == 255U){
                  (void)CLM_SetPWMs(ledIndex,65535U, 65535U, 65535U, (uint16_t)color->ColorPWM.transitionTime*100U);
              }else{
                  (void)CLM_SetPWMs(ledIndex,(uint16_t)color->ColorPWM.red*256U, (uint16_t)color->ColorPWM.green*256U, (uint16_t)color->ColorPWM.blue*256U, (uint16_t)color->ColorPWM.transitionTime*100U);
              }
              break;
            case CMD_COLOR_CTRL_LUV:
              (void)CLM_SetLUV(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorLUV.u, color->ColorLUV.v, (uint16_t)color->ColorLUV.level*10U, (uint16_t)color->ColorLUV.transitionTime*100U);
              break;
              
            case CMD_COLOR_CTRL_XY_ABS_Y:
              (void)CLM_SetXYAbsY(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorXY_Intensity.x, color->ColorXY_Intensity.y, color->ColorXY_Intensity.intensity, 0U);
              break;
            case CMD_COLOR_CTRL_ABS_L_UV:
              (void)CLM_SetAbsLUV(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorIntensity_UV.u, color->ColorIntensity_UV.v, color->ColorIntensity_UV.intensity, 0U);
              break;
            case CMD_COLOR_CTRL_sRGB_ABS_L:
              (void)CLM_SetSRGBAbsL(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorSRGB_Intensity.red, color->ColorSRGB_Intensity.green, color->ColorSRGB_Intensity.blue,color->ColorSRGB_Intensity.intensity, 0U);
              break;
            default:
              break;
            }
        }
    }
}

uint8_t APPL_HandleLedParamConfigRequest(uint8_t reqSid,uint8_t *const dataBuff,DiagRspInfo_t *const diagRsp)
{
    uint8_t response = FALSE;
    CommLedParamInfo_t *const info    = (CommLedParamInfo_t *)((void *)dataBuff);
    CommLedParamInfo_t *const infoRsp = (CommLedParamInfo_t *)((void *)diagRsp->payload);

    /* handle response data here  */
    diagRsp->type = PDU_TYPE_SINGLE_PDU;
    diagRsp->sid = reqSid;
    diagRsp->packLength = SF_MAX_DATA_LENGTH;   /* Valid data length not includes sid*/
    diagRsp->payload[0] = 0xFFU;                /* failed   */
    
    infoRsp->command = info->command;
    switch(info->command){
    case APP_DIAG_SET_LED_PHY_PARAM: 
      {
      LedcolorParam_t ledColorParam = info->ledPhysicalParam.colorParam;
      ColorTemperature_t temperature;
      temperature.value[0] = info->ledPhysicalParam.temperature[0];
      temperature.value[1] = info->ledPhysicalParam.temperature[1];
      temperature.value[2] = info->ledPhysicalParam.temperature[2];
      
      if (info->ledPhysicalParam.ledIndex <= (LedNum_t)(LED_NUM -1U)){
          (void)CLM_SetLedPhyParams((LedNum_t)info->ledPhysicalParam.ledIndex,temperature,&ledColorParam);
          diagRsp->payload[0] = 0x00U;
      }
      response = TRUE;/* response enable  */
      break;
      }
    case APP_DIAG_SET_LED_GENERAL_PARAM:
      {
      Coordinate_t    whitePoint = info->ledGeneralParam.whitePoint;
      uint16_t minIntensity[3];
      minIntensity[0] = info->ledGeneralParam.minIntensity[0];
      minIntensity[1] = info->ledGeneralParam.minIntensity[1];
      minIntensity[2] = info->ledGeneralParam.minIntensity[2];
      (void)CLM_SetWhitePointParams(&whitePoint);
      (void)CLM_SetMinimumIntensity(minIntensity[0],minIntensity[1],minIntensity[2]);
      diagRsp->payload[0] = 0x00U;
      response = TRUE;/* response enable  */
      break;
      }
    case APP_DIAG_SET_LED_TYPICAL_PN_VOLT: 
      {
      int16_t volt_R,volt_G,volt_B;
      ColorTemperature_t temperature;
      volt_R = info->ledTypicalPNVolt.ledTypicalPNVolt[0];
      volt_G = info->ledTypicalPNVolt.ledTypicalPNVolt[1];
      volt_B = info->ledTypicalPNVolt.ledTypicalPNVolt[2];
      temperature.value[0] = info->ledTypicalPNVolt.temperature;
      temperature.value[1] = info->ledTypicalPNVolt.temperature;
      temperature.value[2] = info->ledTypicalPNVolt.temperature;
      (void)CLM_SetLedPNVolts((LedNum_t)info->ledTypicalPNVolt.ledIndex,temperature, volt_R, volt_G, volt_B);
      diagRsp->payload[0] = 0x00U;
      response = TRUE;/* response enable  */
      break;
      }
    default:
      break;
    }
    return response;
}

uint8_t APPL_PrepareLedParamRequest(uint8_t reqSid,uint8_t *const dataBuff, DiagRspInfo_t *const diagRsp)
{
    uint8_t response = FALSE;
    CommLedParamInfo_t *const info    = (CommLedParamInfo_t *)((void *)dataBuff);
    CommLedParamInfo_t *const infoRsp = (CommLedParamInfo_t *)((void *)diagRsp->payload);

    diagRsp->sid = reqSid;
    infoRsp->command = info->command;
    switch(info->command){
    case APP_DIAG_GET_LED_PHY_PARAM:
      {
      LedcolorParam_t ledColorParam;
      ColorTemperature_t temperature;
      temperature.value[0] = info->ledPhysicalParam.temperature[0];
      temperature.value[1] = info->ledPhysicalParam.temperature[1];
      temperature.value[2] = info->ledPhysicalParam.temperature[2];
      if (info->ledPhysicalParam.ledIndex <= (LedNum_t)(LED_NUM -1U)){
          (void)CLM_GetLedPhyParams((LedNum_t)info->ledPhysicalParam.ledIndex,temperature,&ledColorParam);
          infoRsp->ledPhysicalParam.colorParam      = ledColorParam;
          infoRsp->ledPhysicalParam.ledIndex        = info->ledPhysicalParam.ledIndex;
          infoRsp->ledPhysicalParam.temperature[0]  = info->ledPhysicalParam.temperature[0];
          infoRsp->ledPhysicalParam.temperature[1]  = info->ledPhysicalParam.temperature[1];
          infoRsp->ledPhysicalParam.temperature[2]  = info->ledPhysicalParam.temperature[2];
          
          diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedPhysicalParam_t);
          diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
          response = TRUE;/* response enable  */
      }
      break;
      }
    case APP_DIAG_GET_LED_GENERAL_PARAM:
      {
      Coordinate_t    whitePoint;
      uint16_t minIntensity[3];
      (void)CLM_GetWhitePointParams(&whitePoint);
      (void)CLM_GetMinimumIntensity(&minIntensity[0],&minIntensity[1],&minIntensity[2]);
      infoRsp->ledGeneralParam.minIntensity[0] = minIntensity[0];
      infoRsp->ledGeneralParam.minIntensity[1] = minIntensity[1];
      infoRsp->ledGeneralParam.minIntensity[2] = minIntensity[2];
      infoRsp->ledGeneralParam.whitePoint = whitePoint;
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedGeneralParam_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      response = TRUE;/* response enable  */
      break;
      }
    case APP_DIAG_GET_LED_TYPICAL_PN_VOLT:
      {
      int16_t volt_R,volt_G,volt_B;
      ColorTemperature_t temperature;
      temperature.value[0] = info->ledTypicalPNVolt.temperature;
      temperature.value[1] = info->ledTypicalPNVolt.temperature;
      temperature.value[2] = info->ledTypicalPNVolt.temperature;
      (void)CLM_GetLedPNVolts((LedNum_t)info->ledTypicalPNVolt.ledIndex,temperature, &volt_R, &volt_G, &volt_B);
      infoRsp->ledTypicalPNVolt.ledIndex    = info->ledTypicalPNVolt.ledIndex;
      infoRsp->ledTypicalPNVolt.temperature = info->ledTypicalPNVolt.temperature;
      infoRsp->ledTypicalPNVolt.ledTypicalPNVolt[0] = volt_R;
      infoRsp->ledTypicalPNVolt.ledTypicalPNVolt[1] = volt_G;
      infoRsp->ledTypicalPNVolt.ledTypicalPNVolt[2] = volt_B;
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedTypicalPNVolt_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      response = TRUE;/* response enable  */
      break;
      }
    case APP_DIAG_GET_LED_RUN_TIME_INFO:
      {
      uint8_t ledNo;
      uint16_t battVolt,buckVolt = 0;
      int16_t chipTemperature;
      ColorTemperature_t temperature;
      int16_t sum;
      (void)MES_GetBatteryVolt(&battVolt);
      (void)MES_GetChipTemperature(&chipTemperature);
      
      infoRsp->ledRunTimeParam.battVolt = battVolt;
      infoRsp->ledRunTimeParam.buckVolt = buckVolt;
      infoRsp->ledRunTimeParam.chipTemp = chipTemperature;
      infoRsp->ledRunTimeParam.ledNum   = LED_NUM;
      for (ledNo = 0U; ledNo < LED_NUM; ledNo++){
        temperature = MES_GetLedTemperature((LedNum_t)ledNo);
        sum = (int16_t)temperature.value[0] + (int16_t)temperature.value[1] + (int16_t)temperature.value[1] + (int16_t)temperature.value[2];
        infoRsp->ledRunTimeParam.ledTemperature[ledNo] = (int16_t)(sum/4);
      }
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedRunTimeParam_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      response = TRUE;/* response enable  */
      break;
      }
    case APP_DIAG_GET_LED_RGB_RUN_TEMP:
      {
        int8_t rTemp,gTemp,bTemp;
        if ((uint8_t)info->LedRunTimePNTemp.ledIndex < LED_NUM ){
            (void)MES_GetLedRGBTemperature(info->LedRunTimePNTemp.ledIndex, &rTemp,&gTemp,&bTemp);
            infoRsp->LedRunTimePNTemp.ledIndex = info->LedRunTimePNTemp.ledIndex;
            infoRsp->LedRunTimePNTemp.ledTemp[0] = rTemp;
            infoRsp->LedRunTimePNTemp.ledTemp[1] = gTemp;
            infoRsp->LedRunTimePNTemp.ledTemp[2] = bTemp;
            diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedRunTimePNTemp_t);
            diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
            response = TRUE;/* response enable  */
        }
        break;
      }
      
    case APP_DIAG_GET_LED_STATIC_PN_VOLT:
      {
      /* force to measure PN volt when Led is off for getting initialized PN voltage */
      uint16_t volt_R,volt_G,volt_B;
      MES_MnftGetLedInfo(info->ledStaticPNVolt.ledIndex, &volt_R,&volt_G,&volt_B);
      infoRsp->ledStaticPNVolt.ledIndex = info->ledStaticPNVolt.ledIndex;
      infoRsp->ledStaticPNVolt.ledPNVolt[0] = volt_R;
      infoRsp->ledStaticPNVolt.ledPNVolt[1] = volt_G;
      infoRsp->ledStaticPNVolt.ledPNVolt[2] = volt_B;
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedStaticPNVolt_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      response = TRUE;/* response enable  */
      break;
      }
      
    case APP_DIAG_GET_LED_RGB_RUN_VOLT:
      {
        uint16_t volt_R,volt_G,volt_B;
        uint16_t battVolt;
        int16_t chipTemperature;
        if ((uint8_t)info->LedRunTimePNTemp.ledIndex < LED_NUM ){
          /* force to measure PN volt when Led is off for getting initialized PN voltage */
          MES_GetRunTimeLedPNVolt(info->ledRunTimePNVolt.ledIndex,&volt_R, &volt_G, &volt_B);
          infoRsp->ledRunTimePNVolt.ledIndex = info->ledRunTimePNVolt.ledIndex;
          infoRsp->ledRunTimePNVolt.ledPNVolt[0] = volt_R;
          infoRsp->ledRunTimePNVolt.ledPNVolt[1] = volt_G;
          infoRsp->ledRunTimePNVolt.ledPNVolt[2] = volt_B;
          
          (void)MES_GetBatteryVolt(&battVolt);
          (void)MES_GetChipTemperature(&chipTemperature);
          
          infoRsp->ledRunTimePNVolt.battVolt = battVolt;
          infoRsp->ledRunTimePNVolt.chipTemp = chipTemperature;
          
          diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedRunTimePNVolt_t);
          diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
          response = TRUE;/* response enable  */
        }
        break;
      }
    case APP_DIAG_GET_SDK_VERSION:
      infoRsp->Version.sdkApplication   = SDK_APPL_VERSION;
      infoRsp->Version.linStack         = ls_get_lin_version();
      infoRsp->Version.ledLibrary       = CLM_GetColorLibraryVersion();
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommVersion_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      response = TRUE;
      break;
    default:
      response = FALSE;
      break;
    }
    return response;
}

static void Led_InitParam(void)
{
	ColorTemperature_t temperature;
	temperature.value[0] = ROOM_TEMPERATURE;
	temperature.value[1] = ROOM_TEMPERATURE;
	temperature.value[2] = ROOM_TEMPERATURE;
	CLM_SetFadingMode(FADING_MODE_DERIVATIVE);
	
	  if(PDS_GetPdsStatus() == PDS_STATUS_USING_DEFAULT)
	  {
			  uint16_t MAX_Intensity_R = DEFAULT_MAX_INTENSITY_R;
			  uint16_t MAX_Intensity_G = DEFAULT_MAX_INTENSITY_G;
			  uint16_t MAX_Intensity_B = DEFAULT_MAX_INTENSITY_B;
			  uint16_t r_pn_vol = DEFAULT_LED_PN_VOLT_R;
			  uint16_t g_pn_vol = DEFAULT_LED_PN_VOLT_G;
			  uint16_t b_pn_vol = DEFAULT_LED_PN_VOLT_B;
			  
			  Coordinate_t red ={
				.x = DEFAULT_COORDINATE_R_X,
				.y = DEFAULT_COORDINATE_R_Y
				};
			  Coordinate_t green ={
				.x = DEFAULT_COORDINATE_G_X,
				.y = DEFAULT_COORDINATE_G_Y
				};
			  Coordinate_t blue ={
				.x = DEFAULT_COORDINATE_B_X,
				.y = DEFAULT_COORDINATE_B_Y
				};

			
	     #ifdef  VE_2_AR_5_TG_P     //橙
           #if(PCBAType == CD701_ZJD)
	            red.x = 0xB202;     red.y = 0x4DEB;
	            green.x = 0x29F8;   green.y = 0xBF21;
	            blue.x = 0x2433;    blue.y = 0x0F97;

	            MAX_Intensity_R = 0x042C;
	            MAX_Intensity_G = 0x08BF;
	            MAX_Intensity_B = 0x01D6;

	            r_pn_vol = 0x0733;
	            g_pn_vol = 0x0912;
	            b_pn_vol = 0x0A00;
			#endif
           #if(PCBAType == E12_LED2)
	            red.x = 0xB208;     red.y = 0x4DC3;
	            green.x = 0x2AA4;   green.y = 0xBD55;
	            blue.x = 0x22E6;    blue.y = 0x0ED1;

	            MAX_Intensity_R = 0x0410;
	            MAX_Intensity_G = 0x07D9;
	            MAX_Intensity_B = 0x01CC;

	            r_pn_vol = 0x072F;
	            g_pn_vol = 0x0923;
	            b_pn_vol = 0x09F5;
			#endif
		#endif
		
			Coordinate_t whitepoint = {0x5555, 0x5555};
			LedcolorParam_t ledColorParam;
			ledColorParam.color[0] = red;
			ledColorParam.color[1] = green;
			ledColorParam.color[2] = blue;
			ledColorParam.maxTypicalIntensity[0] = MAX_Intensity_R;
			ledColorParam.maxTypicalIntensity[1] = MAX_Intensity_G;
			ledColorParam.maxTypicalIntensity[2] = MAX_Intensity_B;
		
			for(uint8_t LEDIndex=0;LEDIndex<LED_NUM;LEDIndex++)
			{
				CLM_SetLedPhyParams((LedNum_t)LEDIndex, temperature, &ledColorParam);
				CLM_SetLedPNVolts((LedNum_t)LEDIndex, temperature, r_pn_vol, g_pn_vol, b_pn_vol);
			}
		  CLM_SetWhitePointParams(&whitepoint);
		  CLM_SetMinimumIntensity((uint16_t)(MAX_Intensity_R * (float)65 / 100), (uint16_t)(MAX_Intensity_G * (float)95 / 100), (uint16_t)(MAX_Intensity_B * (float)95 / 100)); 
		  TM_PostTask(TASK_ID_PDS);
	  }

}


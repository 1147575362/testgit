#ifndef __MYSIMPLERUNNING_H__
#define __MYSIMPLERUNNING_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

// 灯光流动方向
enum dir
{
	Left = 1,
	Right,
	Close,
	Diffuse
};

// 设置高低电平有效
enum Active
{
    High = 0,
    Low
};



/*
	StackedBrickLamp:沿左右两个方向叠砖式移动bits  (大端模式)
		data[]:数组元素个数范围，元素类型uint8_t
        size:数组的大小，传sizeof即可
		led_nums:灯珠数量，需要比数组的bit数小
        dir:移动方向
        times:移动次数
		bits_lens:以多长的bit串为单位移动
        Act:High为高电平有效，low为低电平有效
*/
uint8_t simplerunning(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times, uint8_t bits_lens, uint8_t Act);



uint8_t FlowingLamp(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times, uint8_t Act);

uint8_t VoiceLamp(uint8_t data[], uint8_t size, uint8_t offset_Left, uint8_t offset_Right, uint8_t max_len_left, uint8_t max_len_right);

uint8_t VoiceLampLeft(uint8_t data[], uint8_t size, uint8_t offset, uint8_t max_len, bool rise_flag);

uint8_t VoiceLampRight(uint8_t data[], uint8_t size, uint8_t offset, uint8_t max_len, bool rise_flag);

#endif

#ifndef __MY_DASHEDLINE_H__
#define __MY_DASHEDLINE_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>


#define buf_size	(13)


extern uint8_t data[buf_size];



/**********************************100LED*******************************/

// 灯光流动方向

//100颗LED的联合体
/*
typedef union 
{
    struct
    {
        uint16_t group_1:                   11;
        uint16_t group_2:                   11;
        uint16_t group_3:                   11;
        uint16_t group_4:                   11;
        uint16_t group_5:                   11;
        uint16_t group_6:                   11;
        uint16_t group_7:                   11;
        uint16_t group_8:                   11;
        uint16_t group_9:                   12;
        uint16_t        :                    4;
    } group;
    uint8_t data[13];
} Led_Data_t;
*/

/*
	printf2:调试显示用，打印二进制
		data:uint_8类型的数组
		size:数组的大小，传sizeof即可
*/
void printf2(uint8_t data[], uint8_t size);

/*
	printf2:调试显示用，打印二进制
		n:一个uint16_t的数据
*/
void printf2_16(uint16_t n);

/*
	DashedLineSwimming:虚线游动
		data:uint_8类型的数组
		size:数组的大小，传sizeof即可
        led_nums:传100或？？？
        dir:游动方向
        times:次数，若led_nums传入100则是1~22之间循环，不到达23
*/
bool DashedLineSwimming(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times);

/*
	u16Swimming_odd:奇数组虚线游动
        x:u16类型的变量，用于存储11颗/12颗led的变化
        dir:游动方向
        times:次数，若led_nums传入100则是1~22之间循环，不到达23
        @return:改变后的x
*/
uint16_t u16Swimming_odd(uint16_t x, uint8_t dir, uint8_t times);

/*
	u16Swimming_even:偶数组虚线游动
        x:u16类型的变量，用于存储11颗/12颗led的变化
        dir:游动方向
        times:次数，若led_nums传入100则是1~22之间循环，不到达23
        @return:改变后的x
*/
uint16_t u16Swimming_even(uint16_t x, uint8_t dir, uint8_t times);


/**********************************32/36LED*******************************/


// // 32颗LED用此联合体
// typedef union 
// {
//     struct
//     {
//         uint16_t group_1:                  4;
//         uint16_t group_2:                  4;
//         uint16_t group_3:                  4;
//         uint16_t group_4:                  4;
//         uint16_t group_5:                  4;
//         uint16_t group_6:                  4;
//         uint16_t group_7:                  4;
//         uint16_t group_8:                  4;
//     } group;
//     uint8_t data[4];
// } Led_Data_32_t;

// 34颗LED用此联合体
typedef union 
{
    struct
    {
        uint16_t group_1:                  5;
        uint16_t group_2:                  4;
        uint16_t group_3:                  4;
        uint16_t group_4:                  4;
        uint16_t group_5:                  4;
        uint16_t group_6:                  4;
        uint16_t group_7:                  4;
        uint16_t group_8:                  5;
    } group;
    uint8_t data[5];
} Led_Data_34_t;

/*
// 36颗LED用此联合体
typedef union 
{
    struct
    {
        uint16_t group_1:                  5;
        uint16_t group_2:                  5;
        uint16_t group_3:                  4;
        uint16_t group_4:                  4;
        uint16_t group_5:                  4;
        uint16_t group_6:                  4;
        uint16_t group_7:                  5;
        uint16_t group_8:                  5;
		uint16_t Empty  : 				   4;
    } group;
    uint8_t data[5];
} Led_Data_36_t;
*/
/*
	DashedLineSwimming_side:虚线游动
		data:uint_8类型的数组
		size:数组的大小，传sizeof即可
        led_nums:传32或36
        dir:游动方向
        times:次数，1~8之间循环，不到达9
*/
bool DashedLineSwimming_side(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times);

/*
	u16Swimming_odd_side:奇数组虚线游动 (不可调用)
        x:u16类型的变量，用于存储4颗/5颗led的变化
        led_nums:led颗数,传入32/36
        dir:游动方向
        times:次数，若led_nums传入100则是1~8之间循环，不到达9
        @return:改变后的x
*/
uint16_t u16Swimming_odd_side(uint16_t x, uint8_t led_nums, uint8_t dir, uint8_t times);

/*
	u16Swimming_even_side:偶数组虚线游动 (不可调用)
        x:u16类型的变量，用于存储4颗/5颗led的变化
        led_nums:led颗数,传入32/36
        dir:游动方向
        times:次数，若led_nums传入100则是1~8之间循环，不到达9
        @return:改变后的x
*/
uint16_t u16Swimming_even_side(uint16_t x, uint8_t led_nums, uint8_t dir, uint8_t times);

/*----------------------------------------------------------------------------------------------------------------------------*/

/*
	DashedLineMovement_side:虚线平移
		data:uint_8类型的数组
		size:数组的大小，传sizeof即可
        led_nums:传32/36
        dir:游动方向
         times:次数，若led_nums传入100则是1~8之间循环，不到达9
*/

 bool DashedLineMovement_side(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times);

/*
	u16Movement_odd_side:奇数组虚线平移 (不可调用)
        x:u16类型的变量，用于存储4颗/5颗led的变化
        led_nums:led颗数,传入32/36
        dir:游动方向
         times:次数，若led_nums传入100则是1~8之间循环，不到达9
        @return:改变后的x
*/
 uint16_t u16Movement_odd_side(uint16_t x, uint8_t led_nums, uint8_t dir, uint8_t times);

/*
	u16Movement_even_side:偶数组虚线平移 (不可调用)
        x:u16类型的变量，用于存储4颗/5颗led的变化
        led_nums:led颗数,传入32/36
        dir:游动方向
         times:次数，若led_nums传入100则是1~8之间循环，不到达9
        @return:改变后的x
*/
uint16_t u16Movement_even_side(uint16_t x, uint8_t led_nums, uint8_t dir, uint8_t times);

/*----------------------------------------------------------------------------------------------------------------------------*/


/*
	DashedStep_side:踏步平移
		data:uint_8类型的数组
		size:数组的大小，传sizeof即可
        led_nums:led颗数,传入32/36
        dir:游动方向
        times:次数，dir=left\right则是1~9之间循环，不到达10
                    dir=close\diffuse则是1~5之间循环，不到达6
        @备注：若是虚线依次游动，则调用处实参times改成 i*2-1
*/
bool DashedStep_side(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times);

/*
	DashedStep_lightup:虚线依次点亮
		data:uint_8类型的数组
		size:数组的大小，传sizeof即可
        led_nums:led颗数,传入32/36
        dir:游动方向
        times:次数，dir=left\right则是1~5之间循环，不到达6
                    dir=close\diffuse则是1~3之间循环，不到达4
*/

bool DashedStep_lightup(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times);	//虚线依次点亮

/*----------------------------------------------------------------------------------------------------------------------------*/

/*
	set_empty:将联合体里的数据清零
        led_nums:led颗数,传入32/36
*/

void set_empty(uint8_t led_nums);


/*
	Pushing_Data_From_union:将联合体里的数据写入全局变量Data中
        led_nums:led颗数,传入32/36
*/
void Pushing_Data_From_union(uint8_t led_nums);



#endif

#include "My_DashedLine.h"
Led_Data_t LedData;
bool group_9_flag;  // 因第九组有12bits 所以需要特殊标注处理 
//                  0     1    2     3     4      5    6      7    8     9     10    11    12
//uint8_t data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 仅用于调试 输出二进制 只能输入char类型 其他类型要修改
/*
void printf2(uint8_t data[], uint8_t size)
{
    uint8_t i = 0, j = 0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < 8; j++)
            if (data[i] & (0x80 >> j))
                // printf("1");
                printf("\033[41m1\033[0m");
            else
                printf("0");
        printf("");
    }
    putchar('\n');
}

// 仅用于调试 输出二进制 只能输入uint16_t类型 其他类型要修改
void printf2_16(uint16_t n)
{
    uint8_t i = 0;
    for (i = 0; i < 16; i++)
        if (n & (0x8000 >> i))
            // printf("1");
            printf("\033[41m1\033[0m");
        else
            printf("0");
    printf("");
}
*/
bool DashedLineSwimming(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times)
{
    if (data == NULL)
        return 0;
    if (led_nums > size * 8)
        return 0;

    if (led_nums == 100)
    {
        // 1~8组11bits 9组12bits 从数组中依次取出
        // LedData.group.group_1 = (data[0] << 3) | (data[1] >> 5);
        // LedData.group.group_2 = ((data[2] & 0xfc) >> 2) | ((data[1] & 0x1f) << 6);
        // LedData.group.group_3 = ((data[2] & 0x03) << 9) | (data[3] << 1) | (data[4] >> 7);
        // LedData.group.group_4 = ((data[4] & 0x7f) << 4) | (data[5] >> 4);
        // LedData.group.group_5 = ((data[5] & 0x0f) << 7) | (data[6] >> 1);
        // LedData.group.group_6 = ((data[6] & 0x01) << 10) | (data[7] << 2) | (data[8] >> 6);
        // LedData.group.group_7 = ((data[8] & 0x3f) << 5) | (data[9] >> 3);
        // LedData.group.group_8 = ((data[9] & 0x07) << 8) | (data[10]);
        // LedData.group.group_9 = ((data[11]) << 4) | (data[12] >> 4);

        if(dir == Left || dir == Right)
        {
            // return的是uint16_t类型的数据 用11位类型的数据接收 直接截断低11位
            LedData.group.group_1 = u16Swimming_odd(LedData.group.group_1, dir, times);
            LedData.group.group_2 = u16Swimming_even(LedData.group.group_2, dir, times);
            LedData.group.group_3 = u16Swimming_odd(LedData.group.group_3, dir, times);
            LedData.group.group_4 = u16Swimming_even(LedData.group.group_4, dir, times);
            LedData.group.group_5 = u16Swimming_odd(LedData.group.group_5, dir, times);
            LedData.group.group_6 = u16Swimming_even(LedData.group.group_6, dir, times);
            LedData.group.group_7 = u16Swimming_odd(LedData.group.group_7, dir, times);
            LedData.group.group_8 = u16Swimming_even(LedData.group.group_8, dir, times);
            group_9_flag = 1;
            LedData.group.group_9 = u16Swimming_odd(LedData.group.group_9, dir, times); // 第9组有12颗灯 所以需要特殊处理以保持与其他组同步
            group_9_flag = 0;
        }
        else if(dir == Close)
        {
            //左侧
            LedData.group.group_1 = u16Swimming_odd(LedData.group.group_1, Right, times);
            LedData.group.group_2 = u16Swimming_even(LedData.group.group_2, Right, times);
            LedData.group.group_3 = u16Swimming_odd(LedData.group.group_3, Right, times);
            LedData.group.group_4 = u16Swimming_even(LedData.group.group_4, Right, times);
            //中间
            LedData.group.group_5 = u16Swimming_mid(LedData.group.group_5, Close, times);
            //右侧
            LedData.group.group_6 = u16Swimming_even(LedData.group.group_6, Left, times);
            LedData.group.group_7 = u16Swimming_odd(LedData.group.group_7, Left, times);
            LedData.group.group_8 = u16Swimming_even(LedData.group.group_8, Left, times);
            group_9_flag = 1;
            LedData.group.group_9 = u16Swimming_odd(LedData.group.group_9, Left, times); // 第9组有12颗灯 所以需要特殊处理以保持与其他组同步
            group_9_flag = 0;
        }
        else if(dir == Diffuse)
        {
            //左侧
            LedData.group.group_1 = u16Swimming_odd(LedData.group.group_1, Left, times);
            LedData.group.group_2 = u16Swimming_even(LedData.group.group_2, Left, times);
            LedData.group.group_3 = u16Swimming_odd(LedData.group.group_3, Left, times);
            LedData.group.group_4 = u16Swimming_even(LedData.group.group_4, Left, times);
            //中间
            LedData.group.group_5 = u16Swimming_mid(LedData.group.group_5, Diffuse, times);
            //右侧
            LedData.group.group_6 = u16Swimming_even(LedData.group.group_6, Right, times);
            LedData.group.group_7 = u16Swimming_odd(LedData.group.group_7, Right, times);
            LedData.group.group_8 = u16Swimming_even(LedData.group.group_8, Right, times);
            group_9_flag = 1;
            LedData.group.group_9 = u16Swimming_odd(LedData.group.group_9, Right, times); // 第9组有12颗灯 所以需要特殊处理以保持与其他组同步
            group_9_flag = 0;
        }
        else;
		if(times>= 23)
		{
			for(uint8_t i = 0; i < BUFF_SIZE; i++)
			{
				LedData.data[i] = 0;
			}
				LedData.group.group_1 = 0;
				LedData.group.group_2 = 0;
				LedData.group.group_3 = 0;
				LedData.group.group_4 = 0;
				LedData.group.group_5 = 0;
				LedData.group.group_6 = 0;
				LedData.group.group_7 = 0;
				LedData.group.group_8 = 0;
				LedData.group.group_9 = 0;			
		}
        // 联合体自动写入有bit位错位 现手动写回
        data[0] = LedData.group.group_1 >> 3;
        data[1] = (LedData.group.group_1 & 0x07) << 5;
        data[1] |= LedData.group.group_2 >> 6;
        data[2] = (LedData.group.group_2 & 0x3f) << 2;
        data[2] |= LedData.group.group_3 >> 9;
        data[3] = (LedData.group.group_3 & 0x1fe) >> 1;
        data[4] = (LedData.group.group_3 & 0x01) << 7;
        data[4] |= LedData.group.group_4 >> 4;
        data[5] = (LedData.group.group_4 & 0x0f) << 4;
        data[5] |= LedData.group.group_5 >> 7;
        data[6] = (LedData.group.group_5 & 0x7f) << 1;
        data[6] |= LedData.group.group_6 >> 10;
        data[7] = (LedData.group.group_6 & 0x3fc) >> 2;
        data[8] = (LedData.group.group_6 & 0x03) << 6;
        data[8] |= LedData.group.group_7 >> 5;
        data[9] = (LedData.group.group_7 & 0x1f) << 3;
        data[9] |= LedData.group.group_8 >> 8;
        data[10] = LedData.group.group_8 & 0xff;
        data[11] = LedData.group.group_9 >> 4;
        data[12] = (LedData.group.group_9 & 0x0f) << 4;
    }
}

uint16_t u16Swimming_odd(uint16_t x, uint8_t dir, uint8_t times)
{
    switch (dir)
    {
    case Left:  //  左←————右
        if(group_9_flag == 0)
        {
            for (uint8_t i = 0; i < times; i++)     //  点亮阶段
                x |= 0x01 << i;
            if (times >= 11)                        //  熄灭阶段
                for (uint8_t j = 0; j < times - 11; j++)
                    x &= ~(0x01 << j);
        }
        else
        {
            for (uint8_t i = 0; i < times + 1; i++) //  点亮阶段
                x |= 0x01 << i;
            if (times >= 11)                        //  熄灭阶段
                for (uint8_t j = 0; j < times - 10; j++)
                    x &= ~(0x01 << j);
        }
        break;

    case Right: //  左————→右
        if(group_9_flag == 0)
        {
            for (uint8_t i = 0; i < times; i++)     //  点亮阶段
                x |= 0x400 >> i;
            if (times >= 11)                        //  熄灭阶段
                for (uint8_t j = 0; j <times - 11; j++)
                    x &= ~(0x400 >> j);
        }
        else
        {
            for (uint8_t i = 0; i < times; i++)     //  点亮阶段
                x |= 0x800 >> i;
            if (times >= 11)                        //  熄灭阶段
                for (uint8_t j = 0; j <= times - 11; j++)
                    x &= ~(0x800 >> j);
        }
        break;
    default:
        break;
    }
    return x;
}

uint16_t u16Swimming_even(uint16_t x, uint8_t dir, uint8_t times)
{
    switch (dir)
    {
    case Left:  //  左←————右
        for (uint8_t j = 0; j < times - 11; j++)        //  点亮阶段
            x |= 0x01 << j;
        if (times <= 11)                                //  熄灭阶段
            for (uint8_t i = 0; i < times; i++)
                x &= ~(0x01 << i);
        break;
    case Right:
        for (uint8_t j = 0; j <times - 11; j++)         //  点亮阶段
            x |= 0x400 >> j;
        if (times <= 11)                                //  熄灭阶段
            for (uint8_t i = 0; i < times; i++)
                x &= ~(0x400 >> i);
    default:
        break;
    }
    return x;
}

uint16_t u16Swimming_mid(uint16_t x, uint8_t dir, uint8_t times)
{
    switch (dir)
    {
    case Close:
        for (uint8_t i = 0; i < times; i++)     //  两头点亮阶段
        {
            x |= 0x01 << i;
            x |= 0x400 >> i;
        }
        if (times >= 11)                        //  两头熄灭阶段
        {
            for (uint8_t j = 0; j < times - 11; j++)
            {
                x &= ~(0x01 << j);
                x &= ~(0x400 >> j);
            }
        }
        break;
    case Diffuse:
        if(times > 5)   //先等5个times 再进入流程
        {
            for (uint8_t i = 5; i < times; i++)     //  中间点亮阶段
            {
                x |= 0x01 << i;
                x |= 0x400 >> i;
            }
            if(times >= 16)                             //  中间熄灭阶段
            {
                for (uint8_t j = 5; j < times - 11; j++)
                {
                    x &= ~(0x01 << j);
                    x &= ~(0x400 >> j);
                }
            }
        }
        break;
    default:
        break;
    }
    return x;
}
//times: 1--22
bool DashedLineMovement(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times)
{
    if (data == NULL)
        return 0;
    if (led_nums > size * 8)
        return 0;
    if (led_nums == 100)
    {
        if(dir == Left || dir == Right)
        {
            // return的是uint16_t类型的数据 用11位类型的数据接收 直接截断低11位
            LedData.group.group_1 = u16Movement_odd(LedData.group.group_1, dir, times);
            LedData.group.group_2 = u16Movement_even(LedData.group.group_2, dir, times);
            LedData.group.group_3 = u16Movement_odd(LedData.group.group_3, dir, times);
            LedData.group.group_4 = u16Movement_even(LedData.group.group_4, dir, times);
            LedData.group.group_5 = u16Movement_odd(LedData.group.group_5, dir, times);
            LedData.group.group_6 = u16Movement_even(LedData.group.group_6, dir, times);
            LedData.group.group_7 = u16Movement_odd(LedData.group.group_7, dir, times);
            LedData.group.group_8 = u16Movement_even(LedData.group.group_8, dir, times);
            group_9_flag = 1;
            LedData.group.group_9 = u16Movement_odd(LedData.group.group_9, dir, times); // 第9组有12颗灯 所以需要特殊处理以保持与其他组同步
            group_9_flag = 0;
        }
        else if(dir == Close)
        {
            //左侧
            LedData.group.group_1 = u16Movement_odd(LedData.group.group_1, Right, times);
            LedData.group.group_2 = u16Movement_even(LedData.group.group_2, Right, times);
            LedData.group.group_3 = u16Movement_odd(LedData.group.group_3, Right, times);
            LedData.group.group_4 = u16Movement_even(LedData.group.group_4, Right, times);
            //中间
            LedData.group.group_5 = u16Movement_mid(LedData.group.group_5, Close, times);
            //右侧
            LedData.group.group_6 = u16Movement_even(LedData.group.group_6, Left, times);
            LedData.group.group_7 = u16Movement_odd(LedData.group.group_7, Left, times);
            LedData.group.group_8 = u16Movement_even(LedData.group.group_8, Left, times);
            group_9_flag = 1;
            LedData.group.group_9 = u16Movement_odd(LedData.group.group_9, Left, times); // 第9组有12颗灯 所以需要特殊处理以保持与其他组同步
            group_9_flag = 0;
        }
        else if(dir == Diffuse)
        {
            //左侧
            LedData.group.group_1 = u16Movement_odd(LedData.group.group_1, Left, times);
            LedData.group.group_2 = u16Movement_even(LedData.group.group_2, Left, times);
            LedData.group.group_3 = u16Movement_odd(LedData.group.group_3, Left, times);
            LedData.group.group_4 = u16Movement_even(LedData.group.group_4, Left, times);
            //中间
            LedData.group.group_5 = u16Movement_mid(LedData.group.group_5, Diffuse, times);
            //右侧
            LedData.group.group_6 = u16Movement_even(LedData.group.group_6, Right, times);
            LedData.group.group_7 = u16Movement_odd(LedData.group.group_7, Right, times);
            LedData.group.group_8 = u16Movement_even(LedData.group.group_8, Right, times);
            group_9_flag = 1;
            LedData.group.group_9 = u16Movement_odd(LedData.group.group_9, Right, times); // 第9组有12颗灯 所以需要特殊处理以保持与其他组同步
            group_9_flag = 0;
        }
        else;

		if(times>= 23)
		{
			for(uint8_t i = 0; i < BUFF_SIZE; i++)
			{
				LedData.data[i] = 0;
			}
				LedData.group.group_1 = 0;
				LedData.group.group_2 = 0;
				LedData.group.group_3 = 0;
				LedData.group.group_4 = 0;
				LedData.group.group_5 = 0;
				LedData.group.group_6 = 0;
				LedData.group.group_7 = 0;
				LedData.group.group_8 = 0;
				LedData.group.group_9 = 0;			
		}
        // 联合体自动写入有bit位错位 现手动写回
        data[0] = LedData.group.group_1 >> 3;
        data[1] = (LedData.group.group_1 & 0x07) << 5;
        data[1] |= LedData.group.group_2 >> 6;
        data[2] = (LedData.group.group_2 & 0x3f) << 2;
        data[2] |= LedData.group.group_3 >> 9;
        data[3] = (LedData.group.group_3 & 0x1fe) >> 1;
        data[4] = (LedData.group.group_3 & 0x01) << 7;
        data[4] |= LedData.group.group_4 >> 4;
        data[5] = (LedData.group.group_4 & 0x0f) << 4;
        data[5] |= LedData.group.group_5 >> 7;
        data[6] = (LedData.group.group_5 & 0x7f) << 1;
        data[6] |= LedData.group.group_6 >> 10;
        data[7] = (LedData.group.group_6 & 0x3fc) >> 2;
        data[8] = (LedData.group.group_6 & 0x03) << 6;
        data[8] |= LedData.group.group_7 >> 5;
        data[9] = (LedData.group.group_7 & 0x1f) << 3;
        data[9] |= LedData.group.group_8 >> 8;
        data[10] = LedData.group.group_8 & 0xff;
        data[11] = LedData.group.group_9 >> 4;
        data[12] = (LedData.group.group_9 & 0x0f) << 4;
    }
}

uint16_t u16Movement_odd(uint16_t x, uint8_t dir, uint8_t times)
{
 switch (dir)
    {
    case Left:  //  左←————右
        if(group_9_flag == 0)
        {
            if(times <= 11)
                for (uint8_t i = 0; i < times; i++)     //  点亮阶段
                    x |= 0x01 << i;
            else                                        //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }
        else
        {
            if(times <= 11)
                for (uint8_t i = 0; i < times + 1; i++)     //  点亮阶段
                    x |= 0x01 << i;
            else                                        //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }
        break;

    case Right: //  左————→右
        if(group_9_flag == 0)
        {
            if(times <= 11)
                for (uint8_t i = 0; i < times; i++)     //  点亮阶段
                    x |= 0x400 >> i;
            else                                        //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }
        else
        {
            if(times <= 11)
                for (uint8_t i = 0; i < times + 1; i++)     //  点亮阶段
                    x |= 0x800 >> i;
            else                                        //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }
        break;
    default:
        break;
    }
    return x;
}

uint16_t u16Movement_even(uint16_t x, uint8_t dir, uint8_t times)
{
    switch (dir)
    {
    case Left:  //  左←————右
        if(times >= 11)
            for (uint8_t j = 0; j < times - 11; j++)        //  点亮阶段
                x |= 0x01 << j;
        else                                                //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
           x = 0;
        break;
    case Right:
        if(times >= 11)
            for (uint8_t j = 0; j <times - 11; j++)         //  点亮阶段
                x |= 0x400 >> j;
        else                                                //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
           x = 0;
    default:
        break;
    }
    return x;
}

uint16_t u16Movement_mid(uint16_t x, uint8_t dir, uint8_t times)
{
    switch (dir)
    {
    case Close:
        for (uint8_t i = 0; i < times; i++)     //  两头点亮阶段
        {
            x |= 0x01 << i;
            x |= 0x400 >> i;
        }
        if (times > 11)                        //  两头熄灭阶段
        {
            x = 0;
        }
        break;
    case Diffuse:
        if(times > 5)   //先等5个times 再进入流程
        {
            for (uint8_t i = 5; i < times; i++)     //  中间点亮阶段
            {
                x |= 0x01 << i;
                x |= 0x400 >> i;
            }
            if(times >= 12)                             //  中间熄灭阶段
            {
                x = 0;
            }
        }
        break;
    default:
        break;
    }
    return x;

}


/*
	DashedStep:踏步平移
		data:uint_8类型的数组
		size:数组的大小，传sizeof即可
        led_nums:传100或？？？
        dir:游动方向
        times:次数，若led_nums传入100且dir=left\right则是1~9之间循环，不到达10
                                      dir=close\diffuse则是1~5之间循环，不到达6
*/


bool DashedStep(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times)
{
    if (data == NULL)
        return 0;
    if (led_nums > size * 8)
        return 0;
    if (led_nums == 100)
    {
        if(dir == Left)
        {
            switch (times)
            {
            case 1:
                set_empty(100);
                LedData.group.group_9 = 0xfff;
                break;
            case 2:
                set_empty(100);
                LedData.group.group_8 = 0x7ff;
                break;
            case 3:
                set_empty(100);
                LedData.group.group_7 = 0x7ff;
                break;    
            case 4:
                set_empty(100);
                LedData.group.group_6 = 0x7ff;
                break;
            case 5:
                set_empty(100);
                LedData.group.group_5 = 0x7ff;
                break;
            case 6:
                set_empty(100);
                LedData.group.group_4 = 0x7ff;
                break;
            case 7:
                set_empty(100);
                LedData.group.group_3 = 0x7ff;
                break;
            case 8:
                set_empty(100);
                LedData.group.group_2 = 0x7ff;
                break;
            case 9:
                set_empty(100);
                LedData.group.group_1 = 0x7ff;
                break;
			case 10:     
                set_empty(100);
                break;
			case 11:     
                set_empty(100);
                break;
			
            default:
                break;
            }
        }
        else if(dir == Right)
        {
            switch (times)
            {
            case 1:
                set_empty(100);
                LedData.group.group_1 = 0x7ff;
                break;
            case 2:
                set_empty(100);
                LedData.group.group_2 = 0x7ff;
                break;
            case 3:
                set_empty(100);
                LedData.group.group_3 = 0x7ff;
                break;    
            case 4:
                set_empty(100);
                LedData.group.group_4 = 0x7ff;
                break;
            case 5:
                set_empty(100);
                LedData.group.group_5 = 0x7ff;
                break;
            case 6:
                set_empty(100);
                LedData.group.group_6 = 0x7ff;
                break;
            case 7:
                set_empty(100);
                LedData.group.group_7 = 0x7ff;
                break;
            case 8:
                set_empty(100);
                LedData.group.group_8 = 0x7ff;
                break;
            case 9:
                set_empty(100);
                LedData.group.group_9 = 0xfff;
                break;
			case 10:     
                set_empty(100);
                break;
			case 11:     
                set_empty(100);
                break;

            default:
                break;
            }
        }
        else if(dir == Close)
        {
            switch (times)
            {
            case 1:
                set_empty(100);
                LedData.group.group_1 = 0x7ff;
                LedData.group.group_9 = 0xfff;
                break;
            case 2:
                set_empty(100);
                LedData.group.group_2 = 0x7ff;
                LedData.group.group_8 = 0x7ff;
                break;
            case 3:
                set_empty(100);
                LedData.group.group_3 = 0x7ff;
                LedData.group.group_7 = 0x7ff;
                break;    
            case 4:
                set_empty(100);
                LedData.group.group_4 = 0x7ff;
                LedData.group.group_6 = 0x7ff;
                break;
            case 5:
                set_empty(100);
                LedData.group.group_5 = 0x7ff;
                break;
			case 6:
                set_empty(100);
                break;
			case 7:
                set_empty(100);
                break;
            default:
                break;
            }
        }
        else if(dir == Diffuse)
        {
            switch (times)
            {
            case 1:
                LedData.group.group_5 = 0x7ff;
                break;
            case 2:
                set_empty(100);
                LedData.group.group_4 = 0x7ff;
                LedData.group.group_6 = 0x7ff;
                break;
            case 3:
                set_empty(100);
                LedData.group.group_3 = 0x7ff;
                LedData.group.group_7 = 0x7ff;
                break;    
            case 4:
                set_empty(100);
                LedData.group.group_2 = 0x7ff;
                LedData.group.group_8 = 0x7ff;
                break;
            case 5:
                set_empty(100);
                LedData.group.group_1 = 0x7ff;
                LedData.group.group_9 = 0xfff;
				break;
            case 6:
                set_empty(100);
                break;
            case 7:
                set_empty(100);
                break;
            default:
                break;
            }
        }
        else;

         // 联合体自动写入有bit位错位 现手动写回
        data[0] = LedData.group.group_1 >> 3;
        data[1] = (LedData.group.group_1 & 0x07) << 5;
        data[1] |= LedData.group.group_2 >> 6;
        data[2] = (LedData.group.group_2 & 0x3f) << 2;
        data[2] |= LedData.group.group_3 >> 9;
        data[3] = (LedData.group.group_3 & 0x1fe) >> 1;
        data[4] = (LedData.group.group_3 & 0x01) << 7;
        data[4] |= LedData.group.group_4 >> 4;
        data[5] = (LedData.group.group_4 & 0x0f) << 4;
        data[5] |= LedData.group.group_5 >> 7;
        data[6] = (LedData.group.group_5 & 0x7f) << 1;
        data[6] |= LedData.group.group_6 >> 10;
        data[7] = (LedData.group.group_6 & 0x3fc) >> 2;
        data[8] = (LedData.group.group_6 & 0x03) << 6;
        data[8] |= LedData.group.group_7 >> 5;
        data[9] = (LedData.group.group_7 & 0x1f) << 3;
        data[9] |= LedData.group.group_8 >> 8;
        data[10] = LedData.group.group_8 & 0xff;
        data[11] = LedData.group.group_9 >> 4;
        data[12] = (LedData.group.group_9 & 0x0f) << 4;
    }

}

// modify 2023.12.26
_Pragma("optimize=none") void set_empty(uint8_t led_nums)
{
	if(led_nums == 100)
	{
        LedData.group.group_1 = 0;	
        LedData.group.group_2 = 0;
        LedData.group.group_3 = 0;
        LedData.group.group_4 = 0;
        LedData.group.group_5 = 0;
        LedData.group.group_6 = 0;
        LedData.group.group_7 = 0;
        LedData.group.group_8 = 0;
        LedData.group.group_9 = 0;
	}
}

bool DashedStep_lightup(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times)
{
	if (data == NULL)
			return 0;
		if (led_nums > size * 8)
			return 0;
		if(dir == Left)
		{
			switch (times)
			{
                        case 1:
               //                          for(uint8_t i = 0; i < BUFF_SIZE; i++)
                 //                 LedData.data[i] = 0;
                                
                                LedData.group.group_1 = 0;
                                LedData.group.group_2 = 0;
                                LedData.group.group_3 = 0;
                                LedData.group.group_4 = 0;
                                LedData.group.group_5 = 0;
                                LedData.group.group_6 = 0;
                                LedData.group.group_7 = 0;
                                LedData.group.group_8 = 0;
                                LedData.group.group_9 = 0;
                                break;
			case 2:
                                           
				LedData.group.group_9 = 0xfff;
				break;
			case 3:
				LedData.group.group_7 = 0x7ff;
				break;
			case 4:
				LedData.group.group_5 = 0x7ff;
				break;	  
			case 5:
				LedData.group.group_3 = 0x7ff;
				break;
			case 6:
				LedData.group.group_1 = 0x7ff;
				break;
                                /*
			case 6:
                                for(uint8_t i = 0; i < BUFF_SIZE; i++)
                                  LedData.data[i] = 0;
                                                                LedData.group.group_1 = 0;
                                LedData.group.group_3 = 0;
                                LedData.group.group_5 = 0;
                                LedData.group.group_7 = 0;
                                LedData.group.group_9 = 0;
                                break;
*/
			default:
				break;
			}
		}
		if(dir == Right)
		{
			switch (times)
			{
                        case 1:
              //                           for(uint8_t i = 0; i < BUFF_SIZE; i++)
                //                  LedData.data[i] = 0;
                                
                                LedData.group.group_1 = 0;
                                LedData.group.group_2 = 0;
                                LedData.group.group_3 = 0;
                                LedData.group.group_4 = 0;
                                LedData.group.group_5 = 0;
                                LedData.group.group_6 = 0;
                                LedData.group.group_7 = 0;
                                LedData.group.group_8 = 0;
                                LedData.group.group_9 = 0;
                                break;
			case 2:

				LedData.group.group_1 = 0x7ff;
				break;
			case 3:
				LedData.group.group_3 = 0x7ff;
				break;
			case 4:
				LedData.group.group_5 = 0x7ff;
				break;	  
			case 5:
				LedData.group.group_7 = 0x7ff;
				break;
			case 6:
				LedData.group.group_9 = 0xfff;
				break;
                                /*
                        case 6:
                                for(uint8_t i = 0; i < BUFF_SIZE; i++)
                                  LedData.data[i] = 0;
                                LedData.group.group_1 = 0;
                                LedData.group.group_3 = 0;
                                LedData.group.group_5 = 0;
                                LedData.group.group_7 = 0;
                                LedData.group.group_9 = 0;
                                break;
*/
			default:
				break;
			}
		}
		if(dir == Close)
		{
			switch (times)
			{
                        case 1:
                 //                        for(uint8_t i = 0; i < BUFF_SIZE; i++)
                   //               LedData.data[i] = 0;
                                
                                LedData.group.group_1 = 0;
                                LedData.group.group_2 = 0;
                                LedData.group.group_3 = 0;
                                LedData.group.group_4 = 0;
                                LedData.group.group_5 = 0;
                                LedData.group.group_6 = 0;
                                LedData.group.group_7 = 0;
                                LedData.group.group_8 = 0;
                                LedData.group.group_9 = 0;
                                break;
			case 2:

				LedData.group.group_1 = 0x7ff;			
				LedData.group.group_9 = 0xfff;
				break;
			case 3:
				LedData.group.group_3 = 0x7ff;
				LedData.group.group_7 = 0x7ff;
				break;
			case 4:
				LedData.group.group_5 = 0x7ff;
				break;
                                /*
                        case 5:
                                for(uint8_t i = 0; i < BUFF_SIZE; i++)
                                  LedData.data[i] = 0;
                                LedData.group.group_1 = 0;
                                LedData.group.group_3 = 0;
                                LedData.group.group_5 = 0;
                                LedData.group.group_7 = 0;
                                LedData.group.group_9 = 0;
                                break;
          */
			default:
				break;
			}
		}
		if(dir == Diffuse)
		{
			switch (times)
			{
                        case 1:
              //                          for(uint8_t i = 0; i < BUFF_SIZE; i++)
                //                  LedData.data[i] = 0;
                                
                                LedData.group.group_1 = 0;
                                LedData.group.group_2 = 0;
                                LedData.group.group_3 = 0;
                                LedData.group.group_4 = 0;
                                LedData.group.group_5 = 0;
                                LedData.group.group_6 = 0;
                                LedData.group.group_7 = 0;
                                LedData.group.group_8 = 0;
                                LedData.group.group_9 = 0;
                                break;
			case 2:

				LedData.group.group_5 = 0x7ff;
				break;
			case 3:
				LedData.group.group_3 = 0x7ff;
				LedData.group.group_7 = 0x7ff;
				break;
			case 4:
				LedData.group.group_1 = 0x7ff;			
				LedData.group.group_9 = 0xfff;
				break;	  
                        /*case 5:
                          
                                for(uint8_t i = 0; i < BUFF_SIZE; i++)
                                  LedData.data[i] = 0;
                                
                                                                LedData.group.group_1 = 0;
                                LedData.group.group_3 = 0;
                                LedData.group.group_5 = 0;
                                LedData.group.group_7 = 0;
                                LedData.group.group_9 = 0;
                                break;		
      */
			default:
				break;
			}
		}
  // 联合体自动写入有bit位错位 现手动写回
        data[0] = LedData.group.group_1 >> 3;
        data[1] = (LedData.group.group_1 & 0x07) << 5;
        data[1] |= LedData.group.group_2 >> 6;
        data[2] = (LedData.group.group_2 & 0x3f) << 2;
        data[2] |= LedData.group.group_3 >> 9;
        data[3] = (LedData.group.group_3 & 0x1fe) >> 1;
        data[4] = (LedData.group.group_3 & 0x01) << 7;
        data[4] |= LedData.group.group_4 >> 4;
        data[5] = (LedData.group.group_4 & 0x0f) << 4;
        data[5] |= LedData.group.group_5 >> 7;
        data[6] = (LedData.group.group_5 & 0x7f) << 1;
        data[6] |= LedData.group.group_6 >> 10;
        data[7] = (LedData.group.group_6 & 0x3fc) >> 2;
        data[8] = (LedData.group.group_6 & 0x03) << 6;
        data[8] |= LedData.group.group_7 >> 5;
        data[9] = (LedData.group.group_7 & 0x1f) << 3;
        data[9] |= LedData.group.group_8 >> 8;
        data[10] = LedData.group.group_8 & 0xff;
        data[11] = LedData.group.group_9 >> 4;
        data[12] = (LedData.group.group_9 & 0x0f) << 4;
}




/*
int main(int argc, char const *argv[])
{
    for (uint8_t i = 1; i < 23; i++)
    {
        DashedLineSwimming(data, sizeof(data), 100, Diffuse, i);
        // // // 数组打印调试
        printf("i = %2d, data = ", i);
        printf2(data, sizeof(data));

        // 单组打印调试
        // printf("0x%3x, ", LedData.group.group_1);
        // printf2_16(LedData.group.group_1);
        // putchar('\n');
        if (i >= 22)
            i = 0;
        usleep(50000);
    }
    return 0;
}
*/

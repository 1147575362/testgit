#include "My_DashedLine.h"
#include "My_simplerunning.h"
Led_Data_40_t LedData_40;

//                  0     1    2     3     4      5    6      7    8     9     10    11    12
uint8_t data[buf_size] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//uint8_t data[buf_size] = {0x00,};


/**********************************100LED*******************************/

/**********************************32/40LED*******************************/

_Pragma("optimize=none") bool DashedLineSwimming_side(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times)
{
    if (data == NULL)
        return 0;
    if (led_nums > size * 8)
        return 0;
    if(led_nums == 40)
    {
        if(dir == Left || dir == Right)
        {
            LedData_40.group.group_1 = u16Swimming_even_side(LedData_40.group.group_1, led_nums, dir, times);
            LedData_40.group.group_2 = u16Swimming_odd_side(LedData_40.group.group_2, led_nums, dir, times);
            LedData_40.group.group_3 = u16Swimming_even_side(LedData_40.group.group_3, led_nums, dir, times);
            LedData_40.group.group_4 = u16Swimming_odd_side(LedData_40.group.group_4, led_nums, dir, times);
            LedData_40.group.group_5 = u16Swimming_even_side(LedData_40.group.group_5, led_nums, dir, times);
            LedData_40.group.group_6 = u16Swimming_odd_side(LedData_40.group.group_6, led_nums, dir, times);
            LedData_40.group.group_7 = u16Swimming_even_side(LedData_40.group.group_7, led_nums, dir, times);
            LedData_40.group.group_8 = u16Swimming_odd_side(LedData_40.group.group_8, led_nums, dir, times);
        }
        else if(dir == Close)
        {
            //左侧
            LedData_40.group.group_1 = u16Swimming_odd_side(LedData_40.group.group_1, led_nums, Right, times);
            LedData_40.group.group_2 = u16Swimming_even_side(LedData_40.group.group_2, led_nums, Right, times);
            LedData_40.group.group_3 = u16Swimming_odd_side(LedData_40.group.group_3, led_nums, Right, times);
            LedData_40.group.group_4 = u16Swimming_even_side(LedData_40.group.group_4, led_nums, Right, times);
            //右侧
            LedData_40.group.group_5 = u16Swimming_even_side(LedData_40.group.group_5, led_nums, Left, times);
            LedData_40.group.group_6 = u16Swimming_odd_side(LedData_40.group.group_6, led_nums, Left, times);
            LedData_40.group.group_7 = u16Swimming_even_side(LedData_40.group.group_7, led_nums, Left, times);
            LedData_40.group.group_8 = u16Swimming_odd_side(LedData_40.group.group_8, led_nums, Left, times);
        }
        else if(dir == Diffuse)
        {
            //左侧
            LedData_40.group.group_1 = u16Swimming_odd_side(LedData_40.group.group_1, led_nums, Left, times);
            LedData_40.group.group_2 = u16Swimming_even_side(LedData_40.group.group_2, led_nums, Left, times);
            LedData_40.group.group_3 = u16Swimming_odd_side(LedData_40.group.group_3, led_nums, Left, times);
            LedData_40.group.group_4 = u16Swimming_even_side(LedData_40.group.group_4, led_nums, Left, times);
            //右侧
            LedData_40.group.group_5 = u16Swimming_even_side(LedData_40.group.group_5, led_nums, Right, times);
            LedData_40.group.group_6 = u16Swimming_odd_side(LedData_40.group.group_6, led_nums, Right, times);
            LedData_40.group.group_7 = u16Swimming_even_side(LedData_40.group.group_7, led_nums, Right, times);
            LedData_40.group.group_8 = u16Swimming_odd_side(LedData_40.group.group_8, led_nums, Right, times);
        }
        else;   
    }
	if(times >= 9)
	{
		set_empty(led_nums);
	}
    Pushing_Data_From_union(led_nums);
}

uint16_t u16Swimming_odd_side(uint16_t x,uint8_t led_nums, uint8_t dir, uint8_t times)
{
    
    switch (dir)
    {
    case Left:  //  左←————右
	    for (uint8_t i = 0; i < times + 1; i++)     //  点亮阶段
	        x |= 0x01 << i;
	    if (times >= 4)                        //  熄灭阶段
	        for (uint8_t j = 0; j < times - 3; j++)
	            x &= ~(0x01 << j);
        break;

    case Right: //  左————→右
            for (uint8_t i = 0; i < times; i++)     //  点亮阶段
                x |= 0x10 >> i;
            if (times >= 4)                        //  熄灭阶段
                for (uint8_t j = 0; j <= times - 4; j++)
                    x &= ~(0x10 >> j);
        break;
    default:
        break;
    }
    return x;
}

_Pragma("optimize=none") uint16_t u16Swimming_even_side(uint16_t x, uint8_t led_nums, uint8_t dir, uint8_t times)
{  
    switch (dir)
    {
    case Left:  //  左←————右
            for (uint8_t j = 0; j < times - 4 + 1; j++)        //  点亮阶段
                x |= 0x01 << j;
            if (times <= 4)                                //  熄灭阶段
                for (uint8_t i = 0; i < times + 1; i++)
                    x &= ~(0x01 << i);
        break;

    case Right: //  左————→右
            for (uint8_t j = 0; j <= times - 4; j++)         //  点亮阶段
                x |= 0x10 >> j;
            if (times <= 4)                                //  熄灭阶段
                for (uint8_t i = 0; i <= times; i++)
                    x &= ~(0x10 >> i);
        break;
    default:
        break;
    }
    return x;
}


_Pragma("optimize=none") bool DashedLineMovement_side(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times)
{
    if (data == NULL)
        return 0;
    if (led_nums > size * 8)
        return 0;
    
    if (led_nums == 40)
    {
        if(dir == Left || dir == Right)
        {
            LedData_40.group.group_1 = u16Movement_odd_side(LedData_40.group.group_1, led_nums, dir, times);
            LedData_40.group.group_2 = u16Movement_even_side(LedData_40.group.group_2, led_nums, dir, times);
            LedData_40.group.group_3 = u16Movement_odd_side(LedData_40.group.group_3, led_nums, dir, times);
            LedData_40.group.group_4 = u16Movement_even_side(LedData_40.group.group_4, led_nums, dir, times);
            LedData_40.group.group_5 = u16Movement_odd_side(LedData_40.group.group_5, led_nums, dir, times);
            LedData_40.group.group_6 = u16Movement_even_side(LedData_40.group.group_6, led_nums, dir, times);
            LedData_40.group.group_7 = u16Movement_odd_side(LedData_40.group.group_7, led_nums, dir, times);
            LedData_40.group.group_8 = u16Movement_even_side(LedData_40.group.group_8, led_nums, dir, times);
        }
        else if(dir == Close)
        {
            //左侧
            LedData_40.group.group_1 = u16Movement_odd_side(LedData_40.group.group_1, led_nums, Right, times);
            LedData_40.group.group_2 = u16Movement_even_side(LedData_40.group.group_2, led_nums, Right, times);
            LedData_40.group.group_3 = u16Movement_odd_side(LedData_40.group.group_3, led_nums, Right, times);
            LedData_40.group.group_4 = u16Movement_even_side(LedData_40.group.group_4, led_nums, Right, times);
            //右侧
            LedData_40.group.group_5 = u16Movement_even_side(LedData_40.group.group_5, led_nums, Left, times);
            LedData_40.group.group_6 = u16Movement_odd_side(LedData_40.group.group_6, led_nums, Left, times);
            LedData_40.group.group_7 = u16Movement_even_side(LedData_40.group.group_7, led_nums, Left, times);
            LedData_40.group.group_8 = u16Movement_odd_side(LedData_40.group.group_8, led_nums, Left, times);
        }
        else if(dir == Diffuse)
        {
            //左侧
            LedData_40.group.group_1 = u16Movement_odd_side(LedData_40.group.group_1, led_nums, Left, times);
            LedData_40.group.group_2 = u16Movement_even_side(LedData_40.group.group_2, led_nums, Left, times);
            LedData_40.group.group_3 = u16Movement_odd_side(LedData_40.group.group_3, led_nums, Left, times);
            LedData_40.group.group_4 = u16Movement_even_side(LedData_40.group.group_4, led_nums, Left, times);
            //右侧
            LedData_40.group.group_5 = u16Movement_even_side(LedData_40.group.group_5, led_nums, Right, times);
            LedData_40.group.group_6 = u16Movement_odd_side(LedData_40.group.group_6, led_nums, Right, times);
            LedData_40.group.group_7 = u16Movement_even_side(LedData_40.group.group_7, led_nums, Right, times);
            LedData_40.group.group_8 = u16Movement_odd_side(LedData_40.group.group_8, led_nums, Right, times);
        }
        else;
    }
    // 跑完次数熄灭
    if (times >= 9)
        set_empty(led_nums);


    Pushing_Data_From_union(led_nums); 
}

_Pragma("optimize=none") uint16_t u16Movement_odd_side(uint16_t x, uint8_t led_nums, uint8_t dir, uint8_t times)
{
 switch (dir)
    {
    case Left:  //  左←————右
        if(times <= 4)
            for (uint8_t i = 0; i < times + 1; i++)     //  点亮阶段
                x |= 0x01 << i;
        else                                        //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
            x = 0;
        break;
			
    case Right: //  左————→右
        if(times <= 4)
            for (uint8_t i = 0; i < times + 1; i++)     //  点亮阶段
                x |= 0x10 >> i;
        else                                        //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
            x = 0;    
        break;
    default:
        break;
    }
    return x;
}

_Pragma("optimize=none") uint16_t u16Movement_even_side(uint16_t x, uint8_t led_nums, uint8_t dir, uint8_t times)
{
    switch (dir)
    {
    case Left:  //  左←————右
        if(times >= 4)
            for (uint8_t j = 0; j < times - 4 + 1; j++)        //  点亮阶段
                x |= 0x01 << j;
        else                                                //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
        x = 0;
        break;
			
    case Right:
        if(times >= 4)
            for (uint8_t j = 0; j <times - 4 + 1; j++)         //  点亮阶段
                x |= 0x10 >> j;
        else                                                //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
        x = 0;  
    default:
        break;
    }
    return x;
}


_Pragma("optimize=none") bool DashedStep_side(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times)
{
    if (data == NULL)
        return 0;
    if (led_nums > size * 8)
        return 0;

    if(led_nums == 40)
    {
        if(dir == Left)
        {
            switch (times)
            {
            case 1:
				set_empty(led_nums);
                LedData_40.group.group_8 = 0x1f;
                break;
            case 2:
				set_empty(led_nums);
                LedData_40.group.group_7 = 0x1f;
                break;    
            case 3:
				set_empty(led_nums);
                LedData_40.group.group_6 = 0x1f;
                break;
            case 4:
				set_empty(led_nums);
                LedData_40.group.group_5 = 0x1f;
                break;
            case 5:
				set_empty(led_nums);
                LedData_40.group.group_4 = 0x1f;
                break;
            case 6:
				set_empty(led_nums);
                LedData_40.group.group_3 = 0x1f;
                break;
            case 7:
				set_empty(led_nums);
                LedData_40.group.group_2 = 0x1f;
                break;
            case 8:
				set_empty(led_nums);
                LedData_40.group.group_1 = 0x1f;
                break;
			case 9:
				set_empty(led_nums);
				break;
			case 10:
				set_empty(led_nums);
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
				set_empty(led_nums);
                LedData_40.group.group_1 = 0x1f;
                break;
            case 2:
				set_empty(led_nums);
                LedData_40.group.group_2 = 0x1f;
                break;
            case 3:
				set_empty(led_nums);
                LedData_40.group.group_3 = 0x1f;
                break;    
            case 4:
				set_empty(led_nums);
                LedData_40.group.group_4 = 0x1f;
                break;
            case 5:
				set_empty(led_nums);
                LedData_40.group.group_5 = 0x1f;
                break;
            case 6:
				set_empty(led_nums);
                LedData_40.group.group_6 = 0x1f;
                break;
            case 7:
				set_empty(led_nums);
                LedData_40.group.group_7 = 0x1f;
                break;
            case 8:
				set_empty(led_nums);
                LedData_40.group.group_8 = 0x1f;
                break;
			case 9:
				set_empty(led_nums);
				break;
			case 10:
				set_empty(led_nums);
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
				set_empty(led_nums);
                LedData_40.group.group_1 = 0x1f;
                LedData_40.group.group_8 = 0x1f;
                break;
            case 2:
				set_empty(led_nums);
                LedData_40.group.group_2 = 0x1f;
                LedData_40.group.group_7 = 0x1f;
                break;
            case 3:
				set_empty(led_nums);
                LedData_40.group.group_3 = 0x1f;
                LedData_40.group.group_6 = 0x1f;
                break;    
            case 4:
				set_empty(led_nums);
                LedData_40.group.group_4 = 0x1f;
                LedData_40.group.group_5 = 0x1f;
                break;
			case 5:
				set_empty(led_nums);
                break;
			case 6:
				set_empty(led_nums);
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
				set_empty(led_nums);
                LedData_40.group.group_4 = 0x1f;
                LedData_40.group.group_5 = 0x1f;
                break;
            case 2:
				set_empty(led_nums);
                LedData_40.group.group_3 = 0x1f;
                LedData_40.group.group_6 = 0x1f;
                break;
            case 3:
				set_empty(led_nums);
                LedData_40.group.group_2 = 0x1f;
                LedData_40.group.group_7 = 0x1f;
                break;    
            case 4:
				set_empty(led_nums);
                LedData_40.group.group_1 = 0x1f;
                LedData_40.group.group_8 = 0x1f;
                break;
			case 5:
				set_empty(led_nums);
				break;
			case 6:
				set_empty(led_nums);
                break;
            default:
                break;
            }
        }
        else;
    }
    Pushing_Data_From_union(led_nums);
}

_Pragma("optimize=none") void set_empty(uint8_t led_nums)
{
	if(led_nums == 40)
	{
//		for(uint8_t i = 0; i < sizeof(LedData_40.data); i++)
//	        LedData_40.data[i] = 0;
		LedData_40.group.group_1 = 0;
			
		LedData_40.group.group_2 = 0;
		
		LedData_40.group.group_3 = 0;
		
		LedData_40.group.group_4 = 0;
		
		LedData_40.group.group_5 = 0;
		
		LedData_40.group.group_6 = 0;
		
		LedData_40.group.group_7 = 0;
		
		LedData_40.group.group_8 = 0;
		


	}
}

_Pragma("optimize=none") bool DashedStep_lightup(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times)	//虚线依次点亮
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
				set_empty(led_nums);
				break;
			case 2:
                LedData_40.group.group_8 = 0x1f;
                break;
            case 3:
                LedData_40.group.group_6 = 0x1f;
                break;
            case 4:
                LedData_40.group.group_4 = 0x1f;
                break;
            case 5:
                LedData_40.group.group_2 = 0x1f;
                break;

            default:
                break;
            }
		}

		if(dir == Right)
		{
			switch (times)
            {
            case 1:
				set_empty(led_nums);
				break;
			case 2:
                LedData_40.group.group_2 = 0x1f;
                break;
            case 3:
                LedData_40.group.group_4 = 0x1f;
                break;
            case 4:
                LedData_40.group.group_6 = 0x1f;
                break;
            case 5:
                LedData_40.group.group_8 = 0x1f;
                break;

            default:
                break;
            }
		}

		if(dir == Close)
		{
			 switch (times)
            {
            case 1:
				set_empty(led_nums);
				break;
			case 2:
                LedData_40.group.group_2 = 0x1f;
                LedData_40.group.group_7 = 0x1f;
                break;

            case 3:
                LedData_40.group.group_4 = 0x1f;
                LedData_40.group.group_5 = 0x1f;
                break;    


            default:
                break;
            }
		}

		if(dir == Diffuse)
		{
			switch (times)
            {
            case 1:
				
				set_empty(led_nums);
				break;
				
			case 2:
                LedData_40.group.group_4 = 0x1f;
                LedData_40.group.group_5 = 0x1f;
                break;

            case 3:
                LedData_40.group.group_2 = 0x1f;
                LedData_40.group.group_7 = 0x1f;
                break;    


            default:
                break;
            }
		}
		
    Pushing_Data_From_union(led_nums);
	}


void Pushing_Data_From_union(uint8_t led_nums)
{

    if(led_nums == 40)
    {
    /* 36LEDS
        // 联合体自动写入有bit位错位 现手动写回
        data[0] = (LedData_40.group.group_1 & 0x1f) << 3;
        data[0] |= LedData_40.group.group_2 >> 2;
        data[1] = (LedData_40.group.group_2 & 0x03) << 6;
        data[1] |= (LedData_40.group.group_3 & 0x0f) << 2;
        data[1] |= LedData_40.group.group_4 >> 2;
        data[2] = (LedData_40.group.group_4 & 0x03) << 6;
        data[2] |= (LedData_40.group.group_5 & 0x0f) << 2;
        data[2] |= LedData_40.group.group_6 >> 2;
        data[3] = (LedData_40.group.group_6 & 0x03) << 6;
        data[3] |= (LedData_40.group.group_7 & 0x1f) << 1;
        data[3] |= LedData_40.group.group_8 >> 4;
		data[4] = 0x00;
		data[4] |= (LedData_40.group.group_8 & 0x0f) << 4;
		//data[4] |= 0x00 >> 4;
	*/

	/*40LEDS*/
		data[0] = (LedData_40.group.group_1 & 0x1f) << 3;
		data[0] |= LedData_40.group.group_2 >> 2;
		
		data[1] = (LedData_40.group.group_2 & 0x03) << 6;
		data[1] |= (LedData_40.group.group_3 & 0x1f) << 1;
		data[1] |= LedData_40.group.group_4 >> 4;

		data[2] = (LedData_40.group.group_4 & 0x0f) << 4;
		data[2] |= LedData_40.group.group_5 >> 1;

		data[3] = (LedData_40.group.group_5 & 0x01) << 7;
		data[3] |= (LedData_40.group.group_6 & 0x1f) << 2;
		data[3] |= LedData_40.group.group_7 >> 3;

		data[4] = (LedData_40.group.group_7 & 0x07) << 5;
		data[4] |= LedData_40.group.group_8 & 0x1f;
    }
    else;
}


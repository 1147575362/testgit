#include "My_DashedLine.h"
#include "My_simplerunning.h"

Led_Data_34_t LedData_34;

bool group_9_flag;       // 因第九组有12bits 所以需要特殊标注处理
bool group_special_flag; // 因第1 8组有5bits 所以需要特殊标注处理

//                  0     1    2     3     4      5    6      7    8     9     10    11    12
uint8_t data[buf_size] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// uint8_t data[buf_size] = {0x00,};

/**********************************100LED*******************************/

/**********************************34/36LED*******************************/

_Pragma("optimize=none") bool DashedLineSwimming_side(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times)
{
    if (data == NULL)
        return 0;
    if (led_nums > size * 8)
        return 0;

    if (dir == Left || dir == Right)
    {
        // return的是uint16_t类型的数据 用11位类型的数据接收 直接截断低11位
        group_special_flag = 1;
            LedData_34.group.group_1 = u16Swimming_odd_side(LedData_34.group.group_1, led_nums, dir, times);
        group_special_flag = 0;
        LedData_34.group.group_2 = u16Swimming_even_side(LedData_34.group.group_2, led_nums, dir, times);
        LedData_34.group.group_3 = u16Swimming_odd_side(LedData_34.group.group_3, led_nums, dir, times);
        LedData_34.group.group_4 = u16Swimming_even_side(LedData_34.group.group_4, led_nums, dir, times);
        LedData_34.group.group_5 = u16Swimming_odd_side(LedData_34.group.group_5, led_nums, dir, times);
        LedData_34.group.group_6 = u16Swimming_even_side(LedData_34.group.group_6, led_nums, dir, times);
        LedData_34.group.group_7 = u16Swimming_odd_side(LedData_34.group.group_7, led_nums, dir, times);
        group_special_flag = 1;
        LedData_34.group.group_8 = u16Swimming_even_side(LedData_34.group.group_8, led_nums, dir, times);
        group_special_flag = 0;
    }
    else if (dir == Close)
    {
        // 左侧
        group_special_flag = 1;
        LedData_34.group.group_1 = u16Swimming_odd_side(LedData_34.group.group_1, led_nums, Right, times);
        group_special_flag = 0;
        LedData_34.group.group_2 = u16Swimming_even_side(LedData_34.group.group_2, led_nums, Right, times);
        LedData_34.group.group_3 = u16Swimming_odd_side(LedData_34.group.group_3, led_nums, Right, times);
        LedData_34.group.group_4 = u16Swimming_even_side(LedData_34.group.group_4, led_nums, Right, times);
        // 右侧
        LedData_34.group.group_5 = u16Swimming_even_side(LedData_34.group.group_5, led_nums, Left, times);
        LedData_34.group.group_6 = u16Swimming_odd_side(LedData_34.group.group_6, led_nums, Left, times);
        LedData_34.group.group_7 = u16Swimming_even_side(LedData_34.group.group_7, led_nums, Left, times);
        group_special_flag = 1;
        LedData_34.group.group_8 = u16Swimming_odd_side(LedData_34.group.group_8, led_nums, Left, times);
        group_special_flag = 0;
    }
    else if (dir == Diffuse)
    {
        // 左侧
        group_special_flag = 1;
        LedData_34.group.group_1 = u16Swimming_odd_side(LedData_34.group.group_1, led_nums, Left, times);
        group_special_flag = 0;
        LedData_34.group.group_2 = u16Swimming_even_side(LedData_34.group.group_2, led_nums, Left, times);
        LedData_34.group.group_3 = u16Swimming_odd_side(LedData_34.group.group_3, led_nums, Left, times);
        LedData_34.group.group_4 = u16Swimming_even_side(LedData_34.group.group_4, led_nums, Left, times);
        // 右侧
        LedData_34.group.group_5 = u16Swimming_even_side(LedData_34.group.group_5, led_nums, Right, times);
        LedData_34.group.group_6 = u16Swimming_odd_side(LedData_34.group.group_6, led_nums, Right, times);
        LedData_34.group.group_7 = u16Swimming_even_side(LedData_34.group.group_7, led_nums, Right, times);
        group_special_flag = 1;
        LedData_34.group.group_8 = u16Swimming_odd_side(LedData_34.group.group_8, led_nums, Right, times);
        group_special_flag = 0;
    }
    else
        ;

    if (times >= 9)
        set_empty(led_nums);

    Pushing_Data_From_union(led_nums);
}

_Pragma("optimize=none") uint16_t u16Swimming_odd_side(uint16_t x, uint8_t led_nums, uint8_t dir, uint8_t times)
{

    switch (dir)
    {
    case Left: //  左←————右
        if (group_special_flag == 0)
        {
            for (uint8_t i = 0; i < times; i++) //  点亮阶段
                x |= 0x01 << i;
            if (times >= 4) //  熄灭阶段
                for (uint8_t j = 0; j < times - 4; j++)
                    x &= ~(0x01 << j);
        }
        else
        {
            for (uint8_t i = 0; i < times + 1; i++) //  点亮阶段
                x |= 0x01 << i;
            if (times >= 4) //  熄灭阶段
                for (uint8_t j = 0; j < times - 3; j++)
                    x &= ~(0x01 << j);
        }
        break;

    case Right: //  左————→右
        if (group_special_flag == 0)
        {
            for (uint8_t i = 0; i < times; i++) //  点亮阶段
                x |= 0x08 >> i;
            if (times >= 4) //  熄灭阶段
                for (uint8_t j = 0; j < times - 4; j++)
                    x &= ~(0x08 >> j);
        }
        else
        {
            for (uint8_t i = 0; i < times + 1; i++) //  点亮阶段
                x |= 0x10 >> i;
            if (times >= 4) //  熄灭阶段
                for (uint8_t j = 0; j <= times - 4; j++)
                    x &= ~(0x10 >> j);
        }
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
    case Left: //  左←————右
        if (group_special_flag == 0)
        {
            for (uint8_t j = 0; j < times - 4; j++) //  点亮阶段
                x |= 0x01 << j;
            if (times <= 4) //  熄灭阶段
                for (uint8_t i = 0; i < times; i++)
                    x &= ~(0x01 << i);
        }
        else
        {
            for (uint8_t j = 0; j < times - 4 + 1; j++) //  点亮阶段
                x |= 0x01 << j;
            if (times <= 4) //  熄灭阶段
                for (uint8_t i = 0; i < times + 1; i++)
                    x &= ~(0x01 << i);
        }
        break;

    case Right: //  左————→右
        if (group_special_flag == 0)
        {
            for (uint8_t j = 0; j < times - 4; j++) //  点亮阶段
                x |= 0x08 >> j;
            if (times <= 4) //  熄灭阶段
                for (uint8_t i = 0; i < times; i++)
                    x &= ~(0x08 >> i);
        }
        else
        {
            for (uint8_t j = 0; j <= times - 4; j++) //  点亮阶段
                x |= 0x10 >> j;
            if (times <= 4) //  熄灭阶段
                for (uint8_t i = 0; i <= times; i++)
                    x &= ~(0x10 >> i);
        }
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

    if (dir == Left || dir == Right)
    {
        group_special_flag = 1;
        LedData_34.group.group_1 = u16Movement_odd_side(LedData_34.group.group_1, led_nums, dir, times);
        group_special_flag = 0;
        LedData_34.group.group_2 = u16Movement_even_side(LedData_34.group.group_2, led_nums, dir, times);
        LedData_34.group.group_3 = u16Movement_odd_side(LedData_34.group.group_3, led_nums, dir, times);
        LedData_34.group.group_4 = u16Movement_even_side(LedData_34.group.group_4, led_nums, dir, times);
        LedData_34.group.group_5 = u16Movement_odd_side(LedData_34.group.group_5, led_nums, dir, times);
        LedData_34.group.group_6 = u16Movement_even_side(LedData_34.group.group_6, led_nums, dir, times);
        LedData_34.group.group_7 = u16Movement_odd_side(LedData_34.group.group_7, led_nums, dir, times);
        group_special_flag = 1;
        LedData_34.group.group_8 = u16Movement_even_side(LedData_34.group.group_8, led_nums, dir, times);
        group_special_flag = 0;
    }
    else if (dir == Close)
    {
        // 左侧
        group_special_flag = 1;
        LedData_34.group.group_1 = u16Movement_odd_side(LedData_34.group.group_1, led_nums, Right, times);
        group_special_flag = 0;
        LedData_34.group.group_2 = u16Movement_even_side(LedData_34.group.group_2, led_nums, Right, times);
        LedData_34.group.group_3 = u16Movement_odd_side(LedData_34.group.group_3, led_nums, Right, times);
        LedData_34.group.group_4 = u16Movement_even_side(LedData_34.group.group_4, led_nums, Right, times);
        // 右侧
        LedData_34.group.group_5 = u16Movement_even_side(LedData_34.group.group_5, led_nums, Left, times);
        LedData_34.group.group_6 = u16Movement_odd_side(LedData_34.group.group_6, led_nums, Left, times);
        LedData_34.group.group_7 = u16Movement_even_side(LedData_34.group.group_7, led_nums, Left, times);
        group_special_flag = 1;
        LedData_34.group.group_8 = u16Movement_odd_side(LedData_34.group.group_8, led_nums, Left, times);
        group_special_flag = 0;
    }
    else if (dir == Diffuse)
    {
        // 左侧
        group_special_flag = 1;
        LedData_34.group.group_1 = u16Movement_odd_side(LedData_34.group.group_1, led_nums, Left, times);
        group_special_flag = 0;
        LedData_34.group.group_2 = u16Movement_even_side(LedData_34.group.group_2, led_nums, Left, times);
        LedData_34.group.group_3 = u16Movement_odd_side(LedData_34.group.group_3, led_nums, Left, times);
        LedData_34.group.group_4 = u16Movement_even_side(LedData_34.group.group_4, led_nums, Left, times);
        // 右侧
        LedData_34.group.group_5 = u16Movement_even_side(LedData_34.group.group_5, led_nums, Right, times);
        LedData_34.group.group_6 = u16Movement_odd_side(LedData_34.group.group_6, led_nums, Right, times);
        LedData_34.group.group_7 = u16Movement_even_side(LedData_34.group.group_7, led_nums, Right, times);
        group_special_flag = 1;
        LedData_34.group.group_8 = u16Movement_odd_side(LedData_34.group.group_8, led_nums, Right, times);
        group_special_flag = 0;
    }
    else
        ;
    
    // 跑完次数熄灭
    if (times >= 9)
        set_empty(led_nums);

    Pushing_Data_From_union(led_nums);
}

_Pragma("optimize=none") uint16_t u16Movement_odd_side(uint16_t x, uint8_t led_nums, uint8_t dir, uint8_t times)
{
    switch (dir)
    {
    case Left: //  左←————右
        if (group_special_flag == 0)
        {
            if (times <= 4)
                for (uint8_t i = 0; i < times; i++) //  点亮阶段
                    x |= 0x01 << i;
            else //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }
        else
        {
            if (times <= 4)
                for (uint8_t i = 0; i < times + 1; i++) //  点亮阶段
                    x |= 0x01 << i;
            else //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }
        break;
    case Right: //  左————→右
        if (group_special_flag == 0)
        {
            if (times <= 4)
                for (uint8_t i = 0; i < times; i++) //  点亮阶段
                    x |= 0x08 >> i;
            else //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }
        else
        {
            if (times <= 4)
                for (uint8_t i = 0; i < times + 1; i++) //  点亮阶段
                    x |= 0x10 >> i;
            else //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }

        break;
    default:
        break;

        return x;
    }
}

_Pragma("optimize=none") uint16_t u16Movement_even_side(uint16_t x, uint8_t led_nums, uint8_t dir, uint8_t times)
{
    switch (dir)
    {
    case Left: //  左←————右
        if (group_special_flag == 0)
        {
            if (times >= 4)
                for (uint8_t j = 0; j < times - 4; j++) //  点亮阶段
                    x |= 0x01 << j;
            else //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }
        else
        {
            if (times >= 4)
                for (uint8_t j = 0; j < times - 4 + 1; j++) //  点亮阶段
                    x |= 0x01 << j;
            else //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }

        break;
    case Right:
        if (group_special_flag == 0)
        {
            if (times >= 4)
                for (uint8_t j = 0; j < times - 4; j++) //  点亮阶段
                    x |= 0x08 >> j;
            else //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }
        else
        {
            if (times >= 4)
                for (uint8_t j = 0; j < times - 4 + 1; j++) //  点亮阶段
                    x |= 0x10 >> j;
            else //  熄灭阶段 这里应该是渐变熄灭 现直接灭灯处理
                x = 0;
        }

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
     set_empty(led_nums);
    if (dir == Left)
    {
        switch (times)
        {
        case 1:
            LedData_34.group.group_8 = 0x1f;
            break;
        case 2:
            LedData_34.group.group_7 = 0x0f;
            break;
        case 3:
            LedData_34.group.group_6 = 0x0f;
            break;
        case 4:
            LedData_34.group.group_5 = 0x0f;
            break;
        case 5:
            LedData_34.group.group_4 = 0x0f;
            break;
        case 6:
            LedData_34.group.group_3 = 0x0f;
            break;
        case 7:
            LedData_34.group.group_2 = 0x0f;
            break;
        case 8:
            LedData_34.group.group_1 = 0x1f;
            break;
        case 9:
            LedData_34.group.group_1 = 0;
            break;
        case 11:
            break;
        default:
            break;
        }
    }
    else if (dir == Right)
    {
        switch (times)
        {
        case 1:
            LedData_34.group.group_1 = 0x1f;
            break;
        case 2:
            LedData_34.group.group_2 = 0x0f;
            break;
        case 3:
            LedData_34.group.group_3 = 0x0f;
            break;
        case 4:
            LedData_34.group.group_4 = 0x0f;
            break;
        case 5:
            LedData_34.group.group_5 = 0x0f;
            break;
        case 6:
            LedData_34.group.group_6 = 0x0f;
            break;
        case 7:
            LedData_34.group.group_7 = 0x0f;
            break;
        case 8:
            LedData_34.group.group_8 = 0x1f;
            break;
        case 9:
            LedData_34.group.group_8 = 0;
            break;
        case 11:
            break;
        default:
            break;
        }
    }
    else if (dir == Close)
    {
        switch (times)
        {
        case 1:
            LedData_34.group.group_1 = 0x1f;
            LedData_34.group.group_8 = 0x1f;
            break;
        case 2:
            LedData_34.group.group_2 = 0x0f;
            LedData_34.group.group_7 = 0x0f;
            break;
        case 3:
            LedData_34.group.group_3 = 0x0f;
            LedData_34.group.group_6 = 0x0f;
            break;
        case 4:
            LedData_34.group.group_4 = 0x0f;
            LedData_34.group.group_5 = 0x0f;
            break;
        case 5:
            break;
        case 7:

            break;
        default:
            break;
        }
    }
    else if (dir == Diffuse)
    {
        switch (times)
        {
        case 1:
            LedData_34.group.group_4 = 0x0f;
            LedData_34.group.group_5 = 0x0f;
            break;
        case 2:
            LedData_34.group.group_3 = 0x0f;
            LedData_34.group.group_6 = 0x0f;
            break;
        case 3:
            LedData_34.group.group_2 = 0x0f;
            LedData_34.group.group_7 = 0x0f;
            break;
        case 4:
            LedData_34.group.group_1 = 0x1f;
            LedData_34.group.group_8 = 0x1f;
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        default:
            break;
        }
    }
    else
        ;
    

    Pushing_Data_From_union(led_nums);
}

_Pragma("optimize=none") void set_empty(uint8_t led_nums)
{
    if (led_nums == 34)
    {
        for (uint8_t i = 0; i < sizeof(LedData_34.data); i++)
            LedData_34.data[i] = 0;
    }
}

_Pragma("optimize=none") bool DashedStep_lightup(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times) // 虚线依次点亮
{
    if (data == NULL)
        return 0;
    if (led_nums > size * 8)
        return 0;
    if (dir == Left)
    {
        switch (times)
        {
        case 1:
            set_empty(led_nums);
            break;
        case 2:
            LedData_34.group.group_7 = 0x0f;
            break;

        case 3:
            LedData_34.group.group_5 = 0x0f;
            break;

        case 4:
            LedData_34.group.group_3 = 0x0f;
            break;

        case 5:
            LedData_34.group.group_1 = 0x1f;
            break;

        default:
            break;
        }
    }

    if (dir == Right)
    {
        switch (times)
        {
        case 1:
            set_empty(led_nums);
            break;
        case 2:
            LedData_34.group.group_1 = 0x1f;
            break;

        case 3:
            LedData_34.group.group_3 = 0x0f;
            break;

        case 4:
            LedData_34.group.group_5 = 0x0f;
            break;

        case 5:
            LedData_34.group.group_7 = 0x0f;
            break;

        default:
            break;
        }
    }

    if (dir == Close)
    {
        switch (times)
        {
        case 1:
            set_empty(led_nums);
            break;
        case 2:
            LedData_34.group.group_1 = 0x1f;
            LedData_34.group.group_8 = 0x1f;
            break;

        case 3:
            LedData_34.group.group_3 = 0x0f;
            LedData_34.group.group_6 = 0x0f;
            break;

        default:
            break;
        }
    }

    if (dir == Diffuse)
    {
        switch (times)
        {
        case 1:

            set_empty(led_nums);
            break;

        case 2:
            LedData_34.group.group_3 = 0x0f;
            LedData_34.group.group_6 = 0x0f;
            break;

        case 3:
            LedData_34.group.group_1 = 0x1f;
            LedData_34.group.group_8 = 0x1f;
            break;

        default:
            break;
        }
    }

    Pushing_Data_From_union(led_nums);
}

void Pushing_Data_From_union(uint8_t led_nums)
{
    if (led_nums == 34)
    {
        data[0] = (LedData_34.group.group_1 & 0x1f) << 3;
        data[0] |= LedData_34.group.group_2 >> 1;
        data[1] = (LedData_34.group.group_2 & 0x01) << 7;
        data[1] |= LedData_34.group.group_3 << 3;
        data[1] |= LedData_34.group.group_4 >> 1;
        data[2] = (LedData_34.group.group_4 & 0x01) << 7;
        data[2] |= LedData_34.group.group_5 << 3;
        data[2] |= LedData_34.group.group_6 >> 1;
        data[3] = (LedData_34.group.group_6 & 0x01) << 7;
        data[3] |= LedData_34.group.group_7 << 3;
        data[3] |= LedData_34.group.group_8 >> 2;
        data[4] = 0x00;
        data[4] |= (LedData_34.group.group_8 & 0x03) << 6;
    }
}

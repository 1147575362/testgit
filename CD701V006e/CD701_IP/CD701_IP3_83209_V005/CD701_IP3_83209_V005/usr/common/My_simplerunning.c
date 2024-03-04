#include "My_simplerunning.h"

uint8_t VoiceLamp(uint8_t data[], uint8_t size, uint8_t offset_Left, uint8_t offset_Right, uint8_t max_len_left, uint8_t max_len_right)
{
	if (data == NULL)
		return 0;
	// if (max_len_left > offset_Left || max_len_right > size * 8 - offset_Left)
	// 	return 0;
	memset(data, 0x00, size);

	// 处理左边
	if (offset_Left)
		for (uint8_t i = offset_Left - 1; i > offset_Left - max_len_left - 1; i--)
		{
			data[i / 8] |= 0x80 >> (i % 8);
		}
	if (offset_Right)
		for (uint8_t i = offset_Right - 1; i > offset_Right - max_len_left - 1; i--)
		{
			data[i / 8] |= 0x80 >> (i % 8);
		}

	// 处理右边
	if (offset_Left)
		for (uint8_t i = offset_Left - 1; i < offset_Left + max_len_right - 1; i++)
		{
			data[i / 8] |= 0x80 >> (i % 8);
		}
	if (offset_Right)
		for (uint8_t i = offset_Right - 1; i < offset_Right + max_len_right - 1; i++)
		{
			data[i / 8] |= 0x80 >> (i % 8);
		}
}

uint8_t VoiceLampLeft(uint8_t data[], uint8_t size, uint8_t offset, uint8_t max_len, bool rise_flag)
{
	if (data == NULL)
		return 0;
	if (max_len > offset)
		return 0;
	uint8_t i = offset - max_len - 1;
	if(rise_flag)	// 增长态
	{
		data[i / 8] |= 0x80 >> (i % 8);
	}
	else			// 收回态
	{
		data[(i-1) / 8] &= ~(0x80 >> 8 - ((i-1) % 8));
	}

}

uint8_t VoiceLampRight(uint8_t data[], uint8_t size, uint8_t offset, uint8_t max_len, bool rise_flag)
{
	if (data == NULL)
		return 0;
	if (max_len > offset)
		return 0;
	uint8_t i = offset + max_len - 1;
	if(rise_flag)	// 增长态
	{
		data[i / 8] |= 0x80 >> (i % 8);
	}
	else			// 收回态
	{
		data[(i+1) / 8] &= ~(0x80 >> 8 - ((i+1) % 8));
	}
}



uint8_t simplerunning(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times, uint8_t bits_lens, uint8_t Act)
{
	if (data == NULL)
		return 0;
	if (led_nums > size * 8)
		return 0;
	uint8_t brick_nums = 0;
	switch (dir)
	{
	case Left:
	{
		if (Act == High)
		{
			for (int16_t i = 1, j = led_nums; i <= times; i++, j--) // i控制次数， j控制位置
			{
				// 正常移动阶段
				data[(j - 1) / 8] |= 0x80 >> ((j - 1) % 8);
				if ((j + bits_lens) <= led_nums)
				{
					data[(j - 1 + bits_lens) / 8] &= ~(0x80 >> ((j - 1 + bits_lens) % 8));
				}
			}
		}
		else
		{
			memset(data, 0xff, size);	//得全部置1才看得出效果
			for (int16_t i = 1, j = led_nums; i <= times; i++, j--) // i控制次数， j控制位置
			{
				// 正常移动阶段
				data[(j - 1) / 8] &= ~(0x80 >> ((j - 1) % 8));
				if ((j + bits_lens) <= led_nums)
				{
					data[(j - 1 + bits_lens) / 8] |= 0x80 >> ((j - 1 + bits_lens) % 8);
				}
			}
		}
		break;
	}

	case Right:
	{
		if (Act == High)
		{
			for (int16_t i = 1, j = 1; i <= times; i++, j++) // i控制次数， j控制位置
			{
				// 正常移动阶段
				data[(j - 1) / 8] |= 0x80 >> ((j - 1) % 8);	
				if ((j - bits_lens) >= 0)
					data[(j - 1 - bits_lens) / 8] &= ~(0x80 >> ((j - 1 - bits_lens) % 8));
			}
		}
		else
		{
			memset(data, 0xff, size);	//得全部置1才看得出效果
			for (int16_t i = 1, j = 1; i <= times; i++, j++) // i控制次数， j控制位置
			{
				// 正常移动阶段
				data[(j - 1) / 8] &= ~(0x80 >> ((j - 1) % 8));
				if ((j - bits_lens) >= 0)
					data[(j - 1 - bits_lens) / 8] |= 0x80 >> ((j - 1 - bits_lens) % 8);
			}
		}
		break;	
	}

	case Close:	// 0~22
		if (Act == High) 	
		{
			for (uint8_t i = 0, j = 0, k = led_nums - 1; i < times; i++, j++, k--)
			{
				// if (!(i % mid))
				// {
				// 	memset(data, 0, size);
				// 	j = 0;
				// 	k = led_nums - 1;
				// }
				if(i <= k)
				{
					data[j / 8] |= 0x80 >> (j % 8);
					data[k / 8] |= 0x80 >> (k % 8);				
				}

				if(j + bits_lens >= 0)
					data[(j - 1 - bits_lens) / 8] &= ~(0x80 >> ((j - 1 - bits_lens) % 8));
				
				if ((k + bits_lens) <= led_nums)
					data[(k + bits_lens) / 8] &= ~(0x80 >> ((k + bits_lens) % 8));
			}
		}
		else
		{
			memset(data, 0xff, size); // 将数组置1
			for (uint8_t i = 0, j = 0, k = led_nums - 1; i < times; i++, j++, k--)
			{
				// if (!(i % mid))
				// {
				// 	memset(data, 0, size);
				// 	j = 0;
				// 	k = led_nums - 1;
				// }
				if(i <= k)
				{
					data[j / 8] &= ~(0x80 >> (j % 8));
					data[k / 8] &= ~(0x80 >> (k % 8));				
				}

				if(j + bits_lens >= 0)
					data[(j - 1 - bits_lens) / 8] |= (0x80 >> ((j - 1 - bits_lens) % 8));
				
				if ((k + bits_lens) <= led_nums)
					data[(k + bits_lens) / 8] |= (0x80 >> ((k + bits_lens) % 8));
			}
		}
		break;

		case Diffuse: //0~22
			if (Act == High)
			{
				//	若led个数为奇数则j = led_nums/2
				//	若led个数为偶数则j = led_nums/2 - 1
				for (int8_t i = 0, j = led_nums % 2 ? led_nums / 2 : led_nums / 2 - 1, k = led_nums / 2; i < times; i++, j--, k++)
				{
					data[j / 8] |= 0x80 >> (j % 8);
					if(k < led_nums)
						data[k / 8] |= 0x80 >> (k % 8);
	
					if((j + bits_lens) < led_nums/2)
						data[(j + bits_lens) / 8] &= ~(0x80 >> ((j + bits_lens) % 8));
					if(i >= bits_lens)
						data[(k - bits_lens) / 8] &= ~(0x80 >> ((k - bits_lens) % 8));
	
				}
			}
			else
			{
				memset(data, 0xff, size); // 将数组置1
				for (int8_t i = 0, j = led_nums % 2 ? led_nums / 2 : led_nums / 2 - 1, k = led_nums / 2; i < times; i++, j--, k++)
				{
					data[j / 8] &= ~(0x80 >> (j % 8));
					if(k < led_nums)
						data[k / 8] &= ~(0x80 >> (k % 8));
	
					if((j + bits_lens) < led_nums/2)
						data[(j + bits_lens) / 8] |= (0x80 >> ((j + bits_lens) % 8));
					if(i >= bits_lens)
						data[(k - bits_lens) / 8] |= (0x80 >> ((k - bits_lens) % 8));
	
				}
			}
			break;
	default:
		break;
	}
    return 1;
}


uint8_t FlowingLamp(uint8_t data[], uint8_t size, uint8_t led_nums, uint8_t dir, uint16_t times, uint8_t Act)
{
	if (data == NULL)
		return 0;
	if (led_nums > size * 8)
		return 0;
	if(times == 1 )	//	2024.02.02 modify 修复每次循环第一颗灯常亮第二色
	{
		memset(data, 0, size);	
		return 0;
	}
		
	//	mid用于控制close、diffuse模式的中间位置
	uint8_t mid = led_nums % 2 ? led_nums / 2 + 1 : led_nums / 2;
	switch (dir)
	{
	case Left:
		if (Act == High)
		{
			for (uint8_t i = 0, j = led_nums - 1; i < times; i++, j--) // i控制次数，j控制位置
			{
				if (!(i % led_nums))
				{
					memset(data, 0, size);
					j = led_nums - 1;
				}
				data[j / 8] |= 0x80 >> (j % 8);
			}
		}
		else
		{
			memset(data, 0xff, size);								   // 将数组置1
			for (uint8_t i = 0, j = led_nums - 1; i < times; i++, j--) // i控制次数，j控制位置
			{
				if (!(i % led_nums))
				{
					memset(data, 0xff, size);
					j = led_nums - 1;
				}
				data[j / 8] &= ~(0x80 >> (j % 8));
			}
		}
		break;
	case Right:
		if (Act == High)
		{
			for (uint8_t i = 0, j = 0; i < times; i++, j++)
			{
				if (!(i % led_nums))
				{
					memset(data, 0, size);
					j = 0;
				}
				data[j / 8] |= 0x80 >> (j % 8);
			}
		}
		else
		{
			memset(data, 0xff, size); // 将数组置1
			for (uint8_t i = 0, j = 0; i < times; i++, j++)
			{
				if (!(i % led_nums))
				{
					memset(data, 0xff, size);
					j = 0;
				}
				data[j / 8] &= ~(0x80 >> (j % 8));
			}
		}
		break;
	case Close:
		if (Act == High)
		{
			for (uint8_t i = 0, j = 0, k = led_nums - 1; i < times; i++, j++, k--)
			{
				if (!(i % mid))
				{
					memset(data, 0, size);
					j = 0;
					k = led_nums - 1;
				}
				data[j / 8] |= 0x80 >> (j % 8);
				data[k / 8] |= 0x80 >> (k % 8);
			}
		}
		else
		{
			memset(data, 0xff, size); // 将数组置1
			for (uint8_t i = 0, j = 0, k = led_nums - 1; i < times; i++, j++, k--)
			{
				if (!(i % mid))
				{
					memset(data, 0xff, size); // 将数组置1
					j = 0;
					k = led_nums - 1;
				}
				data[j / 8] &= ~(0x80 >> (j % 8));
				data[k / 8] &= ~(0x80 >> (k % 8));
			}
		}
		break;
	case Diffuse:
		if (Act == High)
		{
			//	若led个数为奇数则j = led_nums/2
			//	若led个数为偶数则j = led_nums/2 - 1
			for (uint8_t i = 0, j = led_nums % 2 ? led_nums / 2 : led_nums / 2 - 1, k = led_nums / 2; i < times; i++, j--, k++)
			{
				if (!(i % mid))
				{
					memset(data, 0, size);
					j = led_nums % 2 ? led_nums / 2 : led_nums / 2 - 1;
					k = led_nums / 2;
				}
				data[j / 8] |= 0x80 >> (j % 8);
				data[k / 8] |= 0x80 >> (k % 8);
			}
		}
		else
		{
			memset(data, 0xff, size); // 将数组置1
			for (uint8_t i = 0, j = led_nums % 2 ? led_nums / 2 : led_nums / 2 - 1, k = led_nums / 2; i < times; i++, j--, k++)
			{
				if (!(i % mid))
				{
					memset(data, 0xff, size); // 将数组置1
					j = led_nums % 2 ? led_nums / 2 : led_nums / 2 - 1;
					k = led_nums / 2;
				}
				data[j / 8] &= ~(0x80 >> (j % 8));
				data[k / 8] &= ~(0x80 >> (k % 8));
			}
		}
		break;
	default:
		break;
	}
}

/*
int main(int argc, char const *argv[])
{
	uint8_t data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	for(int i = 0; i < 100; i++)
	{
		// 叠砖函数示例 (大端模式)
		bool ret = simplerunning(data, sizeof(data), 100, Right, i, 5, High);
		// if(ret == 0)
		// 	若想循环执行，则在此处置i为0
		// 	break;
		// 打印二进制
		printf("i = %3d, ret = %d, data = ", i, ret);
		for (int i = 0; i < sizeof(data); i++)
		{
			// printf("%d ",data[i]);
			printf2(data[i]);
		}
		putchar('\n');
	}

	return 0;
}
*/

#include <appConfig.h>


#if (CODE_DEBUG_EN == 1U)

#include <stdint.h>
#include <uart_device.h>

int putchar(int c)
{	
    int value = EOF;
    /* put character to stdout */
    if (c != EOF) {
        uint8_t data = (uint8_t)c;
        (void)UART_SendBuff(&data, 1U);
        value = c;
    }
    return value;
}










#endif
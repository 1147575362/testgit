#include <appConfig.h>

#if (CODE_DEBUG_EN == 1U)

#include <stdint.h>
#include <uart_device.h>

/* The char which has been ungotten, or EOF. */
static int _UngetChar = EOF;

int getchar(void)
{	
    int value = EOF;
    /* get a character from stdin */
    if (_UngetChar != EOF){
        value = _UngetChar;
        _UngetChar = EOF;
    }else{
        /*if (uart_read(uart_console, buf) == 1){*/
        /*
        unsigned char buf[1];
        if (1U){
            value = (int)buf[0];
        }
        */
    }
    return value;
}


int __ungetchar(int c)
{       
    /* Unget a character from stdin */
    _UngetChar = c;
    return c;
}


#endif
#ifndef UART_DEVICE_H__
#define UART_DEVICE_H__

#include <stdint.h>

#define TX_MAX_FIFO_SIZE        (8U)
#define RX_MAX_FIFO_SIZE        (8U)

#define TX_BUFF_SIZE_POS       (7U)
#define TX_BUFF_SIZE           (1U << TX_BUFF_SIZE_POS)

#define RX_BUFF_SIZE           RX_MAX_FIFO_SIZE
#define RX_FIFO_INT_SIZE       (RX_MAX_FIFO_SIZE/2)
#define TX_FIFO_INT_SIZE        (1U)

#define BAUDRATE_OSR_16MHZ_HIGH_SPEED  (16U)
#define BAUDRATE_OSR_16MHZ  (14U)
#define BAUDRATE_OSR_8MHZ   (14U)
#define BAUDRATE_OSR_4MHZ   (13U)
#define BAUDRATE_OSR_2MHZ   (13U)


typedef enum{
  BAUDRATE_DIV_16MHz_9600   = 118U,
  BAUDRATE_DIV_16MHz_19200  = 59U,
  BAUDRATE_DIV_16MHz_38400  = 29U,
  BAUDRATE_DIV_16MHz_57600  = 19U,
  BAUDRATE_DIV_16MHz_115200 = 9U,
  BAUDRATE_DIV_16MHz_250000 = 3U,
  BAUDRATE_DIV_16MHz_500000 = 1U,
  BAUDRATE_DIV_16MHz_1000000= 0U,
}Baudrate_DIV_16MHz_t;


typedef enum{
  BAUDRATE_DIV_8MHz_9600   = 59U,
  BAUDRATE_DIV_8MHz_19200  = 29U,
  BAUDRATE_DIV_8MHz_38400  = 14U,
  BAUDRATE_DIV_8MHz_57600  = 9U,
  BAUDRATE_DIV_8MHz_115200 = 4U,
}Baudrate_DIV_8MHz_t;


typedef enum{
  BAUDRATE_DIV_4MHz_9600   = 31U,
  BAUDRATE_DIV_4MHz_19200  = 15U,
  BAUDRATE_DIV_4MHz_38400  = 7U,
}Baudrate_DIV_4MHz_t;

typedef enum{
  BAUDRATE_DIV_2MHz_9600   = 15U,
  BAUDRATE_DIV_2MHz_19200  = 7U,
  BAUDRATE_DIV_2MHz_38400  = 3U,
}Baudrate_DIV_2MHz_t;



typedef enum{
  BAUDRATE_9600 = 0U,
  BAUDRATE_19200,
  BAUDRATE_38400,
  BAUDRATE_57600,
  BAUDRATE_115200,
  BAUDRATE_250000,
  BAUDRATE_500000,
  BAUDRATE_1000000,
}Baudrate_t;

typedef enum{
  BITSIZE_5BITS = 0U,
  BITSIZE_6BITS,
  BITSIZE_7BITS,
  BITSIZE_8BITS,
}BitSize_t;

/*!< Parity: 00: NONE, 01: EVEN, 11: ODD */
typedef enum{
  PARITY_NONE   = 0U,
  PARITY_ODD    = 1U,
  PARITY_EVEN   = 3U,
}Parity_t;


typedef enum{
  STOPS_1BITS = 0U,
  STOPS_2BITS,
}StopBits_t;


#define RX_DATA_SIZE    (128U)
#define VALID_RX_PACKAGE_SIZE   (81U)


typedef void (*uartRx_cb_func_t)(uint8_t* buff, uint8_t length);
typedef void (*uartTx_cb_func_t)(void);


int8_t UART_Init(Baudrate_DIV_16MHz_t baudRate, BitSize_t bitSize, Parity_t parity,StopBits_t stopBits);
void UART_RegisterIRQ(uint8_t address,uartRx_cb_func_t rxDowncallback);
void UART_UnRegisterIRQ(void);
int8_t UART_SendBuff(uint8_t *buff, uint16_t length);






























#endif
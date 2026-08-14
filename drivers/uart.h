#ifndef UART_H
#define UART_H

#include <stdint.h>

typedef enum
{
    UART_OK = 0,
    UART_ERROR = 1
} UART_Status;

UART_Status UART_Init(uint32_t baud_rate);
UART_Status UART_SendByte(uint8_t data);
UART_Status UART_SendString(const char *str);
uint32_t UART_GetBaudRate(void);

#endif
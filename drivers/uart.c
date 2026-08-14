#include "uart.h"
#include "../mcu_sim/mcu_registers.h"
#include <stddef.h>
#define UART_ENABLE (1U << 0)
#define UART_TX_READY (1U << 0)

UART_Status UART_Init(uint32_t baud_rate)
{
    if (baud_rate == 0)
        return UART_ERROR;

    UART0.BAUD = baud_rate;
    UART0.CTRL = UART_ENABLE;
    UART0.STATUS = UART_TX_READY;

    return UART_OK;
}

UART_Status UART_SendByte(uint8_t data)
{
    if ((UART0.CTRL & UART_ENABLE) == 0)
        return UART_ERROR;

    if ((UART0.STATUS & UART_TX_READY) == 0)
        return UART_ERROR;

    UART0.DATA = data;

    return UART_OK;
}

UART_Status UART_SendString(const char *str)
{
    if (str == NULL)
        return UART_ERROR;

    while (*str != '\0')
    {
        if (UART_SendByte((uint8_t)*str) != UART_OK)
            return UART_ERROR;

        str++;
    }

    return UART_OK;
}

uint32_t UART_GetBaudRate(void)
{
    return UART0.BAUD;
}
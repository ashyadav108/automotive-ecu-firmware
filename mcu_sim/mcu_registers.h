#ifndef MCU_REGISTERS_H
#define MCU_REGISTERS_H

#include <stdint.h>

/* Simulated GPIO registers */
typedef struct
{
    uint32_t DIR;
    uint32_t OUT;
    uint32_t IN;
} GPIO_Registers;

/* Simulated UART registers */
typedef struct
{
    uint32_t CTRL;
    uint32_t STATUS;
    uint32_t DATA;
    uint32_t BAUD;
} UART_Registers;

/* Simulated ADC registers */
typedef struct
{
    uint32_t CTRL;
    uint32_t STATUS;
    uint32_t DATA;
} ADC_Registers;

/* Global simulated peripherals */
extern GPIO_Registers GPIO0;
extern UART_Registers UART0;
extern ADC_Registers ADC0;

#endif
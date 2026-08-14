#include "gpio.h"
#include "../mcu_sim/mcu_registers.h"

void GPIO_Init(
    uint8_t pin,
    int mode)
{
    if (pin >= 32)
    {
        return;
    }

    if (mode)
    {
        /* Configure pin as output */
        GPIO0.DIR |= (1UL << pin);
    }
    else
    {
        /* Configure pin as input */
        GPIO0.DIR &= ~(1UL << pin);
    }
}

void GPIO_Write(
    uint8_t pin,
    int value)
{
    if (pin >= 32)
    {
        return;
    }

    /* Only write to output pins */
    if ((GPIO0.DIR & (1UL << pin)) == 0)
    {
        return;
    }

    if (value)
    {
        GPIO0.OUT |= (1UL << pin);
    }
    else
    {
        GPIO0.OUT &= ~(1UL << pin);
    }
}

int GPIO_Read(
    uint8_t pin)
{
    if (pin >= 32)
    {
        return 0;
    }

    return (GPIO0.OUT & (1UL << pin))
               ? 1
               : 0;
}
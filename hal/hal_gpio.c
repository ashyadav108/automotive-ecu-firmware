#include "hal_gpio.h"
#include "../drivers/gpio.h"

void HAL_GPIO_Init(uint8_t pin, HAL_GPIO_Mode mode)
{
    GPIO_Init(pin, (int)mode);
}

void HAL_GPIO_Write(uint8_t pin, HAL_GPIO_Level level)
{
    GPIO_Write(pin, (int)level);
}

HAL_GPIO_Level HAL_GPIO_Read(uint8_t pin)
{
    return (HAL_GPIO_Level)GPIO_Read(pin);
}
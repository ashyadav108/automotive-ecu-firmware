#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void GPIO_Init(
    uint8_t pin,
    int mode);

void GPIO_Write(
    uint8_t pin,
    int value);

int GPIO_Read(
    uint8_t pin);

#endif
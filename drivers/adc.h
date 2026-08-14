#ifndef ADC_H
#define ADC_H

#include <stdint.h>

typedef enum
{
    ADC_OK = 0,
    ADC_ERROR = 1
} ADC_Status;

ADC_Status ADC_Init(void);

ADC_Status ADC_Read(
    uint8_t channel,
    uint16_t *value);

void ADC_SetSimulatedValue(
    uint16_t value);

#endif
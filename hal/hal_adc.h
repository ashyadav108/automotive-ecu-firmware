#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>

typedef enum
{
    HAL_ADC_OK = 0,
    HAL_ADC_ERROR = -1
} HAL_ADC_Status;

HAL_ADC_Status HAL_ADC_Init(void);

HAL_ADC_Status HAL_ADC_Read(
    uint8_t channel,
    uint16_t *value
);

void HAL_ADC_SetSimulatedValue(
    uint16_t value
);

#endif
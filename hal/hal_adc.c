#include "hal_adc.h"
#include "../drivers/adc.h"

HAL_ADC_Status HAL_ADC_Init(void)
{
    return (HAL_ADC_Status)ADC_Init();
}

HAL_ADC_Status HAL_ADC_Read(
    uint8_t channel,
    uint16_t *value)
{
    return (HAL_ADC_Status)ADC_Read(
        channel,
        value);
}

void HAL_ADC_SetSimulatedValue(
    uint16_t value)
{
    ADC_SetSimulatedValue(value);
}
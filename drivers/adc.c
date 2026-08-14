#include "adc.h"
#include "../mcu_sim/mcu_registers.h"
#include <stddef.h>

#define ADC_ENABLE (1U << 0)
#define ADC_MAX_CHANNEL 8

static uint16_t simulated_adc_value = 2500;

ADC_Status ADC_Init(void)
{
    ADC0.CTRL = ADC_ENABLE;
    ADC0.STATUS = 0;

    return ADC_OK;
}

ADC_Status ADC_Read(
    uint8_t channel,
    uint16_t *value)
{
    if (value == NULL)
        return ADC_ERROR;

    if (channel >= ADC_MAX_CHANNEL)
        return ADC_ERROR;

    if ((ADC0.CTRL & ADC_ENABLE) == 0)
        return ADC_ERROR;

    ADC0.DATA = simulated_adc_value;

    *value = (uint16_t)ADC0.DATA;

    return ADC_OK;
}

void ADC_SetSimulatedValue(uint16_t value)
{
    simulated_adc_value = value;
}
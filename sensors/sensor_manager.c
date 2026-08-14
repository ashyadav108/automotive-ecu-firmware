#include "sensor_manager.h"
#include <stddef.h>

void Sensor_Init(void)
{
    /*
     * Sensor initialization.
     * Nothing required for the simulator.
     */
}

void Sensor_ReadTemperature(
    uint16_t adc_value,
    Temperature_Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    /*
     * Simulator conversion:
     *
     * 2500 ADC -> 25.00 C
     */
    sensor->temperature =
        (float)adc_value / 100.0f;

    if (sensor->temperature >= 100.0f)
    {
        sensor->state =
            SENSOR_CRITICAL;
    }
    else if (sensor->temperature >= 80.0f)
    {
        sensor->state =
            SENSOR_WARNING;
    }
    else
    {
        sensor->state =
            SENSOR_NORMAL;
    }
}

const char *Sensor_GetStateName(
    Sensor_State state)
{
    switch (state)
    {
    case SENSOR_NORMAL:
        return "NORMAL";

    case SENSOR_WARNING:
        return "WARNING";

    case SENSOR_CRITICAL:
        return "CRITICAL";

    default:
        return "UNKNOWN";
    }
}
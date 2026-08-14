#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <stdint.h>

typedef enum
{
    SENSOR_NORMAL = 0,
    SENSOR_WARNING,
    SENSOR_CRITICAL
} Sensor_State;

typedef struct
{
    float temperature;
    Sensor_State state;
} Temperature_Sensor;

void Sensor_Init(void);

void Sensor_ReadTemperature(
    uint16_t adc_value,
    Temperature_Sensor *sensor);

const char *Sensor_GetStateName(
    Sensor_State state);

#endif
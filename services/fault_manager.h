#ifndef FAULT_MANAGER_H
#define FAULT_MANAGER_H

#include <stdint.h>

typedef enum
{
    FAULT_NONE = 0x00,
    FAULT_TEMP_WARNING = 0x01,
    FAULT_OVER_TEMPERATURE = 0x02

} Fault_Code;

typedef struct
{
    Fault_Code code;
    uint8_t active;

} Fault_Status;

void Fault_Manager_Init(
    Fault_Status *fault);

void Fault_Manager_Update(
    float temperature,
    Fault_Status *fault);

const char *Fault_Manager_GetDescription(
    Fault_Code code);

#endif
#ifndef RESET_MANAGER_H
#define RESET_MANAGER_H

#include <stdint.h>

typedef enum
{
    RESET_POWER_ON = 0,
    RESET_WATCHDOG,
    RESET_SOFTWARE,
    RESET_FAULT
} Reset_Reason;

typedef struct
{
    uint32_t reset_count;
    Reset_Reason reason;
} Reset_Status;

void Reset_Manager_Init(Reset_Status *status);

void Reset_Manager_Record(
    Reset_Status *status,
    Reset_Reason reason);

const char *Reset_Manager_GetReason(
    Reset_Reason reason);

uint32_t Reset_Manager_GetCount(
    const Reset_Status *status);

void Reset_Manager_Execute(
    Reset_Status *status,
    Reset_Reason reason);

#endif
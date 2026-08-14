#include "reset_manager.h"
#include <stddef.h>
void Reset_Manager_Init(
    Reset_Status *status)
{
    if (status == NULL)
    {
        return;
    }

    status->reset_count = 0;
    status->reason = RESET_POWER_ON;
}

void Reset_Manager_Record(
    Reset_Status *status,
    Reset_Reason reason)
{
    if (status == NULL)
    {
        return;
    }

    status->reset_count++;
    status->reason = reason;
}

const char *Reset_Manager_GetReason(
    Reset_Reason reason)
{
    switch (reason)
    {
    case RESET_POWER_ON:
        return "POWER ON";

    case RESET_WATCHDOG:
        return "WATCHDOG";

    case RESET_SOFTWARE:
        return "SOFTWARE";

    case RESET_FAULT:
        return "FAULT";

    default:
        return "UNKNOWN";
    }
}

uint32_t Reset_Manager_GetCount(
    const Reset_Status *status)
{
    if (status == NULL)
    {
        return 0;
    }

    return status->reset_count;
}

void Reset_Manager_Execute(
    Reset_Status *status,
    Reset_Reason reason)
{
    if (status == NULL)
    {
        return;
    }

    Reset_Manager_Record(
        status,
        reason);
}
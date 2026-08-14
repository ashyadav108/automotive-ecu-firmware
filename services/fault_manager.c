#include "fault_manager.h"
#include <stddef.h>

void Fault_Manager_Init(
    Fault_Status *fault)
{
    if (fault == NULL)
    {
        return;
    }

    fault->code = FAULT_NONE;
    fault->active = 0;
}

void Fault_Manager_Update(
    float temperature,
    Fault_Status *fault)
{
    if (fault == NULL)
    {
        return;
    }

    if (temperature > 100.0f)
    {
        fault->code =
            FAULT_OVER_TEMPERATURE;

        fault->active = 1;
    }
    else if (temperature >= 80.0f)
    {
        fault->code =
            FAULT_TEMP_WARNING;

        fault->active = 1;
    }
    else
    {
        fault->code =
            FAULT_NONE;

        fault->active = 0;
    }
}

const char *Fault_Manager_GetDescription(
    Fault_Code code)
{
    switch (code)
    {
    case FAULT_NONE:
        return "No Fault";

    case FAULT_TEMP_WARNING:
        return "Temperature Warning";

    case FAULT_OVER_TEMPERATURE:
        return "Over Temperature";

    default:
        return "Unknown Fault";
    }
}
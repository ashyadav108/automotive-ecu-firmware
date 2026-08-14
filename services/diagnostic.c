#include "diagnostic.h"
#include <stddef.h>

void Diagnostic_Init(
    Diagnostic_Context *context)
{
    if (context == NULL)
    {
        return;
    }

    context->sensor = NULL;
    context->fault = NULL;
}

CAN_Status Diagnostic_ProcessRequest(
    const CAN_Frame *request,
    CAN_Frame *response,
    Diagnostic_Context *context)
{
    if (request == NULL ||
        response == NULL ||
        context == NULL)
    {
        return CAN_ERROR;
    }

    if (request->id != DIAG_REQUEST_ID)
    {
        return CAN_ERROR;
    }

    if (request->dlc < 1)
    {
        return CAN_ERROR;
    }

    if (context->sensor == NULL ||
        context->fault == NULL)
    {
        return CAN_ERROR;
    }

    response->id = DIAG_RESPONSE_ID;
    response->dlc = 2;

    response->data[0] = request->data[0];

    switch (request->data[0])
    {
    case DIAG_READ_STATUS:

        response->data[1] =
            (uint8_t)context->sensor->state;

        break;

    case DIAG_READ_TEMPERATURE:

        response->data[1] =
            (uint8_t)context->sensor->temperature;

        break;

    case DIAG_READ_FAULT:

        response->data[1] =
            (uint8_t)context->fault->code;

        break;

    case DIAG_CLEAR_FAULT:

        context->fault->code = FAULT_NONE;
        context->fault->active = 0;

        response->data[1] = 0x00;

        break;

    case DIAG_READ_DTC:

        /*
         * Return the current fault/DTC code.
         */
        response->data[1] =
            (uint8_t)context->fault->code;

        break;

    default:

        response->data[0] = 0x7F;
        response->data[1] = 0x11;

        break;
    }

    return CAN_OK;
}
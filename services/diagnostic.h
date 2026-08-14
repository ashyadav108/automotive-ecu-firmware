#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <stdint.h>

#include "../drivers/can.h"
#include "../services/fault_manager.h"
#include "../sensors/sensor_manager.h"

#define DIAG_REQUEST_ID 0x700
#define DIAG_RESPONSE_ID 0x708

#define DIAG_READ_STATUS 0x01
#define DIAG_READ_TEMPERATURE 0x02
#define DIAG_READ_FAULT 0x03
#define DIAG_CLEAR_FAULT 0x04
#define DIAG_READ_DTC 0x05

typedef struct
{
    Temperature_Sensor *sensor;
    Fault_Status *fault;
} Diagnostic_Context;

void Diagnostic_Init(
    Diagnostic_Context *context);

CAN_Status Diagnostic_ProcessRequest(
    const CAN_Frame *request,
    CAN_Frame *response,
    Diagnostic_Context *context);

#endif
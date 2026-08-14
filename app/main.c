#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "gpio.h"
#include "uart.h"
#include "adc.h"
#include "can.h"
#include "watchdog.h"
#include "sensor_manager.h"
#include "fault_manager.h"
#include "diagnostic.h"
#include "reset_manager.h"
#include "dtc_manager.h"

/* =========================================================
   MAIN
   ========================================================= */

int main(int argc, char *argv[])
{
    uint16_t adc_value = 2500;

    if (argc > 1)
    {
        adc_value = (uint16_t)atoi(argv[1]);
    }

    printf("CAN initialized successfully\n");
    printf("# Automotive ECU Firmware\n\n");

    /* =================================================
       WATCHDOG
       ================================================= */

    Watchdog watchdog;

    if (Watchdog_Init(&watchdog, 1000) != WATCHDOG_OK)
    {
        printf("WATCHDOG : INIT FAILED\n");
        return 1;
    }

    printf("CAN initialized successfully\n\n");
    printf("WATCHDOG : ACTIVE\n\n");

    /* =================================================
       RESET MANAGER
       ================================================= */

    Reset_Status reset;

    Reset_Manager_Init(&reset);

    printf("--- ECU RESET MANAGER ---\n");

    printf(
        "RESET COUNT : %lu\n",
        (unsigned long)Reset_Manager_GetCount(&reset));

    printf(
        "RESET REASON: %s\n\n",
        Reset_Manager_GetReason(reset.reason));

    /* =================================================
       GPIO
       ================================================= */

    GPIO_Init(0, 1);
    GPIO_Write(0, 1);

    printf("GPIO DIR : 0x00000001\n");
    printf("GPIO OUT : 0x00000001\n");

    /* =================================================
       UART
       ================================================= */

    UART_Init(115200);

    printf(
        "UART BAUD: %lu\n",
        (unsigned long)UART_GetBaudRate());

    /* =================================================
       ADC
       ================================================= */

    ADC_Init();

    printf(
        "ADC VALUE: %u\n",
        adc_value);

    /* =================================================
       SENSOR MANAGER
       ================================================= */

    Temperature_Sensor sensor;

    Sensor_Init();

    Sensor_ReadTemperature(
        adc_value,
        &sensor);

    printf(
        "TEMP     : %.2f C\n",
        sensor.temperature);

    printf(
        "STATUS   : %s\n",
        Sensor_GetStateName(sensor.state));

    /* =================================================
       FAULT MANAGER
       ================================================= */

    Fault_Status fault;

    Fault_Manager_Init(&fault);

    Fault_Manager_Update(
        sensor.temperature,
        &fault);

    printf(
        "FAULT CODE: 0x%02X\n",
        fault.code);

    printf(
        "FAULT     : %s\n",
        Fault_Manager_GetDescription(fault.code));

    if (fault.active)
    {
        printf(
            "ACTION    : FAULT DETECTED\n");
    }
    else
    {
        printf(
            "ACTION    : SYSTEM OPERATING NORMALLY\n");
    }

    /* =================================================
       DTC MANAGER
       ================================================= */

    DTC_Manager_Init();

    if (fault.code == FAULT_OVER_TEMPERATURE)
    {
        DTC_Manager_Set(
            DTC_OVER_TEMPERATURE);
    }

    printf("\n--- DTC STATUS ---\n");

    if (DTC_Manager_IsActive(
            DTC_OVER_TEMPERATURE))
    {
        printf(
            "DTC STATUS: ACTIVE\n");
    }
    else
    {
        printf(
            "DTC STATUS: CLEAR\n");
    }

    /* =================================================
       NORMAL CAN SENSOR FRAME
       ================================================= */

    CAN_Frame can_frame;

    can_frame.id = 0x100;
    can_frame.dlc = 2;

    can_frame.data[0] =
        (uint8_t)sensor.state;

    can_frame.data[1] =
        (uint8_t)sensor.temperature;

    printf("\n--- CAN FRAME ---\n");

    printf(
        "CAN ID : 0x%03X\n",
        can_frame.id);

    printf(
        "DLC    : %d\n",
        can_frame.dlc);

    printf(
        "DATA   : %02X %02X\n",
        can_frame.data[0],
        can_frame.data[1]);

    CAN_Send(&can_frame);

    /* =================================================
       DIAGNOSTIC CONTEXT
       ================================================= */

    Diagnostic_Context diagnostic_context;

    Diagnostic_Init(
        &diagnostic_context);

    diagnostic_context.sensor = &sensor;
    diagnostic_context.fault = &fault;

    CAN_Frame request;
    CAN_Frame response;

    /* =================================================
       DIAGNOSTIC READ STATUS
       ================================================= */

    request.id = DIAG_REQUEST_ID;
    request.dlc = 1;
    request.data[0] = DIAG_READ_STATUS;

    printf(
        "\n--- DIAGNOSTIC REQUEST ---\n");

    printf(
        "CAN ID : 0x%03X\n",
        request.id);

    printf(
        "DLC    : %d\n",
        request.dlc);

    printf(
        "DATA   : %02X\n",
        request.data[0]);

    if (Diagnostic_ProcessRequest(
            &request,
            &response,
            &diagnostic_context) == CAN_OK)
    {
        printf(
            "\n--- DIAGNOSTIC RESPONSE ---\n");

        printf(
            "CAN ID : 0x%03X\n",
            response.id);

        printf(
            "DLC    : %d\n",
            response.dlc);

        printf(
            "DATA   : %02X %02X\n",
            response.data[0],
            response.data[1]);

        CAN_Send(&response);
    }

    /* =================================================
       DIAGNOSTIC READ DTC
       ================================================= */

    request.id = DIAG_REQUEST_ID;
    request.dlc = 1;
    request.data[0] = DIAG_READ_DTC;

    printf(
        "\n--- DIAGNOSTIC DTC REQUEST ---\n");

    printf(
        "CAN ID : 0x%03X\n",
        request.id);

    printf(
        "DLC    : %d\n",
        request.dlc);

    printf(
        "DATA   : %02X\n",
        request.data[0]);

    if (Diagnostic_ProcessRequest(
            &request,
            &response,
            &diagnostic_context) == CAN_OK)
    {
        printf(
            "\n--- DIAGNOSTIC DTC RESPONSE ---\n");

        printf(
            "CAN ID : 0x%03X\n",
            response.id);

        printf(
            "DLC    : %d\n",
            response.dlc);

        printf(
            "DATA   : %02X %02X\n",
            response.data[0],
            response.data[1]);

        CAN_Send(&response);
    }

    /* =================================================
       DIAGNOSTIC CLEAR FAULT
       ================================================= */

    request.id = DIAG_REQUEST_ID;
    request.dlc = 1;
    request.data[0] = DIAG_CLEAR_FAULT;

    printf(
        "\n--- DIAGNOSTIC CLEAR FAULT ---\n");

    if (Diagnostic_ProcessRequest(
            &request,
            &response,
            &diagnostic_context) == CAN_OK)
    {
        printf(
            "RESPONSE : %02X %02X\n",
            response.data[0],
            response.data[1]);
    }

    /* =================================================
       WATCHDOG KICK
       ================================================= */

    printf("\n--- WATCHDOG ---\n");

    if (Watchdog_Kick(&watchdog) == WATCHDOG_OK)
    {
        printf(
            "WATCHDOG : KICKED\n");
    }
    else
    {
        printf(
            "WATCHDOG : KICK FAILED\n");

        return 1;
    }

    /* =================================================
       COMPLETE
       ================================================= */

    printf(
        "\nECU cycle completed successfully.\n");

    return 0;
}
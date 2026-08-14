#include <stdio.h>
#include <stdint.h>

/* Drivers */
#include "../drivers/gpio.h"
#include "../drivers/adc.h"
#include "../drivers/can.h"
#include "../drivers/watchdog.h"

/* HAL */
#include "../hal/hal_gpio.h"
#include "../hal/hal_adc.h"

/* Sensors */
#include "../sensors/sensor_manager.h"

/* Services */
#include "../services/fault_manager.h"
#include "../services/diagnostic.h"
#include "../services/reset_manager.h"
#include "../services/dtc_manager.h"

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name, condition)            \
    do                                   \
    {                                    \
        tests_run++;                     \
        if (condition)                   \
        {                                \
            tests_passed++;              \
            printf("[PASS] %s\n", name); \
        }                                \
        else                             \
        {                                \
            tests_failed++;              \
            printf("[FAIL] %s\n", name); \
        }                                \
    } while (0)

int main(void)
{
    printf("\n");
    printf("========================================\n");
    printf("   AUTOMOTIVE ECU FIRMWARE TESTS\n");
    printf("========================================\n\n");

    /* =========================================================
       GPIO TESTS
       ========================================================= */

    printf("--- GPIO TESTS ---\n");

    GPIO_Init(0, 1);

    TEST(
        "GPIO initialization",
        1);

    GPIO_Write(
        0,
        1);

    TEST(
        "GPIO write HIGH",
        GPIO_Read(0) == 1);

    GPIO_Write(
        0,
        0);

    TEST(
        "GPIO write LOW",
        GPIO_Read(0) == 0);

    /* =========================================================
       ADC TESTS
       ========================================================= */

    printf("\n--- ADC TESTS ---\n");

    ADC_Init();

    TEST(
        "ADC initialization",
        1);

    /* =========================================================
       SENSOR TESTS
       ========================================================= */

    printf("\n--- SENSOR TESTS ---\n");

    Temperature_Sensor sensor;

    Sensor_Init();

    TEST(
        "Sensor initialization",
        1);

    Sensor_ReadTemperature(
        2500,
        &sensor);

    TEST(
        "Temperature conversion 2500 -> 25 C",
        sensor.temperature == 25.0f);

    TEST(
        "Normal temperature state",
        sensor.state == SENSOR_NORMAL);

    Sensor_ReadTemperature(
        8000,
        &sensor);

    TEST(
        "Temperature conversion 8000 -> 80 C",
        sensor.temperature == 80.0f);

    TEST(
        "Warning temperature state",
        sensor.state == SENSOR_WARNING);

    Sensor_ReadTemperature(
        10500,
        &sensor);

    TEST(
        "Temperature conversion 10500 -> 105 C",
        sensor.temperature == 105.0f);

    TEST(
        "Critical temperature state",
        sensor.state == SENSOR_CRITICAL);

    /* =========================================================
       FAULT MANAGER TESTS
       ========================================================= */

    printf("\n--- FAULT MANAGER TESTS ---\n");

    Fault_Status fault;

    Fault_Manager_Init(
        &fault);

    TEST(
        "Fault manager initialization",
        fault.code == FAULT_NONE);

    TEST(
        "Fault inactive after initialization",
        fault.active == 0);

    Fault_Manager_Update(
        25.0f,
        &fault);

    TEST(
        "No fault at normal temperature",
        fault.code == FAULT_NONE);

    TEST(
        "Fault inactive at normal temperature",
        fault.active == 0);

    Fault_Manager_Update(
        105.0f,
        &fault);

    TEST(
        "Over-temperature fault detected",
        fault.code == FAULT_OVER_TEMPERATURE);

    TEST(
        "Over-temperature fault active",
        fault.active == 1);

    TEST(
        "Fault description available",
        Fault_Manager_GetDescription(
            fault.code) != NULL);

    /* =========================================================
       DTC MANAGER TESTS
       ========================================================= */

    printf("\n--- DTC MANAGER TESTS ---\n");

    DTC_Manager_Init();

    TEST(
        "DTC initialization",
        DTC_Manager_GetStatus(
            DTC_OVER_TEMPERATURE) ==
            DTC_STATUS_CLEAR);

    DTC_Manager_Set(
        DTC_OVER_TEMPERATURE);

    TEST(
        "DTC set",
        DTC_Manager_GetStatus(
            DTC_OVER_TEMPERATURE) ==
            DTC_STATUS_ACTIVE);

    TEST(
        "DTC active check",
        DTC_Manager_IsActive(
            DTC_OVER_TEMPERATURE) == 1);

    DTC_Manager_Clear(
        DTC_OVER_TEMPERATURE);

    TEST(
        "DTC clear",
        DTC_Manager_GetStatus(
            DTC_OVER_TEMPERATURE) ==
            DTC_STATUS_CLEAR);

    TEST(
        "DTC inactive after clear",
        DTC_Manager_IsActive(
            DTC_OVER_TEMPERATURE) == 0);

    TEST(
        "DTC description available",
        DTC_Manager_GetDescription(
            DTC_OVER_TEMPERATURE) != NULL);

    /* =========================================================
       LOW VOLTAGE DTC
       ========================================================= */

    DTC_Manager_Set(
        DTC_LOW_VOLTAGE);

    TEST(
        "Low voltage DTC set",
        DTC_Manager_GetStatus(
            DTC_LOW_VOLTAGE) ==
            DTC_STATUS_ACTIVE);

    DTC_Manager_Clear(
        DTC_LOW_VOLTAGE);

    TEST(
        "Low voltage DTC clear",
        DTC_Manager_GetStatus(
            DTC_LOW_VOLTAGE) ==
            DTC_STATUS_CLEAR);

    /* =========================================================
       RESET MANAGER TESTS
       ========================================================= */

    printf("\n--- RESET MANAGER TESTS ---\n");

    Reset_Status reset;

    Reset_Manager_Init(
        &reset);

    TEST(
        "Reset manager initialization",
        reset.reset_count == 0);

    TEST(
        "Initial reset reason",
        reset.reason == RESET_POWER_ON);

    Reset_Manager_Record(
        &reset,
        RESET_WATCHDOG);

    TEST(
        "Watchdog reset recorded",
        reset.reason == RESET_WATCHDOG);

    TEST(
        "Reset count incremented",
        reset.reset_count == 1);

    Reset_Manager_Record(
        &reset,
        RESET_SOFTWARE);

    TEST(
        "Software reset recorded",
        reset.reason == RESET_SOFTWARE);

    TEST(
        "Reset count incremented again",
        reset.reset_count == 2);

    /* =========================================================
       WATCHDOG TESTS
       ========================================================= */

    printf("\n--- WATCHDOG TESTS ---\n");

    Watchdog watchdog;

    TEST(
        "Watchdog initialization",
        Watchdog_Init(
            &watchdog,
            1000) == WATCHDOG_OK);

    TEST(
        "Watchdog kick",
        Watchdog_Kick(
            &watchdog) == WATCHDOG_OK);

    /* =========================================================
       CAN TESTS
       ========================================================= */

    printf("\n--- CAN TESTS ---\n");

    TEST(
        "CAN initialization",
        CAN_Init() == CAN_OK);

    CAN_Frame frame;

    frame.id = 0x100;
    frame.dlc = 2;

    frame.data[0] = 0x01;
    frame.data[1] = 0x19;

    TEST(
        "CAN frame send",
        CAN_Send(&frame) == CAN_OK);

    /* =========================================================
       DIAGNOSTIC TESTS
       ========================================================= */

    printf("\n--- DIAGNOSTIC TESTS ---\n");

    Diagnostic_Context diagnostic_context;

    Diagnostic_Init(
        &diagnostic_context);

    diagnostic_context.sensor =
        &sensor;

    diagnostic_context.fault =
        &fault;

    CAN_Frame request;
    CAN_Frame response;

    /* READ STATUS */

    request.id =
        DIAG_REQUEST_ID;

    request.dlc = 1;

    request.data[0] =
        DIAG_READ_STATUS;

    TEST(
        "Diagnostic read status",
        Diagnostic_ProcessRequest(
            &request,
            &response,
            &diagnostic_context) == CAN_OK);

    /* READ DTC */

    request.id =
        DIAG_REQUEST_ID;

    request.dlc = 1;

    request.data[0] =
        DIAG_READ_DTC;

    TEST(
        "Diagnostic read DTC",
        Diagnostic_ProcessRequest(
            &request,
            &response,
            &diagnostic_context) == CAN_OK);

    /* CLEAR FAULT */

    request.id =
        DIAG_REQUEST_ID;

    request.dlc = 1;

    request.data[0] =
        DIAG_CLEAR_FAULT;

    TEST(
        "Diagnostic clear fault",
        Diagnostic_ProcessRequest(
            &request,
            &response,
            &diagnostic_context) == CAN_OK);

    /* =========================================================
       FINAL SUMMARY
       ========================================================= */

    printf("\n");
    printf("========================================\n");
    printf("           TEST SUMMARY\n");
    printf("========================================\n");

    printf(
        "Total Tests : %d\n",
        tests_run);

    printf(
        "Passed      : %d\n",
        tests_passed);

    printf(
        "Failed      : %d\n",
        tests_failed);

    printf("========================================\n");

    if (tests_failed == 0)
    {
        printf("ALL TESTS PASSED\n");
        return 0;
    }

    printf("SOME TESTS FAILED\n");

    return 1;
}
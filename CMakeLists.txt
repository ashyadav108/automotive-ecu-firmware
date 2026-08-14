cmake_minimum_required(VERSION 3.16)

project(AutomotiveECUFirmware
    VERSION 1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# =====================================================
# COMPILER WARNINGS
# =====================================================

if(MSVC)

    add_compile_options(/W4)

else()

    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
    )

endif()

# =====================================================
# INCLUDE DIRECTORIES
# =====================================================

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/mcu_sim
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/hal
    ${CMAKE_CURRENT_SOURCE_DIR}/sensors
    ${CMAKE_CURRENT_SOURCE_DIR}/services
)

# =====================================================
# ECU FIRMWARE
# =====================================================

add_executable(ecu

    # Application
    app/main.c

    # MCU simulation
    mcu_sim/mcu_registers.c

    # Drivers
    drivers/gpio.c
    drivers/uart.c
    drivers/adc.c
    drivers/can.c
    drivers/watchdog.c

    # Hardware Abstraction Layer
    hal/hal_gpio.c
    hal/hal_adc.c

    # Sensor layer
    sensors/sensor_manager.c

    # Application services
    services/fault_manager.c
    services/diagnostic.c
    services/reset_manager.c
    services/dtc_manager.c
)

# =====================================================
# FIRMWARE UNIT TESTS
# =====================================================

add_executable(firmware_tests

    # Test runner
    tests/test_runner.c

    # MCU simulation
    mcu_sim/mcu_registers.c

    # Drivers
    drivers/gpio.c
    drivers/adc.c
    drivers/can.c
    drivers/watchdog.c

    # Hardware Abstraction Layer
    hal/hal_adc.c

    # Sensor layer
    sensors/sensor_manager.c

    # Application services
    services/fault_manager.c
    services/diagnostic.c
    services/reset_manager.c
    services/dtc_manager.c
)

# =====================================================
# TESTING
# =====================================================

enable_testing()

add_test(
    NAME ECU_Firmware_Unit_Tests
    COMMAND firmware_tests
)

# =====================================================
# BUILD INFORMATION
# =====================================================

message(STATUS "========================================")
message(STATUS " Automotive ECU Firmware")
message(STATUS "========================================")
message(STATUS " C Standard : C11")
message(STATUS " Unit Tests : Enabled")
message(STATUS " Watchdog   : Enabled")
message(STATUS " Reset Mgr  : Enabled")
message(STATUS " DTC Manager: Enabled")
message(STATUS "========================================")
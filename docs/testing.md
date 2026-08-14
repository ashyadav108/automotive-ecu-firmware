# Automotive ECU Firmware Testing

## 1. Overview

The project uses CMake for building and CTest for executing the automated test suite.

Unit tests are implemented in:

```text
tests/test_runner.c
```

The tests validate the main ECU software modules including GPIO, ADC, sensor processing, fault management, DTC management, reset management, watchdog, CAN, and diagnostics.

## 2. Testing Architecture

The test environment is:

```text
test_runner.c
     |
     +---- GPIO Driver
     +---- ADC Driver
     +---- CAN Driver
     +---- Watchdog Driver
     |
     +---- GPIO HAL
     +---- ADC HAL
     +---- CAN HAL
     |
     +---- Sensor Manager
     +---- Fault Manager
     +---- DTC Manager
     +---- Diagnostic Manager
     +---- Reset Manager
     |
     v
Simulated MCU Registers
```

## 3. Test Source

The main test file is:

```text
tests/test_runner.c
```

The test runner validates the behavior of the firmware modules without requiring physical MCU hardware.

## 4. Tested Modules

### GPIO

Implementation:

```text
drivers/gpio.c
drivers/gpio.h

hal/hal_gpio.c
hal/hal_gpio.h
```

Tests include:

* GPIO initialization
* GPIO output HIGH
* GPIO output LOW

### ADC

Implementation:

```text
drivers/adc.c
drivers/adc.h

hal/hal_adc.c
hal/hal_adc.h
```

Tests include:

* ADC initialization

### Sensor Manager

Implementation:

```text
sensors/sensor_manager.c
sensors/sensor_manager.h
```

Tests include:

* Sensor initialization
* Temperature conversion
* Normal temperature state
* Warning temperature state
* Critical temperature state

Test examples:

```text
2500 ADC -> 25°C
8000 ADC -> 80°C
10500 ADC -> 105°C
```

### Fault Manager

Implementation:

```text
services/fault_manager.c
services/fault_manager.h
```

Tests include:

* Fault Manager initialization
* Initial fault state
* Normal temperature
* Over-temperature detection
* Fault activation
* Fault description

### DTC Manager

Implementation:

```text
services/dtc_manager.c
services/dtc_manager.h
```

Tests include:

* DTC initialization
* DTC set
* DTC active check
* DTC clear
* DTC inactive check
* DTC description
* Low-voltage DTC operations

### Reset Manager

Implementation:

```text
services/reset_manager.c
services/reset_manager.h
```

Tests include:

* Reset Manager initialization
* Initial reset reason
* Watchdog reset recording
* Reset count increment
* Software reset recording
* Reset count update

### Watchdog

Implementation:

```text
drivers/watchdog.c
drivers/watchdog.h
```

Tests include:

* Watchdog initialization
* Watchdog kick

The watchdog implementation also supports simulated timer progression and timeout detection.

### CAN

Implementation:

```text
drivers/can.c
drivers/can.h

hal/hal_can.c
hal/hal_can.h
```

Tests include:

* CAN initialization
* CAN frame transmission

### Diagnostic Manager

Implementation:

```text
services/diagnostic.c
services/diagnostic.h
```

Tests include:

* Diagnostic status read
* Diagnostic DTC read
* Diagnostic fault clear

## 5. Simulated Hardware

The simulated hardware is implemented in:

```text
mcu_sim/mcu_registers.c
mcu_sim/mcu_registers.h
```

It provides simulated peripheral registers for:

* GPIO
* UART
* ADC

This allows the firmware to be tested on a PC.

## 6. Build Configuration

The project uses:

```text
CMake
C11
GCC
CTest
```

Configure the project:

```bash
cmake -S . -B build
```

## 7. Build Unit Tests

Build the test executable:

```bash
cmake --build build --target firmware_tests
```

A successful build ends with:

```text
[100%] Built target firmware_tests
```

## 8. Run Tests

Run the complete test suite:

```bash
ctest --test-dir build --output-on-failure
```

Latest verified result:

```text
1/1 Test #1: ECU_Firmware_Unit_Tests .......... Passed

100% tests passed, 0 tests failed
```

The complete test runner contains:

```text
Total Tests : 39
Passed      : 39
Failed      : 0
```

## 9. ECU Build

The main ECU application is implemented in:

```text
app/main.c
```

Build it using:

```bash
cmake --build build --target ecu
```

A successful build produces:

```text
build/ecu
```

## 10. ECU Functional Testing

The ECU can be executed with different simulated ADC values.

### Normal Condition

```bash
./build/ecu 2500
```

Expected:

```text
TEMP     : 25.00 C
STATUS   : NORMAL
FAULT CODE: 0x00
DTC STATUS: CLEAR
```

### Warning Condition

```bash
./build/ecu 8000
```

Expected:

```text
TEMP     : 80.00 C
STATUS   : WARNING
FAULT CODE: 0x01
```

### Critical Condition

```bash
./build/ecu 10500
```

Expected:

```text
TEMP     : 105.00 C
STATUS   : CRITICAL
FAULT CODE: 0x02
DTC STATUS: ACTIVE
```

## 11. Test Summary

The current automated test suite covers the major ECU software components:

```text
GPIO
ADC
Sensor Manager
Fault Manager
DTC Manager
Reset Manager
Watchdog
CAN
Diagnostic Manager
```

The latest verified CTest execution completed successfully with:

```text
100% tests passed
0 tests failed
```

## 12. Testing Goal

The testing system demonstrates how embedded firmware modules can be automatically validated on a PC before deployment to physical automotive hardware.

The combination of CMake, CTest, simulated MCU registers, modular drivers, HAL modules, and service-level tests provides a foundation for further firmware verification and CI/CD automation.

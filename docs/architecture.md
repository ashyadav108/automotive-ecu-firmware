# Automotive ECU Firmware Architecture

## 1. Overview

This project implements a simulated Automotive ECU firmware using C and CMake.

The firmware demonstrates a layered embedded software architecture consisting of:

* Application
* Services
* Hardware Abstraction Layer (HAL)
* Peripheral Drivers
* Simulated MCU Registers

The architecture allows the firmware to be developed and tested on a PC without physical automotive hardware.

## 2. Software Architecture

```text
Application
    |
    v
Services
    |
    +---- Sensor Manager
    +---- Fault Manager
    +---- DTC Manager
    +---- Diagnostic Manager
    +---- Reset Manager
    |
    v
HAL
    |
    +---- GPIO HAL
    +---- ADC HAL
    +---- CAN HAL
    |
    v
Drivers
    |
    +---- GPIO
    +---- UART
    +---- ADC
    +---- CAN
    +---- Watchdog
    |
    v
Simulated MCU Registers
```

## 3. Application Layer

The application is implemented in:

```text
app/main.c
```

The application coordinates the complete ECU execution cycle.

The main sequence is:

1. Read the simulated ADC value.
2. Initialize CAN.
3. Initialize the watchdog.
4. Initialize the reset manager.
5. Initialize GPIO.
6. Initialize UART.
7. Initialize ADC.
8. Read and process the sensor.
9. Convert ADC input to temperature.
10. Determine sensor state.
11. Update the Fault Manager.
12. Update the DTC Manager.
13. Transmit the sensor CAN frame.
14. Process diagnostic CAN requests.
15. Clear the fault through diagnostics.
16. Kick the watchdog.
17. Complete the ECU cycle.

## 4. Services Layer

The `services/` directory contains the higher-level ECU software modules.

### Fault Manager

Files:

```text
services/fault_manager.c
services/fault_manager.h
```

The Fault Manager evaluates the sensor condition and determines whether a fault is active.

### DTC Manager

Files:

```text
services/dtc_manager.c
services/dtc_manager.h
```

The DTC Manager manages Diagnostic Trouble Codes.

It supports operations including:

* Initialization
* Setting a DTC
* Checking DTC status
* Clearing a DTC
* Retrieving DTC information

### Diagnostic Manager

Files:

```text
services/diagnostic.c
services/diagnostic.h
```

The Diagnostic Manager processes diagnostic CAN requests and generates responses.

Supported operations include:

* Read ECU status
* Read DTC
* Clear fault

### Reset Manager

Files:

```text
services/reset_manager.c
services/reset_manager.h
```

The Reset Manager maintains:

* Reset reason
* Reset count
* Power-on reset information
* Watchdog reset information
* Software reset information

## 5. Sensor Manager

Files:

```text
sensors/sensor_manager.c
sensors/sensor_manager.h
```

The Sensor Manager converts the simulated ADC input into temperature and determines the corresponding temperature state.

Example values:

|   ADC | Temperature | State    |
| ----: | ----------: | -------- |
|  2500 |        25°C | NORMAL   |
|  8000 |        80°C | WARNING  |
| 10500 |       105°C | CRITICAL |

## 6. Hardware Abstraction Layer

The HAL is implemented in the `hal/` directory.

The project contains:

```text
hal/hal_gpio.c
hal/hal_gpio.h

hal/hal_adc.c
hal/hal_adc.h

hal/hal_can.c
hal/hal_can.h
```

### GPIO HAL

The GPIO HAL provides an abstraction for GPIO operations.

### ADC HAL

The ADC HAL provides an abstraction for ADC operations.

### CAN HAL

The CAN HAL provides an abstraction for CAN communication.

The CAN software path is:

```text
Application / Services
        |
        v
      CAN HAL
        |
        v
    CAN Driver
        |
        v
Simulated Hardware
```

This separation helps keep higher-level firmware code independent from low-level peripheral implementation.

## 7. Peripheral Drivers

The `drivers/` directory contains the low-level peripheral implementations.

### GPIO Driver

Files:

```text
drivers/gpio.c
drivers/gpio.h
```

Provides GPIO initialization and digital output operations.

### UART Driver

Files:

```text
drivers/uart.c
drivers/uart.h
```

Provides UART initialization and baud-rate configuration.

The simulated ECU uses:

```text
115200 baud
```

### ADC Driver

Files:

```text
drivers/adc.c
drivers/adc.h
```

Provides ADC initialization and simulated ADC operations.

### CAN Driver

Files:

```text
drivers/can.c
drivers/can.h
```

Provides:

* CAN initialization
* CAN frame transmission
* CAN frame validation

The simulated CAN frame supports up to 8 data bytes.

### Watchdog Driver

Files:

```text
drivers/watchdog.c
drivers/watchdog.h
```

Provides:

* Watchdog initialization
* Watchdog kick
* Simulated timer progression
* Timeout detection
* Expiration status

## 8. Simulated MCU

Files:

```text
mcu_sim/mcu_registers.c
mcu_sim/mcu_registers.h
```

The simulated MCU provides software representations of peripheral registers.

Currently simulated peripherals include:

* GPIO
* UART
* ADC

The driver layer interacts with these simulated registers instead of physical MCU hardware.

## 9. Data Flow

The main sensor-processing flow is:

```text
Simulated ADC Input
        |
        v
ADC Driver / HAL
        |
        v
Sensor Manager
        |
        v
Temperature
        |
        v
Fault Manager
        |
        v
DTC Manager
        |
        +---------> CAN
        |
        +---------> Diagnostic Manager
```

## 10. Build System

The project uses:

```text
CMake
C11
GCC
```

The main build targets are:

```text
ecu
firmware_tests
```

Configuration:

```bash
cmake -S . -B build
```

ECU build:

```bash
cmake --build build --target ecu
```

Test build:

```bash
cmake --build build --target firmware_tests
```

## 11. Testing

Unit tests are implemented in:

```text
tests/test_runner.c
```

The test suite is executed using CTest.

```bash
ctest --test-dir build --output-on-failure
```

The latest verified result is:

```text
Total Tests : 39
Passed      : 39
Failed      : 0

100% tests passed
```

## 12. Architecture Summary

The project follows a modular architecture where:

```text
Application
    |
Services
    |
HAL
    |
Drivers
    |
Simulated MCU
```

Each layer has a defined responsibility, making the project easier to understand, test, maintain, and extend.

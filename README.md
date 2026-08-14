# Automotive ECU Firmware

A modular C-based Automotive ECU firmware project developed using **CMake** and tested with **CTest**. The project simulates MCU peripherals and demonstrates embedded automotive software concepts including GPIO, ADC, UART, CAN communication, sensor processing, fault management, DTC handling, diagnostics, watchdog supervision, and reset management.

## Overview

This project implements a simplified Automotive Electronic Control Unit (ECU) running in a simulated MCU environment.

The firmware is organized into multiple layers:

```text
Application
    |
    v
Services
    |
    +-- Sensor Manager
    +-- Fault Manager
    +-- DTC Manager
    +-- Diagnostic Manager
    +-- Reset Manager
    |
    v
HAL
    |
    +-- GPIO HAL
    +-- ADC HAL
    +-- CAN HAL
    |
    v
Drivers
    |
    +-- GPIO
    +-- UART
    +-- ADC
    +-- CAN
    +-- Watchdog
    |
    v
Simulated MCU Registers
```

The project can be built and tested on a PC using GCC and CMake without physical automotive hardware.

## Features

* Modular embedded C firmware architecture
* C11 standard
* CMake build system
* CTest-based unit testing
* Simulated MCU registers
* GPIO driver
* UART driver
* ADC driver
* CAN driver
* Watchdog driver
* GPIO HAL
* ADC HAL
* CAN HAL
* Temperature sensor processing
* Fault detection and management
* Diagnostic Trouble Code (DTC) management
* CAN-based diagnostic communication
* ECU reset management
* PC-based firmware simulation

## Project Structure

```text
automotive-ecu-firmware/
│
├── app/
│   ├── main.c
│   └──
│
├── drivers/
│   ├── adc.c
│   ├── adc.h
│   ├── can.c
│   ├── can.h
│   ├── gpio.c
│   ├── gpio.h
│   ├── uart.c
│   ├── uart.h
│   ├── watchdog.c
│   └── watchdog.h
│
├── hal/
│   ├── hal_adc.c
│   ├── hal_adc.h
│   ├── hal_can.c
│   ├── hal_can.h
│   ├── hal_gpio.c
│   └── hal_gpio.h
│
├── mcu_sim/
│   ├── mcu_registers.c
│   └── mcu_registers.h
│
├── sensors/
│   ├── sensor_manager.c
│   └── sensor_manager.h
│
├── services/
│   ├── diagnostic.c
│   ├── diagnostic.h
│   ├── dtc_manager.c
│   ├── dtc_manager.h
│   ├── fault_manager.c
│   ├── fault_manager.h
│   ├── reset_manager.c
│   └── reset_manager.h
│
├── tests/
│   └── test_runner.c
│
├── docs/
│   ├── architecture.md
│   ├── diagnostics.md
│   └── testing.md
│
├── .gitignore
├── CMakeLists.txt
└── README.md
```

The `.h` files contain module interfaces, data types, constants, and function declarations, while the `.c` files contain the corresponding implementations.

## Temperature Monitoring

The simulated ADC input is converted into a temperature value by the Sensor Manager.

Example conditions tested by the firmware:

| ADC Input | Temperature | State    | Fault               |
| --------: | ----------: | -------- | ------------------- |
|      2500 |        25°C | NORMAL   | No fault            |
|      8000 |        80°C | WARNING  | Temperature warning |
|     10500 |       105°C | CRITICAL | Over temperature    |

At the critical temperature condition, the over-temperature DTC becomes active.

## Fault and DTC Management

The temperature processing flow is:

```text
ADC
 |
 v
Sensor Manager
 |
 v
Temperature State
 |
 v
Fault Manager
 |
 v
DTC Manager
 |
 v
DTC Status
```

The Fault Manager determines the current fault condition.

The DTC Manager handles setting, checking, and clearing Diagnostic Trouble Codes.

## CAN Communication

CAN communication is implemented through the CAN driver and CAN HAL.

Relevant files are:

```text
drivers/can.c
drivers/can.h

hal/hal_can.c
hal/hal_can.h
```

The normal sensor CAN frame uses:

```text
CAN ID : 0x100
DLC    : 2
```

The first data byte contains the sensor state and the second data byte contains the temperature value.

## Diagnostic CAN Communication

Diagnostic communication uses:

```text
Request ID  : 0x700
Response ID : 0x708
```

Supported diagnostic operations include:

* Read ECU status
* Read DTC
* Clear fault

The diagnostic implementation is located in:

```text
services/diagnostic.c
services/diagnostic.h
```

Example diagnostic request:

```text
CAN ID : 0x700
DLC    : 1
DATA   : 05
```

Example response for an active over-temperature DTC:

```text
CAN ID : 0x708
DLC    : 2
DATA   : 05 02
```

## Watchdog

The watchdog is implemented in:

```text
drivers/watchdog.c
drivers/watchdog.h
```

It supports:

* Initialization
* Watchdog kick
* Simulated time progression
* Timeout detection

The application initializes and periodically kicks the watchdog during the ECU execution cycle.

## Reset Management

Reset management is implemented in:

```text
services/reset_manager.c
services/reset_manager.h
```

The Reset Manager tracks:

* Reset reason
* Reset count
* Power-on reset
* Watchdog reset
* Software reset

## Hardware Abstraction Layer

The project contains three HAL modules:

```text
hal/hal_gpio.c
hal/hal_gpio.h

hal/hal_adc.c
hal/hal_adc.h

hal/hal_can.c
hal/hal_can.h
```

The HAL layer provides an abstraction between higher-level firmware modules and the peripheral drivers.

## Simulated MCU

The simulated MCU is implemented in:

```text
mcu_sim/mcu_registers.c
mcu_sim/mcu_registers.h
```

The current simulated registers include:

* GPIO registers
* UART registers
* ADC registers

This allows the drivers to operate in a PC-based simulated hardware environment.

## Building the Project

### Requirements

* GCC
* CMake
* CTest
* Linux or WSL
* Git

### Configure

```bash
cmake -S . -B build
```

### Build ECU

```bash
cmake --build build --target ecu
```

### Build Unit Tests

```bash
cmake --build build --target firmware_tests
```

## Running the ECU

The ECU executable accepts a simulated ADC value as a command-line argument.

### Normal Temperature

```bash
./build/ecu 2500
```

Expected result:

```text
TEMP     : 25.00 C
STATUS   : NORMAL
FAULT CODE: 0x00
DTC STATUS: CLEAR
```

### Warning Temperature

```bash
./build/ecu 8000
```

Expected result:

```text
TEMP     : 80.00 C
STATUS   : WARNING
FAULT CODE: 0x01
```

### Critical Temperature

```bash
./build/ecu 10500
```

Expected result:

```text
TEMP     : 105.00 C
STATUS   : CRITICAL
FAULT CODE: 0x02
DTC STATUS: ACTIVE
```

## Running Tests

Build the test executable:

```bash
cmake --build build --target firmware_tests
```

Run the test suite:

```bash
ctest --test-dir build --output-on-failure
```

Latest verified result:

```text
1/1 Test #1: ECU_Firmware_Unit_Tests .......... Passed

100% tests passed
0 tests failed
```

The test runner is implemented in:

```text
tests/test_runner.c
```

## Testing Coverage

The test suite covers:

* GPIO initialization and output
* ADC initialization
* Temperature conversion
* Sensor state detection
* Fault detection
* DTC activation
* DTC clearing
* Reset management
* Watchdog operation
* CAN initialization
* CAN frame transmission
* Diagnostic status request
* Diagnostic DTC request
* Diagnostic fault clearing

The complete test suite contains **39 tests**.

## Documentation

Additional project documentation is available in `docs/`:

* `docs/architecture.md` — Software architecture and module organization
* `docs/diagnostics.md` — CAN communication, diagnostics, and DTC handling
* `docs/testing.md` — Build process and testing information

## Technologies Used

| Category          | Technology           |
| ----------------- | -------------------- |
| Language          | C                    |
| Standard          | C11                  |
| Build System      | CMake                |
| Testing           | CTest                |
| Compiler          | GCC                  |
| Communication     | CAN, UART            |
| Hardware Concepts | GPIO, ADC, Watchdog  |
| Diagnostics       | DTC, CAN Diagnostics |
| Development       | VS Code, Linux/WSL   |

## Project Goals

This project was developed to gain practical experience with:

* Embedded C programming
* Automotive ECU architecture
* Peripheral drivers
* Hardware abstraction
* CAN communication
* Sensor processing
* Fault handling
* Diagnostic systems
* Watchdog supervision
* Reset management
* Unit testing
* CMake-based development
* PC-based embedded firmware simulation

## Future Improvements

Possible future extensions include:

* Additional automotive sensors
* More diagnostic services
* UDS-style diagnostic services
* CAN message filtering
* EEPROM/NVM-based DTC storage
* Fault injection
* Static analysis
* Coding-standard checks
* Hardware-in-the-loop testing
* GitHub Actions CI/CD

## Author

**Ashish Yadav**

M.Tech — Communication System Engineering
National Institute of Technology, Jamshedpur

This project is intended as an embedded and automotive firmware portfolio project demonstrating modular C development, hardware abstraction, CAN communication, diagnostics, fault handling, watchdog supervision, reset management, and automated testing.

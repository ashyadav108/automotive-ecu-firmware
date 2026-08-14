# Automotive ECU Firmware

A modular C-based Automotive ECU firmware project developed with **CMake** and tested using **CTest**. The project simulates MCU peripherals and demonstrates core embedded automotive software concepts including sensor processing, fault management, DTC handling, diagnostics, CAN communication, watchdog supervision, and reset management.

## Overview

This project models a simplified automotive Electronic Control Unit (ECU) running on a simulated microcontroller environment.

The firmware is organized into multiple layers to resemble a typical embedded software architecture:

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

## Features

* Modular embedded C firmware architecture
* C11 standard
* CMake-based build system
* Simulated MCU peripheral registers
* GPIO driver
* UART driver
* ADC driver
* CAN communication
* Temperature sensor processing
* Fault detection and management
* Diagnostic Trouble Code (DTC) management
* Diagnostic CAN request/response handling
* ECU reset management
* Watchdog supervision
* Unit testing using CTest
* PC-based firmware simulation without physical hardware

## Project Structure

```text
automotive-ecu-firmware/
│
├── app/
│   └── main.c
│
├── drivers/
│   ├── gpio.c
│   ├── uart.c
│   ├── adc.c
│   ├── can.c
│   └── watchdog.c
│
├── hal/
│   ├── hal_gpio.c
│   └── hal_adc.c
│
├── mcu_sim/
│   ├── mcu_registers.c
│   └── mcu_registers.h
│
├── sensors/
│   └── sensor_manager.c
│
├── services/
│   ├── fault_manager.c
│   ├── diagnostic.c
│   ├── dtc_manager.c
│   └── reset_manager.c
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

## Temperature Monitoring

The simulated ADC input is converted into a temperature value.

Example test conditions:

| ADC Input | Temperature | State    | Fault               |
| --------: | ----------: | -------- | ------------------- |
|      2500 |        25°C | NORMAL   | No fault            |
|      8000 |        80°C | WARNING  | Temperature warning |
|     10500 |       105°C | CRITICAL | Over temperature    |

At the critical temperature condition, the over-temperature DTC becomes active.

## Fault and DTC Management

The firmware contains separate Fault Manager and DTC Manager modules.

The basic flow is:

```text
Temperature Sensor
       |
       v
Fault Manager
       |
       v
Fault Detected
       |
       v
DTC Manager
       |
       v
DTC ACTIVE
```

The DTC can subsequently be cleared through the diagnostic interface.

## CAN Communication

The project contains a simulated CAN interface.

The normal sensor CAN frame uses:

```text
CAN ID : 0x100
DLC    : 2
```

The first data byte represents the sensor state and the second data byte contains the temperature value.

### Diagnostic CAN

Diagnostic communication uses:

```text
Request ID  : 0x700
Response ID : 0x708
```

Supported diagnostic operations include:

* Read ECU status
* Read DTC
* Clear fault

Example DTC request:

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

The project includes a simulated watchdog timer.

The watchdog supports:

* Initialization
* Periodic kick
* Simulated time progression
* Timeout detection

The ECU application kicks the watchdog during the normal ECU execution cycle.

## Reset Management

The Reset Manager tracks ECU reset information including:

* Reset reason
* Reset count
* Power-on reset
* Watchdog reset
* Software reset

## Building the Project

### Requirements

* GCC
* CMake
* CTest
* Linux, WSL, or another GCC-compatible environment

### Configure

```bash
cmake -S . -B build
```

### Build the ECU

```bash
cmake --build build --target ecu
```

### Build Unit Tests

```bash
cmake --build build --target firmware_tests
```

## Running the ECU

The ECU executable accepts a simulated ADC value as a command-line argument.

### Normal temperature

```bash
./build/ecu 2500
```

Expected output includes:

```text
TEMP     : 25.00 C
STATUS   : NORMAL
FAULT CODE: 0x00
DTC STATUS: CLEAR
```

### Warning temperature

```bash
./build/ecu 8000
```

Expected output includes:

```text
TEMP     : 80.00 C
STATUS   : WARNING
FAULT CODE: 0x01
```

### Critical temperature

```bash
./build/ecu 10500
```

Expected output includes:

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

Run the complete test suite:

```bash
ctest --test-dir build --output-on-failure
```

Latest verified result:

```text
1/1 Test #1: ECU_Firmware_Unit_Tests .......... Passed

100% tests passed, 0 tests failed
```

## Testing Coverage

The unit test suite covers:

* GPIO initialization and output
* ADC initialization
* Temperature conversion
* Sensor state detection
* Fault detection
* DTC activation and clearing
* Reset management
* Watchdog operation
* CAN initialization
* CAN frame transmission
* Diagnostic status request
* Diagnostic DTC request
* Diagnostic fault clearing

## Documentation

Additional documentation is available in the `docs/` directory:

* [`docs/architecture.md`](docs/architecture.md) — Software architecture and module organization
* [`docs/diagnostics.md`](docs/diagnostics.md) — CAN diagnostics, DTCs, and diagnostic services
* [`docs/testing.md`](docs/testing.md) — Unit and integration testing

## Technologies Used

| Category          | Technologies                          |
| ----------------- | ------------------------------------- |
| Language          | C                                     |
| Standard          | C11                                   |
| Build System      | CMake                                 |
| Testing           | CTest                                 |
| Compiler          | GCC                                   |
| Communication     | CAN, UART                             |
| Embedded Concepts | GPIO, ADC, Watchdog, DTC, Diagnostics |
| Development       | VS Code, Linux/WSL                    |

## Project Goals

This project was developed to gain practical experience with:

* Embedded C programming
* Automotive ECU software architecture
* Hardware abstraction
* Peripheral drivers
* CAN communication
* Fault and diagnostic systems
* Watchdog and reset handling
* Unit testing
* CMake-based embedded development
* Software modularity and maintainability

## Future Improvements

Possible future extensions include:

* UDS-style diagnostic services
* More CAN diagnostic messages
* EEPROM/NVM-based DTC storage
* Additional automotive sensors
* CAN message filtering
* More comprehensive fault injection
* Hardware-in-the-loop testing
* Static analysis and coding-standard checks
* CI/CD testing using GitHub Actions

## Author

**Ashish Yadav**

M.Tech — Communication System Engineering
National Institute of Technology, Jamshedpur

This project is intended as a practical embedded/automotive firmware portfolio project demonstrating modular C development, testing, diagnostics, and ECU software concepts.

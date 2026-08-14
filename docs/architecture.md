# Automotive ECU Firmware Architecture

## 1. Overview

This project implements a simulated Automotive ECU firmware using C and CMake.

The firmware is designed to demonstrate the basic software architecture used in an embedded automotive ECU, including hardware drivers, hardware abstraction, sensor processing, fault management, diagnostic communication, watchdog supervision, reset management, and DTC handling.

The project uses simulated MCU registers so that the firmware can be developed and tested on a PC without requiring physical automotive hardware.

## 2. Software Architecture

The firmware is organized into multiple software layers:

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
HAL (Hardware Abstraction Layer)
    |
    +---- GPIO HAL
    +---- ADC HAL
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

The application layer is implemented in `app/main.c`.

It coordinates the complete ECU cycle:

1. Initialize the watchdog.
2. Initialize the reset manager.
3. Initialize GPIO.
4. Initialize UART.
5. Initialize ADC.
6. Read the simulated sensor value.
7. Convert the ADC value into temperature.
8. Determine the sensor state.
9. Detect faults.
10. Update the corresponding DTC.
11. Send sensor information through CAN.
12. Process diagnostic requests.
13. Clear faults when requested.
14. Kick the watchdog.
15. Complete the ECU cycle.

## 4. Drivers

The `drivers/` directory contains low-level peripheral drivers.

### GPIO

The GPIO driver provides initialization and digital output control.

### UART

The UART driver provides UART initialization and baud-rate configuration.

The current simulated configuration uses:

```text
Baud Rate: 115200
```

### ADC

The ADC driver provides initialization and simulated ADC data handling.

### CAN

The CAN driver provides CAN initialization and CAN frame transmission.

### Watchdog

The watchdog driver provides:

* Watchdog initialization
* Watchdog kick
* Simulated time progression
* Timeout detection

## 5. Hardware Abstraction Layer

The `hal/` directory provides an abstraction between application-level software and low-level hardware drivers.

Currently the project contains HAL implementations for:

* GPIO
* ADC

This structure allows higher-level software to interact with hardware through a consistent interface.

## 6. Sensor Manager

The Sensor Manager is implemented in `sensors/`.

It converts the simulated ADC value into temperature and determines the temperature state.

The current tested examples are:

| ADC Value | Temperature | State    |
| --------- | ----------- | -------- |
| 2500      | 25°C        | NORMAL   |
| 8000      | 80°C        | WARNING  |
| 10500     | 105°C       | CRITICAL |

## 7. Fault Manager

The Fault Manager monitors the sensor state and determines whether an ECU fault is active.

The current temperature-related fault conditions include:

* No fault
* Temperature warning
* Over-temperature fault

When the temperature reaches the critical condition, the over-temperature fault becomes active.

## 8. DTC Manager

The DTC Manager handles Diagnostic Trouble Codes.

The current implementation supports DTC operations such as:

* Initialize DTC manager
* Set DTC
* Check DTC status
* Clear DTC
* Retrieve DTC information

For the critical temperature condition, the over-temperature DTC becomes active.

## 9. Diagnostic Manager

The Diagnostic Manager processes diagnostic CAN requests and generates CAN responses.

The simulated diagnostic communication uses:

```text
Request ID  : 0x700
Response ID : 0x708
```

The current diagnostic functions include:

* Read ECU status
* Read DTC
* Clear fault

## 10. Reset Manager

The Reset Manager tracks ECU reset information.

The implementation records:

* Reset reason
* Reset count

The initial reset condition is represented as a power-on reset.

The manager also supports recording subsequent reset events such as watchdog and software resets.

## 11. Watchdog Supervision

The watchdog is used to monitor ECU software execution.

The application periodically calls the watchdog kick function.

If the simulated watchdog timer reaches its configured timeout without being kicked, the watchdog reports a timeout condition.

This demonstrates the basic concept of watchdog supervision used in embedded systems.

## 12. Simulated MCU

The `mcu_sim/` directory contains simulated MCU peripheral registers.

The project currently models:

* GPIO registers
* UART registers
* ADC registers

This allows the firmware drivers to interact with simulated hardware registers during PC-based development and testing.

## 13. Build System

The project uses CMake with the C11 standard.

The project provides separate build targets for:

```text
ecu
firmware_tests
```

Example build commands:

```bash
cmake -S . -B build
cmake --build build --target ecu
cmake --build build --target firmware_tests
```

## 14. Testing

Unit tests are implemented in the `tests/` directory.

The project uses CTest to execute the firmware test suite.

The current test suite contains 39 tests, covering major ECU software components.

The latest verified result is:

```text
100% tests passed
0 tests failed
```

## 15. Project Goal

The goal of this project is to demonstrate practical embedded firmware development concepts including modular C programming, driver development, hardware abstraction, sensor processing, fault handling, diagnostics, watchdog supervision, reset management, CAN communication, unit testing, and CMake-based builds.

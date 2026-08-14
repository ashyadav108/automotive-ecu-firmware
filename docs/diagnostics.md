# Automotive ECU Diagnostic Communication

## 1. Overview

The ECU firmware contains a simulated diagnostic communication system based on CAN frames.

The Diagnostic Manager receives diagnostic requests, processes them, and generates a response frame.

The diagnostic communication implemented in this project demonstrates basic ECU diagnostic functions including ECU status reading, DTC reading, and fault clearing.

## 2. CAN Diagnostic IDs

The simulated diagnostic communication uses the following CAN identifiers:

| CAN ID  | Purpose             |
| ------- | ------------------- |
| `0x700` | Diagnostic request  |
| `0x708` | Diagnostic response |

## 3. Supported Diagnostic Services

The current implementation supports:

| Service            | Function                   |
| ------------------ | -------------------------- |
| `DIAG_READ_STATUS` | Read current ECU status    |
| `DIAG_READ_DTC`    | Read current DTC status    |
| `DIAG_CLEAR_FAULT` | Clear the active fault/DTC |

## 4. Read ECU Status

The tester sends a diagnostic request using CAN ID `0x700`.

Example:

```text
CAN ID : 0x700
DLC    : 1
DATA   : 01
```

The ECU processes the request and sends a response using CAN ID `0x708`.

Example for a normal ECU condition:

```text
CAN ID : 0x708
DLC    : 2
DATA   : 01 00
```

The first byte identifies the diagnostic service and the second byte represents the current status/fault information.

## 5. Read DTC

The tester can request the current Diagnostic Trouble Code status.

Example request:

```text
CAN ID : 0x700
DLC    : 1
DATA   : 05
```

The ECU responds using CAN ID `0x708`.

For an ECU with no active DTC:

```text
CAN ID : 0x708
DLC    : 2
DATA   : 05 00
```

For an over-temperature condition:

```text
CAN ID : 0x708
DLC    : 2
DATA   : 05 02
```

## 6. Over-Temperature DTC

When the temperature reaches the critical condition, the Fault Manager detects an over-temperature fault.

The DTC Manager then activates the corresponding over-temperature DTC.

Example:

```text
ADC VALUE: 10500
TEMP     : 105.00 C
STATUS   : CRITICAL
FAULT CODE: 0x02
FAULT     : Over Temperature
```

The DTC status becomes:

```text
DTC STATUS: ACTIVE
```

## 7. Clear Fault

The diagnostic system also supports clearing the current fault.

The tester sends the clear-fault service:

```text
CAN ID : 0x700
DLC    : 1
DATA   : 04
```

The ECU generates a diagnostic response indicating that the clear operation was processed.

Example:

```text
RESPONSE : 04 00
```

## 8. Diagnostic Flow

The diagnostic request processing follows this sequence:

```text
Tester
   |
   | CAN Request 0x700
   v
Diagnostic Manager
   |
   +---- Read ECU Status
   |
   +---- Read DTC
   |
   +---- Clear Fault
   |
   v
CAN Response 0x708
   |
   v
Tester
```

## 9. Example: Normal Condition

For an ADC input of `2500`:

```text
ADC VALUE: 2500
TEMP     : 25.00 C
STATUS   : NORMAL
FAULT CODE: 0x00
DTC STATUS: CLEAR
```

The diagnostic status response is:

```text
CAN ID : 0x708
DATA   : 01 00
```

## 10. Example: Warning Condition

For an ADC input of `8000`:

```text
ADC VALUE: 8000
TEMP     : 80.00 C
STATUS   : WARNING
FAULT CODE: 0x01
```

The diagnostic status response contains the corresponding warning fault code.

## 11. Example: Critical Condition

For an ADC input of `10500`:

```text
ADC VALUE: 10500
TEMP     : 105.00 C
STATUS   : CRITICAL
FAULT CODE: 0x02
DTC STATUS: ACTIVE
```

The DTC read response is:

```text
CAN ID : 0x708
DLC    : 2
DATA   : 05 02
```

## 12. Diagnostic Processing Sequence

The complete diagnostic sequence is:

1. ECU initializes the diagnostic context.
2. Sensor data is processed.
3. Fault Manager determines the fault condition.
4. DTC Manager updates the DTC state.
5. A diagnostic request is received.
6. Diagnostic Manager identifies the requested service.
7. ECU generates the appropriate response.
8. Response is transmitted through CAN.
9. A clear-fault request can reset the diagnostic fault state.

## 13. Current Implementation

The diagnostic functionality is implemented using the following project components:

```text
services/
├── diagnostic.c
├── diagnostic.h
├── dtc_manager.c
└── dtc_manager.h
```

The diagnostic system is integrated with the Sensor Manager, Fault Manager, DTC Manager, and CAN driver.

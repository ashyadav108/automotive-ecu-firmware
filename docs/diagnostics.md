# Automotive ECU Diagnostics

## 1. Overview

The project includes a simulated automotive diagnostic system based on CAN communication.

The diagnostic functionality is implemented using the CAN driver, CAN HAL, Diagnostic Manager, Fault Manager, and DTC Manager.

Relevant files are:

```text
drivers/can.c
drivers/can.h

hal/hal_can.c
hal/hal_can.h

services/diagnostic.c
services/diagnostic.h

services/fault_manager.c
services/fault_manager.h

services/dtc_manager.c
services/dtc_manager.h
```

## 2. Diagnostic Architecture

The diagnostic communication path is:

```text
CAN Request
     |
     v
CAN Driver / CAN HAL
     |
     v
Diagnostic Manager
     |
     +---- Read Status
     |
     +---- Read DTC
     |
     +---- Clear Fault
     |
     v
CAN Response
```

## 3. CAN Communication

The CAN peripheral is implemented in:

```text
drivers/can.c
drivers/can.h
```

The CAN abstraction is implemented in:

```text
hal/hal_can.c
hal/hal_can.h
```

The CAN driver provides:

* CAN initialization
* CAN frame transmission
* CAN frame validation

A CAN frame contains:

```text
CAN ID
DLC
Data
```

The maximum simulated CAN data length is 8 bytes.

## 4. Normal Sensor CAN Message

The ECU transmits sensor information using:

```text
CAN ID : 0x100
DLC    : 2
```

The frame format is:

```text
DATA[0] = Sensor State
DATA[1] = Temperature
```

For example, at 25°C:

```text
CAN ID : 0x100
DLC    : 2
DATA   : 00 19
```

At 80°C:

```text
CAN ID : 0x100
DLC    : 2
DATA   : 01 50
```

At 105°C:

```text
CAN ID : 0x100
DLC    : 2
DATA   : 02 69
```

## 5. Diagnostic CAN IDs

The simulated diagnostic communication uses:

```text
Request ID  : 0x700
Response ID : 0x708
```

The Diagnostic Manager is implemented in:

```text
services/diagnostic.c
services/diagnostic.h
```

## 6. Diagnostic Commands

The current diagnostic implementation supports:

```text
01 - Read ECU Status
05 - Read DTC
04 - Clear Fault
```

### Read ECU Status

Request:

```text
CAN ID : 0x700
DLC    : 1
DATA   : 01
```

Response:

```text
CAN ID : 0x708
DLC    : 2
DATA   : 01 XX
```

The second byte represents the current fault/status information.

## 7. Read DTC

Request:

```text
CAN ID : 0x700
DLC    : 1
DATA   : 05
```

Response:

```text
CAN ID : 0x708
DLC    : 2
DATA   : 05 XX
```

For an active over-temperature condition:

```text
DATA   : 05 02
```

## 8. Clear Fault

The clear-fault diagnostic request uses:

```text
CAN ID : 0x700
DLC    : 1
DATA   : 04
```

The Diagnostic Manager processes the request and returns a response.

The implementation is located in:

```text
services/diagnostic.c
services/diagnostic.h
```

## 9. Fault Management

Fault handling is implemented in:

```text
services/fault_manager.c
services/fault_manager.h
```

The temperature conditions include:

| Temperature | State    | Fault               |
| ----------: | -------- | ------------------- |
|        25°C | NORMAL   | No fault            |
|        80°C | WARNING  | Temperature warning |
|       105°C | CRITICAL | Over temperature    |

At the critical temperature, the over-temperature fault becomes active.

## 10. DTC Management

DTC functionality is implemented in:

```text
services/dtc_manager.c
services/dtc_manager.h
```

The DTC Manager supports:

* DTC initialization
* DTC setting
* DTC status checking
* DTC clearing
* DTC information retrieval

For the critical temperature condition, the over-temperature DTC becomes active.

## 11. Complete Diagnostic Flow

For an over-temperature condition:

```text
ADC Input
   |
   v
Sensor Manager
   |
   v
105°C / CRITICAL
   |
   v
Fault Manager
   |
   v
Over-Temperature Fault
   |
   v
DTC Manager
   |
   v
DTC ACTIVE
   |
   v
Diagnostic Manager
   |
   v
CAN Response
```

## 12. Example

Running:

```bash
./build/ecu 10500
```

produces the critical temperature condition:

```text
ADC VALUE: 10500
TEMP     : 105.00 C
STATUS   : CRITICAL
FAULT CODE: 0x02
FAULT     : Over Temperature
DTC STATUS: ACTIVE
```

The diagnostic DTC request then returns:

```text
CAN ID : 0x708
DLC    : 2
DATA   : 05 02
```

## 13. Diagnostic Test Coverage

Diagnostic functionality is tested by:

```text
tests/test_runner.c
```

The test suite verifies:

* Diagnostic status reading
* Diagnostic DTC reading
* Diagnostic fault clearing
* CAN initialization
* CAN frame transmission
* DTC activation
* DTC clearing

The latest complete test suite passes successfully.

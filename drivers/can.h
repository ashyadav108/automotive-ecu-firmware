#ifndef CAN_H
#define CAN_H

#include <stdint.h>

#define CAN_MAX_DATA_LENGTH 8

typedef enum
{
    CAN_OK = 0,
    CAN_ERROR = 1
} CAN_Status;

typedef struct
{
    uint32_t id;
    uint8_t dlc;
    uint8_t data[CAN_MAX_DATA_LENGTH];
} CAN_Frame;

CAN_Status CAN_Init(void);

CAN_Status CAN_Send(
    const CAN_Frame *frame
);

#endif
#include "can.h"
#include <stdio.h>
#include <stddef.h>

CAN_Status CAN_Init(void)
{
    printf("CAN initialized successfully\n");

    return CAN_OK;
}

CAN_Status CAN_Send(
    const CAN_Frame *frame)
{
    if (frame == NULL)
        return CAN_ERROR;

    if (frame->dlc > CAN_MAX_DATA_LENGTH)
        return CAN_ERROR;

    printf("\n--- CAN FRAME ---\n");

    printf("CAN ID : 0x%03X\n", frame->id);
    printf("DLC    : %u\n", frame->dlc);

    printf("DATA   : ");

    for (uint8_t i = 0; i < frame->dlc; i++)
    {
        printf("%02X ", frame->data[i]);
    }

    printf("\n");

    return CAN_OK;
}
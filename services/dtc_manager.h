#ifndef DTC_MANAGER_H
#define DTC_MANAGER_H

#include <stdint.h>

typedef enum
{
    DTC_STATUS_CLEAR = 0,
    DTC_STATUS_ACTIVE = 1,
    DTC_STATUS_STORED = 2
} DTC_Status;

typedef struct
{
    uint32_t code;
    DTC_Status status;
} DTC;

#define DTC_OVER_TEMPERATURE 0x0217
#define DTC_LOW_VOLTAGE 0x0562

void DTC_Manager_Init(void);

void DTC_Manager_Set(uint32_t code);

void DTC_Manager_Clear(uint32_t code);

void DTC_Manager_StoreActive(void);

uint8_t DTC_Manager_IsActive(uint32_t code);

DTC_Status DTC_Manager_GetStatus(uint32_t code);

const char *DTC_Manager_GetDescription(uint32_t code);

#endif
#include "dtc_manager.h"

static DTC dtcs[] =
    {
        {0x0217, DTC_STATUS_CLEAR},
        {0x0562, DTC_STATUS_CLEAR}};

#define DTC_COUNT \
    (sizeof(dtcs) / sizeof(dtcs[0]))

void DTC_Manager_Init(void)
{
    uint8_t i;

    for (i = 0; i < DTC_COUNT; i++)
    {
        dtcs[i].status = DTC_STATUS_CLEAR;
    }
}

void DTC_Manager_Set(uint32_t code)
{
    uint8_t i;

    for (i = 0; i < DTC_COUNT; i++)
    {
        if (dtcs[i].code == code)
        {
            dtcs[i].status = DTC_STATUS_ACTIVE;
            return;
        }
    }
}

void DTC_Manager_Clear(uint32_t code)
{
    uint8_t i;

    for (i = 0; i < DTC_COUNT; i++)
    {
        if (dtcs[i].code == code)
        {
            dtcs[i].status = DTC_STATUS_CLEAR;
            return;
        }
    }
}

void DTC_Manager_StoreActive(void)
{
    uint8_t i;

    for (i = 0; i < DTC_COUNT; i++)
    {
        if (dtcs[i].status == DTC_STATUS_ACTIVE)
        {
            dtcs[i].status = DTC_STATUS_STORED;
        }
    }
}

uint8_t DTC_Manager_IsActive(uint32_t code)
{
    return DTC_Manager_GetStatus(code) ==
           DTC_STATUS_ACTIVE;
}

DTC_Status DTC_Manager_GetStatus(uint32_t code)
{
    uint8_t i;

    for (i = 0; i < DTC_COUNT; i++)
    {
        if (dtcs[i].code == code)
        {
            return dtcs[i].status;
        }
    }

    return DTC_STATUS_CLEAR;
}

const char *DTC_Manager_GetDescription(uint32_t code)
{
    switch (code)
    {
    case 0x0217:
        return "Engine Over Temperature";

    case 0x0562:
        return "System Voltage Low";

    default:
        return "Unknown DTC";
    }
}
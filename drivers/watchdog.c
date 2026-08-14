#include <stddef.h>

#include "watchdog.h"

/* =====================================================
   WATCHDOG INITIALIZATION
   ===================================================== */

Watchdog_Status Watchdog_Init(
    Watchdog *watchdog,
    uint32_t timeout_ms)
{
    if (watchdog == NULL)
    {
        return WATCHDOG_ERROR;
    }

    if (timeout_ms == 0)
    {
        return WATCHDOG_ERROR;
    }

    watchdog->timeout_ms = timeout_ms;

    watchdog->elapsed_ms = 0;

    watchdog->enabled = 1;

    return WATCHDOG_OK;
}

/* =====================================================
   WATCHDOG KICK
   ===================================================== */

Watchdog_Status Watchdog_Kick(
    Watchdog *watchdog)
{
    if (watchdog == NULL)
    {
        return WATCHDOG_ERROR;
    }

    if (watchdog->enabled == 0)
    {
        return WATCHDOG_ERROR;
    }

    /*
     * Reset watchdog counter.
     */
    watchdog->elapsed_ms = 0;

    return WATCHDOG_OK;
}

/* =====================================================
   WATCHDOG TICK
   ===================================================== */

Watchdog_Status Watchdog_Tick(
    Watchdog *watchdog,
    uint32_t elapsed_ms)
{
    if (watchdog == NULL)
    {
        return WATCHDOG_ERROR;
    }

    if (watchdog->enabled == 0)
    {
        return WATCHDOG_ERROR;
    }

    /*
     * Advance simulated time.
     */
    watchdog->elapsed_ms += elapsed_ms;

    /*
     * Check timeout.
     */
    if (watchdog->elapsed_ms >=
        watchdog->timeout_ms)
    {
        return WATCHDOG_TIMEOUT;
    }

    return WATCHDOG_OK;
}

/* =====================================================
   WATCHDOG STATUS
   ===================================================== */

uint8_t Watchdog_IsExpired(
    const Watchdog *watchdog)
{
    if (watchdog == NULL)
    {
        return 0;
    }

    if (watchdog->enabled == 0)
    {
        return 0;
    }

    if (watchdog->elapsed_ms >=
        watchdog->timeout_ms)
    {
        return 1;
    }

    return 0;
}
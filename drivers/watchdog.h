#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdint.h>

/*
 * Simulated automotive watchdog timer.
 *
 * The watchdog must be periodically kicked by
 * the ECU application. If the timeout is reached,
 * the watchdog reports a timeout condition.
 */

typedef enum
{
    WATCHDOG_OK = 0,
    WATCHDOG_ERROR = -1,
    WATCHDOG_TIMEOUT = 1

} Watchdog_Status;

typedef struct
{
    uint32_t timeout_ms;
    uint32_t elapsed_ms;
    uint8_t enabled;

} Watchdog;

/*
 * Initialize watchdog.
 */
Watchdog_Status Watchdog_Init(
    Watchdog *watchdog,
    uint32_t timeout_ms);

/*
 * Reset watchdog timer.
 */
Watchdog_Status Watchdog_Kick(
    Watchdog *watchdog);

/*
 * Advance simulated watchdog time.
 */
Watchdog_Status Watchdog_Tick(
    Watchdog *watchdog,
    uint32_t elapsed_ms);

/*
 * Check whether watchdog has expired.
 */
uint8_t Watchdog_IsExpired(
    const Watchdog *watchdog);

#endif
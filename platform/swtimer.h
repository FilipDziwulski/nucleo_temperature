/**
 * @file swtimer.h
 *
 * Software timer module. Provides simple polled SW timers,
 * based on the ARM systick count.
 */

#ifndef SWTIMER_H
#define SWTIMER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * struct represent a timer. Users should not access these fields
 * directly, instead, use the swtimer_ functions.
 */
typedef struct swtimer_s {
    bool running;
    uint32_t start;
    uint32_t period;
} swtimer_t;

/**
 * Start the timer for a given period.
 *
 * @param period The period of the timer in systicks.
 */
void swtimer_Start(swtimer_t *timer, uint32_t period);

/**
 * Restart a timer. It will expire at time of last expiration + the period.
 */
void swtimer_Restart(swtimer_t *timer);

/**
 * Check a swtimer for expiration. Returns true iff the timer's period
 * has expired since the timer was started.
 */
bool swtimer_Expired(swtimer_t *timer);

#endif /* SWTIMER_H */

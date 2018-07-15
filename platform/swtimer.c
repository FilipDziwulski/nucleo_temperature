/**
 * @file swtimer.c
 *
 * Software timer implementation
 */

#include "swtimer.h"

#include "stm32f0xx_hal.h"

void swtimer_Start(swtimer_t *timer, uint32_t period)
{
    timer->period = period;
    timer->start = HAL_GetTick();
    timer->running = true;
}

void swtimer_Restart(swtimer_t *timer)
{
    timer->start += timer->period;
    timer->running = true;
}

bool swtimer_Expired(swtimer_t *timer)
{
    if (!timer->running) {
        return true;
    }

    uint32_t now = HAL_GetTick();
    if ((now - timer->start) >= timer->period) {
        timer->running = false;
        return true;
    }

    return false;
}

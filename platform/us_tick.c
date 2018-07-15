/**
 * Functions for dealing with time in microseconds.
 */


#include "us_tick.h"
#include "stm32f0xx_hal.h"

static uint32_t ticks_per_us;

void us_tick_init(void)
{
    ticks_per_us = HAL_RCC_GetSysClockFreq() / 1000000;
}

uint32_t get_us(void)
{
    uint32_t ms = HAL_GetTick();
    uint32_t tick = SysTick->VAL;

    // systick may have rolled over between checks
    // if it did, try again.
    if (ms != HAL_GetTick()) {
        ms = HAL_GetTick();
        tick = SysTick->VAL;
    }

    // SysTick->VAL is a downcounter, reaching zero every ms
    uint32_t ticks_per_ms = ticks_per_us * 1000;

    return (ms * 1000) + (ticks_per_ms - tick) / ticks_per_us;
}

void delay_us(uint16_t us)
{
    uint32_t start = get_us();
    while (get_us() - start < (uint32_t) us);
}

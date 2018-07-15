/**
 * Functions for measuring time in microseconds.
 */

#ifndef US_TICK_H
#define US_TICK_H

#include <stdint.h>

/**
 * Initialize nanosecond timekeeping. Typically, this is called
 * by sys_clk_config() - most apps will not call this directly.
 * (Though calling it extra times won't hurt).
 */
void us_tick_init(void);

/**
 * Get the current microsecond count, based on the systick counter.
 */
uint32_t get_us(void);

/**
 * Busywait the specified number of milliseconds
 */
void delay_us(uint16_t us);


#endif /* US_TICK_H */

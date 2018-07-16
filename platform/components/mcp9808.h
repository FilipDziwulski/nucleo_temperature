/**
 * @file mcp9808.h
 *
 * mcp9808 temperature sensor driver.
 */



#ifndef MPC9808_H
#define MPC9808_H

#include <stdint.h>
#include <stdbool.h>

#include "i2c_gpio.h"


/**
 * Initialize the temperature sensor. TODO: Configure registers
 */
void mcp9808_init(i2c_bus_t *bus);

/**
 * Retrieves current temp reading from mcp9808
 */
bool mcp9808_getTemp(i2c_bus_t *bus, float *temp);

#endif /* MCP9808_H */
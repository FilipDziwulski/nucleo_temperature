/**
 * @file i2c_gpio.h
 *
 * Bit-banged I2C driver, using STM32 GPIO peripheral.
 * Supports master mode reads and writes. Does not support:
 *   - Slave clock stretching. This is ok, since bit-banged access is so slow.
 *   - Slave mode. We're master-only.
 *   - Little-Indian byte transfer
 *   - 10-bit device addresses.
 *
 * This driver is not reentrant: do not use it from interrupts.
 *
 * Addresses are right-justified. That is, a device with address
 * 0011000 should be addressed as 0x18.
 */

#ifndef I2C_GPIO_H
#define I2C_GPIO_H

#include <stdint.h>
#include <stdbool.h>

#include "stm32f0xx_hal.h"

/** Parameters representing a GPIO i2c bus. */
typedef struct i2c_bus_s {
    uint16_t tick;          /**< Half the desired clock rate in us. */
    GPIO_TypeDef *sda_port; /**< GPIO port for SDA                  */
    GPIO_TypeDef *scl_port; /**< GPIO port to use for SCL           */
    uint16_t sda_pin;       /**< GPIO pin number for SDA            */
    uint16_t scl_pin;       /**< GPIO pin number for SCL            */
} i2c_bus_t;

/**
 * Initialize a GPIO i2c bus. Configures GPIO, sets bus to idle state.
 */
void i2c_Init(i2c_bus_t *bus);

/**
 * Write N bytes to a given i2c address.
 *
 * Generates a start condition, writes the address then
 * the data bytes, followed by a stop.
 *
 * @param dev The i2c bus to use.
 * @param addr The i2c address to use.
 * @param buf Pointer to data to send.
 * @param count Number of bytes to send.
 *
 * @return On success, returns 0. On nack, returns N,
 *         where N is the byte that was nack'd. If the
 *         address byte is nacked, returns -1
 */
ssize_t i2c_Write(i2c_bus_t *bus, uint8_t addr, const uint8_t *buf, size_t count);

/**
 * Read N bytes from an i2c address.
 *
 * The process is:
 *  - Generate a start condition
 *  - Send the address byte
 *  - Read N bytes-1 bytes, transmitting an ack after each.
 *  - Read the last byte and transmit an ack after it.
 *  - Generate a stop condition.
 *
 * @param i2c bus to use
 * @param addr device address to read from.
 * @param buf pointer to write received data to.
 * @param count Number of bytes to read.
 *
 * @return On success, return 0. If address byte is nack'd, return -1.
 */
ssize_t i2c_Read(i2c_bus_t *bus, uint8_t addr, uint8_t *buf, size_t count);

#endif /* I2C_GPIO_H */

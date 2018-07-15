/**
 * Provide a serial port for printf(), etc. output.
 *
 * To use, simply include serial_io.c in your application, and call
 * serial_io_init() before your first use of printf(). The implementation
 * includes a definition for _write() which will override the dummy version
 * provided by newlib.
 *
 * @note Using iprintf() instead of printf() will avoid pulling
 * in the floating point libraries, which is a win for code size.
 *
 * @todo Make the UART selection configurable per-app.
 * @todo Implement input by implementing _read().
 */

#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#include "stm32f0xx_hal.h"

// Definition for USARTx clock resources.
// This maps to the usb serial device on the nucleo32 board.
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

// GPIO assignments for the debug UART
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF1_USART1
#define USARTx_RX_PIN                    GPIO_PIN_15
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF1_USART1

#if defined(DEBUG)
#    define DBG_IPRINTF(...) iprintf(__VA_ARGS__)
#else
#    define DBG_IPRINTF(...)
#endif

/**
 * Initialize the serial port to the given baudrate.
 *
 * @param clk CPU clock rate in Hz (e.g. 48000000)
 * @param baud desired baud rate.
 */
void serial_io_init(uint32_t clk, uint32_t baud);

#endif /* SERIAL_IO_H */

/**
 * @file    main.c
 *
 * This example blabs on the serial port.
 */

#include "hw.h"
#include "serial_io.h"
#include "stm32f0xx_hal.h"
#include "sys_clk_config.h"

#include <stdio.h>

int main(void)
{
    // STM32F0xx HAL library initialization:
    HAL_Init();

    // Configure the system clock to 48 MHz
    sys_clk_config();

    // Initialize serial io to 115200 baud, w/ 48MHz system clock
    serial_io_init(48000000, 115200);


    LED3_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStruct;

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = LED3_PIN;
    HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);

    iprintf("HELLO!!!\n");

    int i = 0;
    while (1)
    {
        HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);
        /* Insert delay 100 ms */
        HAL_Delay(1000);

        iprintf("%d\n", i++);
    }
}

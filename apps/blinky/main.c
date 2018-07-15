/**
 * @file    main.c
 * This example describes how to configure and use GPIOs through
 * the STM32F0xx HAL API.
 */

/* Includes ------------------------------------------------------------------*/
#include "hw.h"
#include "stm32f0xx_hal.h"
#include "sys_clk_config.h"


int main(void)
{
    HAL_Init();

    /* Configure the system clock to 48 MHz */
    sys_clk_config();

    LED3_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStruct;

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = LED3_PIN;
    HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);

    while (1)
    {
        HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);
        /* Insert delay 100 ms */
        HAL_Delay(100);
    }
}

/**
 * Make newlib printf() write to the Nucleo debug serial port, by implementing _write().
 */

#include "serial_io.h"
#include <stdio.h>

void serial_io_init(uint32_t clk, uint32_t baud)
{
    // reset UART
    USARTx_FORCE_RESET();
    USARTx_RELEASE_RESET();

    /* Enable GPIO TX/RX clock */
    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();

    /* Enable USARTx clock */
    USARTx_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStruct;
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USARTx_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;

    HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = USARTx_RX_PIN;
    GPIO_InitStruct.Alternate = USARTx_RX_AF;

    HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

    // Init UART peripheral

    // Disable UART
    USART1->CR1 &= ~USART_CR1_UE;

    USART1->BRR = clk / baud;

    //8 bits, 1 startbit, 1 stop bit, no parity, enable the UART
    // oversample by 16x
    USART1->CR1 = USART_CR1_TE | USART_CR1_UE;
}


/**
 * Provide our own write() implementation.
 *
 * This will override newlib's default, which is __attribute__((weak)).
 * The same thing could be done for _read(), but there is currently no need.
 */
int _write(int fd, char *buf, int count)
{
    (void)fd;

    for (int i = 0; i < count; i++)
    {
        // wait for TX buffer ready
        while (!((USART1->ISR & USART_ISR_TXE) == USART_ISR_TXE));

        // send the next byte
        USART1->TDR = buf[i];
    }

    // wait for TX complete
    while (!((USART1->ISR & USART_ISR_TC) == USART_ISR_TC));

    return count;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{
    iprintf("Assert failed. File: %s, line: %d", file, line);

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

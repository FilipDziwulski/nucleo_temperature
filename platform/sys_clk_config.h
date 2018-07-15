/**
 * Code for configuring system clock to 48MHz.
 * More-or-less copied from STMCube example code.
 */

/**
  * System Clock Configuration
  * The system Clock is configured as follow :
  *    System Clock source            = PLL (HSI/2)
  *    SYSCLK(Hz)                     = 48000000
  *    HCLK(Hz)                       = 48000000
  *    AHB Prescaler                  = 1
  *    APB1 Prescaler                 = 1
  *    HSI Frequency(Hz)              = 8000000
  *    PREDIV                         = 1
  *    PLLMUL                         = 12
  *    Flash Latency(WS)              = 1
  */
void sys_clk_config(void);

/**
 * Hardware assignments. Pindefs, etc.
 */


/* LED3 */
#define LED3_PIN                           GPIO_PIN_3
#define LED3_GPIO_PORT                     GPIOB
#define LED3_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()


/* bit bang i2c */
#define I2C_BITBANG_SDA_PIN                GPIO_PIN_7
#define I2C_BITBANG_SDA_PORT               GPIOB
#define I2C_BITBANG_SCL_PIN                GPIO_PIN_6
#define I2C_BITBANG_SCL_PORT               GPIOB
#define I2C_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C_GPIO_CLK_DISABLE()             __HAL_RCC_GPIOB_CLK_DISABLE()


/**
 * @file    main.c
 * Filip Dziwulski
 */

/* Includes ------------------------------------------------------------------*/
#include "hw.h"
#include "serial_io.h"
#include "i2c_gpio.h"
#include "components/mcp9808.h"
#include "stm32f0xx_hal.h"
#include "sys_clk_config.h"
#include "temps.h"

#define DECIMAL_PORTION(number)         abs((((int)(number * 100.0)) - (((int)number)*100)))

/* declares the i2c bus used  */
static i2c_bus_t i2c_bus = {
        .tick = 5,
        .sda_port = I2C_BITBANG_SDA_PORT,
        .scl_port = I2C_BITBANG_SCL_PORT,
        .sda_pin = I2C_BITBANG_SDA_PIN,
        .scl_pin = I2C_BITBANG_SCL_PIN
    };


/*****************************************************************************
* Private Declarations
******************************************************************************/
static inline void init_Peripherals(void);
static inline void init_Devices(void);
static inline void print_Temps(void);

/*******************************************
*   MAIN
*******************************************/

int main(void)
{
    /* hardware abstraction layer init */
    HAL_Init();
    /* Configure the system clock to 48 MHz */
    sys_clk_config();
    /* Configure IC peripherals */
    init_Peripherals();
    /* Configure connected devices */
    init_Devices();


    /* intialize memory and starting values for temperatures */
    if(initTemperatures() == false)
    {
        // if init fails, exit
        return -1;
    }

    /* MAIN APPLICATION */
    while (1)
    {
        /* temperature reading variable */
        float temp;

        /* read temp from mcp9808 */
         if(mcp9808_getTemp(&i2c_bus, &temp))
         {
            /* add temp to averages */
            runAverages(temp);
            /* print averages */
            print_Temps();
         }
         else
         {
            iprintf("Failed to read temp from sensor!  \n");
         }
        

        /* Toggle LED */
        HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);
        /* Interval Delay */
        HAL_Delay(TEMP_SAMPLE_INTERVAL_MS);
    }
}


/*******************************************
*   Private Definitions
*******************************************/
static inline void init_LED3()
{
    // initialize led3 gpio

    LED3_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef  GPIO_InitStruct;

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = LED3_PIN;
    HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);
}

static inline void init_UART()
{
   // Initialize serial io to 115200 baud, w/ 48MHz system clock
    serial_io_init(48000000, 115200);
}

static inline void init_Peripherals()
{
    // internal peripherals
    init_LED3();
    init_UART();

    I2C_GPIO_CLK_ENABLE();
    i2c_Init(&i2c_bus);
}

static inline void init_Devices()
{
    // init connected devices
    mcp9808_init(&i2c_bus);
}

/* prints the avergae temps onto UART */
static inline void print_Temps(void)
{
    // Current: 22.45C  Avg(60s): 22.31C  Avg(1h):  21.92C  Avg(24h): 22.19C
    iprintf("Current: %d.%dC  Avg(60s): %d.%dC  Avg(1h): %d.%dC  Avg(24h):  %d.%dC  \r\n", 
            (int)getLatest(), DECIMAL_PORTION(getLatest()),
            (int)get60sAverage(), DECIMAL_PORTION(get60sAverage()),
            (int)get1hrAverage(), DECIMAL_PORTION(get1hrAverage()),
            (int)get24hrAverage(), DECIMAL_PORTION(get24hrAverage()));

}


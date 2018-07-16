/**
 * @file mcp9808.c
 *
 * mcp9808 temperature sensor driver.
 */

#include "mcp9808.h"
#include "serial_io.h"

#define MCP9808_I2C_ADDRESS     (0b0011000)   // 7 bits. i2c driver handles shifting and r/w flag
#define MCP_9808_TEMP_ADDR      (0b00000101)  // 8 bit temp register address

/*****************************************************************************
* Private Declarations
******************************************************************************/


/*****************************************************************************
* Public Definitions
******************************************************************************/

/* For configuring device registers */
void mcp9808_init(i2c_bus_t *bus)
{
    // register configs go here
}

/* queries temp sensor for current temperature readings */
bool mcp9808_getTemp(i2c_bus_t *bus, float *temp)
{
    /* output temp variable*/
    float temperature;

    /* tx buffer */
    uint8_t tx_buff[1];
    tx_buff[0] = MCP_9808_TEMP_ADDR;
    /* rx buffer */
    uint8_t rx_buff[2];
    
    /* write address + temp register */
    i2c_Write(bus, MCP9808_I2C_ADDRESS, tx_buff, 1);
    /* read temperature data*/
    ssize_t rx = i2c_Read(bus, MCP9808_I2C_ADDRESS, rx_buff, 2);


    unsigned int upperByte = rx_buff[0];
    unsigned int lowerByte = rx_buff[1];


    /* Exit if we didn't recive two bytes of data */
    if(0 != rx)
    {
        return false;
    }

    upperByte = upperByte & 0x1F; // scrub flags

    /* convert received bytes to temperature */
    if((upperByte & 0x10) == 0x10) //if less than 0 degrees
    {
        temperature = 256.0 - ((((float)upperByte) * 16.0) + (((float)lowerByte) / 16.0));
    }
    else
    {
        temperature = ((((float)upperByte) * 16.0) + (((float)lowerByte) / 16.0));
    }

    *temp = temperature;

    return true;

}
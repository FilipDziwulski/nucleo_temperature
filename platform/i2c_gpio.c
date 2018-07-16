/**
 * @file i2c_gpio.h
 *
 * bit-banged gpio implementation.
 */

#include "i2c_gpio.h"
#include "us_tick.h"

/*****************************************************************************
* Private Declarations
******************************************************************************/
static inline void tick(i2c_bus_t *bus);
static inline void sda(i2c_bus_t *bus, bool lvl);
static inline void scl(i2c_bus_t *bus, bool lvl);

static inline void idle(i2c_bus_t *bus);
static inline void start(i2c_bus_t *bus);
static inline void stop(i2c_bus_t *bus);

static inline void tx_bit(i2c_bus_t *bus, uint8_t b);
static inline uint8_t rx_bit(i2c_bus_t *bus);

static inline bool tx_byte(i2c_bus_t *bus, uint8_t c);
static inline uint8_t rx_byte(i2c_bus_t *bus, bool ack);

/*****************************************************************************
* Public Definitions
******************************************************************************/
void i2c_Init(i2c_bus_t *bus)
{
    // Set pin outputs before initializing them, to avoid glitching
    idle(bus);

    GPIO_InitTypeDef  g;

    g.Mode  = GPIO_MODE_OUTPUT_OD;
    g.Pull  = GPIO_NOPULL; 
    g.Speed = GPIO_SPEED_FREQ_HIGH;

    g.Pin = bus->sda_pin;
    HAL_GPIO_Init(bus->sda_port, &g);

    g.Pin = bus->scl_pin;
    HAL_GPIO_Init(bus->scl_port, &g);

    idle(bus);
}

ssize_t i2c_Write(i2c_bus_t *bus, uint8_t addr, const uint8_t *buf, size_t count)
{
    /*
    start(bus);

    if (!tx_byte(bus, addr << 1)) {
        idle(bus);
        return -1;
    }

    for (size_t i = 0; i < count; i++) {
        if (!tx_byte(bus, buf[i])) {
            idle(bus);
            return i;
        }
    }

    stop(bus);
    */

    return 0;
}

ssize_t i2c_Read(i2c_bus_t *bus, uint8_t addr, uint8_t *buf, size_t count)
{
    /*
    start(bus);

    if (!tx_byte(bus, (addr << 1) | 1)) {
        idle(bus);
        return -1;
    }

    if (count == 0) {
        stop(bus);
        return 0;
    }

    for (size_t i = 0; i < count - 1; i++) {
        buf[i] = rx_byte(bus, true);
    }

    buf[count - 1] = rx_byte(bus, false);

    stop(bus);
    */
    static uint8_t testBuffer[2] = {0xC1,0x7C};


    if(testBuffer[1] < 0xFF)
    {
        testBuffer[1] += 0x01;
    }

    buf[0] = testBuffer[0];
    buf[1] = testBuffer[1];

    return 0;
}


/*****************************************************************************
* Private Definitions
******************************************************************************/
static inline void sda(i2c_bus_t *bus, bool lvl)
{
    HAL_GPIO_WritePin(bus->sda_port, bus->sda_pin, lvl);
}

static inline void scl(i2c_bus_t *bus, bool lvl)
{
    HAL_GPIO_WritePin(bus->scl_port, bus->scl_pin, lvl);
}

static inline void tick(i2c_bus_t *bus)
{
    delay_us(bus->tick);
}

static void idle(i2c_bus_t *bus)
{
    sda(bus, 1);
    scl(bus, 1);
}

static void start(i2c_bus_t *bus)
{
    scl(bus, 1);
    sda(bus, 1);
    tick(bus);
    sda(bus, 0);
    tick(bus);
    scl(bus, 0);
}

static void stop(i2c_bus_t *bus)
{
    sda(bus, 0);
    tick(bus);
    scl(bus, 1);
    tick(bus);
    sda(bus, 1);
}

static inline void tx_bit(i2c_bus_t *bus, uint8_t b)
{
    sda(bus, b ? 1 : 0);
    tick(bus);
    scl(bus, 1);
    tick(bus);
    scl(bus, 0);
}

static inline uint8_t rx_bit(i2c_bus_t *bus)
{
    uint8_t b;

    sda(bus, 1);
    tick(bus);
    scl(bus, 1);
    tick(bus);
    b = HAL_GPIO_ReadPin(bus->sda_port, bus->sda_pin);
    scl(bus, 0);

    return b;
}

static inline bool tx_byte(i2c_bus_t *bus, uint8_t c)
{
    // bang out the bits
    for(size_t i = 0; i < 8; i++) {
        uint8_t b = (c & 0x80) ? 1 : 0;
        tx_bit(bus, b);
        c <<= 1;
    }

    //read the ack
    uint8_t ack = rx_bit(bus);

    return (ack == 0);
}

static inline uint8_t rx_byte(i2c_bus_t *bus, bool ack)
{
    // bang in the bits
    uint8_t c = 0;
    for(size_t i = 0; i < 8; i++) {
        c <<= 1;
        c |= rx_bit(bus);
    }

    //send the (n)ack
    // ack = 0, nack = 1
    tx_bit(bus, ack ? 0 : 1);

    return c;
}

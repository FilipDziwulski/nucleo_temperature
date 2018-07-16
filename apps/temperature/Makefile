include ../../make/stm32f031k6_nucleo_32_app.mk

INCLUDE += ./

DRIVER_SRCS += \
	STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c

PLATFORM_SRCS += \
	sys_clk_config.c \
	serial_io.c \
	i2c_gpio.c \
	components/mcp9808.c


SRCS += \
	main.c \
	temps.c

APP_NAME=temperature


include ../../make/rules.mk

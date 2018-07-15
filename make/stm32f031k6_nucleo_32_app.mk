# Commands
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
MKDIR_P = mkdir -p
RM = rm

# Paths
DRIVER_PATH := ../../third_party/STM32Cube_FW_F0_V1.9.0/Drivers
PLATFORM_PATH := ../../platform



DEFINES += 	                \
	STM32F031x6             \
	USE_STM32F0XX_NUCLEO_32 \
	USE_HAL_DRIVER

INCLUDE += 	                                         \
	$(DRIVER_PATH)/CMSIS/Device/ST/STM32F0xx/Include \
	$(DRIVER_PATH)/STM32F0xx_HAL_Driver/Inc          \
	$(DRIVER_PATH)/CMSIS/Include                     \
	$(PLATFORM_PATH)

DRIVER_SRCS =                                       \
	STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c        \
	STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c    \
	STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c

PLATFORM_SRCS =                   \
	startup/startup_stm32f031x6.s \
	startup/stm32f0xx_it.c        \
	startup/system_stm32f0xx.c    \
	us_tick.c


#Compiler/linker flags
CFLAGS  = --std=gnu11 -MMD -MP
CFLAGS += -Os -g -Wall -Werror
CFLAGS += -mlittle-endian -mcpu=cortex-m0 -mthumb

LDFLAGS  = -T../../platform/startup/STM32F031K6Tx_FLASH.ld
LDFLAGS += -Wl,--gc-sections

#No os (use newlib-provided stubs for open(), fork(), etc.)
LDFLAGS += --specs=nosys.specs

## newlib-nano is compiled -fshort-wchar, but links against
## crti.o, crtibegin.o which are -fno-short-wchar
## This throws warnings, but shouldn't matter, since I doubt
## the crti libraries are doing much with unicode strings.
## To use newlib-nano, uncomment _both_ of the lines below
LDFLAGS += --specs=nano.specs -Wl,--no-wchar-size-warning
CFLAGS += -fshort-wchar


BUILD_DIR=build

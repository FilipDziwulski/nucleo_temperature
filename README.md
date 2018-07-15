# Background #
Framework for building applications for STM32Cube eval boards.

# Setup #

Tested on Ubuntu 16.04. These instructions probably work for most debian-based distros.
Most stuff taken from http://regalis.com.pl/en/arm-cortex-stm32-gnulinux/

To install the toolchain, run: `sudo apt-get install gcc-arm-none-eabi gdb-arm-none-eabi openocd`

To build an app:
  - `cd apps/talky`
  - `make` - Optional build-only step
  - `make copy` - Build and flash the app to the STM32Cube board.

# What's in here #

## third_party/STM32Cube_FW_F0_V1.9.0 ##
STM32Cube sw from http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-mcu-packages/stm32cubef0.html
Most of the files it came with have been removed to reduce repo size, leaving just the /Driver library.

## platform/ ##
Library code developed locally, meant to be used by multiple apps. Code in
platform/startup is copied and (very) lightly modified from the STMCube
examples.

## make/ ##
Generic and board-specific makefile rules.

## apps/ ##
Application code. An app generally consists of:
  - `hw.h` - App/board specific defines (pindefs, etc.)
  - `main.c` - App entry point.
  - `Makefile` - It's a makefile
  - `stm32f0xx_hal_conf.h` - HAL configuration. This file must
     be present if HAL libraries are used, the HAL sources include it.

### Demo Applications ###
  - blinky - First attempt at compiling an STM32Cube example w/ gcc+make.
  - talky - A simple UART example.

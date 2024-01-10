# Overview

This is a C-version of C++ TFT_eSPI with removal of arduino/ESP32/etc support
and targeting only STM32 devices. It expects to have a `board.h` file available
in include path, which will include target-specific headers (such as
stm32f4xx.h) as well as a couple system-dependent functions like
`delayWaitms`.

API mostly follows C++ version with removal of classes, tho some overloaded
functions have been renamed (`pushImage` now has `pushImage8`, `drawPixelAlpha`
has been added etc).

Start using by making and including a `setup_xxxxx.h` file which configures the
SPI LCD driver, then including `tft_espi.h` into `board.h` as usual.

Hardware is initialized and configured inside `display_hal_xx.c` and 
`display_hal_xx.h` where different devices/pinouts can be added if necessary.

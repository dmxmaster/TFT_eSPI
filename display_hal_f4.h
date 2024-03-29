#pragma once

#define SPI_HAS_TRANSACTION 1
#define SUPPORT_TRANSACTIONS

#if defined(STM32F40_41xxx)
/* F407 target on SPI2 and ILI9341 LCD */

#define RCC_POWER_GPIO (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC)

#define CS_PORT GPIOB
#undef FONT_CS_PORT
#define DC_PORT GPIOC
#define RES_PORT GPIOA

#define CS_PIN_MASK (GPIO_Pin_12)
#undef FONT_CS_PIN_MASK
#define DC_PIN_MASK (GPIO_Pin_5)
#define RES_PIN_MASK (GPIO_Pin_8)

#elif defined(STM32F401xx)
/* F401 target on SPI2 and ST7735 LCD */

#define RCC_POWER_GPIO (RCC_AHB1Periph_GPIOB)

#define CS_PORT GPIOB
#define FONT_CS_PORT GPIOB
#define DC_PORT GPIOB
#define RES_PORT GPIOB

#define CS_PIN_MASK (GPIO_Pin_0)
#define FONT_CS_PIN_MASK (GPIO_Pin_12)
#define DC_PIN_MASK (GPIO_Pin_1)
#define RES_PIN_MASK (GPIO_Pin_10)

#else

#error unsupported platform!

#endif

#define CS_L CS_PORT->BSRR = CS_PIN_MASK << 16
#define CS_H CS_PORT->BSRR = CS_PIN_MASK

#define RES_L RES_PORT->BSRR = RES_PIN_MASK << 16
#define RES_H RES_PORT->BSRR = RES_PIN_MASK

#define FONT_CS_L FONT_CS_PORT->BSRR = FONT_CS_PIN_MASK << 16
#define FONT_CS_H FONT_CS_PORT->BSRR = FONT_CS_PIN_MASK

#define DC_DELAY

#define DC_C DC_DELAY; DC_PORT->BSRR = DC_PIN_MASK << 16
#define DC_D DC_DELAY; DC_PORT->BSRR = DC_PIN_MASK

#define SPI_MODE0 0

#define tft_Write_8(C) displayTransfer8(C)
#define tft_Write_16(C) displayTransfer8(C >> 8); displayTransfer8(C & 0xff)
#define tft_Write_32D(C) tft_Write_16(C); tft_Write_16(C)
#define tft_Write_32C(C,D) tft_Write_16(C); tft_Write_16(D)
#define tft_Read_8() displayTransfer8(0xAA)

#define DISPLAY_DMA_BENEFIT_LENGTH  (16)

void displayHardwareInit(void);
void displayHardwareReset(void);
void displaySpeed(uint16_t prescaler);
uint8_t displayTransfer8(uint8_t dat);
void displayTransfer16(const uint16_t *buffer, int len, bool incr, bool nowait);
void displayTransfer16End(void);
void displayTransfer16Slow(uint16_t *buffer, int len, bool incr);

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "display_hal_f4.h"

#ifdef STM32F401xx /* TODO add additional F4-based platforms */

#define SPI_SPEED SPI_BaudRatePrescaler_2
#define SPIx_TX_DMA_STREAM      DMA1_Stream4
#define SPIx_RXTX_DMA_CHANNEL   DMA_Channel_0
#define SPIx_TX_DMA_FLAG_TCIF   DMA_FLAG_TCIF4

void displayHardwareInit(void)
{
    GPIO_InitTypeDef gpio;
    SPI_InitTypeDef spi;
    DMA_InitTypeDef dma;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    // SPI pins configuration
    gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;;
    GPIO_Init(GPIOB, &gpio);

    // FONT_CS + LCD_RES + LCD_DC
    FONT_CS_H;
    DC_D;
    RES_H;
    gpio.GPIO_Pin = FONT_CS_PIN_MASK | DC_PIN_MASK | RES_PIN_MASK;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOB, &gpio);

    // LCD_CS
    gpio.GPIO_Pin = CS_PIN_MASK;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(CS_PORT, &gpio);

    SPI_StructInit(&spi);
    SPI_I2S_DeInit(SPI2);
    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_DataSize = SPI_DataSize_8b;
    spi.SPI_CPOL = SPI_CPOL_High;
    spi.SPI_CPHA = SPI_CPHA_2Edge;
    spi.SPI_NSS = SPI_NSS_Soft;
    spi.SPI_BaudRatePrescaler = SPI_SPEED;
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial = 7;
    spi.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI2, &spi);

    DMA_DeInit(SPIx_TX_DMA_STREAM);
    DMA_StructInit(&dma);
    dma.DMA_BufferSize = 1;
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dma.DMA_MemoryInc = DMA_MemoryInc_Disable;
    dma.DMA_Mode = DMA_Mode_Normal;
    dma.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI2->DR));
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_Priority = DMA_Priority_High;
    // Configure TX DMA
    dma.DMA_Channel = SPIx_RXTX_DMA_CHANNEL;
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    dma.DMA_Memory0BaseAddr = 0;
    DMA_Init(SPIx_TX_DMA_STREAM, &dma);
    // Enable SPI
    SPI_Cmd(SPI2, ENABLE);
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
}

static void dff(uint16_t datasize)
{
    uint16_t tmpreg;

    while (SPI2->SR & SPI_I2S_FLAG_BSY);

    tmpreg = SPI2->CR1;

    // mask DFF and SPE
    tmpreg &= ~(uint16_t)(0x0800 | SPI_CR1_SPE); // DFF
    SPI2->CR1 = tmpreg;

    // set dff
    if (datasize) {
        tmpreg |= datasize;
        SPI2->CR1 = tmpreg;
    }

    // enable SPI
    tmpreg |= SPI_CR1_SPE;
    SPI2->CR1 = tmpreg;
}

uint8_t displayTransfer8(uint8_t dat)
{
    while (!(SPI2->SR & SPI_I2S_FLAG_TXE));
    SPI2->DR = dat;
    while (!(SPI2->SR & SPI_I2S_FLAG_RXNE));
    return (uint8_t)(SPI2->DR);
}

void displayTransfer16(const uint16_t *buffer, int len, bool incr, bool nowait)
{
    uint16_t xfersize = 0xFFFF;
    uint32_t tmpreg = 0;

    dff(SPI_DataSize_16b);

    // set start address
    SPIx_TX_DMA_STREAM->M0AR = (uint32_t)buffer;

    // enable fixed/incr mode
    tmpreg = SPIx_TX_DMA_STREAM->CR;
    tmpreg &= ~(DMA_SxCR_MINC);
    if (incr)
        tmpreg |= DMA_MemoryInc_Enable;
    SPIx_TX_DMA_STREAM->CR = tmpreg;

    do {
        if (len > 0xFFFF)
            xfersize = 0xFFFF;
        else
            xfersize = (uint16_t)(len & 0xFFFF);

        if (xfersize == 0)
            break;

        SPIx_TX_DMA_STREAM->NDTR = xfersize;

        SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
        DMA_Cmd(SPIx_TX_DMA_STREAM, ENABLE);

        if (nowait)
            return;

        while (DMA_GetFlagStatus(SPIx_TX_DMA_STREAM, SPIx_TX_DMA_FLAG_TCIF) == RESET);
        while (!(SPI2->SR & SPI_I2S_FLAG_TXE));
        while (SPI2->SR & SPI_I2S_FLAG_BSY);

        DMA_ClearFlag(SPIx_TX_DMA_STREAM, SPIx_TX_DMA_FLAG_TCIF);
        DMA_Cmd(SPIx_TX_DMA_STREAM, DISABLE);
        // wait for DMA to really disable
        while (SPIx_TX_DMA_STREAM->CR & DMA_SxCR_EN);

        len -= xfersize;
        if (len && incr)
            SPIx_TX_DMA_STREAM->M0AR += (xfersize * sizeof(uint16_t));
    } while (len > 0);

    // clear rx buffer
    SPI_I2S_ReceiveData(SPI2);

    dff(SPI_DataSize_8b);
}

void displayTransfer16End(void)
{
    while (DMA_GetFlagStatus(SPIx_TX_DMA_STREAM, SPIx_TX_DMA_FLAG_TCIF) == RESET);
    while (!(SPI2->SR & SPI_I2S_FLAG_TXE));
    while (SPI2->SR & SPI_I2S_FLAG_BSY);

    DMA_ClearFlag(SPIx_TX_DMA_STREAM, SPIx_TX_DMA_FLAG_TCIF);
    DMA_Cmd(SPIx_TX_DMA_STREAM, DISABLE);
    // wait for DMA to really disable
    while (SPIx_TX_DMA_STREAM->CR & DMA_SxCR_EN);

    // clear rx buffer
    SPI_I2S_ReceiveData(SPI2);

    dff(SPI_DataSize_8b);
}

void displayTransfer16Slow(uint16_t *buffer, int len, bool incr)
{
    int i;

    dff(SPI_DataSize_16b);
    for (i = 0; i < len; i++) {
        SPI2->DR = *buffer;
        if (incr)
            buffer++;
        while (!(SPI2->SR & SPI_I2S_FLAG_RXNE));
        SPI2->DR;
    }
    dff(SPI_DataSize_8b);
}

void displaySpeed(uint16_t prescaler)
{
    uint16_t tmpreg;

    while (SPI2->SR & SPI_I2S_FLAG_BSY);

    tmpreg = SPI2->CR1;

    // mask BR
    tmpreg &= ~((uint16_t)0x0038); // BR[2:0]
    SPI2->CR1 = tmpreg;

    // set prescaler
    tmpreg |= prescaler;
    SPI2->CR1 = tmpreg;
}

#endif

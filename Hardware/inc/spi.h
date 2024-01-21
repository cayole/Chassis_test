#pragma once

#include "delay.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define SPI_MOSI_Pin  GPIO_Pin_0
#define SPI_MOSI_GPIO GPIOC
#define SPI_MOSI_H	  GPIO_SetBits(SPI_MOSI_GPIO, SPI_MOSI_Pin)
#define SPI_MOSI_L	  GPIO_ResetBits(SPI_MOSI_GPIO, SPI_MOSI_Pin)

#define SPI_SCK_Pin	 GPIO_Pin_1
#define SPI_SCK_GPIO GPIOC
#define SPI_SCK_H	 GPIO_SetBits(SPI_SCK_GPIO, SPI_SCK_Pin)
#define SPI_SCK_L	 GPIO_ResetBits(SPI_SCK_GPIO, SPI_SCK_Pin)

#define SPI_MISO_Pin  GPIO_Pin_2
#define SPI_MISO_GPIO GPIOC

#define SPI_CS_Pin	GPIO_Pin_3
#define SPI_CS_GPIO GPIOC
#define SPI_CS_H	GPIO_SetBits(SPI_CS_GPIO, SPI_CS_Pin)
#define SPI_CS_L	GPIO_ResetBits(SPI_CS_GPIO, SPI_CS_Pin)

void SpiInit(void);
void SpiStart(void);
void SpiStop(void);
u8	 SpiRWByte(u8 data);

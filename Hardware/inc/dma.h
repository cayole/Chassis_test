#ifndef _DMA_H
#define _DMA_H
#include "stm32f4xx.h"
//#include "stm32f4xx_dma.h"
void DMA_ENABLE(DMA_Stream_TypeDef *DMA_Streamx,uint16_t ndtr);
void DMA_Config(DMA_Stream_TypeDef *DMA_Streamx, uint32_t \
	chx,uint32_t par,uint32_t mar,uint16_t ndtr);
void DMA_NVIC_Config(void);

#endif

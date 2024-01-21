#include "dma.h"

void DMA_Config(DMA_Stream_TypeDef *DMA_Streamx, uint32_t chx,uint32_t par,\
	uint32_t mar,uint16_t ndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	if((uint32_t) DMA_Streamx>(uint32_t)DMA2)
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);	
	else
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);	

	DMA_DeInit(DMA_Streamx);
	while(DMA_GetCmdStatus(DMA_Streamx)!=RESET)
	{}
		
	DMA_InitStructure.DMA_Channel=chx;// 通道选择
	DMA_InitStructure.DMA_Memory0BaseAddr=mar;// 存储器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr=par;// 外设基地址
		
	DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;// 存储器到外设模式
	DMA_InitStructure.DMA_BufferSize=ndtr;// 数据传输量
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;// 外设非增量模式
		
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;// 存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;// 外设数据长度为8位
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;// 存储器数据长度为8位
		
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;// 使用普通模式
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;// 中等优先级
	DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;// FIFO模式禁止
		
	DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;// FIFO模式阈值
	DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;// 存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;// 外设突发单次传输
		
	DMA_Init(DMA_Streamx,&DMA_InitStructure);//初始化DMA Stream
	
	
}

void DMA_ENABLE(DMA_Stream_TypeDef *DMA_Streamx,uint16_t ndtr)
{
	DMA_Cmd(DMA_Streamx,DISABLE);// 关闭DMA传输
	while(DMA_GetCmdStatus (DMA_Streamx)!=DISABLE){}// 确保DMA可以设置
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);// 数据传输量
	DMA_Cmd(DMA_Streamx,ENABLE);//开启DMA传输
}

void DMA_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_Structure;
	NVIC_Structure.NVIC_IRQChannel=DMA2_Stream7_IRQn;
	NVIC_Structure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Structure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_Structure.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVIC_Structure);
	
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
}



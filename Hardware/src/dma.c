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
		
	DMA_InitStructure.DMA_Channel=chx;// ͨ��ѡ��
	DMA_InitStructure.DMA_Memory0BaseAddr=mar;// �洢����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr=par;// �������ַ
		
	DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;// �洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize=ndtr;// ���ݴ�����
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;// ���������ģʽ
		
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;// �洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;// �������ݳ���Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;// �洢�����ݳ���Ϊ8λ
		
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;// ʹ����ͨģʽ
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;// �е����ȼ�
	DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;// FIFOģʽ��ֹ
		
	DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;// FIFOģʽ��ֵ
	DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;// �洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;// ����ͻ�����δ���
		
	DMA_Init(DMA_Streamx,&DMA_InitStructure);//��ʼ��DMA Stream
	
	
}

void DMA_ENABLE(DMA_Stream_TypeDef *DMA_Streamx,uint16_t ndtr)
{
	DMA_Cmd(DMA_Streamx,DISABLE);// �ر�DMA����
	while(DMA_GetCmdStatus (DMA_Streamx)!=DISABLE){}// ȷ��DMA��������
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);// ���ݴ�����
	DMA_Cmd(DMA_Streamx,ENABLE);//����DMA����
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



#include "usart2.h"

bool getmsgflag;
u8 USART2_RX_STA = 0;

void USART2_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能串口2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //使能DMA1

	//将PA2\PA3复用为USART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //接受中断

	//配置NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //就改了一下中断优先级就进不去中断了？
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_DeInit(DMA1_Stream6);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(usart.TxBuffer_USART2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //以下为f4特有
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);

	DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6); //清除中断标志
	DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE); //关闭了DMA传输完成中断，导致数据无法传输。10/14/2018

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //解决了遥控无法给主控板发消息，是因为优先级过低（7/1）		11/16/2020
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

	USART_Cmd(USART2, ENABLE);
}

void DMA1_Stream6_IRQHandler(void) //数据传输完成，产生中断，检查是否还有没有传输的数据，继续传输
{
	if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET)
	{
		DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6); //清除中断标志
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
		flag.Usart2DMASendFinish = 0;
	}
}

u8 sum=0;
void USART2_IRQHandler(void)
{
	u8 temp = 0;
	if (USART_GetITStatus(USART2, USART_IT_ORE_RX) != RESET)
	{
		temp = USART_ReceiveData(USART2);
		USART2_RX_STA = 0;
		// memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2)); //先将RXBUFFER清零
	}
	else if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{

		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); //清楚标志位

		temp = USART_ReceiveData(USART2); //开始接收数据,暂存到临时变量temp中

		if ((USART2_RX_STA & 0X40) != 0) //标志寄存器的第三位不等于0，接收开始
		{
			if ((USART2_RX_STA & 0x80) == 0) //标志位的第四位等于0，接收未完成
			{
				if (temp == 0x5A) //帧尾，接收完成，进入指令判断区
				{
					for(int i=0;i<(USART2_RX_STA & 0X3F)-1;i++)
						sum+=usart.RxBuffer_USART2[i];
					if(sum == usart.RxBuffer_USART2[(USART2_RX_STA & 0X3F) - 1])
					{
						USART2_RX_STA = 0X00;
						getmsgflag=true;
						sum=0;
					}
					else
					{
						usart.RxBuffer_USART2[USART2_RX_STA & 0X3F] = temp; //存储
						USART2_RX_STA++;
						sum=0;
						if ((USART2_RX_STA & 0X3F) > 40) //超出接收范围
							USART2_RX_STA = 0;
					}
				}
				else
				{
					usart.RxBuffer_USART2[USART2_RX_STA & 0X3F] = temp; //存储
					USART2_RX_STA++;

					if ((USART2_RX_STA & 0X3F) > 40) //超出接收范围
						USART2_RX_STA = 0;
				}
			}
		}
		else if (temp == 0xA5)
			USART2_RX_STA |= 0X40;
		}
}

void USART2_Send(u8 count)
{
	if (!flag.Usart2DMASendFinish) //若上一次传输未完成，则舍弃本次传输
	{
		DMA_SetCurrDataCounter(DMA1_Stream6, count);
		DMA_Cmd(DMA1_Stream6, ENABLE);
		flag.Usart2DMASendFinish = 1;
	}
}

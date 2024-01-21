#include "spi.h"

void SpiInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// TODO；时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin	  = SPI_MOSI_Pin;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		  // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	  // 100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		  // 上拉
	GPIO_Init(SPI_MOSI_GPIO, &GPIO_InitStructure);		  // 初始化 GPIO

	GPIO_InitStructure.GPIO_Pin	  = SPI_SCK_Pin;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		  // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	  // 100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		  // 上拉
	GPIO_Init(SPI_SCK_GPIO, &GPIO_InitStructure);		  // 初始化 GPIO

	GPIO_InitStructure.GPIO_Pin	  = SPI_CS_Pin;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		  // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	  // 100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		  // 上拉
	GPIO_Init(SPI_CS_GPIO, &GPIO_InitStructure);		  // 初始化 GPIO

	GPIO_InitStructure.GPIO_Pin	  = SPI_MISO_Pin;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;	 // 普通输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	  // 100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		  // 上拉
	GPIO_Init(SPI_MISO_GPIO, &GPIO_InitStructure);

	// 模式0：CPOL=0，CPHA=0，SCK空闲低电平，SCK上升沿读取
	// 模式1：CPOL=0，CPHA=1，SCK空闲低电平，SCK下降沿读取
	// 模式2：CPOL=1，CPHA=0，SCK空闲高电平，SCK下降沿读取
	// 模式3：CPOL=1，CPHA=1，SCK空闲高电平，SCK上升沿读取
	// 空闲状态
	SPI_SCK_L;
	SPI_CS_H;
}

void SpiStart(void) { SPI_CS_L;__NOP(); };

void SpiStop(void) { SPI_CS_H;__NOP(); };

void SpiDely(void){
	for(u8 i=0;i<4;i++)
		__NOP();
}

// 模式一，下降沿发送
u8	 SpiRWByte(u8 data)
{
	u8 rec = 0x00;
	for (u8 i = 0; i < 8; i++) {
		SPI_SCK_H;
		SpiDely();
		// TODO:先发送的是数据的高位
		if (data & 0x80)
			SPI_MOSI_H;
		else
			SPI_MOSI_L;
		data = data << 1;	 // 数据移位
		
		SpiDely();
		SPI_SCK_L;
		SpiDely();
		// 接受数据
		rec	 = rec << 1;
		if (GPIO_ReadInputDataBit(SPI_MISO_GPIO, SPI_MISO_Pin))
			rec++;// TODO: 先接收到的是数据的高位
	}
	return rec;
}

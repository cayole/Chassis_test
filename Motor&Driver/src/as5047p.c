#include "as5047p.h"

u8 AS5047Check(u16 msg)
{
	u8 check = 0;
	for (u8 i = 0; i < 16; i++) {
		check += (msg & 0x01);
		msg	   = msg >> 1;
	}
	return (check % 2 == 0);
};

/**
 * @brief 得到对as5047的地址命令报文
 * @param addr 需要读取的数据地址
 * @return u16 发送的报文
 */
u16 GetAS5047SpiSend(u16 addr)
{
	addr |= 0x4000;	   // 读指令
	if (!AS5047Check(addr))
		addr |= 0x8000;
	return addr;
}

u8 ReadAS5047pData(u16* rec)
{
	*rec=0x00;
	u16 sendMsg=GetAS5047SpiSend(0x3FFF);
	// 发送读取地址0x3FFF；带动态补偿的测量角度
	SpiStart();
	*rec |= (SpiRWByte((u8)((sendMsg & 0xff00)>>8))<<8);
	*rec |= SpiRWByte((u8)(sendMsg & 0x00ff));
	SpiStop();
	if (!AS5047Check(*rec))
		return false;
	
	*rec = (*rec) & 0x3FFF;	   // 去掉前两位校验和读写指令位
	if((*rec) & 0x4000)	   // error位和校验
		return false;

	return true;
}

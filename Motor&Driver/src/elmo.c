#include "elmo.h"

ELMOParam EC_4P_22, U10, U8, U5;
ELMOLimit ELMOlimit[5];
ELMOArgum ELMOargum;
ELMO_Motor ELMOmotor[5];

void ELMO_Motor_Init(void)
{
    EC_4P_22.PULSE=4000;
    EC_4P_22.RATIO=3.7f;//MAXON Gear 166930
    
    U10.PULSE=4096*4;
    U10.RATIO=1;
    
    U8.PULSE=4096*4;
    U8.RATIO=1;
    
    U5.PULSE=1000*4;
    U5.RATIO=1;

    // 电机限制保护
    {
        ELMOlimit[0].isPosLimitON=false;
		ELMOlimit[0].maxAngle=720.f;
		ELMOlimit[0].isPosSPLimitOn=false;
		ELMOlimit[0].posSPlimit=1000;
		ELMOlimit[0].isRealseWhenStuck=false;
		ELMOlimit[0].zeroSP=500;
		ELMOlimit[0].zeroCurrent=1000;
		ELMOlimit[0].timeoutMotion=1;
		
		ELMOlimit[1].isPosLimitON=false;
		ELMOlimit[1].maxAngle=720.f;
		ELMOlimit[1].isPosSPLimitOn=false;
		ELMOlimit[1].posSPlimit=500;
		ELMOlimit[1].isRealseWhenStuck=false;
		ELMOlimit[1].zeroSP=500;
		ELMOlimit[1].zeroCurrent=1000;
		ELMOlimit[1].timeoutMotion=1;
		
		ELMOlimit[2].isPosLimitON=false;
		ELMOlimit[2].maxAngle=720.f;
		ELMOlimit[2].isPosSPLimitOn=false;
		ELMOlimit[2].posSPlimit=500;
		ELMOlimit[2].isRealseWhenStuck=false;
		ELMOlimit[2].zeroSP=500;
		ELMOlimit[2].zeroCurrent=1000;
		ELMOlimit[2].timeoutMotion=1;
    }
    // 电机其他参数设置
    {
        ELMOargum.lastRxTim=0;
		ELMOargum.timeoutTicks=1000;//2000ms
    }

    ELMOmotor[0].intrinsic=U5;
	ELMOmotor[0].enable=DISABLE;
	ELMOmotor[0].mode=RPM;
	ELMOmotor[0].valueSet.angle=0;
	ELMOmotor[0].valueSet.speed=0;
	ELMOmotor[0].valueSet.current=0;
	
	ELMOmotor[0].limit=ELMOlimit[0];
	ELMOmotor[0].argum=ELMOargum;
    
	ELMOmotor[1].intrinsic=U5;
	ELMOmotor[1].enable=DISABLE;
	ELMOmotor[1].mode=RPM;
	ELMOmotor[1].valueSet.angle=360;
	ELMOmotor[1].valueSet.speed=200;
	ELMOmotor[1].valueSet.current=2;
	
	ELMOmotor[1].limit=ELMOlimit[0];
	ELMOmotor[1].argum=ELMOargum;
	
	ELMOmotor[2].intrinsic=U8;
	ELMOmotor[2].enable=DISABLE;
	ELMOmotor[2].mode=RPM;
	ELMOmotor[2].valueSet.angle=360;
	ELMOmotor[2].valueSet.speed=200;
	ELMOmotor[2].valueSet.current=2;
	
	ELMOmotor[2].limit=ELMOlimit[0];
	ELMOmotor[2].argum=ELMOargum;
}

//ELMO初始化
void ELMO_Init(CAN_TypeDef* CANx)
{
	CanTxMsg tx_message;
	tx_message.StdId=0x000; // 表示NMT报文
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Standard;
	tx_message.DLC=8;
	//第0个字节是CS(Command Specifier)表示控制命令 1代表start all 2代表stop all
	tx_message.Data[0]=0x01;
	tx_message.Data[1]=0x00;
	tx_message.Data[2]=0x00;
	tx_message.Data[3]=0x00;
	tx_message.Data[4]=0x00;
	tx_message.Data[5]=0x00;
	tx_message.Data[6]=0x00;
	tx_message.Data[7]=0x00;
	CAN_Transmit(CANx,&tx_message);
}

/***ELMO的CanOpen协议注释****
*报文是4字节的驱动器均视为“查询”命令 8字节的若Byte3的bit6是1则也是“查询”
*数据类型由Byte3的bit7决定
*“设置”命令的数据存在Byte4~Byte7中，低位字存放在低地址中
***************************/

// 加速度
void AC(uint32_t id, uint8_t cmd,uint32_t valueSet)
{
    if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
		if(cmd==GetData)//  读
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='A';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='C';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)// 写
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x08;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='A';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='C';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
			EncodeS32Data(&valueSet,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);
		}
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
		CAN2_SendQueue.Rear=IncCAN2Rear;
}

//启动电机
void BG(u32 id)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
	 	CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='B';
	  	CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='G';
	  	CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
	  	CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
	  	CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

//减速度
void DC(u32 id,u8 cmd,u32 valueSet)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
		if(cmd==GetData)
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='D';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='C';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x08;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='D';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='C';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
			EncodeS32Data(&valueSet,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);// 入队
		}
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;//入队标志符置1
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}


//读取无功电流分量  浮点型的形式
void IQ(u32 id)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='I';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='Q';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x80;//Byte3的bit7决定是浮点型还是整型 
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

//设定速度模式下的持续速度
void JV(u32 id,u8 cmd)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
		if(cmd==GetData)
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='J';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='V';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
		}
		else if(cmd==SetData)
		{
			s32 speed=ELMOmotor[id-1].valueSet.speed*ELMOmotor[id-1].intrinsic.PULSE/60;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x08;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='J';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='V';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
			EncodeS32Data(&speed,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);
		}
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
	  CAN2_SendQueue.Rear=IncCAN2Rear;
}

//使能、失能电机
void MO(u32 id,u8 cmd,u32 status)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
		if(cmd==GetData)
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='M';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='O';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x40;// Byte3的bit6为1 驱动器响应为查询即读取 而不是设置 
		}
		else if(cmd==SetData)
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x08;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='M';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='O';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]=status;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[5]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[6]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[7]=0;
		}
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
		CAN2_SendQueue.Rear=IncCAN2Rear;
}

//设定位置模式下转的角度
void PA(u32 id,u8 cmd)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
	  CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
		if(cmd==GetData)
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='P';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='A';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			s32 position_set=ELMOmotor[id-1].valueSet.angle/360.f*ELMOmotor[id-1].intrinsic.PULSE*ELMOmotor[id-1].intrinsic.RATIO;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x08;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='P';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='A';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
			EncodeS32Data(&position_set,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);
		}
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
        CAN2_SendQueue.Rear=IncCAN2Rear;
}

/*获得相对位置*/
void ELMO_PR(u32 id,u8 cmd)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
        if(cmd==GetData)
        {
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='P';
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='R';
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
        }
        else if(cmd==SetData)
        {
            s32 position_set=ELMOmotor[id-1].valueSet.angle/360.f*ELMOmotor[id-1].intrinsic.PULSE*ELMOmotor[id-1].intrinsic.RATIO;
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='P';
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='R';
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
            EncodeS32Data(&position_set,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);
        }
            CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
        CAN2_SendQueue.Rear=IncCAN2Rear;
}

/*获取位置误差*/
void PE(u32 id)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='P';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='E';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

//查询电机主位置
void PX(u32 id,u8 cmd)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		if(cmd==GetData)
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='P';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='X';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			s32 position_set=ELMOmotor[id-1].valueSet.angle/360.f*ELMOmotor[id-1].intrinsic.PULSE*ELMOmotor[id-1].intrinsic.RATIO;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x08;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='P';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='X';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
			EncodeS32Data(&position_set,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);
		}
		 CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
		 CAN2_SendQueue.Rear=IncCAN2Rear;
}


//设置位置模式下的转速SP
void SP(u32 id,u8 cmd)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
		if(cmd==GetData)
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='S';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='P';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x40;
		}
		else if(cmd==SetData)
		{
			s32 speed_position;
			speed_position=ELMOmotor[id-1].valueSet.speed*ELMOmotor[id-1].intrinsic.PULSE/60;//转换为脉冲每秒
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x08;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='S';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='P';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
			EncodeS32Data(&speed_position,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);
		}
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
		    CAN2_SendQueue.Rear=IncCAN2Rear;
}

//设定电流
void TC(u32 id)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		float current=ELMOmotor[id-1].valueSet.current;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='T';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='C';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;

      	EncodeS32Data(&current,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}


//运动模式
void UM(u32 id,u8 cmd,u32 status)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
    	CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;		
		if(cmd==GetData)
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='U';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='M';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x08;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='U';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='M';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]=status;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[5]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[6]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[7]=0;
		}
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
	    CAN2_SendQueue.Rear=IncCAN2Rear;
}

//制动电机
void ST(u32 id)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='S';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='T';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
		  CAN2_SendQueue.Rear=IncCAN2Rear;
}

//查询电机转速(VX)
void VX(u8 id)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID=0x300+id;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC=0x04;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='V';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='X';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].InConGrpFlag=1;
	}
		CAN2_SendQueue.Rear=IncCAN2Rear;
}




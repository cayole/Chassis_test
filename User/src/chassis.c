#include "chassis.h"

volatile u8 ModeFlag=0xff;

void ENUNableChassis(CanRxMsg rx_message)
{
//		while(VESCmotor[0].enable!=rx_message.Data[1]
//				||VESCmotor[1].enable!=rx_message.Data[1]
//				||VESCmotor[2].enable!=rx_message.Data[1]
//				||VESCmotor[3].enable!=rx_message.Data[1])	
	  {	
			for(int i=0;i<4;i++)
			{
				VESCmotor[i].enable = rx_message.Data[1];
				VESCmotor[i].begin = rx_message.Data[1];
			}
		}
	answer_master(rx_message);   //反馈使能状态
}


void SetChassisSpeed(CanRxMsg rx_message)
{
	int index=0;
	for(int i=0;i<4;i++)
	{
		VESCmotor[i].mode=VESCRPM;
	}
	VESCmotor[0].valueSet.speed= get_s16_from_buffer(rx_message.Data , &index );//speed FL
	VESCmotor[1].valueSet.speed= get_s16_from_buffer(rx_message.Data , &index );//speed FR
	VESCmotor[2].valueSet.speed= get_s16_from_buffer(rx_message.Data , &index );//speed BR
	VESCmotor[3].valueSet.speed= get_s16_from_buffer(rx_message.Data , &index );//speed BL
	
	Chassis_Answer_VX();
}


void SetChassisAngle(CanRxMsg rx_message)
{
	int index=0;
	for(int i=0;i<4;i++)
	{
		VESCmotor[i].mode=VESCPOSITION;
	}
	VESCmotor[0].valueSet.angle = buffer_16_to_float(rx_message.Data ,1e1, &index );//angle FL
	VESCmotor[1].valueSet.angle = buffer_16_to_float(rx_message.Data ,1e1, &index );//angle FR
	VESCmotor[2].valueSet.angle = buffer_16_to_float(rx_message.Data ,1e1, &index );//angle BR
	VESCmotor[3].valueSet.angle = buffer_16_to_float(rx_message.Data ,1e1, &index );//angle BL
	
	Chassis_Answer_PX();
}


void AskChassisSpeed(void)
{
	if(IncCAN1Rear == CAN1_SendQueue.Front)
  {
    flag.Can1SendqueueFULL++;
    return;
  }
  else
  {
    CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x02020104;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE = CAN_ID_EXT ;
    CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0x08;
    CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = (uint16_t)VESCmotor[0].valueReal.speed>>8;
    CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = (uint16_t)VESCmotor[0].valueReal.speed;
    CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = (uint16_t)VESCmotor[1].valueReal.speed>>8;
    CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = (uint16_t)VESCmotor[1].valueReal.speed;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[4] = (uint16_t)VESCmotor[2].valueReal.speed>>8;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[5] = (uint16_t)VESCmotor[2].valueReal.speed;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[6] = (uint16_t)VESCmotor[3].valueReal.speed>>8;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[7] = (uint16_t)VESCmotor[3].valueReal.speed;
    
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].InConGrpFlag = 0;
  }
  CAN1_SendQueue.Rear = IncCAN1Rear;
}


void AskChassisCurrent(void)
{
	if (IncCAN1Rear == CAN1_SendQueue.Front)
	{
		flag.Can1SendqueueFULL++;
		return;
	}
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x02020105;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE = CAN_ID_EXT ;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0x08;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = (uint16_t)VESCmotor[0].valueReal.current >>8;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = (uint16_t)VESCmotor[0].valueReal.current ;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = (uint16_t)VESCmotor[1].valueReal.current >>8;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = (uint16_t)VESCmotor[1].valueReal.current ;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[4] = (uint16_t)VESCmotor[2].valueReal.current >>8;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[5] = (uint16_t)VESCmotor[2].valueReal.current ;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[6] = (uint16_t)VESCmotor[3].valueReal.current >>8;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[7] = (uint16_t)VESCmotor[3].valueReal.current;
			
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].InConGrpFlag = 0;
	}
	CAN1_SendQueue.Rear = IncCAN1Rear;
}



/********************************************************************************************************************************/	

/*使能状态反馈**********************************************************************/
void answer_master(CanRxMsg rx_message)          
{
	if (IncCAN1Rear == CAN1_SendQueue.Front)
	{
		flag.Can1SendqueueFULL++;
		return;
	}
	else 
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x02020101;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE = CAN_ID_EXT ;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 2;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'M';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = rx_message.Data[1];
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].InConGrpFlag = 0;
	}
	CAN1_SendQueue.Rear = IncCAN1Rear;
	B1p.answer_master_flag = true;
}


/*设置速度后反馈*****************************************************************************************/
void Chassis_Answer_VX(void)
{
	if(IncCAN1Rear == CAN1_SendQueue.Front)
	{
		flag.Can1SendqueueFULL++;
		return;
	}
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x02020102;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE = CAN_ID_EXT ;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 2;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'O';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'K';
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].InConGrpFlag = 0;
	}
	CAN1_SendQueue.Rear = IncCAN1Rear;
}


/*设置角度后反馈******************************************************************************************/
void Chassis_Answer_PX(void)
{
    
    if (IncCAN1Rear == CAN1_SendQueue.Front)
    {
      flag.Can1SendqueueFULL++;
      return;
    }
    else
    {
      CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x02020103;
			CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE = CAN_ID_EXT ;
      CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 2;
      CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'O';
      CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'K'; 
      CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].InConGrpFlag = 0;
    }
    CAN1_SendQueue.Rear = IncCAN1Rear;
}

void Can1Funtion(CanRxMsg rx_message)
{
	Rx_message=rx_message;
	// 根据不同的协议ID执行不同的操作
	if (rx_message.ExtId == 0x01020201 && rx_message.Data[0] == 'M')	// 使能/失能
		ModeFlag = 0x21;

	if (rx_message.ExtId == 0x01020202 )	// 速度设置
		ModeFlag = 0x22;

	if (rx_message.ExtId == 0x01020203)	  // 角度设置
		ModeFlag = 0x23;

	if (rx_message.ExtId == 0x01020204 && rx_message.Data[0] == 'A' && rx_message.Data[1] == 'V')	// 速度查询
		ModeFlag = 0x24;

	if (rx_message.ExtId == 0x01020205 && rx_message.Data[0] == 'A' && rx_message.Data[1] == 'I')	// 电流查询
		ModeFlag = 0x25;

	if (rx_message.ExtId == 0x010202FF && rx_message.Data[0] == 'R' && rx_message.Data[1] == 'S')	// 板子复位
	{
		__set_FAULTMASK(1);	   // 关闭所有的中断，确保执行复位时不被中断打断
		NVIC_SystemReset();	   // 系统软件复位，配置好的外设寄存器也一起复位
	}
}

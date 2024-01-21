#include "canctrllist.h"

// 初始化报文控制表
void MesgCtrlList_Init(MesgControlGrpTypeDef* CAN_MesgSendList, uint8_t CAN_X)
{
    unsigned char NodeNum = 0;
    if(CAN_1 == CAN_X)
        // flag.Can1CtrlList_Enable=1;
        NodeNum = CAN1_NodeNumber;
    else
        //flag.Can1CtrlList_Enable=1;
        NodeNum = CAN2_NodeNumber;

    for(unsigned char i=0; i<NodeNum;i++)
    {
		CAN_MesgSendList[i].SendSem=0;
		CAN_MesgSendList[i].TimeOut=0;
		CAN_MesgSendList[i].SendNumber = 0;
		CAN_MesgSendList[i].ReceiveNumber = 0;
		CAN_MesgSendList[i].SendQueue.Rear = 0;
		CAN_MesgSendList[i].SendQueue.Front = 0;
		CAN_MesgSendList[i].QueueFullTimeOut = 0;
		CAN_MesgSendList[i].SendQueue.Can_DataSend[0].InConGrpFlag=0;
    }

}

void Can_MesgCtrlList(MesgControlGrpTypeDef* CAN_MesgSendList, Can_SendQueueTypedef* can_queue, uint8_t CAN_x)
{
	u8 CAN_NodeNumber=0;
	u16 ControlListID=0xFF;
	
	if(can_queue->Can_DataSend[can_queue->Front].InConGrpFlag==0)// 入控制表标志位为0时就直接退出
		return;

	if(CAN_x==CAN_1)
	{
		CAN_NodeNumber=CAN1_NodeNumber;
	}
	else
	{
		CAN_NodeNumber=CAN2_NodeNumber;
		ControlListID=can_queue->Can_DataSend[can_queue->Front].ID-0x301;//报文控制块映射(这是与主控对接部分的代码，到时候要针对主控发的报文进行相应调整)
	}

	if(ControlListID<CAN_NodeNumber)//临时屏蔽某板子只需在此处增加 &&ControlListID != X
	{
		if(CanQueue_ifEmpty(&CAN_MesgSendList[ControlListID].SendQueue))// 队列是否为空
		{
			if(CAN_x==CAN_1)
				flag.Can1SendqueueFULL++;
			else 
				flag.Can2SendqueueFULL++;
		}
		else
		{
			CAN_MesgSendList[ControlListID].SendSem++;
			CAN_MesgSendList[ControlListID].SendNumber++;

			CAN_MesgSendList[ControlListID].SendQueue.Can_DataSend[CAN_MesgSendList[ControlListID].SendQueue.Rear].ID=can_queue->Can_DataSend[can_queue->Front].ID;
			CAN_MesgSendList[ControlListID].SendQueue.Can_DataSend[CAN_MesgSendList[ControlListID].SendQueue.Rear].DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
			CAN_MesgSendList[ControlListID].SendQueue.Can_DataSend[CAN_MesgSendList[ControlListID].SendQueue.Rear].InConGrpFlag = can_queue->Can_DataSend[can_queue->Front].InConGrpFlag;
			memcpy(CAN_MesgSendList[ControlListID].SendQueue.Can_DataSend[CAN_MesgSendList[ControlListID].SendQueue.Rear].Data,  can_queue->Can_DataSend[can_queue->Front].Data,
				sizeof(u8)*CAN_MesgSendList[ControlListID].SendQueue.Can_DataSend[CAN_MesgSendList[ControlListID].SendQueue.Rear].DLC);
			CAN_MesgSendList[ControlListID].SendQueue.Rear=(CAN_MesgSendList[ControlListID].SendQueue.Rear+1)%CAN_QUEUESIZE;
		}
	}


    
} 

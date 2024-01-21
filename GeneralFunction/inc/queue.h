#ifndef _QUEUE_H
#define _QUEUE_H

#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"	//include后memcpy函数才不会报warning 

#include "param.h"
#include "can1.h"
#include "led.h"

#define CAN_HaveSentQueueSize 8
#define CAN_QUEUESIZE 100		//[发送队列长度]

#define IncCAN1Rear (CAN1_SendQueue.Rear+1)%CAN_QUEUESIZE
#define IncCAN2Rear	(CAN2_SendQueue.Rear+1)%CAN_QUEUESIZE
#define VESCRear	(VESC_SendQueue.Rear+1)%CAN_QUEUESIZE
#define ZdriveRear	(Zdrive_SendQueue.Rear+1)%CAN_QUEUESIZE

#define CAN_1 1
#define CAN_2 2

#define CAN1_NodeNumber 6u
#define CAN2_NodeNumber 6u

typedef struct{
	u8 IDE;
	u32 ID;
	u8 DLC;
	u8 Data[8];
	u8 InConGrpFlag;
}Can_DatasendTypeDef;

typedef struct{
	u8 Front,Rear;
	Can_DatasendTypeDef Can_DataSend[CAN_QUEUESIZE];
}Can_SendQueueTypedef;

typedef struct{
	s32 SendNumber;// 计数发送时自增，接收时自减
	s32 ReceiveNumber;// 发送到此节点的报文总数
	u32 QueueFullTimeOut;// 发送队列已满时自增
	u8 TimeOut;// SendSem>0时的自增量
	s32 SendSem;// 发送时自增，接收时自减
	Can_SendQueueTypedef SendQueue;// 发送消息的临时队列，当Timeout大于超时阈值时，丢失的消息再次发送到总线。
}MesgControlGrpTypeDef;

extern Can_DatasendTypeDef Can_Datasend;
extern Can_SendQueueTypedef CAN1_SendQueue,CAN2_SendQueue,VESC_SendQueue,Zdrive_SendQueue;


void Can_SendQueueInit(void);
bool CanQueue_ifEmpty(Can_SendQueueTypedef* can_queue);
bool CanQueue_ifFull(Can_SendQueueTypedef* can_queue);
void Can_DeQueue(CAN_TypeDef* CANx,Can_SendQueueTypedef* can_queue);

void MesgCtrlList_Init(MesgControlGrpTypeDef* CtrlList, u8 CAN_x);
void Mail_Queue2CtlList(MesgControlGrpTypeDef* CtrlList,Can_SendQueueTypedef* can_queue,u8 CAN_x);
void CtrlList_Func(MesgControlGrpTypeDef* CtrlList, Can_SendQueueTypedef* can_queue,u8 CAN_x );


void Can_MesgCtrlList(MesgControlGrpTypeDef* CAN_MesgSendList, Can_SendQueueTypedef* can_queue, uint8_t CAN_x);

extern MesgControlGrpTypeDef Can1MsgCtrllist[CAN1_NodeNumber],Can2MsgCtrllist[CAN2_NodeNumber];

#endif

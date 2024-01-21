#include "queue.h"

Can_DatasendTypeDef Can_Datasend;
Can_SendQueueTypedef CAN1_SendQueue,CAN2_SendQueue,VESC_SendQueue,Zdrive_SendQueue;
MesgControlGrpTypeDef Can1MsgCtrllist[CAN1_NodeNumber],\
                  Can2MsgCtrllist[CAN2_NodeNumber];


// uint8_t CANTransmit_ERROR;
// uint8_t FirstIgnore;

void Can_SendQueueInit(void)
{
    CAN1_SendQueue.Front = CAN1_SendQueue.Rear = 0;
    CAN2_SendQueue.Front = CAN2_SendQueue.Rear = 0;
    Zdrive_SendQueue.Front = Zdrive_SendQueue.Rear = 0;
		VESC_SendQueue.Front = VESC_SendQueue.Rear = 0;
}


bool CanQueue_ifEmpty( Can_SendQueueTypedef* can_queue)
{
    return( (can_queue->Front) == (can_queue->Rear));// 头尾相同则队空
} 

bool CanQueue_ifFUll(Can_SendQueueTypedef* can_queue)
{
    return( ((can_queue->Rear + 1)%CAN_QUEUESIZE) == can_queue->Front );//循环队列
}


/**
 * @brief 出队函数
 * 
 */
void Can_DeQueue(CAN_TypeDef* CANX, Can_SendQueueTypedef* can_queue)
{
    CanTxMsg TxMessage;

    if(CanQueue_ifEmpty(can_queue))
    {
        flag.CanSendqueueEMPTY = 1;
    }
    else
    {
        // 以下主要是根据不同的通讯对象的ID，选择报文的形式 
        if( can_queue->Can_DataSend[can_queue->Front].IDE == CAN_ID_STD)
        {
            TxMessage.IDE = can_queue->Can_DataSend[can_queue->Front].IDE;
            TxMessage.StdId = can_queue->Can_DataSend[can_queue->Front].ID;
        }
        else
        {
            if((can_queue->Can_DataSend[can_queue->Front].ID & 0xf0000000) != 0) //vesc
            {
                // 这里为什么这样写未知
                can_queue->Can_DataSend[can_queue->Front].ID = can_queue->Can_DataSend[can_queue->Front].ID & 0x0fffffff;
            }
            TxMessage.IDE=CAN_ID_EXT;
            TxMessage.ExtId = can_queue->Can_DataSend[can_queue->Front].ID;
        }
       if(CAN1 == CANX)
           Can_MesgCtrlList(Can1MsgCtrllist, &CAN1_SendQueue, CAN_1);// CAN1_SendQueue实际上并没有进入控制表
       else
           Can_MesgCtrlList(Can2MsgCtrllist, &CAN2_SendQueue, CAN_2);

        TxMessage.DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
        TxMessage.RTR = CAN_RTR_Data;

        memcpy(TxMessage.Data, (can_queue->Can_DataSend[can_queue->Front].Data), TxMessage.DLC * sizeof(int8_t));

        can_queue->Front = (can_queue->Front + 1) % CAN_QUEUESIZE; // 出队后头指针位置向后移动

        CAN_Transmit(CANX , &TxMessage); //发送报文到总线上。
    }
} 

#ifndef _PARAM_H
#define _PARAM_H
#include "stm32f4xx.h"
#include "stdbool.h"

#include "macro.h"
#include "DJmotor.h"
#include "vesc.h"
#include "led8.h"

typedef struct{
 u8 TxBuffer_USART1[USART1_Tx_BufferSize]; //串口一发送数据
 u8 RxBuffer_USART1[USART1_Rx_BufferSize]; //串口一接收数据

 u8 TxBuffer_USART2[USART2_Tx_BufferSize]; //串口二发送数据
 u8 RxBuffer_USART2[USART2_Rx_BufferSize]; //串口二接收数据

 u8 TxBuffer_USART3[USART3_Tx_BufferSize]; //串口三发送数据
 u8 RxBuffer_USART3[USART3_Rx_BufferSize]; //串口三接收数据
} Usart_Struct;

typedef struct{	
    // bool F4Valve_Status; 							//当前F4电磁阀的状态
	// bool Throw_When_Arrived;					//是否进行到达位置后就扔箭操作
	// bool Take_Arrow;
	// bool Pick_Up_Arrow;
	volatile bool Can1CtrlList_Enable; // CAN1报文控制表启动标志位
	volatile bool Can2CtrlList_Enable; // CAN2报文控制表启动标志位
	volatile bool Can1SendqueueFULL;   // CAN1发送队列满
	volatile bool Can2SendqueueFULL;   // CAN2发送队列满
	volatile bool VESCSendqueueFULL;   // CAN2发送队列满
	volatile bool CanSendqueueEMPTY;   // CAN队列为空
	volatile bool has_timeout[8];      // CAN报文接收超时
	volatile bool Usart1DMASendFinish; // Usart1_DMA传输
	volatile bool Usart2DMASendFinish; // Usart2_DMA传输
	volatile bool FlagMusic;   //唱歌标志位
	bool FlagShooting;         //发射标志位
	bool FlagShootSpeedCheck;  //发射速度检测标志位
	bool FlagShootSpeedArrived; //发射速度达到指定速度阈值范围
	u8 FlagTakeArrow;  	   //取箭标志位
	bool valve[4];                     //当前电磁阀状态	
	bool if_pick_up_arrow;
	bool if_when_arrived_release;
} FlagTypeDef;


typedef struct{
	volatile bool Normal;
	volatile bool Stuck;
	volatile bool Timeout;
	volatile bool CANErr;
}LEDTaskTypedef;

typedef struct{
	bool AllowFlag;
	u16 BeepOnNum;
	bool error;
	bool answer_master_flag;
}BeepStructTypedef;

void param_Init(void);

extern Usart_Struct usart;
extern FlagTypeDef flag;
extern LEDTaskTypedef LedTask;
extern BeepStructTypedef  B1p;
#endif

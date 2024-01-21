#ifndef _PARAM_H
#define _PARAM_H
#include "stm32f4xx.h"
#include "stdbool.h"

#include "macro.h"
#include "DJmotor.h"
#include "vesc.h"
#include "led8.h"

typedef struct{
 u8 TxBuffer_USART1[USART1_Tx_BufferSize]; //����һ��������
 u8 RxBuffer_USART1[USART1_Rx_BufferSize]; //����һ��������

 u8 TxBuffer_USART2[USART2_Tx_BufferSize]; //���ڶ���������
 u8 RxBuffer_USART2[USART2_Rx_BufferSize]; //���ڶ���������

 u8 TxBuffer_USART3[USART3_Tx_BufferSize]; //��������������
 u8 RxBuffer_USART3[USART3_Rx_BufferSize]; //��������������
} Usart_Struct;

typedef struct{	
    // bool F4Valve_Status; 							//��ǰF4��ŷ���״̬
	// bool Throw_When_Arrived;					//�Ƿ���е���λ�ú���Ӽ�����
	// bool Take_Arrow;
	// bool Pick_Up_Arrow;
	volatile bool Can1CtrlList_Enable; // CAN1���Ŀ��Ʊ�������־λ
	volatile bool Can2CtrlList_Enable; // CAN2���Ŀ��Ʊ�������־λ
	volatile bool Can1SendqueueFULL;   // CAN1���Ͷ�����
	volatile bool Can2SendqueueFULL;   // CAN2���Ͷ�����
	volatile bool VESCSendqueueFULL;   // CAN2���Ͷ�����
	volatile bool CanSendqueueEMPTY;   // CAN����Ϊ��
	volatile bool has_timeout[8];      // CAN���Ľ��ճ�ʱ
	volatile bool Usart1DMASendFinish; // Usart1_DMA����
	volatile bool Usart2DMASendFinish; // Usart2_DMA����
	volatile bool FlagMusic;   //�����־λ
	bool FlagShooting;         //�����־λ
	bool FlagShootSpeedCheck;  //�����ٶȼ���־λ
	bool FlagShootSpeedArrived; //�����ٶȴﵽָ���ٶ���ֵ��Χ
	u8 FlagTakeArrow;  	   //ȡ����־λ
	bool valve[4];                     //��ǰ��ŷ�״̬	
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

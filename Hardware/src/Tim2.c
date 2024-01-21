#include "Tim2.h"
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = (TIM2_Cycle*1000)-1;// TIM2_Cycle=0.2
	TIM_TimeBaseInitStructure.TIM_Prescaler =84-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	//tout = 200*84/84Mhz = 1ms

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//指定抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//指定响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static int VESC_Dequeue_Cnt=0;
static int Zdrive_Dequeue_Cnt=0;
void TIM2_IRQHandler(void) // 0.2ms一次中断 
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update)==SET)
    {
        Can_DeQueue(CAN2, &CAN2_SendQueue); // 回复主控的报文队列

        Can_DeQueue(CAN1, &CAN1_SendQueue); 


        if((VESC_Dequeue_Cnt++)>=15) // 0.2*15=3ms发送一次
        {
            VESC_Dequeue_Cnt=0;
            Can_DeQueue(CAN2, &VESC_SendQueue);
        }
				if((Zdrive_Dequeue_Cnt++) > 20)
				{
					  Zdrive_Dequeue_Cnt=0;
					  Can_DeQueue(CAN2, &Zdrive_SendQueue);
				}
		}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

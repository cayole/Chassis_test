#include "main.h"

int main(void)

{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	CAN1_Configuration();
	CAN2_Config();

	Tim_Config(1000, 84);
	TIM2_Configuration();
	USART2_Configuration();

	LED_Init();
	Led8_Configuration();
	Beep_Config();
	
	Valve_Init();
	SpiInit();

	Can_SendQueueInit();
	param_Init();

	int i=0;
    int j=0;
	int k=0;
	AnswerReset(); // 重启
	OSInit();
	OSTaskCreate(start_task, (void *)0, (OS_STK *)&START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO); // 创建起始任务
	OSStart();			
 	// 启动UCOS
}

void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;

	pdata = pdata;
	OS_CPU_SysTickInit();
	OS_ENTER_CRITICAL();
	RUN = OSSemCreate(0);

#if BEEP_TASK
	OSTaskCreate(Beep_Task, (void *)0, &BEEP_TASK_STK[TASK_STK_SIZE - 1], BEEP_TASK_PRIO);
#endif
#if LED_TASK
	OSTaskCreate(Led_Task, (void *)0, (OS_STK *)&LED_TASK_STK[TASK_STK_SIZE - 1], LED_TASK_PRIO);
#endif
	OSTaskCreate(Send_Task, (void *)0, &SEND_TASK_STK[TASK_STK_SIZE - 1], SEND_TASK_PRIO);
	OSTaskCreate(ChassisTest_Task,(void *)0,&CHASSISTEST_TASK_STK[TASK_STK_SIZE-1],CHASSISTEST_TASK_STK_PRIO);
	OSTaskCreate(Chassis_Task,(void *)0,&CHASSIS_TASK_STK[TASK_STK_SIZE-1],CHASSIS_TASK_STK_PRIO);
	
	OS_EXIT_CRITICAL(); // 退出临界区
	OSTimeDly(1000);
	beep_show(2);
	OSTaskSuspend(START_TASK_PRIO); // 挂起起始任务
}



/**********************************************/

/*底盘测试
mode:
0x00失能
0x01使能
0x02启动(速度模式)
0xff保留
*/
u8 mode;
float testspeed;
static void ChassisTest_Task(void *pdata)
{
	pdata=pdata;
	
	testspeed=0,mode=0xff;
	

	while(1)
	{
		switch(mode)
		{
			case(0x00):
			{
				VESCmotor[0].enable=false;
				VESCmotor[1].enable=false;
				VESCmotor[2].enable=false;
				VESCmotor[3].enable=false;
				
				VESCmotor[0].begin = false;
				VESCmotor[1].begin = false;
				VESCmotor[2].begin = false;
				VESCmotor[3].begin = false;
				break;
			}
			case(0x01):
			{
				VESCmotor[0].enable = true;
				VESCmotor[1].enable = true;
				VESCmotor[2].enable = true;
				VESCmotor[3].enable = true;
				break;
			}
			case(0x02):
			{
				VESCmotor[0].begin = 1;
				VESCmotor[1].begin = 1;
				VESCmotor[2].begin = 1;
				VESCmotor[3].begin = 1;
				
				VESCmotor[0].valueSet.speed = testspeed;
				VESCmotor[1].valueSet.speed = testspeed;
				VESCmotor[2].valueSet.speed = testspeed;
				VESCmotor[3].valueSet.speed = testspeed;
				
				mode=0xff;
				break;
			}
			default:
				break;
		}
		

	 if(ABS(VESCmotor[0].valueReal.speed)>5000
		||ABS(VESCmotor[1].valueReal.speed)>5000
		||ABS(VESCmotor[2].valueReal.speed)>5000
		||ABS(VESCmotor[3].valueReal.speed)>5000)//速度限制
	 {
		 VESCmotor[0].enable = false;
		 VESCmotor[1].enable = false;
		 VESCmotor[2].enable = false;
		 VESCmotor[3].enable = false;
	 }
		 
	OSTimeDly(1000);

	}
}

CanRxMsg Rx_message;
static void Chassis_Task(void *pdata)
{
	pdata=pdata;
	for(;;)
	{
		switch (ModeFlag) {
			case 0x21:
				ENUNableChassis(Rx_message);	   // 使能，失能
				ModeFlag = 0xff;
				break;
			case 0x22:
				SetChassisSpeed(Rx_message);	 // 设置速度
				ModeFlag = 0xff;
				break;
			case 0x23:
				SetChassisAngle(Rx_message);	  // 设置角度
				ModeFlag = 0xff;
				break;
			case 0x24:
				AskChassisSpeed();	   // 查询速度
				ModeFlag = 0xff;
				break;
			case 0x25:
				AskChassisCurrent();       //查询电流
				ModeFlag = 0xff;
				break;

			default:
				break;
		}
		OSTimeDly(100);
	}
}



/**************************************************/




static void Beep_Task(void *pdata)
{
	pdata = pdata;
	for (;;)
	{		
		BeepTask();
		OSTimeDly(100);
	}
}
u16 as5047pRecMsg;
float recAngle;
static void Send_Task(void *pdata)
{
	pdata=pdata;
	OSTimeDly(1000);
	ReadAS5047pData(&as5047pRecMsg); // 第一次cmd指令
	while(1)
	{
		if(ReadAS5047pData(&as5047pRecMsg))
			recAngle=as5047pRecMsg * 0.02197265625f;
		OSTimeDly(100);
	}
}

static void Led_Task(void *pdata)
{
	while (1)
	{
		if (LedTask.Normal)
		{
			LED_GREEN_TOGGLE;
		}
		if (LedTask.Stuck)
		{
			LED_RED_TOGGLE; // 堵转
		}
		if (LedTask.Timeout)
		{
			LED_YELLOW_TOGGLE; // 超时报警
		}
		if (LedTask.CANErr)
		{
			LED_BLUE_TOGGLE;
		}
		OSTimeDly(5000);
	}
}



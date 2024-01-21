#ifndef _MAIN_H
#define _MAIN_H


#include "stm32f4xx.h"
#include "led.h"
#include "Tim3.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "dma.h"
#include "can1.h"
#include "includes.h"
#include "sys.h"
#include "beep.h"
#include "DJmotor.h"
#include "can2.h"
#include "Tim2.h"
#include "led8.h"
#include "param.h"
#include "cylinder.h"
#include "as5047p.h"


#define LED_TASK   1
#define BEEP_TASK  1
#define LCD_TASK   1

OS_EVENT *RUN;
#define delay_osrunning		OSRunning


#define START_TASK_PRIO             5  //任务优先级
#define START_STK_SIZE              1024  //任务堆栈大小
OS_STK START_TASK_STK[START_STK_SIZE];  //任务堆栈
void start_task(void *pdata);           //任务函数


#define	BEEP_TASK_PRIO	43	//设置任务优先级
#define	LED_TASK_PRIO	44 //设置任务优先级
#define SEND_TASK_PRIO 11

#define CHASSISTEST_TASK_STK_PRIO 15//底盘测试任务优先级
#define CHASSIS_TASK_STK_PRIO 14//底盘任务优先级
 
#define TASK_STK_SIZE 1024

__align(8) OS_STK BEEP_TASK_STK[TASK_STK_SIZE];	//任务堆栈
__align(8) OS_STK LED_TASK_STK[TASK_STK_SIZE];	//任务堆栈
__align(8) OS_STK SEND_TASK_STK[TASK_STK_SIZE];

__align(8) OS_STK CHASSISTEST_TASK_STK[TASK_STK_SIZE];//底盘测试任务堆栈
__align(8) OS_STK CHASSIS_TASK_STK[TASK_STK_SIZE];	//底盘任务堆栈

static void Beep_Task(void *pdata);	//任务函数
static void Led_Task(void *pdata);	//任务函数
static void Send_Task(void *pdata);

static void ChassisTest_Task(void *pdata);//底盘测试任务
static void Chassis_Task(void *pdata);//底盘任务


#endif



/*
 * @Author: qijunhao 2020302191427@whu.edu.cn
 * @Date: 2022-10-03 20:26:14
 * @LastEditors: qijunhao 2020302191427@whu.edu.cn
 * @LastEditTime: 2022-12-07 22:01:57
 * @FilePath: \Chassis_f\User\src\Tim3.c
 * @Description:
 *
 * Copyright (c) 2022 by qijunhao 2020302191427@whu.edu.cn, All Rights Reserved.
 */
#include "Tim3.h"

void Tim_Config(uint16_t per, uint16_t pre)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef Tim3_BaseInitStruct;
	Tim3_BaseInitStruct.TIM_Period = per - 1;
	Tim3_BaseInitStruct.TIM_Prescaler = pre - 1;
	Tim3_BaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	Tim3_BaseInitStruct.TIM_ClockDivision = 0;

	TIM_TimeBaseInit(TIM3, &Tim3_BaseInitStruct);

	NVIC_InitTypeDef NVIC_TIM3_Struct;
	NVIC_TIM3_Struct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_TIM3_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_TIM3_Struct.NVIC_IRQChannelSubPriority = 0;
	NVIC_TIM3_Struct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init(&NVIC_TIM3_Struct);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void TIM_PWM_Config(void)
{
	//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE); // TIM13
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); // TIM14
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // GPIOA

	// GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   // 复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // 上拉

	GPIO_Init(GPIOA, &GPIO_InitStructure);		  // 初始化 GPIO
	GPIO_SetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7); // 置位，灯灭

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM13); // 初始化GPIO口的复用功能
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM14);

	// 初始化定时器
	TIM_TimeBaseInitTypeDef Tim_BaseInitStruct;
	Tim_BaseInitStruct.TIM_Period = 5000 - 1; //  2.5ms
	Tim_BaseInitStruct.TIM_Prescaler = 84 - 1;
	Tim_BaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	Tim_BaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM13, &Tim_BaseInitStruct);
	TIM_TimeBaseInit(TIM14, &Tim_BaseInitStruct);

	TIM_OCInitTypeDef TIM_OCInitStruce_13;
	TIM_OCInitStruce_13.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruce_13.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruce_13.TIM_OutputState = TIM_OutputState_Enable;

	TIM_OCInitTypeDef TIM_OCInitStruce_14;
	TIM_OCInitStruce_14.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruce_14.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruce_14.TIM_OutputState = TIM_OutputState_Enable;

	TIM_OC1Init(TIM13, &TIM_OCInitStruce_13);
	TIM_OC1Init(TIM14, &TIM_OCInitStruce_14);

	TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM13, ENABLE);
	TIM_ARRPreloadConfig(TIM14, ENABLE);

	TIM_Cmd(TIM13, ENABLE);
	TIM_Cmd(TIM14, ENABLE);

	TIM_CtrlPWMOutputs(TIM13, ENABLE);
	TIM_CtrlPWMOutputs(TIM14, ENABLE);
}

static int VESCCount = 0;
static int ZdriveCount = 0;
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{

#ifdef USE_DJ
		DJFunction(); // 每1ms处理一次
#endif

#ifdef USE_VESC

		if (VESCCount++ >= 20) // 50ms 1次
		{
			VESCCount = 0;
			VESCFunc();
		}
#endif
		
#ifdef USE_ZDRIVE
		if(ZdriveCount++ >= 20) // 50ms
		{
			ZdriveFunction();
			ZdriveCount=0;
		}
#endif

		// #ifdef USE_ELMO
		// for (int i = 0; i < 5;i++)
		// 	ifElmoTimeout(i);
		// #endif
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清理定时中断标志位
}

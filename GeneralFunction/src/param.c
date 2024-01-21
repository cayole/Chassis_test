/*
 * @Author: qijunhao 2020302191427@whu.edu.cn
 * @Date: 2022-10-03 20:26:14
 * @LastEditors: qijunhao 2020302191427@whu.edu.cn
 * @LastEditTime: 2022-12-07 21:46:12
 * @FilePath: \Chassis_f\User\src\param.c
 * @Description:
 *
 * Copyright (c) 2022 by qijunhao 2020302191427@whu.edu.cn, All Rights Reserved.
 */
#include "param.h"

Usart_Struct usart;
FlagTypeDef flag;
LEDTaskTypedef LedTask;
BeepStructTypedef B1p;
void param_Init(void)
{
	//	flag.Can1MsgCtrllistEnable=1;
	//	flag.Can2MsgCtrllistEnable=0;

	Led8DisData(0);
	LedTask.Normal = 1;
	B1p.AllowFlag = true;
	
	Zdrive_Init();
	DJmotor_Init();
	VESC_Init();
}

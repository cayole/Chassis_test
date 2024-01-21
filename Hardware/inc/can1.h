#ifndef _CAN_H
#define _CAN_H
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "tim3.h"
#include "param.h"
#include "macro.h"
#include "includes.h"
#include "chassis.h"


void CAN1_Configuration(void);
void AnswerReset(void);
#endif

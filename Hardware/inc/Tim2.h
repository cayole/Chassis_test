#ifndef _TIM2_H
#define _TIM2_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "macro.h"
#include "param.h"
#include "queue.h"
//#include "canctrllist.h"

#include "chassis.h"

#define TIM2_Cycle 0.2 //单位ms

void TIM2_Configuration(void);



#endif

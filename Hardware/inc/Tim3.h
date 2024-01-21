#ifndef _TIM3_H
#define _TIM3_H
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "led.h"
#include "can1.h"
#include "DJmotor.h"
#include "VESC.h"
#include "Zdrive.h"
#include "elmo.h"
void Tim_Config(uint16_t per,uint16_t pre);
void TIM_PWM_Config(void);

#endif

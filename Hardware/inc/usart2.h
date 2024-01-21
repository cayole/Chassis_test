#ifndef __USART2_H
#define __USART2_H

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"

#include "param.h"
#include "vesc.h"
#include "command.h"
#include "beep.h"
#include "delay.h"

extern bool getmsgflag;
void USART2_Configuration(void);
void UsartLCDshow(void);
void USART2_Send(u8 count);

#endif

#ifndef _CAN2_H
#define _CAN2_H

#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "includes.h"

#include "DJmotor.h"
#include "macro.h"
#include "elmo.h"
#include "vesc.h"
#include "zdrive.h"

void CAN2_Config(void);

#endif

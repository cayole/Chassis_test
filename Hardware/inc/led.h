#ifndef _LED_H
#define _LED_H
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "sys.h"
#include "stm32f4xx_rcc.h"
#include "includes.h"
#include "stdbool.h"
#include "delay.h"

#define LED_GPIO_PORT    	GPIOA
#define LED_R_GPIO_PIN		GPIO_Pin_4	
#define LED_Y_GPIO_PIN		GPIO_Pin_5
#define LED_B_GPIO_PIN		GPIO_Pin_6	
#define LED_G_GPIO_PIN		GPIO_Pin_7	

#define LED_RED           PIN4
#define LED_YELLOW        PIN5
#define LED_BLUE          PIN6
#define LED_GREEN         PIN7

#define LED_RED_OFF  			GPIOA->BSRRL = PIN4		//输出高电平
#define LED_YELLOW_OFF  		GPIOA->BSRRL = PIN5
#define LED_BLUE_OFF    	    GPIOA->BSRRL = PIN6
#define LED_GREEN_OFF  			GPIOA->BSRRL = PIN7

#define LED_RED_ON   			GPIOA->BSRRH = PIN4		//输出低电平·
#define LED_YELLOW_ON   		GPIOA->BSRRH = PIN5
#define LED_BLUE_ON   			GPIOA->BSRRH = PIN6
#define LED_GREEN_ON   			GPIOA->BSRRH = PIN7

#define LED_RED_TOGGLE			GPIOA->ODR ^= PIN4		//toggle切换
#define LED_YELLOW_TOGGLE 	GPIOA->ODR ^= PIN5
#define LED_BLUE_TOGGLE			GPIOA->ODR ^= PIN6
#define LED_GREEN_TOGGLE		GPIOA->ODR ^= PIN7




#define LED_R_ON            GPIO_ResetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);

#define LED_R_OFF           GPIO_SetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);

#define LED_B_ON            GPIO_ResetBits(LED_GPIO_PORT,LED_B_GPIO_PIN);

#define LED_B_OFF           GPIO_SetBits(LED_GPIO_PORT,LED_B_GPIO_PIN);

#define LED_G_ON            GPIO_ResetBits(LED_GPIO_PORT,LED_G_GPIO_PIN);

#define LED_G_OFF           GPIO_SetBits(LED_GPIO_PORT,LED_G_GPIO_PIN);

#define LED_Y_ON            GPIO_ResetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);

#define LED_Y_OFF           GPIO_SetBits(LED_GPIO_PORT,LED_Y_GPIO_PIN);

void LED_Init(void);
void LED_WATER_1(void);
//void LED_WATER_2(void);
void LED_PWM_BREATHE(uint16_t H,uint16_t L);// ˫������

#endif


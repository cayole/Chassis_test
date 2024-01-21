#ifndef _BEEP_H
#define _BEEP_H
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "delay.h"
#include "param.h"
#include "stdbool.h"




// BSRR 端口设置/清除寄存器
#define BEEP_OFF         GPIOA->BSRRH=GPIO_Pin_8
#define BEEP_ON          GPIOA->BSRRL=GPIO_Pin_8

#define BEEP_TOGGLE      GPIOA->ODR ^=GPIO_Pin_8

#define beep_show(x)     B1p.BeepOnNum += (x)
#define beep_clear()     B1p.BeepOnNum =0;

#define s_0  2048

void BEEP_Init(void);
void music_1(void);
void music_2(void);
void Sound(u16 frq);
void Beep_Config(void);
void BeepTask(void);

enum low_musical_scale{
    L_1 = 262,
    L_2 = 294,
    L_3 = 330,
    L_4 = 349,
    L_5 = 392,
    L_6 = 440,
    L_7 = 494,
};
enum normal_musical_scale
{
    N_1 = 523,
    N_2 = 587,
    N_3 = 659,
    N_4 = 698,
    N_5 = 784,
    N_6 = 880,
    N_7 = 988,
};
enum high_musical_scale
{
    H_1 = 1046,
    H_2 = 1175,
    H_3 = 1318,
    H_4 = 1397,
    H_5 = 1568,
    H_6 = 1760,
    H_7 = 1967,
};



#endif



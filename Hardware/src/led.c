#include "led.h"
//LED IO ��ʼ��

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;// ����ṹ�����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ�� GPIOF ʱ��
	
	GPIO_InitStructure.GPIO_Pin = LED_R_GPIO_PIN | LED_Y_GPIO_PIN | LED_B_GPIO_PIN | LED_G_GPIO_PIN;//LED0 �� LED1 ��Ӧ IO ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);//��ʼ�� GPIO
	GPIO_SetBits(LED_GPIO_PORT,LED_R_GPIO_PIN | LED_Y_GPIO_PIN | LED_B_GPIO_PIN | LED_G_GPIO_PIN);// ��λ������
}



void LED_WATER_1()
{
	// ���
	GPIO_ResetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);
	Delay_us(5000); 	
	GPIO_SetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);
	
	// �Ƶ�
	GPIO_ResetBits(LED_GPIO_PORT,LED_Y_GPIO_PIN);
	Delay_us(5000); 	
	GPIO_SetBits(LED_GPIO_PORT,LED_Y_GPIO_PIN); 
	
	// ����
	GPIO_ResetBits(LED_GPIO_PORT,LED_B_GPIO_PIN);
	Delay_us(5000); 	
	GPIO_SetBits(LED_GPIO_PORT,LED_B_GPIO_PIN); 
	// �̵�
	GPIO_ResetBits(LED_GPIO_PORT,LED_G_GPIO_PIN);
	Delay_us(5000); 	
	GPIO_SetBits(LED_GPIO_PORT,LED_G_GPIO_PIN); 

}


//void LED_WATER_2(void)
//{
//	if(time==0)
//		{
//			GPIO_SetBits(LED_GPIO_PORT,LED_G_GPIO_PIN); 
//			GPIO_ResetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);
//		}
//		else if(time==1)
//		{
//			GPIO_SetBits(LED_GPIO_PORT,LED_R_GPIO_PIN);
//			GPIO_ResetBits(LED_GPIO_PORT,LED_Y_GPIO_PIN);
//		}
//		else if(time==2)
//		{
//			GPIO_SetBits(LED_GPIO_PORT,LED_Y_GPIO_PIN); 
//			GPIO_ResetBits(LED_GPIO_PORT,LED_B_GPIO_PIN);
//		}
//		else if(time==3)
//		{
//			GPIO_SetBits(LED_GPIO_PORT,LED_B_GPIO_PIN); 
//			GPIO_ResetBits(LED_GPIO_PORT,LED_G_GPIO_PIN);
//		}
//}

// ˫�����ƺ���
void LED_PWM_BREATHE(uint16_t H,uint16_t L)
{
	uint16_t id,CRR=0,CRR2=5000;
//	LED_WATER_1();
//	LED_WATER_2();
	
	if(CRR<=0||CRR2<=L)
		id=1;
	if(CRR>=H||CRR2>=4999) 
		id=0;
	
	if(id)
	{CRR++;CRR2++;}
	else 
	{CRR--;CRR2--;}
	
	TIM_SetCompare1(TIM13,CRR);
	TIM_SetCompare1(TIM14,CRR2);
}

//�������´���,֧�� printf ����,������Ҫѡ�� use MicroLIB
#include "usart.h"
#if 1
#pragma import(__use_no_semihosting) 
//��׼����Ҫ��֧�ֺ��� 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout; 
//����_sys_exit()�Ա���ʹ�ð�����ģʽ 
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض��� fputc ����
int fputc(int ch, FILE *f)
{ 
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
  USART_SendData(USART1,(uint8_t)ch); 
  return ch;
}
#endif


uint16_t USART_RX_STA=0;
uint8_t USART_RX_BUFF[USART_REC_LEN];


void USART_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);// USART1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);// GPIOA

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;

    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

    USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_BaudRate=115200;
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;
    USART_InitStructure.USART_Parity=USART_Parity_No;
    USART_InitStructure.USART_StopBits=USART_StopBits_1;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    
    USART_Init(USART1,&USART_InitStructure);

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);// �����ж�ʹ��

    NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;

    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1,ENABLE);
}

//void USART1_IRQHandler(void)
//{
//	 
//    uint8_t res;
//    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
//    {
//        res=USART_ReceiveData(USART1);
//        if((USART_RX_STA & 0x8000)==0)// û�н��յ�0x0a
//		{
//			if(res!=0x0a)// ���ǽ�����
//			{
//				if(res==0x23)// �յ�#�ַ�
//					USART_RX_STA|=0x4000;// ���Խ��ս���
//				
///*	
//				if(USART_RX_STA & 0x4000)
//			{
//				if(res!=0x0a)
//					USART_RX_STA=0;
//				else
//					USART_RX_STA |=0x8000;
//		
//			}
//*/
//				else 
//				{
//					if(USART_RX_STA & 0x4000)// �Ƿ���Խ���
//					{
//						USART_RX_BUFF[USART_RX_STA&0x3fff]=res;
//						USART_RX_STA++;
//						if(USART_RX_STA>(0x8000))
//							USART_RX_STA=0;// ���մ������¿�ʼ
//				    }
//			
///*	
//					if(res==0x0d)
//					USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUFF[USART_RX_STA&0x3fff]=res;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))
//						USART_RX_STA=0;
//				}

//					*/
//				}
//		    }
//			
//			else //�ǽ�����
//			{
//				USART_RX_STA |=0x8000;
//			}
//        }
//    }
//}

/*****	 ����#.......0x0a��ʽ���루����жϣ�	*********
	SystemInit();
	uint8_t t,len;
	LED_Init();
	USART_Config();
	Delay_ms(1000);
	
	while(1)
	{


		
	
		if(USART_RX_STA & 0x8000)
		{
			len=USART_RX_STA & 0x3fff;
			printf("\r\n�����͵���ϢΪ��\r\n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUFF[t];
				while((USART1->SR & 0x40)==0); 
			}
			printf("\r\n\r\n");
			USART_RX_STA=0;
	
		}else
		{
			times++;
			
			if(times%200==0)
				printf("stm32f405-����ʵ��,�������ݣ��س�����\r\n");
			if(times%30==0)
			{
				GPIO_ResetBits(LED_GPIO_PORT,LED_B_GPIO_PIN);
				Delay_ms(100);
				GPIO_SetBits(LED_GPIO_PORT,LED_B_GPIO_PIN);
				Delay_ms(1000);
			}
		}
		
****************/

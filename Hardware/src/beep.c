#include "beep.h"



void Beep_Config(void)
{
    GPIO_InitTypeDef GPIO_Structure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);

    GPIO_Structure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Structure.GPIO_OType = GPIO_OType_PP;
    GPIO_Structure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Structure);

    BEEP_OFF;

    B1p.AllowFlag = true;
}

//TODO:可以考虑玩点花活
void BeepTask(void)
{
    for (;;)
    {
        if (B1p.AllowFlag)
        {
            if (B1p.BeepOnNum)
            {
                BEEP_ON;
                OSTimeDly(2000);
                BEEP_OFF;
                OSTimeDly(2000);
                B1p.BeepOnNum--;
            }
            if (B1p.error)
            {
                BEEP_ON;
                OSTimeDly(200);
                BEEP_OFF;
                OSTimeDly(500);
                BEEP_ON;
                OSTimeDly(200);
                BEEP_OFF;
                OSTimeDly(20000);
                B1p.error--;
            }
        }
        else
        {
            B1p.BeepOnNum = 0;
        }
//		太吵啦
//        if(B1p.answer_master_flag)
//        {
//            BEEP_ON;
//            OSTimeDly(300);
//            BEEP_OFF;
//            B1p.answer_master_flag = false;
//        }
        OSTimeDly(500);
    }
}


// void Sound(u16 frq)
// {
// 	u32 time;
// 	if(frq != 1000)
// 	{
// //		time = 500000/((u32)frq);
// 		time = 1000000/((u32)frq);
// //		PBeep = 1;
// 		BEEP_ON;//打开蜂鸣器--根据自己的硬件情况调整，通常就是控制蜂鸣器的gpio引脚置1
 
// 		// delay_us(time);
// //		PBeep = 0;
// 		BEEP_OFF;//关闭蜂鸣器--根据自己的硬件情况调整，通常就是控制蜂鸣器的gpio引脚置0
// 		// delay_us(time);
// 	}else
// 		// delay_us(300);
// }
// void music_1(void)
// {
// 	//             低7  1   2   3   4   5   6   7  高1 高2  高3 高4 高5 不发音
//  // uc16 tone[] = {247,262,294,330,349,392,440,494,523,587,659,698,784,1000};//音频数据表
 
// 	//小燕子
// 		// u8 music[]={3,5,8,6,5,13,//音调
// 	    //             3,5,6,8,5,13,
// 	    //             8,10,9,8,9,8,6,8,5,13,
// 		// 			3,5,6,5,6,8,9,5,6,13,
// 		// 			3,2,1,2,13,
// 		// 			2,2,3,5,5,8,2,3,5,13};
// 	uint16_t music[] = {N_3,N_5,H_1,N_6,N_5,s_0,
// 						N_3,N_5,N_6,H_1,N_5,s_0,
// 						H_1,H_3,H_2,H_1,H_2,H_1,N_6,H_1,N_5,s_0,
// 						N_3,N_5,N_6,N_5,N_6,H_1,H_2,N_5,N_6,s_0,
// 						N_3,N_2,N_1,N_2,s_0,
// 						N_2,N_2,N_3,N_5,N_5,H_1,N_2,N_3,N_5,s_0,
// 						};

// 	u8 time[] = {2, 2, 2, 2, 6, 4, //时间
// 				 2, 2, 2, 2, 6, 4,
// 				 6, 2, 4, 4, 2, 2, 2, 2, 6, 4,
// 				 6, 2, 4, 2, 2, 4, 2, 2, 6, 4,
// 				 2, 2, 4, 6, 4,
// 				 4, 2, 2, 4, 4, 4, 2, 2, 6, 4};
// 	u32 yanshi;
// 	u16 i,e;
// 	yanshi = 1;//10;  4;  2
// 	for(i=0;i<sizeof(music)/sizeof(music[0]);i++){
// 		for(e=0;e<((u16)time[i])*music[i]/yanshi;e++){
// 			Sound((u32)music[i]);
// 		}	
// 	}
// }


// void music_2(void)
// {
// 	uint16_t music[] = {N_3,N_3,N_4,N_5,N_5,N_4,N_3,N_2,s_0,
// 						N_1,N_1,N_2,N_3,H_3,L_2,N_2,
// 						};
// 	uint8_t time[16] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
// 	u32 yanshi;
// 	u16 i,e;
// 	yanshi =22 ;//10;  4;  2
// 	for(i=0;i<sizeof(music)/sizeof(music[0]);i++){
// 		for(e=0;e<((u16)time[i])*music[i]/yanshi;e++){
// 			Sound((u32)music[i]);
// 		}	
// 	}
// }

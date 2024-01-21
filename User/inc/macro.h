#ifndef __MACRO_H
#define __MACRO_H

#define USE_DJ
#define USE_VESC
#define USE_ZDRIVE

/* BIT��ţ��������̺��ж� ��δ֪���ܣ�*/
#define BIT0 (0x01 << 0)
#define BIT1 (0x01 << 1)
#define BIT2 (0x01 << 2)
#define BIT3 (0x01 << 3)
#define BIT4 (0x01 << 4)
#define BIT5 (0x01 << 5)
#define BIT6 (0x01 << 6)
#define BIT7 (0x01 << 7)

#define USART1_Tx_BufferSize 256
#define USART2_Tx_BufferSize 256
#define USART3_Tx_BufferSize 32
#define USART1_Rx_BufferSize 32
#define USART2_Rx_BufferSize 32
#define USART3_Rx_BufferSize 32

#define BroadCast_RX 0x00010000
#define ZhuKong_RX 0x00010500
#define ZhuKong_TX 0x00050101
#define Master2DuoJi_TX 0x00070800
#define Duoji2Master_RX 0x00080700
#define PwmDriver2Master_RX 0x00030001

#define RESET_PRO           \
    {                       \
        __set_FAULTMASK(1); \
        NVIC_SystemReset(); \
    }

#define SemPend(sem)                   \
    {                                  \
        sem->OSEventCnt = 0;           \
        OSSemPend(sem, 0, Error##sem); \
    }

#endif

/*
VESC���ٶȺ�λ��ģʽ�ĳ����Լ����㣬Ϊ��ʵʱ����PID ����
�ø�������һ���Լ���PID�㷨		������û��Ҫ��
3.AK80��������		(�����ϰ�Ҫ)
���Ͻ��ն��У����Խ���Ƕȶ�ʧ�������Ѵ󽮵����ݷ��ڶ�ʱ���ﴦ��Ȼ����CAN���ÿ����һ�η���һ���ź������������еȴ��ź�������
(����û��Ҫ��˵����Ϊ��ȫ�Կ��ǣ����������Ļ�����һ�����ȼ��ܸ߶���Ƶ�ʺܿ����������Ӳ���ϵͳ�ĸ���)

TODO:
��һ���µĵ�ŷ���		(û����)


*/

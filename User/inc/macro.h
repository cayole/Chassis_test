#ifndef __MACRO_H
#define __MACRO_H

#define USE_DJ
#define USE_VESC
#define USE_ZDRIVE

/* BIT编号，用于流程号判断 （未知功能）*/
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
VESC的速度和位置模式改成了自己来算，为了实时更改PID 参数
拿个轮子试一下自己的PID算法		（可能没必要）
3.AK80驱动代码		(找乔老板要)
加上接收队列（可以解决角度丢失？），把大疆的数据放在定时器里处理，然后在CAN里边每接收一次发送一个信号量，在任务中等待信号量发送
(可能没必要，说是因为安全性考虑，但是这样的话创建一个优先级很高而且频率很快的任务会增加操作系统的负担)

TODO:
试一下新的电磁阀板		(没问题)


*/

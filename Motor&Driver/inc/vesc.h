#ifndef _VESC_H
#define _VESC_H
#include "queue.h"
#include "mathFunc.h"
#include "pid.h"

#define USEVESC 4

enum VESC_Motor_Mode{VESCCURRENT,VESCRPM,VESCPOSITION,VESCDUTY,VESCHANDBREAK};

typedef struct{

    uint8_t Motor_PolePairs;

}VESCParamTypedef;

typedef struct{
    float current;
    float speed;
    float angle;
    float duty;
    vs32 HandBreak_Current;
    vs32 Break_Current;
    float Angle_ABS;
}VESCValueTypedef;

typedef struct{
    bool timeout;
    bool stuck;
}VESCStatusTypedef;

typedef struct{
    bool Timeout_IfDect;
    bool Stuck_IfDect;
    bool IfReleaseWhenStuck;
    uint16_t MAX_Current;
}VESCLimitTypedef;

typedef struct{
    uint32_t lastRxTim;
    uint32_t timeoutTicks;
    uint32_t stuckcnt;

    vu16 angleNow;
    vu16 anglePrv;
    vs16 distance;
    vs32 position;
}VESCArgumTypedef;

typedef struct {
    bool enable;
    bool begin;
    bool Brake;
    uint8_t mode;
    VESCParamTypedef Param;
    VESCValueTypedef valueSet,valueReal;
    VESCStatusTypedef Status;
    VESCLimitTypedef limit;
    VESCArgumTypedef Argum;
    VESC_PIDTypedef P_Pid,S_Pid;   
}VESCMotorTypedef;

typedef enum {
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,//1
	CAN_PACKET_SET_CURRENT_BRAKE,
	CAN_PACKET_SET_RPM,
	CAN_PACKET_SET_POS,
	CAN_PACKET_FILL_RX_BUFFER,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,
	CAN_PACKET_STATUS,
	CAN_PACKET_SET_CURRENT_REL,
	CAN_PACKET_SET_CURRENT_BRAKE_REL,
	CAN_PACKET_SET_CURRENT_HANDBRAKE,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
	CAN_PACKET_STATUS_2,
	CAN_PACKET_STATUS_3,
	CAN_PACKET_STATUS_4,
	CAN_PACKET_PING,
	CAN_PACKET_PONG,
	CAN_PACKET_DETECT_APPLY_ALL_FOC,
	CAN_PACKET_DETECT_APPLY_ALL_FOC_RES,
	CAN_PACKET_CONF_CURRENT_LIMITS,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS,
	CAN_PACKET_CONF_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_FOC_ERPMS,
	CAN_PACKET_CONF_STORE_FOC_ERPMS,
	CAN_PACKET_STATUS_5
}CAN_PACKET_ID_Enum;


extern VESCMotorTypedef VESCmotor[8];

void VESC_Init(void);
void VESC_ReceiveData_CAN2(CanRxMsg Rx_message1);
void VESC_Set_Pos(u8 controller_ID,float pos,u8 InConGrpFlag);
void VESC_Set_Duty_Cycle(uint8_t controller_ID, float duty_cycle, uint8_t InConGrpFlag);
void VESC_Set_Speed(uint8_t controller_ID, float speed, uint8_t InConGrpFlag);
void VESC_Set_Current(uint8_t controller_ID, float current, uint8_t InConGrpFlag);
void VESC_Set_Brake_Current(u8 controller_ID, float brake_current, u8 InConGrpFlag);
void VESC_Set_Handbrake_Current(u8 controller_ID, float handbrake_current, u8 InConGrpFlag);
void VESC_Feedback_StuckError(uint8_t id,uint8_t InConGrpFlag);
void VESC_Feedback_TimeoutError(uint8_t id, uint8_t InConGrpFlag);


void VESC_IfTimeout(uint8_t id);
void VESC_IfStuck(uint8_t id);
void VESCFunc(void);

#endif

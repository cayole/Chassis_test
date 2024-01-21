#ifndef _ZDRIVE_H
#define _ZDRIVE_H

#include "stm32f4xx.h"
#include "stdbool.h"
#include "stm32f4xx_can.h"
#include "includes.h"

#include "pid.h"
#include "param.h"
#include "mathFunc.h"

typedef enum { Zdrive_Disable,
							 Zdrive_Current,
							 Zdrive_Speed,
							 Zdrive_Postion,
							 Zdrive_EncodeLess,
							 Zdrive_RVCalibration,
							 Zdrive_EncoderLineCalibration,
							 Zdrive_EncoudeOffsetCalibration,
							 Zdrive_VKCalibration,
							 Zdrive_SaveSetting,
							 Zdrive_EraseSetting,
							 Zdrive_Test,
							 Zdrive_ClearErr,
							 Zdrive_Brake}Zdrive_Mode;
typedef enum {Zdrive_Well,
							Zdrive_InsufficientVoltage,
							Zdrive_OverVoltage,
							Zdrive_InstabilityCurrent,
							Zdrive_OverCurrent,
							Zdrive_OverSpeed,
							Zdrive_ExcessiveR,
							Zdrive_ExcessiveInductence,
							Zdrive_LoseEncoder1,
							Zdrive_PolesErr,
							Zdrive_VKCalibrationErr,
							Zdrive_ModeErr,
							Zdrive_ReadErr,
							Zdrive_Hot}Zdrive_Err;

typedef struct{
	float	current;
	float	speed;
	float angle;
}ZdriveValueTypedef;

typedef struct{
    bool timeout;
    bool stuck;
		Zdrive_Err err;
}ZdriveStatusTypedef;

typedef struct{
	 bool Timeout_IfDect;
   bool Stuck_IfDect;
   bool IfReleaseWhenStuck;
   float MAX_Current;  // limit under 20-2 (A)
	 float MAX_Speed; // (r/s)
}ZdriveLimitTypedef;

typedef struct{
   uint32_t lastRxTim;
   uint32_t timeoutTicks;
   uint32_t stuckcnt;
	 uint32_t erraskcnt;
	 uint32_t	clearerrcnt;
}ZdriveArgumTypedef;

typedef struct{
	float GearRadio;
}ZdriveParamTypedef;

typedef struct {
    bool enable;
    bool begin;
    bool Brake;
	  bool errClearFlag;
    Zdrive_Mode mode;
	  Zdrive_Mode modeRead;
    ZdriveParamTypedef Param;
    ZdriveValueTypedef valueSet,valueReal;
    ZdriveStatusTypedef Status;
    ZdriveLimitTypedef limit;
    ZdriveArgumTypedef Argum;
}ZdriveTypedef;

extern ZdriveTypedef Zdrive[8]; // pay attention when id=2(Zdrive[1])

void Zdrive_Init(void);

u32  GetZdriveCanID(u32 operationid , u32 id);
void Zdrive_SetMode(float mode , u8 id);
void Zdrive_SetCurrent(float current,u8 id);
void Zdrive_SetSpeed(float speed,u8 id);
void Zdrive_SetPosition(float position ,u8 id);

void Zdrive_ErrClear(u8 id);
void Zdrive_AskErr(u8 id);

void Zdrive_AskCurrent(u8 id);
void Zdrive_AskSpeed(u8 id);
void Zdrive_AskMode(u8 id);

void Zdrive_IfTimeOut(u8 id);
void Zdrive_IfStuck(u8 id);

void ZdriveFunction(void);
void ZdriveRecHandler(CanRxMsg rx_msg);

#endif

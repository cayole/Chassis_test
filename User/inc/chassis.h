#ifndef _CHASIS_H
#define _CHASIS_H

#include "vesc.h"
#include "queue.h"
#include "param.h"

void Can1Funtion(CanRxMsg rx_message);
void answer_master(CanRxMsg rx_message);

void ENUNableChassis(CanRxMsg rx_message);

void SetChassisSpeed(CanRxMsg rx_message);
void SetChassisAngle(CanRxMsg rx_message);

void AskChassisSpeed(void);
void AskChassisCurrent(void);

void Chassis_Answer_VX(void);
void Chassis_Answer_PX(void);

extern volatile u8 ModeFlag;
extern CanRxMsg Rx_message;
#endif

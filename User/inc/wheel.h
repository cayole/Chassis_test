#pragma once

#include "mathFunc.h"
#include "VESC.h"

typedef struct SpiderGearWheel
{
	VESCMotorTypedef leftMotor;
	VESCMotorTypedef rightMotor;
	float vx;
	float vy;
	
}SpiderGearWheel;


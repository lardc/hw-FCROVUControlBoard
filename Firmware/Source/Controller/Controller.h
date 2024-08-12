﻿#ifndef CONTROLLER_H_
#define CONTROLLER_H_

// Includes
#include "stdinc.h"

// Types
typedef enum __DeviceState
{
	DS_None	= 0,
	DS_Fault,
	DS_Disabled,
	DS_BatteryCharging,
	DS_Ready,
	DS_InProcess
} DeviceState;

typedef enum __DeviceSubState
{
	SDS_None = 0,
	SDS_FirstCharg,
	SDS_Config,
	SDS_ConfigReady,
	SDS_Mensure,
	SDS_WaitSync,
	SDS_RiseEdgeDetected,
	SDS_FallEdge,
	SDS_PostPulseCharg
} DeviceSubState;

// Variables
extern volatile Int64U CONTROL_TimeCounter;
extern volatile DeviceState CONTROL_State;
extern volatile DeviceSubState CONTROL_SubState;
extern volatile Boolean UsedSync;
// Functions
// Initialize controller
void CONTROL_Init();
// Update low-priority states
void CONTROL_Idle();
// Set device state
void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState);
//
void CONTROL_ApplyParameters();

#endif // CONTROLLER_H_

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

// Includes
#include "stdinc.h"

// Types
typedef enum __DeviceState
{
	DS_None					= 0,
	DS_Fault				= 1,
	DS_Disabled				= 2,
	DS_BatteryCharging		= 3,
	DS_Ready				= 4,
	DS_InProcess			= 5
} DeviceState;

typedef enum __DeviceSubState
{
	SDS_None				= 0,
	SDS_FirstCharg			= 1,
	SDS_Config				= 2,
	SDS_ConfigReady			= 3,
	SDS_Mensure				= 4,
	SDS_WaitSync			= 5,
	SDS_RiseEdgeDetected 	= 6,
	SDS_Pause				= 7,
	SDS_PostPulseCharg		= 8,
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

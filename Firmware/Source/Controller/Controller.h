#ifndef CONTROLLER_H_
#define CONTROLLER_H_


// Includes
//
#include "../stdinc.h"


// Constants
//
typedef enum __DeviceState
{
	DS_None			= 0,
	DS_Fault		= 1,
	DS_Disabled		= 2,
	DS_Powered		= 3
} DeviceState;

typedef enum __DeviceSubState
{
	SDS_None				= 0,
	SDS_WaitSync			= 1,
	SDS_RiseEdgeDetected 	= 2
} DeviceSubState;


// Variables
//
extern volatile long CONTROL_TimeCounterTemp;
extern volatile long CONTROL_TimeCounter;
extern volatile DeviceState CONTROL_State;
extern volatile DeviceSubState CONTROL_SubState;
extern volatile long AfterPulseTimeout;


// Functions
//
// Initialize controller
void CONTROL_Init();
// Controller routine
void CONTROL_Cycle();
// Update low-priority states
void CONTROL_Idle();
// Set device state
void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState);
// After pulse process
void CONTROL_AfterPulseProcess();

#endif /* CONTROLLER_H_ */

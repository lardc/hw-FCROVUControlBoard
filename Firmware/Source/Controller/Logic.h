#ifndef __LOGIC_H
#define __LOGIC_H

// Includes
//
#include "stdinc.h"
// Definitions
//
#define CURRENT_RANGE_0				0
#define CURRENT_RANGE_1				1
#define CURRENT_RANGE_2				2
#define CURRENT_RANGE_3				3

// Variables
//
extern volatile Int64U SyncStartTimeout;

//Functions
//
void LOGIC_BatteryCharge(bool State);
void LOGIC_ResetHWToDefaults(bool StopPowerSupply);
void LOGIC_BeginTest();
void LOGIC_ApplyParameters(Int64U CONTROL_TimeCounter);
Int16U LOGIC_SetCurrentRange(Int16U Current);
void LOGIC_SetOutCurrent(Int16U CurrentRange);
void LOGIC_TimePulse(Int16U VRate);
void LOGIC_HandleFan(bool Pulse);
void LOGIC_HandlePanelLamp(bool Pulse);
void LOGIC_HandleBatteryCharge();
void LOGIC_Update();

#endif //__LOGIC_H/

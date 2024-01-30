#ifndef __LOGIC_H
#define __LOGIC_H

// Includes
//
#include "stdinc.h"

// Constants

// Variables
//
volatile Int64U AfterPulseTimeout = 0;

//Functions
//
void LOGIC_BatteryCharge(bool State);
void LOGIC_ResetHWToDefaults(bool StopPowerSupply);
void LOGIC_BeginTest(Int64U TimerTicks);
void LOGIC_ApplyParameters(Int64U TimerTicks);
void LOGIC_SetOutCurrent();
void LOGIC_TimePulse(Int16U VRate);
void LOGIC_AfterPulseProcess();

#endif //__LOGIC_H/

#ifndef __LOGIC_H
#define __LOGIC_H

// Includes
//

#include "stdinc.h"

// Variables
extern volatile Int64U AfterPulseTimeout;

//Functions
//

void LOGIC_ResetHWToDefaults(bool StopPowerSupply);
void LOGIC_BatteryCharge(bool State);
extern void LOGIC_Prepare(Int16U VRate, Int16U IRate, Boolean StartTest , pInt16U UserError);
void LOGIC_SetOutCurrent(Int16U IRate);
void LOGIC_SetGateV(Int16U VRate);
Boolean LOGIC_SyncStart(Boolean StartTest);
void LOGIC_Mensure();
void LOGIC_AfterPulseProcess();
#endif //__LOGIC_H/

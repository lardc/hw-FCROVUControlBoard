#ifndef __LOGIC_H
#define __LOGIC_H

// Includes
//

#include "stdinc.h"

//Functions
//

void LOGIC_ResetHWToDefaults(bool StopPowerSupply);
void LOGIC_BatteryCharge(bool State);
extern void LOGIC_Prepare(float VRate, float IRate, Boolean StartTest);
void LOGIC_SetOutCurrent(float IRate);
#endif //__LOGIC_H/

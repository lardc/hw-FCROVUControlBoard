#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "ZwGPIO.h"

// Functions
//
void LL_ToggleLed();
void LL_PanelLamp(bool State);
void LL_PulseEnable(bool State);
void LL_PulseStart(bool State);
void LL_Fan(bool State);
void LL_PSBoard(bool State);
void LL_SWBoard(bool State);
void LL_SetGateVoltage(Int16U Voltage);

#endif //__LOWLEVEL_H

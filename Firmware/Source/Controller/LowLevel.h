#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "ZwGPIO.h"


// Functions
//
bool LL_IsBlinkLED();
void LL_BlinkLED(bool State);
void LL_ExternalLED(bool State);
void LL_ExternalFan(bool State);
void LL_PsBoard(bool State);
void LL_SwitchBoard(bool State);
void LL_StartPulse(bool State);
void LL_StartPulseEn(bool State);
void LL_CurrentRange(Int16U Current);

#endif //__LOWLEVEL_H

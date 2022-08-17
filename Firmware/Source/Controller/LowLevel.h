#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "ZwGPIO.h"

// Functions
//
void LL_ToggleLed();
void LL_ExternalLED(bool State);
void LL_CurrentLimitEnable(bool State);
void LL_VRateLow();
void LL_VRateMid();
void LL_VRateHigh();
void LL_VRateNone();

#endif //__LOWLEVEL_H

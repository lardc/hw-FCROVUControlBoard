// Header
#include "LowLevel.h"
#include "Board.h"

// Forward functions
//
void LL_VRateSelect(bool LowEnable, bool MidEnable, bool HighEnable);

// Functions
//
void LL_ToggleLed()
{
	GPIO_Toggle(GPIO_LED2);
}
//-----------------------------

void LL_ExternalLED(bool State)
{
	GPIO_SetState(GPIO_LED1, State);
}
//-----------------------------

void LL_CurrentLimitEnable(bool State)
{
	GPIO_SetState(GPIO_I_LIM, State);
}
//-----------------------------


void LL_VRateSelect(bool LowEnable, bool MidEnable, bool HighEnable)
{
	GPIO_SetState(GPIO_LOW_VRATE, !LowEnable);
	GPIO_SetState(GPIO_MID_VRATE, !MidEnable);
	GPIO_SetState(GPIO_HIGH_VRATE, !HighEnable);
}
//-----------------------------

void LL_VRateLow()
{
	LL_VRateSelect(true, false, false);
}
//-----------------------------

void LL_VRateMid()
{
	LL_VRateSelect(false, true, false);
}
//-----------------------------

void LL_VRateHigh()
{
	LL_VRateSelect(false, false, true);
}
//-----------------------------

void LL_VRateNone()
{
	LL_VRateSelect(false, false, false);
}
//-----------------------------

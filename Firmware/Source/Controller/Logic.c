// Header
//

#include "Logic.h"

// Includes
//

#include "Controller.h"
#include "Board.h"
#include "LowLevel.h"
#include "Delay.h"

// Functions
//

void LOGIC_ResetHWToDefaults(bool StopPowerSupply)
{
	LL_SetGateVoltage(0);
	GPIO_SetState(GPIO_FAN, false);
	GPIO_SetState(GPIO_OUT_B0, false);
	GPIO_SetState(GPIO_OUT_B1, false);
	if(StopPowerSupply)
		LOGIC_BatteryCharge(false);
}

//-----------------------------

// Управление зарядом батареи
void LOGIC_BatteryCharge(bool State)
{
	if(State)
	{
		LL_PSBoard(true);
		DELAY_MS(100);
		LL_SWBoard(true);
	}
	else
	{
		LL_PSBoard(false);
		LL_SWBoard(false);
	}
}
//-----------------------------

void LOGIC_Prepare(float VRate, Boolean StartTest)
{
	LOGIC_BatteryCharge(FALSE);
	CONTROL_SetDeviceState(DS_InProcess, SDS_Config);
}
//-----------------------------

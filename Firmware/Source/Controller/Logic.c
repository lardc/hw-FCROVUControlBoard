
// Header
//
#include "Logic.h"

// Includes
//
#include "Global.h"
#include "LowLevel.h"
#include "Board.h"
#include "Delay.h"

// Definitions
//

// Structs
//

// Varibales
//

// Forward functions
//

// Functions
//
// Сброс аппаратных линий в состояния по умолчанию
void LOGIC_ResetHWToDefaults(bool StopPowerSupply)
{
	LL_SetGateVoltage(0);
	GPIO_SetState(GPIO_FAN, false);
	GPIO_SetState(GPIO_OUT_B0, false);
	GPIO_SetState(GPIO_OUT_B1, false);
	if (StopPowerSupply)
			LOGIC_BatteryCharge(false);
}
//-----------------------------

// Включение заряда батареи
void LOGIC_BatteryCharge(bool State)
{
if (State)
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


//-----------------------------


// Header
//
#include "Logic.h"

// Includes
//
#include "LowLevel.h"
#include "Delay.h"


// Управление зарядом батареи
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

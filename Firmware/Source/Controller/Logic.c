// Header
//

#include "Logic.h"

// Includes
//

#include "DeviceObjectDictionary.h"
#include "Controller.h"
#include "Board.h"
#include "Global.h"
#include "LowLevel.h"
#include "Delay.h"

// Definitions
//

#define CURRENT_RANGE_0				0
#define CURRENT_RANGE_1				1
#define CURRENT_RANGE_2				2
#define CURRENT_RANGE_3				3

// Varibales
//

Int64U Timeout;

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

void LOGIC_Prepare(float VRate, float IRate, Boolean StartTest , pInt16U *pUserError)
{
	if(CONTROL_State == DS_Ready)
	{
		LOGIC_BatteryCharge(FALSE);
		LOGIC_SetOutCurrent(IRate);
		LOGIC_SetGateV(VRate);
		if(LOGIC_SyncStart(StartTest))
			CONTROL_SetDeviceState(DS_InProcess, SDS_Mensure);
		else
			CONTROL_SetDeviceState(DS_InProcess, SDS_WaitSync);
	}
	else if(CONTROL_State != DS_Ready)
		*pUserError = ERR_OPERATION_BLOCKED;

}

//-----------------------------

// Выставление тока
void LOGIC_SetOutCurrent(float IRate)
{
	switch((Int16U)IRate)
	{
		case CURRENT_RANGE_0:
			{
				GPIO_SetState(GPIO_FAN, true);
				GPIO_SetState(GPIO_OUT_B0, false);
				GPIO_SetState(GPIO_OUT_B1, false);
			}
			break;

		case CURRENT_RANGE_1:
			{
				GPIO_SetState(GPIO_FAN, true);
				GPIO_SetState(GPIO_OUT_B0, true);
				GPIO_SetState(GPIO_OUT_B1, false);
			}
			break;

		case CURRENT_RANGE_2:
			{
				GPIO_SetState(GPIO_FAN, true);
				GPIO_SetState(GPIO_OUT_B0, false);
				GPIO_SetState(GPIO_OUT_B1, true);
			}
			break;

		case CURRENT_RANGE_3:
			{
				GPIO_SetState(GPIO_FAN, true);
				GPIO_SetState(GPIO_OUT_B0, true);
				GPIO_SetState(GPIO_OUT_B1, true);
			}
			break;

		default:
			{
				GPIO_SetState(GPIO_FAN, false);
				GPIO_SetState(GPIO_OUT_B0, false);
				GPIO_SetState(GPIO_OUT_B1, false);
			}
			break;
	}
}
//-----------------------------

void LOGIC_SetGateV(float VRate)
{
	Int16U GateV = 0;
	if(SP_GetSetpoint(VRate, &GateV))
		LL_SetGateVoltage(GateV);
}

//-----------------------------

Boolean LOGIC_SyncStart(Boolean StartTest)
{
	Timeout = CONTROL_TimeCounter + TEST_PREPARE_TIMEOUT_MS;
	if(StartTest)
		return 0;
	else
		return 1;

}

//-----------------------------

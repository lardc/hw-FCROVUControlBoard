
// Header
//
#include "Logic.h"

// Includes
//
#include "DeviceObjectDictionary.h"
#include "DataTable.h"
#include "Global.h"
#include "LowLevel.h"
#include "Board.h"
#include "Delay.h"
#include "Controller.h"

// Varibales
//
static Int64U Timeout;
static Int64U TimePulse;

// Functions
//
// —брос аппаратных линий в состо€ни€ по умолчанию
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

// ”правление зар€да батареи
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

// ----------------------------

// ”правление реле выбора тока
void LOGIC_SetOutCurrent()
{
	switch ((Int16U)DataTable[REG_CURRENT_SETPOINT])
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
void LOGIC_TimePulse(Int16U VRate)
{
	TimePulse = DataTable[REG_BAT_VOLTAGE] / VRate + DOP_TIME_US;
	return;
}

//-----------------------------

void LOGIC_BeginTest(Int64U CONTROL_TimeCounter)
{
	Timeout = CONTROL_TimeCounter + TEST_PREPARE_TIMEOUT_MS;
	CONTROL_SetDeviceState(DS_InProcess, SDS_Mensure);
}

// ----------------------------

void LOGIC_ApplyParameters(Int64U CONTROL_TimeCounter)
{
	Timeout = CONTROL_TimeCounter + TEST_PREPARE_TIMEOUT_MS;
	CONTROL_SetDeviceState(DS_InProcess, SDS_WaitSync);
}

// ----------------------------

void LOGIC_TestSequence()
{
	LL_PulseStart(true);
	DELAY_US(TimePulse);
	LL_PulseStart(false);
	CONTROL_SetDeviceState(DS_InProcess, SDS_Pause);
}

//-----------------------------

void LOGIC_Update()
{
	if(CONTROL_SubState == SDS_Config)
	{
		CONTROL_ApplyParameters();
	}
	if(CONTROL_SubState == SDS_ConfigReady)
	{
		if(UsedSync)
			LOGIC_BeginTest(CONTROL_TimeCounter);
		else
			LOGIC_ApplyParameters(CONTROL_TimeCounter);
	}
	if(CONTROL_SubState == SDS_Mensure)
	{
		LL_PanelLamp(TRUE);
		LL_Led2(TRUE);
		LOGIC_TestSequence();
	}

	if(CONTROL_SubState == SDS_RiseEdgeDetected)
	{
		CONTROL_SetDeviceState(DS_InProcess, SDS_Pause);
	}
	if(CONTROL_SubState == SDS_Pause)
	{
		CONTROL_SetDeviceState(DS_InProcess, SDS_PostPulseCharg);
	}
	if(CONTROL_SubState == SDS_PostPulseCharg)
	{
		LOGIC_BatteryCharge(true);
		CONTROL_SetDeviceState(DS_BatteryCharging, SDS_PostPulseCharg);
	}
}

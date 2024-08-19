
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
volatile Int64U SwitchTime = 0, SyncStartTimeout = 0, AfterPulseTimeout = 0, FallEdgeTime = 0;
static Int64U TimePulse;
Int16U CurrentRange = 0;
// Functions
//
// —брос аппаратных линий в состо€ни€ по умолчанию
void LOGIC_ResetHWToDefaults(bool StopPowerSupply)
{
	LL_PulseEnable(false);
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

Int16U LOGIC_SetCurrentRange(Int16U Current)
{
	CurrentRange = (Current / CH_CURRENT) + 1;
	return (CurrentRange > CH_MAX) ? CH_MAX : CurrentRange;
}

// ”правление реле выбора тока
void LOGIC_SetOutCurrent(Int16U CurrentRange)
{
	switch (CurrentRange)
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
	CONTROL_SetDeviceState(DS_InProcess, SDS_Meansure);
}

// ----------------------------

void LOGIC_ApplyParameters(Int64U CONTROL_TimeCounter)
{
	CONTROL_SetDeviceState(DS_InProcess, SDS_WaitSync);
	LL_PulseEnable(true);
	SyncStartTimeout = CONTROL_TimeCounter + SYNC_TIMEOUT_US;
	return;
}

// ----------------------------

void LOGIC_TestSequence()
{
	LL_PulseStart(true);
	DELAY_US(TimePulse);
	LL_PulseStart(false);
	CONTROL_SetDeviceState(DS_InProcess, SDS_FallEdge);
}

//-----------------------------

void LOGIC_Update()
{
	if(CONTROL_SubState == SDS_Config)
	{
		CONTROL_ApplyParameters();
		LL_PulseEnable(true);
		SwitchTime = CONTROL_TimeCounter + SWITCH_TIME_US;
	}
	if(CONTROL_SubState == SDS_ConfigReady && (CONTROL_TimeCounter >= SwitchTime))
	{
		if(UsedSync)
			LOGIC_BeginTest(CONTROL_TimeCounter);
		else
			LOGIC_ApplyParameters(CONTROL_TimeCounter);
	}
	if(CONTROL_SubState == SDS_Meansure)
	{
		LL_PanelLamp(true);
		LL_Led2(true);
		LOGIC_TestSequence();
		FallEdgeTime = CONTROL_TimeCounter + FALL_TIME_US;
	}
	if(CONTROL_SubState == SDS_RiseEdgeDetected && !GPIO_GetState(GPIO_SYNC_IN))
	{
		AfterPulseTimeout = CONTROL_TimeCounter + AFTER_PULSE_TIMEOUT;
		CONTROL_SetDeviceState(DS_InProcess, SDS_FallEdge);
	}
	if(CONTROL_SubState == SDS_FallEdge && (CONTROL_TimeCounter >= FallEdgeTime))
	{
		LL_PulseEnable(false);
		LL_PanelLamp(false);
		LL_Led2(false);
		LOGIC_ResetHWToDefaults(false);
		CONTROL_SetDeviceState(DS_InProcess, SDS_PostPulseCharg);
	}
	if(CONTROL_SubState == SDS_PostPulseCharg && (CONTROL_TimeCounter >= AfterPulseTimeout))
	{
		CONTROL_BatteryPostPulseChargeTimeCounter = CONTROL_TimeCounter + DataTable[REG_CHRAGE_TIMEOUT_PULSE];
		LOGIC_BatteryCharge(true);
		CONTROL_SetDeviceState(DS_BatteryCharging, SDS_PostPulseCharg);
	}
}

// Header
//

#include "Logic.h"

// Includes
//

#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Controller.h"
#include "Board.h"
#include "Global.h"
#include "LowLevel.h"
#include "Delay.h"
#include "Setpoint.h"

// Definitions
//

#define CURRENT_RANGE_0				0
#define CURRENT_RANGE_1				1
#define CURRENT_RANGE_2				2
#define CURRENT_RANGE_3				3

// Varibales
//
volatile Int64U AfterPulseTimeout = 0;
volatile Int64U Timeout = 0;

// Functions
//

void LOGIC_RealTime()

{
	if(CONTROL_State == DS_InProcess)
	{
		if(CONTROL_SubState == SDS_Mensure)
		{
			LOGIC_Mensure();
		}
		if(CONTROL_SubState == SDS_Pause)
		{
			LOGIC_AfterPulseProcess();
		}
		if(CONTROL_SubState == SDS_PostPulseCharg)
		{
			CONTROL_SetDeviceState(DS_BatteryCharging, SDS_PostPulseCharg);
		}
	}
}

//-----------------------------

void LOGIC_ResetHWToDefaults(bool StopPowerSupply)
{
	LL_SetGateVoltage(0);
	LOGIC_HandleExtLed(false);
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

void LOGIC_Prepare(Int16U VRate, Int16U IRate, Boolean StartTest, pInt16U UserError)
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
		*UserError = ERR_OPERATION_BLOCKED;
}

//-----------------------------

// Выставление тока
void LOGIC_SetOutCurrent(Int16U IRate)
{
	switch(IRate)
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

void LOGIC_SetGateV(Int16U VRate)
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

void LOGIC_Mensure()
{
	LOGIC_HandleExtLed(true);
	Int32U Delay = 10ul * DataTable[REG_ACTUAL_VOLTAGE] / DataTable[REG_VRATE_SETPOINT] + PRE_PROBE_TIME_US;
	LL_PulseStart(true);
	DELAY_US(Delay);
	LL_PulseStart(false);
	AfterPulseTimeout = CONTROL_TimeCounter + AFTER_PULSE_TIMEOUT;
	CONTROL_SetDeviceState(DS_InProcess, SDS_Pause);
}

//-----------------------------

void LOGIC_AfterPulseProcess()
{
	if(AfterPulseTimeout && (CONTROL_TimeCounter > AfterPulseTimeout))
	{
		LOGIC_ResetHWToDefaults(false);
		AfterPulseTimeout = 0;
		LOGIC_BatteryCharge(true);
		CONTROL_SetDeviceState(DS_InProcess, SDS_PostPulseCharg);
	}
}

//-----------------------------

void LOGIC_HandleExtLed(Boolean IsImpulse)
{
	static Int64U ExtLedTimeout = 0;
	if(IsImpulse)
	{
		LL_PanelLamp(true);
		ExtLedTimeout = CONTROL_TimeCounter + EXT_LED_SWITCH_ON_TIME;
	}
	else
	{
		if(CONTROL_TimeCounter >= ExtLedTimeout)
			LL_PanelLamp(false);
	}
}

//-----------------------------

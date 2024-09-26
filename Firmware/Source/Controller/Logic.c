
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
#include "Measurement.h"

// Definitions
//
#define EXT_LAMP_ON_STATE_TIME		500					// Время работы внешнего индикатора, мс

// Varibales
//
volatile Int64U SwitchTime = 0, SyncStartTimeout = 0, AfterPulseTimeout = 0, FallEdgeTime = 0;
static Int64U PanelLampTimeout = 0;
static Int64U TimePulse;
Int16U CurrentRange = 0;

// Functions
//
// Сброс аппаратных линий в состояния по умолчанию
void LOGIC_ResetHWToDefaults(bool StopPowerSupply)
{
	LL_PulseEnable(false);
	LL_SetGateVoltage(0);
	GPIO_SetState(GPIO_OUT_B0, false);
	GPIO_SetState(GPIO_OUT_B1, false);
	LOGIC_HandlePanelLamp(false);
	SyncStartTimeout = 0;

	if (StopPowerSupply)
			LOGIC_BatteryCharge(false);
}
//-----------------------------

// Управление заряда батареи
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
	CurrentRange = (Current / CH_CURRENT);
	return (CurrentRange > CH_MAX) ? CH_MAX : CurrentRange;
}

// Управление реле выбора тока
void LOGIC_SetOutCurrent(Int16U CurrentRange)
{
	switch (CurrentRange)
	{
		case CURRENT_RANGE_0:
		{
			GPIO_SetState(GPIO_OUT_B0, false);
			GPIO_SetState(GPIO_OUT_B1, false);
			break;
		}
		break;

		case CURRENT_RANGE_1:
		{
			GPIO_SetState(GPIO_OUT_B0, true);
			GPIO_SetState(GPIO_OUT_B1, false);
			break;
		}
		break;

		case CURRENT_RANGE_2:
		{
			GPIO_SetState(GPIO_OUT_B0, false);
			GPIO_SetState(GPIO_OUT_B1, true);
			break;
		}
		break;

		case CURRENT_RANGE_3:
		{
			GPIO_SetState(GPIO_OUT_B0, true);
			GPIO_SetState(GPIO_OUT_B1, true);
			break;
		}
		break;

		default:
		{
			GPIO_SetState(GPIO_OUT_B0, false);
			GPIO_SetState(GPIO_OUT_B1, false);
			break;
		}
		break;
		}
}
void LOGIC_TimePulse(Int16U VRate)
{
	TimePulse = DataTable[REG_BAT_VOLTAGE] / VRate + DOP_TIME_US;
	if (TimePulse > WIDTH_SYNC_LINE_MAX)
		TimePulse = WIDTH_SYNC_LINE_MAX;
	return;
}

//-----------------------------

void LOGIC_BeginTest()
{
	CONTROL_SetDeviceState(DS_InProcess, SDS_Meansure);
}

// ----------------------------

void LOGIC_ApplyParameters(Int64U CONTROL_TimeCounter)
{
	LL_PulseEnable(true);
	SyncStartTimeout = CONTROL_TimeCounter + SYNC_TIMEOUT_MS;
	CONTROL_SetDeviceState(DS_ConfigReady, SDS_WaitSync);
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

void LOGIC_HandleFan(bool Pulse)
{
	static uint64_t FanOnTimeout = 0;

	if(CONTROL_State != DS_None)
	{
		if(DataTable[REG_FAN_CTRL])
		{
			// Включение вентилятора
			if (Pulse)
			{
				FanOnTimeout = CONTROL_TimeCounter + ((uint32_t)DataTable[REG_FAN_TIME] * 1000);
				LL_Fan(true);
			}

			// Отключение вентилятора
			else
			{
				if (FanOnTimeout && (CONTROL_TimeCounter > FanOnTimeout))
				{
					FanOnTimeout = 0;
					LL_Fan(false);
				}
			}
		}
		else
			LL_Fan(false);
	}
	else
		LL_Fan(false);
}

//-----------------------------

void LOGIC_HandlePanelLamp(bool Pulse)
{
	if(CONTROL_State != DS_None)
	{
		if(Pulse)
		{
			LL_PanelLamp(true);
			PanelLampTimeout = CONTROL_TimeCounter + EXT_LAMP_ON_STATE_TIME;
		}
		else
		{
			if(CONTROL_TimeCounter >= PanelLampTimeout)
			{
				LL_PanelLamp(false);
				PanelLampTimeout = 0;
 			}
		}
	}
}

//-----------------------------

void LOGIC_HandleBatteryCharge()
{
	if(CONTROL_State != DS_InProcess || CONTROL_State != DS_ConfigReady)
	{
		// Мониторинг уровня заряда батареи
		float BatteryVoltage = MEASURE_GetBatteryVoltage();
		DataTable[REG_BAT_VOLTAGE] = (uint16_t)(BatteryVoltage * 10);

		if (CONTROL_State == DS_BatteryCharging)
		{
			if (DataTable[REG_BAT_VOLTAGE] >= DataTable[REG_BAT_VOLTAGE_THRESHOLD])
				CONTROL_SetDeviceState(DS_Ready, SDS_None);

			else
			{
				if ((CONTROL_TimeCounter > CONTROL_BatteryFirstChargeTimeCounter && CONTROL_SubState == SDS_FirstCharg) ||
						(CONTROL_TimeCounter > CONTROL_BatteryPostPulseChargeTimeCounter && CONTROL_SubState == SDS_PostPulseCharg))
					CONTROL_SwitchToFault(DF_BATTERY);
			}
		}
	}
}

//-----------------------------

void LOGIC_Update()
{
	if(CONTROL_SubState == SDS_Config)
	{
		CONTROL_ApplyParameters();
		LL_PulseEnable(true);
		SwitchTime = CONTROL_TimeCounter + SWITCH_TIME_MS;
	}
	if(CONTROL_SubState == SDS_ConfigReady && (CONTROL_TimeCounter >= SwitchTime))
	{
		if(UsedSync)
			LOGIC_BeginTest();
		else
			LOGIC_ApplyParameters(CONTROL_TimeCounter);
	}
	if(CONTROL_SubState == SDS_Meansure)
	{
		LOGIC_HandleFan(true);
		LOGIC_HandlePanelLamp(true);
		FallEdgeTime = CONTROL_TimeCounter + FALL_TIME_MS;
		LOGIC_TestSequence();
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

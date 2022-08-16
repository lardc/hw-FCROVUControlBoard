//-----------------------------------------------
// Основная логика
//-----------------------------------------------

// Header
#include "Controller.h"
//
// Includes
#include "SysConfig.h"
#include "DataTable.h"
#include "SCCISlave.h"
#include "DeviceProfile.h"
#include "BCCITypes.h"
#include "Board.h"
#include "Interrupts.h"
#include "Global.h"
#include "LowLevel.h"
#include "InitConfig.h"
#include "Measurement.h"


// Types
//
typedef void (*FUNC_AsyncDelegate)();
//
// Вспомогательные состояния
typedef enum __SubState
{
	SS_None				= 0,	// Неопределённое
	SS_WaitVoltage		= 1,	// Ожидание выхода на заданное напряжение
	SS_VoltageReady		= 2,	// Готовность к измерению
} SubState;


// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile SubState SUB_State = SS_None;
static Boolean CycleActive = FALSE;
//
volatile Int64U CONTROL_TimeCounter = 0;
static Int64U CONTROL_TimeCounterDelay = 0;
static Int64U CONTROL_TimeFanLastTurnOn = 0;


// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SetDeviceState(DeviceState NewState);
void CONTROL_Idle();
void CONTROL_BatteryVoltageMonitor();
void Delay_mS(uint32_t Delay);
void CONTROL_SwitchToFault(Int16U Reason);
void CONTROL_ResetToDefaults();
void CONTROL_WatchDogUpdate();


// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT];
	Int16U EPSized[EP_COUNT];
	pInt16U EPCounters[EP_COUNT];
	pInt16U EPDatas[EP_COUNT];

	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = { (FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT };
	// Инициализация data table
	DT_Init(EPROMService, FALSE);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	// Сброс значений
	DEVPROFILE_ResetControlSection();
	//
	CONTROL_SetDeviceState(DS_None);
	CONTROL_ResetToDefaults();
}
//------------------------------------------------------------------------------

void CONTROL_ResetToDefaults()
{
	DataTable[REG_FAULT_REASON] = 0;
	DataTable[REG_DISABLE_REASON] = 0;
	DataTable[REG_WARNING] = 0;
	DataTable[REG_PROBLEM] = 0;
	DataTable[REG_TEST_FINISHED] = OPRESULT_NONE;
	//
	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//------------------------------------------------------------------------------

void CONTROL_Idle()
{
	CONTROL_WatchDogUpdate();
	DEVPROFILE_ProcessRequests();
	//
	CONTROL_BatteryVoltageMonitor();
}
//-----------------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;

	switch (ActionID)
	{
		case ACT_ENABLE_POWER:
			{
				if ((CONTROL_State == DS_None) || (CONTROL_State == DS_Charging) || (CONTROL_State == DS_Ready) || (CONTROL_State == DS_ConfigReady))
				{
					if (CONTROL_State == DS_None)
					{
						CONTROL_TimeFanLastTurnOn = CONTROL_TimeCounter;
						LL_SwitchBoard(TRUE);
						LL_PsBoard(TRUE);
						//
						CONTROL_TimeCounterDelay = CONTROL_TimeCounter + T_CHARGE_DELAY;
						CONTROL_SetDeviceState(DS_Charging);
					}
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_DISABLE_POWER:
			{
				if ((CONTROL_State == DS_None) || (CONTROL_State == DS_Charging) || (CONTROL_State == DS_Ready) || (CONTROL_State == DS_ConfigReady))
				{
					LL_ExternalFan(FALSE);
					LL_PsBoard(FALSE);
					LL_SwitchBoard(FALSE);
					//
					CONTROL_SetDeviceState(DS_None);
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_START_PULSE:
			{
				if (CONTROL_State == DS_ConfigReady)
				{
					CONTROL_TimeFanLastTurnOn = CONTROL_TimeCounter;
					CONTROL_ResetToDefaults();
					//
					CONTROL_TimeCounterDelay = CONTROL_TimeCounter + T_CHARGE_DELAY_SHORT;
					SUB_State = SS_WaitVoltage;
					CONTROL_SetDeviceState(DS_InProcess);
					//
					LL_ExternalLED(TRUE);
					LL_StartPulse(TRUE);
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_CONFIG:
		{
			if (CONTROL_State == DS_Ready)
			{
				LL_PsBoard(FALSE);
				LL_SwitchBoard(FALSE);
				LL_StartPulseEn(TRUE);

				Int16U VRateToDAC = DataTable[REG_V_RATE_VALUE] * (float)DataTable[REG_VRATE_TO_DAC_GAIN]/100;
				DAC_SetValueCh1(DAC1, VRateToDAC);
				LL_CurrentRange(DataTable[REG_I_SHORT_CIRCUIT]);

				CONTROL_SetDeviceState(DS_ConfigReady);
			}
			else
				*pUserError = ERR_OPERATION_BLOCKED;
		}

		case ACT_FAULT_CLEAR:
			{
				if (CONTROL_State == DS_Fault)
				{
					CONTROL_ResetToDefaults();
					CONTROL_SetDeviceState(DS_None);
				}
			}
			break;

		case ACT_WARNING_CLEAR:
			DataTable[REG_WARNING] = 0;
			break;

		// Debug actions
		//
		case ACT_DBG_PB_CTRL:
			{
				LL_PsBoard(TRUE);
				Delay_mS(1000);
				LL_PsBoard(FALSE);
			}
			break;

		case ACT_DBG_LAMP:
			{
				LL_ExternalLED(TRUE);
				Delay_mS(1000);
				LL_ExternalLED(FALSE);
			}
			break;

		case ACT_DBG_FAN:
			{
				LL_ExternalFan(TRUE);
				Delay_mS(1000);
				LL_ExternalFan(FALSE);
			}
			break;

		case ACT_DBG_START_PULSE:
			{
				LL_StartPulse(TRUE);
				Delay_mS(1000);
				LL_StartPulse(FALSE);
			}
			break;

		case ACT_DBG_PULSE_SRC_EN:
			{
				LL_StartPulseEn(TRUE);
				Delay_mS(1000);
				LL_StartPulseEn(FALSE);
			}
			break;

		case ACT_DBG_DAC_OUT:
			{
				DAC_SetValueCh1(DAC1, DataTable[REG_DBG_DAC]);
			}
			break;

		case ACT_DBG_VSO:
			DataTable[REG_DBG_VSO_VALUE] = MEASURE_BatteryVoltage();
			break;

		default:
			return FALSE;
	}

	return TRUE;
}
//-----------------------------------------------

void CONTROL_BatteryVoltageMonitor()
{
	float BatteryVoltage;
	bool FanEnable = FALSE;

	BatteryVoltage = MEASURE_BatteryVoltage();
	DataTable[REG_DBG_VSO_VALUE] = BatteryVoltage;

	// Логика работы вентиляторов
	if ((CONTROL_State == DS_Charging) || (CONTROL_State == DS_InProcess))
	{
		FanEnable = TRUE;
	}
	else if ((CONTROL_State == DS_Ready) || (CONTROL_State == DS_ConfigReady))
	{
		if (CONTROL_TimeCounter < (CONTROL_TimeFanLastTurnOn + FAN_ACTIVE_TIME))
			FanEnable = TRUE;
		else
			{
				FanEnable = FALSE;

				if (CONTROL_TimeCounter > (CONTROL_TimeFanLastTurnOn + FAN_SLEEP_TIME))
					CONTROL_TimeFanLastTurnOn = CONTROL_TimeCounter;
			}
	}
	LL_ExternalFan(FanEnable);

	// Оцифровка напряжения
	if ((CONTROL_State == DS_Ready) || (CONTROL_State == DS_Charging) || (CONTROL_State == DS_InProcess) || (CONTROL_State == DS_ConfigReady))
	{
		// Таймаут ожидания требуемого напряжения при заряде или старте измерений
		if ((CONTROL_State == DS_Charging) || ((CONTROL_State == DS_InProcess) && (SUB_State == SS_WaitVoltage)))
		{
			if (CONTROL_TimeCounter > CONTROL_TimeCounterDelay)
			{
				LL_ExternalFan(FALSE);
				LL_SwitchBoard(FALSE);
				LL_PsBoard(FALSE);
				CONTROL_SwitchToFault(DF_BATTERY);
				return;
			}

			if ((BatteryVoltage > V_BAT_THRESHOLD_MIN) && (BatteryVoltage < V_BAT_THRESHOLD_MAX))
			{
				// Смена состояния при завершении заряда
				if (CONTROL_State == DS_Charging)
					CONTROL_SetDeviceState(DS_Ready);
				// Переход на следующий шаг измерения
				else
					SUB_State = SS_VoltageReady;
			}
		}
	}
}
//-----------------------------------------------

void CONTROL_InterruptProcess()
{
	// Запуск таймера, если был программный старт
	if (CONTROL_State == DS_InProcess)
	{
		TIM_Reset(TIM4);
		TIM_Start(TIM4);
	}

	CONTROL_TimeFanLastTurnOn = CONTROL_TimeCounter;
	CONTROL_ResetToDefaults();
	//
	CONTROL_TimeCounterDelay = CONTROL_TimeCounter + T_CHARGE_DELAY_SHORT;
	SUB_State = SS_WaitVoltage;
	CONTROL_SetDeviceState(DS_InProcess);
	//
	LL_ExternalLED(TRUE);
}
//-----------------------------------------------

void SoftwareStopPulse()
{
	LL_StartPulse(FALSE);
	LL_StartPulseEn(FALSE);
}
//-----------------------------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
	LL_ExternalFan(FALSE);
	LL_ExternalLED(FALSE);
	LL_SwitchBoard(FALSE);
	LL_PsBoard(FALSE);

	SUB_State = SS_None;
	CONTROL_SetDeviceState(DS_Fault);
	DataTable[REG_FAULT_REASON] = Reason;
}
//-----------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;
}
//-----------------------------------------------

void Delay_mS(uint32_t Delay)
{
	uint64_t Counter = (uint64_t)CONTROL_TimeCounter + Delay;
	while (Counter > CONTROL_TimeCounter)
		CONTROL_WatchDogUpdate();
}
//-----------------------------------------------

void CONTROL_WatchDogUpdate()
{
	if (BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//-----------------------------------------------

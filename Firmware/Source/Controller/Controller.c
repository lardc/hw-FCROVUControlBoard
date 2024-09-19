// Header
#include "Controller.h"

// Includes
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Constraints.h"
#include "DeviceProfile.h"
#include "LowLevel.h"
#include "Board.h"
#include "BCCIxParams.h"
#include "SysConfig.h"
#include "Setpoint.h"
#include "Measurement.h"
#include "Logic.h"


// Variables
volatile Int64U CONTROL_TimeCounter = 0;
volatile Int64U CONTROL_BatteryFirstChargeTimeCounter, CONTROL_BatteryPostPulseChargeTimeCounter = 0;
Int16U VoltageRate, Current = 0;
volatile DeviceState CONTROL_State = DS_None;
volatile DeviceSubState CONTROL_SubState = SDS_None;
static Boolean CycleActive = false;
volatile Boolean UsedSync;

// Forward functions
void CONTROL_FillDefault();
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_PrepareStart(Boolean StartTest);
void CONTROL_HandleBatteryCharge();
void CONTROL_ResetToDefaults(bool StopPowerSupply);


// Functions
void CONTROL_Init()
{
	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};

	// Инициализация data table
	DT_Init(EPROMService, false);
	DT_SaveFirmwareInfo(CAN_SLAVE_NID, 0);
	
	// Сброс значений
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	CONTROL_ResetToDefaults(true);

}
//-----------------------------

void CONTROL_Idle()
{
	// Process battery charge
	CONTROL_HandleBatteryCharge();

	DEVPROFILE_ProcessRequests();

	// Ожидание запроса перехода в бутлоадер
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}

//-----------------------------

void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState)
{
	CONTROL_SubState = NewSubState;
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;
	DataTable[REG_SUB_STATE] = NewSubState;
}
//-----------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
	CONTROL_SetDeviceState(DS_Fault, SDS_None);
	DataTable[REG_FAULT_REASON] = Reason;
	LOGIC_ResetHWToDefaults(true);
}
//-----------------------------------------------

void CONTROL_FillDefault()
{
	DataTable[REG_DEV_STATE] = DS_None;
	DataTable[REG_FAULT_REASON] = DF_NONE;
	DataTable[REG_DISABLE_REASON] = DISABLE_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
}
//-----------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;

	switch(ActionID)
	{
		case ACT_ENABLE_POWER:
			if(CONTROL_State == DS_None)
			{
				CONTROL_BatteryFirstChargeTimeCounter = CONTROL_TimeCounter + DataTable[REG_CHRAGE_TIMEOUT_FULL];
				CONTROL_SetDeviceState(DS_BatteryCharging, SDS_FirstCharg);
				LOGIC_BatteryCharge(true);
				LL_SetGateVoltage(0);
			}
			else if(CONTROL_State != DS_Ready)
				*pUserError = ERR_OPERATION_BLOCKED;
			break;
			
		case ACT_DISABLE_POWER:
			if((CONTROL_State == DS_Ready) || (CONTROL_State == DS_BatteryCharging))
			{
				CONTROL_ResetToDefaults(true);
				CONTROL_SetDeviceState(DS_None, SDS_None);
			}
			else if(CONTROL_State != DS_None)
				*pUserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_CLR_FAULT:
			if (CONTROL_State == DS_Fault)
				CONTROL_ResetToDefaults(true);
			break;

		case ACT_CLR_WARNING:
			DataTable[REG_WARNING] = WARNING_NONE;
			break;

		case ACT_APPLY_PARAMS:
			if(CONTROL_State == DS_Ready)
				CONTROL_PrepareStart(FALSE);
			else
				if (CONTROL_State == DS_InProcess)
					*pUserError = ERR_OPERATION_BLOCKED;
				else
					*pUserError = ERR_DEVICE_NOT_READY;
			break;
			
		case ACT_START_TEST:
			if(CONTROL_State == DS_Ready)
				CONTROL_PrepareStart(TRUE);
			else
				if(CONTROL_State == DS_InProcess)
					*pUserError = ERR_OPERATION_BLOCKED;
				else
					*pUserError = ERR_DEVICE_NOT_READY;
			break;

		case ACT_DIAG_SET_GATE_V:
			LL_SetGateVoltage(DataTable[REG_DEBUG_V_GATE_mV]);
			break;
			
		case ACT_DIAG_SW_RANGE_CURRENT_0:
			GPIO_SetState(GPIO_OUT_B0, false);
			GPIO_SetState(GPIO_OUT_B1, false);
			break;
			
		case ACT_DIAG_SW_RANGE_CURRENT_1:
			GPIO_SetState(GPIO_OUT_B0, true);
			GPIO_SetState(GPIO_OUT_B1, false);
			break;
			
		case ACT_DIAG_SW_RANGE_CURRENT_2:
			GPIO_SetState(GPIO_OUT_B0, false);
			GPIO_SetState(GPIO_OUT_B1, true);
			break;

		case ACT_DIAG_SW_RANGE_CURRENT_3:
			GPIO_SetState(GPIO_OUT_B0, true);
			GPIO_SetState(GPIO_OUT_B1, true);
			break;
			
		case ACT_DIAG_SW_LAMP:
			LL_PanelLamp(TRUE);
			DELAY_US(DataTable[REG_DEBUG_COMM]);
			LL_PanelLamp(FALSE);
			break;

		case ACT_DIAG_SW_FAN:
			LL_Fan(TRUE);
			DELAY_US(DataTable[REG_DEBUG_COMM]);
			LL_Fan(FALSE);
			break;
			
		case ACT_DIAG_SW_DRCUSWBOARD:
			LL_SWBoard(DataTable[REG_DEBUG_COMM]);
			break;

		case ACT_DIAG_SW_ATUPSBOARD:
			LL_PSBoard(DataTable[REG_DEBUG_COMM]);
			break;

		case ACT_DIAG_ENABLE_PULSE:
			if (DataTable[REG_DEBUG_COMM])
				LL_PulseEnable(TRUE);
			else
				LL_PulseEnable(FALSE);
			break;

		case ACT_DIAG_MANUAL_PULSE:
			LL_PulseStart(TRUE);
			DELAY_US(DataTable[REG_DEBUG_COMM]);
			LL_PulseStart(FALSE);
			break;

		case ACT_DIAG_UPDATE_SYNC_IN:
			DataTable[REG_DEBUG_SYNC_IN] = GPIO_GetState(GPIO_SYNC_IN);
			break;

		default:
			return false;
	}
	
	return true;
}

//-----------------------------

void CONTROL_ApplyParameters()
{
	Int16U GateV = 0;

	if(SP_GetSetpoint(VoltageRate, &GateV))
	{
		LL_SetGateVoltage(GateV);
		LOGIC_SetOutCurrent(LOGIC_SetCurrentRange(Current));
		LOGIC_TimePulse(VoltageRate);

		CONTROL_SetDeviceState(DS_InProcess, SDS_ConfigReady);
	}
	else
	{
		CONTROL_SetDeviceState(DS_Ready, SDS_None);
		DataTable[REG_WARNING] = WARNING_BAD_CONFIG;
	}
}

// -----------------------------

void CONTROL_PrepareStart(Boolean StartTest)
{
	VoltageRate = (Int16U)DataTable[REG_VRATE_SETPOINT];
	Current = (Int16U)DataTable[REG_CURRENT_SETPOINT];
	if(VRATE_SET_MIN <= VoltageRate && VoltageRate <= VRATE_SET_MAX && CURRENT_SET_MIN <= Current &&  Current <= CURRENT_SET_MAX)
	 {
		UsedSync = StartTest;
		LOGIC_BatteryCharge(FALSE);
		CONTROL_SetDeviceState(DS_InProcess, SDS_Config);
	 }
	else
		DataTable[REG_WARNING] = WARNING_BAD_CONFIG;
}
//------------------------------
void CONTROL_HandleBatteryCharge()
{
	// Мониторинг уровня заряда батареи
	float BatteryVoltage = MEASURE_GetBatteryVoltage();
	DataTable[REG_BAT_VOLTAGE] = (uint16_t)(BatteryVoltage * 10);

	if (CONTROL_State == DS_BatteryCharging)
	{
		if (DataTable[REG_BAT_VOLTAGE] >= DataTable[REG_BAT_VOLTAGE_THRESHOLD])
		{
				CONTROL_SetDeviceState(DS_Ready, SDS_None);
		}
		else
		{
			if ((CONTROL_TimeCounter > CONTROL_BatteryFirstChargeTimeCounter && CONTROL_SubState == SDS_FirstCharg) ||
					(CONTROL_TimeCounter > CONTROL_BatteryPostPulseChargeTimeCounter && CONTROL_SubState == SDS_PostPulseCharg))
				CONTROL_SwitchToFault(DF_BATTERY);
		}
	}
}

//-----------------------------
void CONTROL_ResetToDefaults(bool StopPowerSupply)
{
	LOGIC_ResetHWToDefaults(StopPowerSupply);

	CONTROL_SetDeviceState(DS_None, SDS_None);
	CONTROL_FillDefault();
}
//-----------------------------

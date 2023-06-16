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

// Variables
volatile Int64U CONTROL_TimeCounter = 0;
volatile Int64U AfterPulseTimeout = 0;
volatile DeviceState CONTROL_State = DS_None;
volatile DeviceSubState CONTROL_SubState = SDS_None;
static Boolean CycleActive = false;

// Forward functions
Boolean CONTROL_ApplyParameters();
void CONTROL_FillDefault();
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);
void CONTROL_HandleBatteryCharge();

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
	CONTROL_FillDefault();
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

Boolean CONTROL_ApplyParameters()
{
	Int16U GateV = 0;
	if(SP_GetSetpoint(DataTable[REG_VRATE_SETPOINT], &GateV))
	{
		LL_SetGateVoltage(GateV);
		CONTROL_SetDeviceState(DS_Powered, SDS_WaitSync);
		return TRUE;
	}
	else
		return FALSE;
}
//-----------------------------

void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState)
{
	CONTROL_SubState = NewSubState;
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;
}
//-----------------------------

void CONTROL_FillDefault()
{
	DataTable[REG_DEV_STATE] = DS_None;
	DataTable[REG_FAULT_REASON] = FAULT_NONE;
	DataTable[REG_DISABLE_REASON] = DISABLE_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
}
//-----------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch(ActionID)
	{
		case ACT_ENABLE_POWER:
			LL_SetGateVoltage(0);
			CONTROL_SetDeviceState(DS_Powered, SDS_None);
			break;
			
		case ACT_DISABLE_POWER:
			LL_SetGateVoltage(0);
			CONTROL_SetDeviceState(DS_None, SDS_None);
			break;

		case ACT_CLR_FAULT:
			break;

		case ACT_CLR_WARNING:
			DataTable[REG_WARNING] = WARNING_NONE;
			break;

		case ACT_APPLY_PARAMS:
			if(!CONTROL_ApplyParameters())
				*UserError = ERR_OPERATION_BLOCKED;
			break;
			
		case ACT_DIAG_SET_GATE_V:
			LL_SetGateVoltage(DataTable[REG_DEBUG_V_GATE_mV]);
			break;
			
		case ACT_DIAG_SW_LOW_CURRENT:
			GPIO_SetState(GPIO_PS_BOARD, true);
			GPIO_SetState(GPIO_FAN, true);
			GPIO_SetState(GPIO_OUT_B0, false);
			GPIO_SetState(GPIO_OUT_B1, false);

			DELAY_US(30000);

			LL_PulseStart(true);
			DELAY_US((DataTable[REG_ACTUAL_VOLTAGE] / DataTable[REG_VRATE_SETPOINT]) + 10);
			LL_PulseStart(false);

			DELAY_US(100);

			GPIO_SetState(GPIO_FAN, false);
			GPIO_SetState(GPIO_OUT_B0, false);
			GPIO_SetState(GPIO_OUT_B1, false);
			GPIO_SetState(GPIO_PS_BOARD, false);
			break;
			
		case ACT_DIAG_SW_MID_CURRENT:
			GPIO_SetState(GPIO_PS_BOARD, true);
			GPIO_SetState(GPIO_FAN, true);
			GPIO_SetState(GPIO_OUT_B0, true);
			GPIO_SetState(GPIO_OUT_B1, false);

			DELAY_US(30000);

			LL_PulseStart(true);
			DELAY_US((DataTable[REG_ACTUAL_VOLTAGE] / DataTable[REG_VRATE_SETPOINT]) + 10);
			LL_PulseStart(false);

			DELAY_US(100);

			GPIO_SetState(GPIO_FAN, false);
			GPIO_SetState(GPIO_OUT_B0, false);
			GPIO_SetState(GPIO_OUT_B1, false);
			GPIO_SetState(GPIO_PS_BOARD, false);
			break;
			
		case ACT_DIAG_SW_HIGH_CURRENT:
			GPIO_SetState(GPIO_PS_BOARD, true);
			GPIO_SetState(GPIO_FAN, true);
			GPIO_SetState(GPIO_OUT_B0, true);
			GPIO_SetState(GPIO_OUT_B1, true);

			DELAY_US(30000);

			LL_PulseStart(true);
			DELAY_US((DataTable[REG_ACTUAL_VOLTAGE] / DataTable[REG_VRATE_SETPOINT]) + 10);
			LL_PulseStart(false);

			DELAY_US(100);

			GPIO_SetState(GPIO_FAN, false);
			GPIO_SetState(GPIO_OUT_B0, false);
			GPIO_SetState(GPIO_OUT_B1, false);
			GPIO_SetState(GPIO_PS_BOARD, false);
			break;
			
		case ACT_DIAG_SW_LAMP:
			LL_PanelLamp(TRUE);
			DELAY_US(1000000);
			LL_PanelLamp(FALSE);
			break;

		case ACT_DIAG_SW_FAN:
			LL_Fan(TRUE);
			DELAY_US(1000000);
			LL_Fan(FALSE);
			break;
			
		case ACT_DIAG_SW_DRCUSWBOARD:
			LL_SWBoard(DataTable[REG_DEBUG_COMM]);
			break;

		case ACT_DIAG_SW_ATUPSBOARD:
			LL_PSBoard(DataTable[REG_DEBUG_COMM]);
			break;

		case ACT_DIAG_ENABLE_PULSE:
			LL_PulseEnable(TRUE);
			DELAY_US(1000000);
			LL_PulseEnable(FALSE);
			break;

		case ACT_DIAG_MANUAL_PULSE:
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

void CONTROL_AfterPulseProcess()
{
	if(AfterPulseTimeout && (CONTROL_TimeCounter > AfterPulseTimeout))
	{
		AfterPulseTimeout = 0;
		LL_PanelLamp(false);
		CONTROL_SetDeviceState(DS_Powered, SDS_WaitSync);
	}
}
//-----------------------------

void CONTROL_HandleBatteryCharge()
{
	// Мониторинг уровня заряда батареи
	float BatteryVoltage = MEASURE_GetBatteryVoltage();
	DataTable[REG_ACTUAL_VOLTAGE] = (uint16_t)(BatteryVoltage * 10);
}
//-----------------------------

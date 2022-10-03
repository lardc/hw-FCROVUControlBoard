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

// Variables
volatile Int64U CONTROL_TimeCounter = 0;
volatile Int64U AfterPulseTimeout = 0;
volatile DeviceState CONTROL_State = DS_None;
volatile DeviceSubState CONTROL_SubState = SDS_None;
static Boolean CycleActive = false;

// Forward functions
void CONTROL_PrepareGateOutput();
static void CONTROL_ApplyParameters();
static void CONTROL_FillDefault();
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);

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
	DEVPROFILE_ProcessRequests();
	IWDG_Refresh();
}
//-----------------------------

void CONTROL_PrepareGateOutput()
{
	LL_SetGateVoltage(DataTable[REG_DESIRED_GATE_V]);
}
//-----------------------------

void CONTROL_ApplyParameters()
{
	CONTROL_PrepareGateOutput();
	CONTROL_SetDeviceState(DS_Powered, SDS_WaitSync);
}
//-----------------------------

void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState)
{
	CONTROL_SubState = NewSubState;
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;
}
//-----------------------------

static void CONTROL_FillDefault()
{
	DataTable[REG_DEV_STATE] = DS_None;
	DataTable[REG_FAULT_REASON] = FAULT_NONE;
	DataTable[REG_DISABLE_REASON] = DISABLE_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
	
	DataTable[REG_DESIRED_GATE_V] = GATE_VOLTAGE_DEF;
}
//-----------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch(ActionID)
	{
		case ACT_ENABLE_POWER:
			DataTable[REG_DESIRED_GATE_V] = GATE_VOLTAGE_DEF;
			CONTROL_ApplyParameters();
			CONTROL_SetDeviceState(DS_Powered, SDS_None);
			break;
			
		case ACT_DISABLE_POWER:
			CONTROL_SetDeviceState(DS_None, SDS_None);
			break;

		case ACT_CLR_FAULT:
			if(CONTROL_State == DS_Fault)
				CONTROL_SetDeviceState(DS_None, SDS_None);
			DataTable[REG_FAULT_REASON] = FAULT_NONE;
			break;

		case ACT_CLR_WARNING:
			DataTable[REG_WARNING] = WARNING_NONE;
			break;

		case ACT_APPLY_PARAMS:
			CONTROL_ApplyParameters();
			break;
			
		case ACT_DIAG_SET_GATE_V:
			CONTROL_PrepareGateOutput();
			break;
			
		case ACT_DIAG_SW_LOW_CURRENT:
			LL_OutCurrentLow();
			break;
			
		case ACT_DIAG_SW_MID_CURRENT:
			LL_OutCurrentMid();
			break;
			
		case ACT_DIAG_SW_HIGH_CURRENT:
			LL_OutCurrentHigh();
			break;
			
		case ACT_DIAG_SW_LAMP:
			{
				LL_PanelLamp(TRUE);
				DELAY_US(1000000);
				LL_PanelLamp(FALSE);
			}
			break;

		case ACT_DIAG_SW_FAN:
			{
				LL_Fan(TRUE);
				DELAY_US(1000000);
				LL_Fan(FALSE);
			}
			break;
			
		case ACT_DIAG_SW_DRCUSWBOARD:
			{
				LL_SWBoard(TRUE);
				DELAY_US(1000000);
				LL_SWBoard(FALSE);
			}
			break;

		case ACT_DIAG_SW_ATUPSBOARD:
			{
				LL_PSBoard(TRUE);
				DELAY_US(1000000);
				LL_PSBoard(FALSE);
			}
			break;

		case ACT_DIAG_ENABLE_PULSE:
			{
				LL_PulseEnable(TRUE);
				DELAY_US(1000000);
				LL_PulseEnable(FALSE);
			}
			break;

		case ACT_DIAG_MANUAL_PULSE:
			LL_PulseStart(true);
			DELAY_US(100);
			LL_PulseStart(false);
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

// Include
//
#include "Controller.h"
//
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Constraints.h"
#include "DeviceProfile.h"
#include "LowLevel.h"
#include "PowerDriver.h"
#include "Board.h"

// Variables
//
volatile long CONTROL_TimeCounter = 0;
volatile long AfterPulseTimeout = 0;
volatile DeviceState CONTROL_State = DS_None;
volatile DeviceSubState CONTROL_SubState = SDS_None;
static volatile Boolean Locked = false;
static Boolean CycleActive = false;

// Forward functions
//
static void CONTROL_ApplyParameters();
static void CONTROL_FillDefault();
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);

// Functions
//
void CONTROL_Init()
{
	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};

	// Инициализация data table
	DT_Init(EPROMService, false);
	
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

void CONTROL_Cycle()
{
	DRIVER_ControlRoutine();
}
//-----------------------------

static void CONTROL_ApplyParameters()
{
	DRIVER_GatePrepare();
	DRIVER_CacheVariables();
	LL_CurrentLimitEnable(false);

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
	
	DataTable[REG_DESIRED_VOLTAGE] = CAP_VOLTAGE_DEF;
	DataTable[REG_DESIRED_GATE_V] = GATE_VOLTAGE_DEF;
}
//-----------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch(ActionID)
	{
		case ACT_ENABLE_POWER:
			DataTable[REG_DESIRED_VOLTAGE] = CAP_VOLTAGE_MIN;
			DataTable[REG_DESIRED_GATE_V] = 0;
			CONTROL_ApplyParameters();
			CONTROL_SetDeviceState(DS_Powered, SDS_None);
			DRIVER_SetMode(true);
			break;
			
		case ACT_DISABLE_POWER:
			DRIVER_SetMode(false);
			CONTROL_SetDeviceState(DS_None, SDS_None);
			break;
			
		case ACT_DIAG_SET_PWM_FB:
			FlybackPWMSet(DataTable[REG_DIAG_PWM_FB]);
			break;
			
		case ACT_DIAG_SET_PWM_BRK:
			BrakePWMSet(DataTable[REG_DIAG_PWM_BRK]);
			break;
			
		case ACT_DIAG_UPDATE_PWM_FREQ:
			break;
			
		case ACT_DIAG_SET_GATE_V:
			DRIVER_GatePrepare();
			break;
			
		case ACT_DIAG_SW_LOW_RATE:
			LL_VRateLow();
			break;
			
		case ACT_DIAG_SW_MID_RATE:
			LL_VRateMid();
			break;
			
		case ACT_DIAG_SW_HIGH_RATE:
			LL_VRateHigh();
			break;
			
		case ACT_DIAG_SW_OFF_RATE:
			LL_VRateNone();
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

		LL_ExternalLED(false);
		LL_CurrentLimitEnable(false);
		CONTROL_SetDeviceState(DS_Powered, SDS_WaitSync);
	}
}
//-----------------------------

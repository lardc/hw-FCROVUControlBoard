// Include
#include "stdinc.h"
#include "Controller.h"
#include "LowLevel.h"
#include "Board.h"
#include "Global.h"
#include "SysConfig.h"
#include "Logic.h"
#include "DeviceObjectDictionary.h"
#include "DataTable.h"

// Functions prototypes
//
void INT_NOExtSyncControl();
void INT_SyncWidthControl();

// Variables
//
Int64U SyncLineTimeCounter = 0;

// Functions
//
void EXTI9_5_IRQHandler()
{
	if(CONTROL_SubState == SDS_WaitSync && !GPIO_GetState(GPIO_SYNC_IN))
	{
			LOGIC_HandleFan(true);
			LOGIC_HandlePanelLamp(true);
			TIM_Start(TIM7);
			SyncLineTimeCounter = CONTROL_TimeCounter + WIDTH_SYNC_LINE_MAX;
			CONTROL_SetDeviceState(DS_InProcess, SDS_RiseEdgeDetected);
	}

	EXTI_FlagReset(EXTI_5);
}
//-----------------------------------------

void USART1_IRQHandler()
{
	if(ZwSCI_RecieveCheck(USART1))
	{
		ZwSCI_RegisterToFIFO(USART1);
		ZwSCI_RecieveFlagClear(USART1);
	}
}
//-----------------------------------------

void USB_LP_CAN_RX0_IRQHandler()
{
	if (NCAN_RecieveCheck())
	{
		NCAN_RecieveData();
		NCAN_RecieveFlagReset();
	}
}
//-----------------------------------------

void TIM3_IRQHandler()
{
	static uint16_t CounterTmp = 0, CounterLed = 0;


	if(TIM_StatusCheck(TIM3))
	{
		if(++CounterTmp >= (1000 / TIMER3_uS))
		{
			CounterTmp = 0;
			CONTROL_TimeCounter++;
			
			if(++CounterLed >= LED_BLINK_TIME)
			{
				LL_ToggleLed();
				CounterLed = 0;
			}
		}
		LOGIC_HandleFan(false);
		LOGIC_HandlePanelLamp(false);

		if(CONTROL_State == DS_InProcess || CONTROL_State == DS_ConfigReady)
		{
			LOGIC_Update();
			INT_NOExtSyncControl();
			INT_SyncWidthControl();
		}
		TIM_StatusClear(TIM3);
	}
}
//-----------------------------------------

void TIM7_IRQHandler()
{
	if(TIM_StatusCheck(TIM7))
	{
		TIM_Stop(TIM7);
		TIM_StatusClear(TIM7);
	}
}
//-----------------------------------------

void INT_NOExtSyncControl()
{
	if(SyncStartTimeout && (CONTROL_TimeCounter >= SyncStartTimeout))
	{
		LOGIC_ResetHWToDefaults(FALSE);
		DataTable[REG_WARNING] = WARNING_NO_SYNC;
		CONTROL_SetDeviceState(DS_Ready, SDS_None);
		LL_PanelLamp(false);
		SyncStartTimeout = 0;
	}
}
//-----------------------------------------

void INT_SyncWidthControl()
{
	if(SyncLineTimeCounter && (CONTROL_TimeCounter >= SyncLineTimeCounter))
	{
		LOGIC_ResetHWToDefaults(FALSE);
		DataTable[REG_WARNING] = WARNING_SYNC_TIMEOUT;
		CONTROL_SetDeviceState(DS_Ready, SDS_None);
		LL_PanelLamp(false);
		SyncLineTimeCounter = 0;
		SyncStartTimeout = 0;
	}
}
//-----------------------------------------

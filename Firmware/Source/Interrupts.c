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



// Functions
//
void EXTI0_IRQHandler()
{
	if(CONTROL_SubState == SDS_WaitSync)
	{
		if(GPIO_GetState(GPIO_SYNC_IN))
		{
			LL_PanelLamp(true);
			TIM_Start(TIM7);

			CONTROL_SetDeviceState(DS_InProcess, SDS_RiseEdgeDetected);
		}
		if (CONTROL_TimeCounter >= StartTimeout)
		{
			CONTROL_SetDeviceState(DS_Ready, SDS_None);
			DataTable[REG_WARNING] = WARNING_NO_SYNC;
		}
	}

	EXTI_FlagReset(EXTI_0);
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

		LOGIC_Update();
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

// Header
#include "LowLevel.h"

// Include
#include "Board.h"
#include "Global.h"

// Forward functions
//
void LL_VRateSelect(bool LowEnable, bool MidEnable, bool HighEnable);

// Functions
//
void LL_ToggleLed()
{
	GPIO_Toggle(GPIO_LED1);
}
//-----------------------------

void LL_Led2(bool State)
{
	GPIO_SetState(GPIO_LED2, State);
}
//-----------------------------

void LL_PanelLamp(bool State)
{
	GPIO_SetState(GPIO_LAMP, State);
}
//-----------------------------

void LL_PulseEnable(bool State)
{
	GPIO_SetState(GPIO_PULSE_EN, State);
}
//-----------------------------

void LL_PulseStart(bool State)
{
	GPIO_SetState(GPIO_START_PULSE, State);
}
//-----------------------------

//void LL_Fan(bool State)
//{
//	GPIO_SetState(GPIO_FAN, State);
//}
//-----------------------------

void LL_PSBoard(bool State)
{
	GPIO_SetState(GPIO_PS_BOARD, !State);
}
//-----------------------------

void LL_SWBoard(bool State)
{
	GPIO_SetState(GPIO_SW_BOARD, State);
}
//-----------------------------

void LL_SetGateVoltage(Int16U Voltage)
{
	Int16U DACValue = (Int16U)((float)Voltage / GATE_ANALOG_GAIN / ANALOG_REF_MV * ADC_DAC_RESOLUTION);
	DAC_SetValueCh1(DAC1, (DACValue > ADC_DAC_RESOLUTION) ? ADC_DAC_RESOLUTION : DACValue);
}
//-----------------------------


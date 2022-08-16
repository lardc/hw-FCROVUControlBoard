// Header
#include "LowLevel.h"

// Define
//
#define CURRENT_RANGE_0		400
#define CURRENT_RANGE_1		800
#define CURRENT_RANGE_2		1200
#define CURRENT_RANGE_3		1600
//


// Functions
//
// LED on board
bool LL_IsBlinkLED()
{
	return GPIO_Read_Bit(GPIOB, Pin_7);
}
//-----------------------------

void LL_BlinkLED(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_7) : GPIO_Bit_Rst(GPIOB, Pin_7);
}
//-----------------------------

// External LED
void LL_ExternalLED(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_3) : GPIO_Bit_Rst(GPIOB, Pin_3);
}
//-----------------------------

// Unit cooler
void LL_ExternalFan(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_15) : GPIO_Bit_Rst(GPIOA, Pin_15);
}
//-----------------------------

// ATUPsBoard control
void LL_PsBoard(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_4) : GPIO_Bit_Rst(GPIOB, Pin_4);
}
//-----------------------------

// DRCU SwitchBoard control
void LL_SwitchBoard(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_5) : GPIO_Bit_Rst(GPIOB, Pin_5);
}
//-----------------------------

// Software start pulse
void LL_StartPulse(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_6) : GPIO_Bit_Rst(GPIOA, Pin_6);
}
//-----------------------------

// Start pulse enable
void LL_StartPulseEn(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_3) : GPIO_Bit_Rst(GPIOA, Pin_3);
}
//-----------------------------

// Change range of current
void LL_CurrentRange(Int16U Current)
{
	switch(Current)
	{
		case CURRENT_RANGE_0:
			{
				GPIO_Bit_Rst(GPIOA, Pin_1);
				GPIO_Bit_Rst(GPIOA, Pin_2);
			}
			break;

		case CURRENT_RANGE_1:
			{
				GPIO_Bit_Set(GPIOA, Pin_1);
				GPIO_Bit_Rst(GPIOA, Pin_2);
			}
			break;

		case CURRENT_RANGE_2:
			{
				GPIO_Bit_Rst(GPIOA, Pin_1);
				GPIO_Bit_Set(GPIOA, Pin_2);
			}
			break;

		case CURRENT_RANGE_3:
			{
				GPIO_Bit_Set(GPIOA, Pin_1);
				GPIO_Bit_Set(GPIOA, Pin_2);
			}
			break;

		default:
			{
				GPIO_Bit_Rst(GPIOA, Pin_1);
				GPIO_Bit_Rst(GPIOA, Pin_2);
			}
			break;
	}
}
//-----------------------------


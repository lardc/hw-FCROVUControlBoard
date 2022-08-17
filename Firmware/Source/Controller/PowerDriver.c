// Includes
#include "PowerDriver.h"
//
#include <stdlib.h>
#include "DeviceObjectDictionary.h"
#include "DataTable.h"
#include "Global.h"
#include "Board.h"
#include "SysConfig.h"
#include "Measurement.h"
#include "LowLevel.h"

// Definitions
#define VRATE_INDEX_LOW		1
#define VRATE_INDEX_MID		2
#define VRATE_INDEX_HIGH	0

// Variables
static bool Shutdown = false;
static volatile bool Active = false;
static volatile bool hystOn = false;
static float ErrorLimit, AcceptableWindow;
static float DesiredVoltage;

// Forward functions
char DRIVER_CalculateVoltageSaturated(Int8U Percent, Int8U LowSat, Int8U HighSat);
Int16U DRIVER_ValueSaturate(Int16U Var, Int16U LowSat, Int16U HighSat);
void DRIVER_SelectVRateChannel(Int16U Channel);
void DRIVER_SetGateVoltage(Int16U Voltage);

// Functions
void DRIVER_SetMode(bool Activate)
{
	Active = Activate;
}
//-----------------------------

char DRIVER_CalculateVoltageSaturated(Int8U Percent, Int8U LowSat, Int8U HighSat)
{
	Int16U t = ((Int16U)Percent) * DataTable[REG_DESIRED_VOLTAGE] / 100;
	return DRIVER_ValueSaturate(t, LowSat, HighSat);
}
//-----------------------------

Int16U DRIVER_ValueSaturate(Int16U Var, Int16U LowSat, Int16U HighSat)
{
	if(Var < LowSat)
		Var = LowSat;
	else if(Var > HighSat)
		Var = HighSat;
	
	return Var;
}
//-----------------------------

void DRIVER_CacheVariables()
{
	ErrorLimit = DRIVER_CalculateVoltageSaturated(CAP_V_DEADZONE, CAP_V_DEADZONE_MIN_V, CAP_V_DEADZONE_MAX_V);
	AcceptableWindow = DRIVER_CalculateVoltageSaturated(CAP_V_WINDOW, CAP_V_WINDOW_MIN_V, CAP_V_WINDOW_MAX_V);
	
	DesiredVoltage = DataTable[REG_DESIRED_VOLTAGE];
}
//-----------------------------

void DRIVER_SelectVRateChannel(Int16U Channel)
{
	switch(Channel)
	{
		case VRATE_INDEX_LOW:
			LL_VRateLow();
			break;

		case VRATE_INDEX_MID:
			LL_VRateMid();
			break;

		default:
			LL_VRateHigh();
			break;
	}
}
//-----------------------------

void DRIVER_SetGateVoltage(Int16U Voltage)
{
	Int16U DACValue = (Int16U)((float)Voltage / GATE_ANALOG_GAIN / ANALOG_REF_MV * ADC_DAC_RESOLUTION);
	DAC_SetValueCh1(DAC1, (DACValue > ADC_DAC_RESOLUTION) ? ADC_DAC_RESOLUTION : DACValue);
}
//-----------------------------

void DRIVER_GatePrepare()
{
	DRIVER_SetGateVoltage(DataTable[REG_DESIRED_GATE_V]);
	DRIVER_SelectVRateChannel(DataTable[REG_VRATE_RANGE]);
}
//-----------------------------

void DRIVER_ControlRoutine()
{
	Int16U pwm_fb, ActualVoltage;
	Int8U pwm_fb_sleep;
	
	float capV = MEASURE_GetBatteryVoltage();
	DataTable[REG_ACTUAL_VOLTAGE] = (Int16U)capV;
	ActualVoltage = capV;
	
	if(Active)
	{
		int err = DesiredVoltage - ActualVoltage;
		
		// Select proper PWM duty cycle
		if(ActualVoltage < 200)
		{
			pwm_fb = FB_BASE_PWM_LOW;
			pwm_fb_sleep = FB_BASE_PWM_IDLE;
		}
		else if(ActualVoltage < 400)
		{
			pwm_fb = FB_BASE_PWM_LOW + FB_BASE_PWM_ZONE_SIZE;
			pwm_fb_sleep = FB_BASE_PWM_IDLE;
		}
		else if(ActualVoltage < 600)
		{
			pwm_fb = FB_BASE_PWM_LOW + FB_BASE_PWM_ZONE_SIZE * 2;
			pwm_fb_sleep = FB_BASE_PWM_IDLE * 2;
		}
		else if(ActualVoltage < 800)
		{
			pwm_fb = FB_BASE_PWM_LOW + FB_BASE_PWM_ZONE_SIZE * 3;
			pwm_fb_sleep = FB_BASE_PWM_IDLE * 3;
		}
		else
		{
			pwm_fb = FB_BASE_PWM_HIGH;
			pwm_fb_sleep = FB_BASE_PWM_IDLE * 4;
		}
		
		// Override startup PWM
		if(ActualVoltage < 30)
			pwm_fb = FB_BASE_PWM_IDLE;
		
		if(err >= ErrorLimit)
		{
			// active charge zone
			hystOn = true;
			FlybackPWMSet(pwm_fb);
			BrakePWMSet(BRK_TOP_PWM);
		}
		else if(err <= -(ErrorLimit * 2))
		{
			// active discharge zone
			hystOn = false;
			FlybackPWMSet(0);
			BrakePWMSet(BRK_BASE_PWM);
		}
		else if(err <= -ErrorLimit)
		{
			// soft self-discharge zone
			hystOn = false;
			FlybackPWMSet(0);
			BrakePWMSet(BRK_TOP_PWM);
		}
		else
		{
			// soft charge zone
			FlybackPWMSet(hystOn ? pwm_fb_sleep : 0);
			BrakePWMSet(BRK_TOP_PWM);
		}
		
		DataTable[REG_VOLTAGE_OK] = (abs(err) < AcceptableWindow) ? 1 : 0;
		Shutdown = false;
	}
	else
	{
		if(!Shutdown)
		{
			FlybackPWMSet(0);
			BrakePWMSet(0);
			
			DataTable[REG_VOLTAGE_OK] = 0;
			Shutdown = true;
		}
	}
}
//-----------------------------

void FlybackPWMSet(uint16_t PWM_Value)
{
	TIMx_PWM_SetValue(TIM15, TIMx_CHANNEL1, PWM_Value);
}
//-----------------------------

void BrakePWMSet(uint16_t PWM_Value)
{
	TIMx_PWM_SetValue(TIM16, TIMx_CHANNEL1, PWM_Value);
}
//-----------------------------

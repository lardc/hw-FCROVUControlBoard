#include "Measurement.h"
#include "DataTable.h"
#include "ZwADC.h"


// Functions
//
uint16_t MEASURE_BatteryVoltageRaw()
{
	return ADC_Measure(ADC1, 1);
}
//------------------------------------------------------------------------------

float MEASURE_BatteryVoltage()
{
	// Оцифровка и удаление смещения
	float Result = (float)MEASURE_BatteryVoltageRaw() * (float)DataTable[REG_VOLTAGE_GAIN] / 10;

	return Result;
}
//------------------------------------------------------------------------------

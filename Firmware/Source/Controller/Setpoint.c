// ----------------------------------------
// Gate voltage setpoint generator
// ----------------------------------------

// Header
#include "Setpoint.h"

// Includes
#include "DeviceObjectDictionary.h"
#include "DataTable.h"
#include "Global.h"
#include "Controller.h"

// Definitions
#define SETPOINT_ARRAY_SIZE		5

// Forward functions
Boolean SP_Generate(Int16U VRate, pInt16U GateV);

// Functions
Boolean SP_Generate(Int16U VRate, pInt16U GateV)
{
	// Определение граничных скоростей нарастания в диапазоне
	Int16U CellRateStartIndex = REG_SETPOINT_GATEV1 + 1;
	Int16U CellRateEndIndex = CellRateStartIndex + (SETPOINT_ARRAY_SIZE - 1) * 2;

	if(!(DataTable[CellRateStartIndex] <= VRate && VRate <= DataTable[CellRateEndIndex]))
		return FALSE;
	
	Int16U BaseDTGateVAddr = REG_SETPOINT_GATEV1;
	Int16U BaseDTVRateAddr = BaseDTGateVAddr + 1;
	
	// Проверка на прямое совпадение
	Int32U i;
	for(i = 0; i < SETPOINT_ARRAY_SIZE; i++)
	{
		if(DataTable[BaseDTVRateAddr + i * 2] == VRate)
		{
			*GateV = DataTable[BaseDTGateVAddr + i * 2];
			return TRUE;
		}
	}
	
	// Интерполяция значений
	for(i = SETPOINT_ARRAY_SIZE - 1; i > 0; i--)
	{
		if(VRate > DataTable[BaseDTVRateAddr + (i - 1) * 2])
		{
			Int32U Base = DataTable[BaseDTGateVAddr + (i - 1) * 2];
			Int32U N = DataTable[BaseDTGateVAddr + i * 2] - DataTable[BaseDTGateVAddr + (i - 1) * 2];
			Int32U D = DataTable[BaseDTVRateAddr + i * 2] - DataTable[BaseDTVRateAddr + (i - 1) * 2];
			Int32U K = VRate - DataTable[BaseDTVRateAddr + (i - 1) * 2];
			
			*GateV = Base + N * K / D;
			return TRUE;
		}
		else
			continue;
	}
	
	return FALSE;
}
// ----------------------------------------

Boolean SP_GetSetpoint(Int16U VRate, pInt16U GateV)
{
	if(SP_Generate(VRate, GateV))
	{
		DataTable[REG_DEBUG_GATE_V_CALC_mV] = *GateV;
		return TRUE;
	}
	else
	{
		DataTable[REG_DEBUG_GATE_V_CALC_mV] = 0;
		return FALSE;
	}
}
// ----------------------------------------

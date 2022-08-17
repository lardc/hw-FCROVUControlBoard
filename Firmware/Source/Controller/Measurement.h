#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

#include "stdinc.h"
#include "SysConfig.h"

// Variables
extern Int16U MEASURE_ADC_BatteryVoltageRaw[ADC_DMA_BUFF_SIZE];

// Functions
float MEASURE_GetBatteryVoltage();

#endif // __MEASUREMENT_H

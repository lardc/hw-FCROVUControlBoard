// -----------------------------------------
// Global definitions
// ----------------------------------------

#ifndef __GLOBAL_H
#define __GLOBAL_H


// Include
#include "stdinc.h"
#include "SysConfig.h"


// Definitions
// 
// Global miscellaneous parameters
#define	SCCI_TIMEOUT_TICKS		1000			// in ms
//
#define EP_WRITE_COUNT			0				// Количество массивов для записи
#define EP_COUNT				0				// Количество массивов для чтения
// Password to unlock non-volatile area for write
#define ENABLE_LOCKING			FALSE
// -----------------------------------------------

// Временные параметры
#define LED_BLINK_TIME			500				// Период моргания светодиода на плате [ms]
#define	T_CHARGE_DELAY			60000			// Время ожидания заряда [мс]
#define	T_CHARGE_DELAY_SHORT	15000			// Время ожидания заряда в процессе измерения [мс]
//
// Параметры вентилятора
#define FAN_ACTIVE_TIME			30000			// Время работы вентилятора (мс)
#define FAN_SLEEP_TIME			180000			// Время повторного запуска вентилятора (мс)

// Параметры измерения заряда батареи
#define V_BAT_THRESHOLD_MIN		(495.0f)		// Нижний порог заряда батареи [В]
#define V_BAT_THRESHOLD_MAX		(510.0f)		// Верхний порог заряда батареи [В]

// Параметры оцифровки
#define ADC_RESOLUTION			4095
#define ADC_REF_MV				3000			// Опорное напряжение АЦП, мВ
// -----------------------------------------------


#endif //  __GLOBAL_H

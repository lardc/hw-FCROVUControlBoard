#ifndef __GLOBAL_H
#define __GLOBAL_H

// Include
#include "stdinc.h"

// Global parameters
#define	SCCI_TIMEOUT_TICKS				1000	// Receive timeout (in ms)
#define EP_WRITE_COUNT					0		// Количество массивов для записи
#define EP_COUNT						0		// Количество массивов для чтения
#define FEP_COUNT						0		// Количество массивов флоат для чтения
#define ENABLE_LOCKING					FALSE
#define LED_BLINK_TIME					500		// (in ms)

// ADC & DAC settings
#define ANALOG_REF_MV					3300	// (in mV)
#define ADC_DAC_RESOLUTION				4095

// Fine tuning
#define CAP_V_CONV						0.265f

// Gate voltage settings
#define GATE_ANALOG_GAIN				1.42f

// Задержки в процессе тестирования (в мс)
#define SWITCH_TIME_US					20			// Задержка включения реле
#define SYNC_TIMEOUT_US					5000		// Таймаут синхронизации после конфигурации ячеек
#define DOP_TIME_US						100			// Длительность полки
#define FALL_TIME_US					20			// Длительность заднего фронта
#define WIDTH_SYNC_LINE_MAX				500			// Максимальная длительность импульса синхронизации, мс
#define AFTER_PULSE_TIMEOUT				500			// (in ms)

// Данные для задания параметров формирования
#define CH_CURRENT						400			// Ток формирующий при КЗ на 1 силовой плате, А
#define CH_MAX							3			// Максимальное количество силовых плат

#endif // __GLOBAL_H

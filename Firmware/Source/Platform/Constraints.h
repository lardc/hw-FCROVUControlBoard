// ----------------------------------------
// Global definitions
// ----------------------------------------

#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"
#include "Global.h"

// Definitions
#define NO							0
#define YES							1

// Затворное напряжение расчётной таблицы (мВ)
#define GATEV_POINT_MIN				0
#define GATEV_POINT_MAX				5000
#define GATEV_POINT_DEF				0

// Скорость нарастания напряжения расчётной таблицы (В/мкс х10)
#define VRATE_POINT_MIN				10
#define VRATE_POINT_MAX				3000
#define VRATE_POINT_DEF				10

// Задание скорости нарастания (В/мкс x10)
#define VRATE_SET_MIN				200
#define VRATE_SET_MAX				2000
#define VRATE_SET_DEF				200

// Задание тока формирующийся при КЗ, А
#define CURRENT_SET_MIN				0
#define CURRENT_SET_MAX				1600
#define CURRENT_SET_DEF				400

// Время Timeout заряда батареи (мкс)
#define TIMECHRAGE_MIN				100
#define TIMECHRAGE_MAX				50000
#define TIMECHRAGE_DEF				5000

// Напряжение заряда батареи (В x10)
#define VCHRAGE_MIN					0
#define VCHRAGE_MAX					6000
#define VCHRAGE_DEF					5000
// Types
typedef struct __TableItemConstraint
{
	float Min;
	float Max;
	float Default;
} TableItemConstraint;

// Variables
extern const TableItemConstraint Constraint[];

#endif // __CONSTRAINTS_H

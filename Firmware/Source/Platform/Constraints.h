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
// Gate voltage (in mV)
#define GATE_VOLTAGE_MIN			0
#define GATE_VOLTAGE_MAX			5000
#define GATE_VOLTAGE_DEF			0

// Rate range limits
#define RATE_RANGE_MIN				0
#define RATE_RANGE_MAX				2
#define RATE_RANGE_DEF				0

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

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
// Capacitor voltage (in V)
#define CAP_VOLTAGE_MIN				30
#define CAP_VOLTAGE_MAX				900
#define CAP_VOLTAGE_DEF				30

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
	Int16U Min;
	Int16U Max;
	Int16U Default;
} TableItemConstraint;

// Variables
extern const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE];
extern const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START];

#endif // __CONSTRAINTS_H

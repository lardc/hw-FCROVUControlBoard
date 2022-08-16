// -----------------------------------------
// Global definitions
// ----------------------------------------

#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"
#include "Global.h"

//Definitions
//
#define GAIN_COEFFICIENT_MIN	1
#define GAIN_COEFFICIENT_MAX	10000
#define VRATE_TO_DAC_GAIN_MIN	1
#define VRATE_TO_DAC_GAIN_MAX	10000
#define VRATE_MIN				20
#define VRATE_MAX				200
#define DBG_DAC_MIN				0
#define DBG_DAC_MAX				4095
#define I_SHORT_CIRCUIT_MIN		400
#define I_SHORT_CIRCUIT_MAX		1600
//
#define GAIN_VOLTAGE_DEF		1100
#define VRATE_TO_DAC_GAIN_DEF	1000
#define VRATE_DEF				20
#define I_SHORT_CIRCUIT_DEF		1600



// Types
//
typedef struct __TableItemConstraint
{
	Int16U Min;
	Int16U Max;
	Int16U Default;
} TableItemConstraint;


// Variables
//
extern const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE];
extern const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START];


#endif // __CONSTRAINTS_H

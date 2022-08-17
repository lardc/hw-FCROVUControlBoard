// ----------------------------------------
// Global definitions
// ----------------------------------------

// Header
#include "Constraints.h"

// Constants
//
const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE] =
{
	{0, 0, 0}																	// 0
};

const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START] =
{
	{CAP_VOLTAGE_MIN, CAP_VOLTAGE_MAX, CAP_VOLTAGE_DEF},						// 1
	{GATE_VOLTAGE_MIN, GATE_VOLTAGE_MAX, GATE_VOLTAGE_DEF},						// 2
	{0, 0, 0},																	// 3
	{RATE_RANGE_MIN, RATE_RANGE_MAX, RATE_RANGE_DEF},							// 4
	{0, 0, 0},																	// 5
	{0, 0, 0},																	// 6
	{0, 0, 0},																	// 7
	{0, 0, 0},																	// 8
	{0, 0, 0},																	// 9
	{0, 0, 0},																	// 10
	{0, 0, 0},																	// 11
	{0, 0, 0},																	// 12
	{0, 0, 0},																 	// 13
	{0, 0, 0},																	// 14
	{0, 0, 0},																	// 15
	{0, 0, 0},																	// 16
	{0, 0, 0},																	// 17
	{0, 0, 0},																	// 18
	{0, 0, 0},																	// 19
	{0, INT16U_MAX, 0},															// 20
	{0, INT16U_MAX, 0},															// 21
	{0, INT16U_MAX, 0},															// 22
	{0, INT16U_MAX, 0},															// 23
	{0, 0, 0},																	// 24
	{0, 0, 0},																 	// 25
	{0, 0, 0},																	// 26
	{0, 0, 0},																	// 27
	{0, 0, 0},																	// 28
	{0, 0, 0},																	// 29
	{0, 0, 0},																	// 30
	{0, 0, 0}																	// 31
};

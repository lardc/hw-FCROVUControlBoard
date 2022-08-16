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
#define EP_WRITE_COUNT			0				// ���������� �������� ��� ������
#define EP_COUNT				0				// ���������� �������� ��� ������
// Password to unlock non-volatile area for write
#define ENABLE_LOCKING			FALSE
// -----------------------------------------------

// ��������� ���������
#define LED_BLINK_TIME			500				// ������ �������� ���������� �� ����� [ms]
#define	T_CHARGE_DELAY			60000			// ����� �������� ������ [��]
#define	T_CHARGE_DELAY_SHORT	15000			// ����� �������� ������ � �������� ��������� [��]
//
// ��������� �����������
#define FAN_ACTIVE_TIME			30000			// ����� ������ ����������� (��)
#define FAN_SLEEP_TIME			180000			// ����� ���������� ������� ����������� (��)

// ��������� ��������� ������ �������
#define V_BAT_THRESHOLD_MIN		(495.0f)		// ������ ����� ������ ������� [�]
#define V_BAT_THRESHOLD_MAX		(510.0f)		// ������� ����� ������ ������� [�]

// ��������� ���������
#define ADC_RESOLUTION			4095
#define ADC_REF_MV				3000			// ������� ���������� ���, ��
// -----------------------------------------------


#endif //  __GLOBAL_H

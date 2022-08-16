#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// �������
// 
#define ACT_ENABLE_POWER				1	// ������� � ��������� ��������
#define ACT_DISABLE_POWER				2	// ���������� �����
#define ACT_FAULT_CLEAR					3	// ������� fault
#define ACT_WARNING_CLEAR				4	// ������� warning
#define ACT_RESET_DEVICE				5	// ���������� ����������
//
// ������� �������
#define ACT_DBG_START_PULSE				10	// �������� ������ START_PULSE
#define ACT_DBG_PULSE_SRC_EN			11	// �������� ������ PulseSourceEN
#define ACT_DBG_DAC_OUT					12	// ����� �������� ���
#define ACT_DBG_FAN						13	// �������� ������ FAN
#define ACT_DBG_LAMP					14	// �������� ������ LAMP
#define ACT_DBG_PB_CTRL					15	// �������� ������ PB_CTRL
#define ACT_DBG_SB_CTRL					16	// �������� ������ SB_CTRL
#define ACT_DBG_VSO						17	// �������� ��������� ���������� �� �������������
//
#define ACT_START_PULSE					100	// ������ ��������
#define ACT_CONFIG						101	// ������������ �����
//
#define ACT_SAVE_TO_ROM					200	// ���������� ���������������� ������ �� FLASH ����������
#define ACT_RESTORE_FROM_ROM			201	// �������������� ������ �� FLASH
#define ACT_RESET_TO_DEFAULT			202	// ����� DataTable � ��������� �� ���������
//
#define ACT_BOOT_LOADER_REQUEST			320	// ���������� ���������� � ����� ��������������������
//
// -----------------------------------------------

// ��������
// 
// ����������� ��������
#define REG_VOLTAGE_GAIN				0	// ����������� ������� ����������� ������ ��������� ���������� (x10)
#define REG_VRATE_TO_DAC_GAIN			1	// ����������� �������������� �������� ���������� ���������� � ������� ��� (�100)

// ������������ �������� ������-������
#define REG_V_RATE_VALUE				128	// �������� ������� �������� ���������� ���������� [�/���]
#define REG_DBG_DAC						129
#define REG_I_SHORT_CIRCUIT				130	// ��� ����� ��� �� (400, 800, 1200, 1600)�
//

// �������� ������ ������
#define REG_DEV_STATE					192	// ������� ���������
#define REG_FAULT_REASON				193	// ������� Fault
#define REG_DISABLE_REASON				194	// ������� Disable
#define REG_WARNING						195	// ������� Warning
#define REG_PROBLEM						196	// ������� Problem
#define REG_TEST_FINISHED				197
//
#define REG_DBG_VSO_VALUE				200	// ���������� �� ������� (� �)
//
// -----------------------------------------------


// ENDPOINTS
//
// -----------------------------------------------


// Operation results
//
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

// Fault and disable codes
// 
#define DF_NONE							0
#define DF_BATTERY						1	// �������� ������ �������

// Problem
//
#define PROBLEM_NONE					0

// Warning
// 
#define WARNING_NONE					0
#define WARNING_I_OUT_OF_RANGE			1	// ���������� �������� ���� ��� ���������

// User Errors
// 
#define ERR_NONE						0	// ������ ���
#define ERR_CONFIGURATION_LOCKED		1	// ���������� �������� �� ������
#define ERR_OPERATION_BLOCKED			2	// �������� �� ����� ���� ��������� � ������� ��������� ����������
#define ERR_DEVICE_NOT_READY			3	// ���������� �� ������ ��� ����� ���������
#define ERR_WRONG_PWD					4	// ������������ ����

// Password to unlock non-volatile area for write
// 
#define ENABLE_LOCKING					FALSE


typedef enum __DeviceState
{
	// �������� ���������
	DS_None							= 0,	// ���� � �������������� ���������
	DS_Fault						= 1,	// ���� � ��������� Fault
	DS_Disabled						= 2,	// ���� � ��������� Disabled
	DS_Charging						= 3,	// ���� � �������� ������ �������
	DS_Ready						= 4,	// ���� � ��������� ����������
	DS_ConfigReady					= 5,	// ���� � ������������������ ���������
	DS_InProcess					= 6,	// ���� � �������� ������������ ��������
} DeviceState;


#endif // __DEV_OBJ_DIC_H

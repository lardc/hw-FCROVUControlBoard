#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// ACTIONS
//
#define ACT_ENABLE_POWER			1	// Enable flyback converter
#define ACT_DISABLE_POWER			2	// Disable flyback converter
#define ACT_CLR_FAULT				3	// Clear fault (try switch state from FAULT to NONE)
#define ACT_CLR_WARNING				4	// Clear warning
//
#define ACT_APPLY_PARAMS			10	// Apply params
//
#define ACT_DIAG_SET_GATE_V			112	// Set gate control voltage
#define ACT_DIAG_UPDATE_PWM_FREQ	113	// Set alternative PWM frequency
#define ACT_DIAG_SW_LOW_CURRENT		114
#define ACT_DIAG_SW_MID_CURRENT		115
#define ACT_DIAG_SW_HIGH_CURRENT	116

#define ACT_SAVE_TO_ROM				200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM		201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT		202	// Сброс DataTable в состояние по умолчанию
//
#define ACT_BOOT_LOADER_REQUEST		320	// Перезапуск процессора с целью перепрограммирования

// REGISTERS
//
#define REG_DESIRED_GATE_V			128	// Desired gate voltage
#define	REG_VRATE_RANGE				129 // Voltage rate selector
//
#define REG_DEV_STATE				192	// Device state
#define REG_FAULT_REASON			193	// Fault reason in the case DeviceState -> FAULT
#define REG_DISABLE_REASON			194	// Fault reason in the case DeviceState -> DISABLED
#define REG_WARNING					195	// Warning if present
#define REG_PROBLEM					196	// Problem reason
#define REG_OP_RESULT				197	// Indicates that test is done and there is result or fault
//
#define REG_ACTUAL_VOLTAGE			200	// Actual capacitor level
#define REG_VOLTAGE_OK				201	// Charged flag
// -----------------------------
#define REG_FWINFO_SLAVE_NID		256	// Device CAN slave node ID
#define REG_FWINFO_MASTER_NID		257	// Device CAN master node ID (if presented)
// 258 - 259
#define REG_FWINFO_STR_LEN			260	// Length of the information string record
#define REG_FWINFO_STR_BEGIN		261	// Begining of the information string record

// FAULT CODES
//
#define FAULT_NONE					0

// WARNING CODES
//
#define WARNING_NONE				0

// DISABLE CODES
//
#define DISABLE_NONE				0

// USER ERROR CODES
//
#define ERR_NONE					0	// No error
#define ERR_OPERATION_BLOCKED		2	// Operation can't be done due to current device state
#define ERR_DEVICE_NOT_READY		3	// Device isn't ready to switch state

#endif // __DEV_OBJ_DIC_H

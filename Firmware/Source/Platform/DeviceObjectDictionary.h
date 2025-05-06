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
#define ACT_START_TEST				100	// Start test
//
#define ACT_DIAG_SET_GATE_V			120	// Задать затворное напряжение
#define ACT_DIAG_SW_RANGE_CURRENT_0	121	// Выбрать 0 диапазон тока
#define ACT_DIAG_SW_RANGE_CURRENT_1	122	// Выбрать 1 диапазон тока
#define ACT_DIAG_SW_RANGE_CURRENT_2	123	// Выбрать 2 диапазон тока
#define ACT_DIAG_SW_RANGE_CURRENT_3	124	// Выбрать 3 диапазон тока
#define ACT_DIAG_SW_LAMP			125	// Управление лампой на передней панели
#define ACT_DIAG_SW_FAN				126	// Управление вентилятором
#define ACT_DIAG_SW_DRCUSWBOARD		127	// Управление ЭМ DRCUSwitchBoard
#define ACT_DIAG_SW_ATUPSBOARD		128	// Управление ЭМ ATUPSBoard
#define ACT_DIAG_ENABLE_PULSE		129	// Разрешение входа запуска импульса
#define ACT_DIAG_MANUAL_PULSE		130	// Ручной запуск импульса
#define ACT_DIAG_UPDATE_SYNC_IN		131	// Записать состояние SYNC_IN в регистр

#define ACT_SAVE_TO_ROM				200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM		201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT		202	// Сброс DataTable в состояние по умолчанию
//
#define ACT_BOOT_LOADER_REQUEST		320	// Перезапуск процессора с целью перепрограммирования

// REGISTERS
//
#define REG_PS_VOLTAGE_K			0	// Коэффициент для пересчёта напряжения питания
#define REG_PS_VOLTAGE_OFFSET		1	// Смещение для пересчёта напряжения питания (в тиках)
#define REG_VOLTAGE_RATE_K			2	// Коэффициент корректировки скорости нарастания напряжения
#define REG_CHRAGE_TIMEOUT_FULL		3 	// Таймаут полного заряда батареи,мс
#define REG_CHRAGE_TIMEOUT_PULSE	4	// Таймаут заряда батареи после формирования,мс
#define	REG_BAT_VOLTAGE_THRESHOLD	5	// Минимальный порог заряда батареи, В *10
#define REG_FAN_CTRL				6	// Разрешение работы вентиляторов
#define REG_FAN_TIME				7	// Время работы вентилятора,c

// 5 - 19
#define REG_SETPOINT_GATEV1			20	// Затворное напряжение, точка 1 (в мВ)
#define REG_SETPOINT_VRATE1			21	// Скорость нарастания напряжения, точка 1 (в В/мкс x10)
// 22 - 31								// Задания для промежуточных значений точек
#define REG_SETPOINT_GATEV5			28	// Затворное напряжение, точка 5 (в мВ)
#define REG_SETPOINT_VRATE5			29	// Скорость нарастания напряжения, точка 5 (в В/мкс x10)

#define REG_VRATE_SETPOINT			129	// Задание скорости нарастания напряжения (В/мкс х 10)
#define REG_CURRENT_SETPOINT		130	// Задание тока (А)
//
#define REG_DEBUG_COMM				150	// Отладочный регистр общего назначения
#define REG_DEBUG_V_GATE_mV			151	// Отладочный регистр задания затворного напряжения

#define REG_DEV_STATE				192	// Регистр состояния
#define REG_FAULT_REASON			193	// Fault reason in the case DeviceState -> FAULT
#define REG_DISABLE_REASON			194	// Fault reason in the case DeviceState -> DISABLED
#define REG_WARNING					195	// Warning if present
#define REG_PROBLEM					196	// Problem reason
#define REG_OP_RESULT				197	// Indicates that test is done and there is result or fault
#define REG_SUB_STATE				198	// Регистр вспомогательного состояния
//
#define REG_BAT_VOLTAGE				200	// Напряжение на конденсаторах
#define REG_VOLTAGE_OK				201	// Charged flag
//
#define REG_DEBUG_SYNC_IN			210	// Состояние входа SYNC_IN
#define REG_DEBUG_GATE_V_CALC_mV	211	// Рассчитанное затворное напряжение
// -----------------------------
#define REG_FWINFO_SLAVE_NID		256	// Device CAN slave node ID
#define REG_FWINFO_MASTER_NID		257	// Device CAN master node ID (if presented)
#define REG_DEVICE_ID				258	// Device ID
// 258 - 259
#define REG_FWINFO_STR_LEN			260	// Length of the information string record
#define REG_FWINFO_STR_BEGIN		261	// Begining of the information string record

// FAULT CODES
//
#define DF_NONE						0
#define DF_BATTERY					1		// Ошибка заряда батареи

// WARNING CODES
//
#define WARNING_NONE				0
#define WARNING_NO_SYNC				1		// Нет внешнего импульса синхронизации
#define WARNING_BAD_CONFIG			2		// Заданая конфигурация за пределами допустимого

// DISABLE CODES
//
#define DISABLE_NONE				0

// USER ERROR CODES
//
#define ERR_NONE					0	// No error
#define ERR_OPERATION_BLOCKED		2	// Operation can't be done due to current device state
#define ERR_DEVICE_NOT_READY		3	// Device isn't ready to switch state

#endif // __DEV_OBJ_DIC_H

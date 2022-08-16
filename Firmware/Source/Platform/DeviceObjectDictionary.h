#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
// 
#define ACT_ENABLE_POWER				1	// Переход в состояние ожидания
#define ACT_DISABLE_POWER				2	// Отключение блока
#define ACT_FAULT_CLEAR					3	// Очистка fault
#define ACT_WARNING_CLEAR				4	// Очистка warning
#define ACT_RESET_DEVICE				5	// Перезапуск процессора
//
// Команды отладки
#define ACT_DBG_START_PULSE				10	// Проверка выхода START_PULSE
#define ACT_DBG_PULSE_SRC_EN			11	// Проверка выхода PulseSourceEN
#define ACT_DBG_DAC_OUT					12	// Вывод значения ЦАП
#define ACT_DBG_FAN						13	// Проверка выхода FAN
#define ACT_DBG_LAMP					14	// Проверка выхода LAMP
#define ACT_DBG_PB_CTRL					15	// Проверка выхода PB_CTRL
#define ACT_DBG_SB_CTRL					16	// Проверка выхода SB_CTRL
#define ACT_DBG_VSO						17	// Проверка измерения напряжение на конденсаторах
//
#define ACT_START_PULSE					100	// Запуск импульса
#define ACT_CONFIG						101	// Конфигурация блока
//
#define ACT_SAVE_TO_ROM					200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM			201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT			202	// Сброс DataTable в состояние по умолчанию
//
#define ACT_BOOT_LOADER_REQUEST			320	// Перезапуск процессора с целью перепрограммирования
//
// -----------------------------------------------

// Регистры
// 
// Сохраняемые регистры
#define REG_VOLTAGE_GAIN				0	// Коэффициент деления аналогового тракта измерения напряжения (x10)
#define REG_VRATE_TO_DAC_GAIN			1	// Коэффициент преобразования скорости нарастания напряжения в уровень ЦАП (х100)

// Несохраняемы регистры чтения-записи
#define REG_V_RATE_VALUE				128	// Значение задания скорости нарастания напряжения [В/мкС]
#define REG_DBG_DAC						129
#define REG_I_SHORT_CIRCUIT				130	// Ток блока при КЗ (400, 800, 1200, 1600)А
//

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_TEST_FINISHED				197
//
#define REG_DBG_VSO_VALUE				200	// Напряжение на батарее (в В)
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
#define DF_BATTERY						1	// Проблема заряда батареи

// Problem
//
#define PROBLEM_NONE					0

// Warning
// 
#define WARNING_NONE					0
#define WARNING_I_OUT_OF_RANGE			1	// Измеренное значение тока вне диапазона

// User Errors
// 
#define ERR_NONE						0	// Ошибок нет
#define ERR_CONFIGURATION_LOCKED		1	// Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	// Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	// Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	// Неправильный ключ

// Password to unlock non-volatile area for write
// 
#define ENABLE_LOCKING					FALSE


typedef enum __DeviceState
{
	// Основные состояния
	DS_None							= 0,	// Блок в неопределенном состоянии
	DS_Fault						= 1,	// Блок в состоянии Fault
	DS_Disabled						= 2,	// Блок в состоянии Disabled
	DS_Charging						= 3,	// Блок в процессе заряда батареи
	DS_Ready						= 4,	// Блок в состоянии готовности
	DS_ConfigReady					= 5,	// Блок в сконфигурированном состоянии
	DS_InProcess					= 6,	// Блок в процессе формирования импульса
} DeviceState;


#endif // __DEV_OBJ_DIC_H

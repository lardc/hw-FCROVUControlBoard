#include "InitConfig.h"
#include "SysConfig.h"
#include "Controller.h"

// Functions
//
void CONTROL_Idle();

int main()
{
	__disable_irq();
	SCB->VTOR = (uint32_t)BOOT_LOADER_MAIN_PR_ADDR;
	__enable_irq();

	// Настройка системной частоты тактирования
	INITCFG_ConfigSystemClock();

	// Настройка внешних прерываний
	INITCFG_ConfigEI();

	// Настройка портов
	INITCFG_ConfigIO();

	// Настройка UART
	INITCFG_ConfigUART();

	// Настройка АЦП
	INITCFG_ConfigADC();

	// Настройка DMA
	INITCFG_ConfigDMA();

	// Настройка таймеров
	INITCFG_ConfigTimer3();
	INITCFG_ConfigTimer6();
	INITCFG_ConfigTimer7();

	// Настройка сторожевого таймера
	INITCFG_ConfigWatchDog();

	// Настройка таймеров ШИМ
	INITCFG_ConfigPWM();

	// Настройка ЦАП
	INITCFG_ConfigDAC();

	// Инициализация логики контроллера
	CONTROL_Init();

	// Фоновый цикл
	while (TRUE)
		CONTROL_Idle();

	return 0;
}

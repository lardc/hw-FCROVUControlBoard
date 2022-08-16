#include "Interrupts.h"
#include "InitConfig.h"
#include "SysConfig.h"

// Functions
//
void CONTROL_Idle();

int main()
{
	__disable_irq();
	SCB->VTOR = (uint32_t)BOOT_LOADER_MAIN_PR_ADDR;
	__enable_irq();

	// ��������� ��������� ������� ������������
	SysClk_Config();

	// ��������� ������
	IO_Config();

	// ��������� CAN
	CAN_Config();

	// ��������� UART
	UART_Config();

	// ��������� ��������
	Timer3_Config();
	Timer4_Config();
	Timer6_Config();

	// ��������� ���
	DAC_Config();

	// ��������� ���
	ADC_Init();

	// ��������� ����������� �������
	WatchDog_Config();

	// ��������� ������� ����������
	EI_Config();

	// ������������� ������ �����������
	InitializeController(false);

	// ������� ����
	while (TRUE)
		CONTROL_Idle();

	return 0;
}

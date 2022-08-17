#include "InitConfig.h"
#include "Board.h"
#include "SysConfig.h"
#include "Controller.h"
#include "LowLevel.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Measurement.h"

// Functions
//
Boolean INITCFG_ConfigSystemClock()
{
	return RCC_PLL_HSE_Config(QUARTZ_FREQUENCY, PREDIV_4, PLL_14);
}
//------------------------------------------------------------------------------

void INITCFG_ConfigEI()
{
	// Sync
	EXTI_Config(EXTI_PB, EXTI_0, BOTH_TRIG, 0);
	EXTI_EnableInterrupt(EXTI0_IRQn, 0, true);
}
//------------------------------------------------------------------------------

void INITCFG_ConfigIO()
{
	// Включение тактирования портов
	RCC_GPIO_Clk_EN(PORTA);
	RCC_GPIO_Clk_EN(PORTB);
	
	// Аналоговые порты
	GPIO_InitAnalog(GPIO_MEASURE_V);
	GPIO_InitAnalog(GPIO_DAC_V);
	
	// Цифровые входы
	GPIO_InitInput(GPIO_SYNC_IN, Pull_Up);
	
	// Выходы
	GPIO_InitPushPullOutput(GPIO_I_LIM);
	GPIO_InitPushPullOutput(GPIO_LED1);
	GPIO_InitPushPullOutput(GPIO_LED2);
	GPIO_InitPushPullOutput(GPIO_LOW_VRATE);
	GPIO_InitPushPullOutput(GPIO_MID_VRATE);
	GPIO_InitPushPullOutput(GPIO_HIGH_VRATE);
	
	GPIO_SetState(GPIO_I_LIM, true);
	
	// Альтернативные функции
	GPIO_InitAltFunction(GPIO_ALT_UART_RX, AltFn_7);
	GPIO_InitOpenDrainAltFunction(GPIO_ALT_UART_TX, AltFn_7, NoPull);
	GPIO_InitAltFunction(GPIO_ALT_PWM_FB, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_PWM_BRAKE, AltFn_1);
}
//------------------------------------------------------------------------------

void INITCFG_ConfigUART()
{
	USART_Init(USART1, SYSCLK, USART_BAUDRATE);
	USART_Recieve_Interupt(USART1, 2, true);
}
//------------------------------------------------------------------------------

void INITCFG_ConfigADC()
{
	RCC_ADC_Clk_EN(ADC_12_ClkEN);
	
	ADC_Calibration(ADC1);
	ADC_SoftTrigConfig(ADC1);
	ADC_ChannelSeqReset(ADC1);
	
	for(uint8_t i = 1; i <= ADC_DMA_BUFF_SIZE; ++i)
		ADC_ChannelSet_Sequence(ADC1, ADC1_V_BAT_CHANNEL, i);
	
	ADC_ChannelSeqLen(ADC1, ADC_DMA_BUFF_SIZE);
	ADC_DMAConfig(ADC1);
	ADC_Enable(ADC1);
}
//------------------------------------------------------------------------------

void INITCFG_ConfigDMA()
{
	DMA_Clk_Enable(DMA1_ClkEN);
	
	// DMA для АЦП напряжения батареи
	DMA_Reset(DMA_ADC_V_BAT_CHANNEL);
	DMAChannelX_Config(DMA_ADC_V_BAT_CHANNEL, DMA_MEM2MEM_DIS, DMA_LvlPriority_LOW, DMA_MSIZE_16BIT, DMA_PSIZE_16BIT,
			DMA_MINC_EN, DMA_PINC_DIS, DMA_CIRCMODE_EN, DMA_READ_FROM_PERIPH);
	DMAChannelX_DataConfig(DMA_ADC_V_BAT_CHANNEL, (uint32_t)(&MEASURE_ADC_BatteryVoltageRaw[0]), (uint32_t)(&ADC1->DR),
			ADC_DMA_BUFF_SIZE);
	DMA_ChannelEnable(DMA_ADC_V_BAT_CHANNEL, true);
}
//------------------------------------------------------------------------------

void INITCFG_ConfigTimer3()
{
	TIM_Clock_En(TIM_3);
	TIM_Config(TIM3, SYSCLK, TIMER3_uS);
	TIM_Interupt(TIM3, 3, true);
	TIM_Start(TIM3);
}
//------------------------------------------------------------------------------

void INITCFG_ConfigTimer6()
{
	TIM_Clock_En(TIM_6);
	TIM_Config(TIM6, SYSCLK, TIMER6_uS);
	TIM_MasterMode(TIM6, MMS_UPDATE);
	TIM_Start(TIM6);
}
//------------------------------------------------------------------------------

void INITCFG_ConfigTimer7()
{
	TIM_Clock_En(TIM_7);
	TIM_Config(TIM7, SYSCLK, TIMER7_uS);
	TIM_Interupt(TIM7, 1, true);
}
//------------------------------------------------------------------------------

void INITCFG_ConfigWatchDog()
{
	IWDG_Config();
	IWDG_ConfigureSlowUpdate();
}
//------------------------------------------------------------------------------

void INITCFG_ConfigPWM()
{
	//PA2 - Flyback PWM
	TIM_Clock_En(TIM_15);
	TIM_Config(TIM15, SYSCLK, TIMER15_uS);
	TIMx_PWM_ConfigChannel(TIM15, TIMx_CHANNEL1);
	TIMx_PWM_SetPolarity(TIM15, TIMx_CHANNEL1, false);
	TIM_Start(TIM15);
	
	//PA6 - Brake PWM
	TIM_Clock_En(TIM_16);
	TIM_Config(TIM16, SYSCLK, TIMER16_uS);
	TIMx_PWM_ConfigChannel(TIM16, TIMx_CHANNEL1);
	TIMx_PWM_SetPolarity(TIM16, TIMx_CHANNEL1, true);
	TIM_Start(TIM16);
}
//------------------------------------------------------------------------------

void INITCFG_ConfigDAC()
{
	DACx_Clk_Enable(DAC_1_ClkEN);
	DACx_Reset();
	DAC_TriggerConfigCh1(DAC1, TRIG1_TIMER6, TRIG1_ENABLE);
	DAC_BufferCh1(DAC1, false);
	DAC_EnableCh1(DAC1);
}
// -----------------------------------------------------------------------------

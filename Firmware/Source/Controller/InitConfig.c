#include <InitConfig.h>


// Forward functions
//
void CONTROL_Init();


// Functions
//
Boolean SysClk_Config()
{
	return RCC_PLL_HSE_Config(QUARTZ_FREQUENCY, PREDIV_4, PLL_14);
}
//------------------------------------------------------------------------------

void IO_Config()
{
	// Ports enable
	RCC_GPIO_Clk_EN(PORTA);
	RCC_GPIO_Clk_EN(PORTB);

	// Analog
	GPIO_Config (GPIOA, Pin_0, Analog, NoPull, HighSpeed, NoPull);				// PA0 - analog input for ADC (battery voltage)
	GPIO_Config (GPIOA, Pin_4, Analog, NoPull, HighSpeed, NoPull);				// PA1 - DAC out

	// Input
	GPIO_Config (GPIOA, Pin_5, Input, NoPull, HighSpeed, NoPull);				// PA5 - Sync trigger

	// Outputs
	GPIO_Config (GPIOA, Pin_1,  Output, PushPull, HighSpeed, NoPull);			// PA1 - OUT_B0_CTRL
	GPIO_Bit_Rst(GPIOA, Pin_1);
	GPIO_Config (GPIOA, Pin_2,  Output, PushPull, HighSpeed, NoPull);			// PA2 - OUT_B1_CTRL
	GPIO_Bit_Rst(GPIOA, Pin_2);
	GPIO_Config (GPIOA, Pin_3,  Output, PushPull, HighSpeed, NoPull);			// PA3 - Start pulse enable
	GPIO_Bit_Rst(GPIOA, Pin_3);
	GPIO_Config (GPIOA, Pin_6,  Output, PushPull, HighSpeed, NoPull);			// PA6 - Start pulse
	GPIO_Bit_Rst(GPIOA, Pin_6);
	GPIO_Config (GPIOA, Pin_15,  Output, PushPull, HighSpeed, NoPull);			// PA15 - Fan control
	GPIO_Bit_Rst(GPIOA, Pin_15);
	GPIO_Config (GPIOB, Pin_3,  Output, PushPull, HighSpeed, NoPull);			// PB3 - Lamp control
	GPIO_Bit_Rst(GPIOB, Pin_3);
	GPIO_Config (GPIOB, Pin_4,  Output, PushPull, HighSpeed, NoPull);			// PB4 - ATUPsBoard control
	GPIO_Bit_Rst(GPIOB, Pin_4);
	GPIO_Config (GPIOB, Pin_5,  Output, PushPull, HighSpeed, NoPull);			// PB5 - DRCU SwitchBoard control
	GPIO_Bit_Set(GPIOB, Pin_5);
	GPIO_Config (GPIOB, Pin_6,  Output, OpenDrain, HighSpeed, NoPull);			// PB6 - LED2
	GPIO_Bit_Rst(GPIOB, Pin_6);
	GPIO_Config (GPIOB, Pin_7,  Output, PushPull, HighSpeed, NoPull);			// PB7 - LED1
	GPIO_Bit_Rst(GPIOB, Pin_7);

	// Alternative functions
	GPIO_Config (GPIOA, Pin_11, AltFn, PushPull, HighSpeed, NoPull);			// PA11 (CAN RX)
	GPIO_AltFn  (GPIOA, Pin_11, AltFn_9);
	GPIO_Config (GPIOA, Pin_12, AltFn, PushPull, HighSpeed, NoPull);			// PA12 (CAN TX)
	GPIO_AltFn  (GPIOA, Pin_12, AltFn_9);
	GPIO_Config (GPIOA, Pin_9, AltFn, PushPull, HighSpeed, NoPull);				// PA9(USART1 TX)
	GPIO_AltFn  (GPIOA, Pin_9, AltFn_7);
	GPIO_Config (GPIOA, Pin_10, AltFn, PushPull, HighSpeed, NoPull);			// PA10(USART1 RX)
	GPIO_AltFn  (GPIOA, Pin_10, AltFn_7);
}
//------------------------------------------------------------------------------

void CAN_Config()
{
	RCC_CAN_Clk_EN(CAN_1_ClkEN);
	NCAN_Init(SYSCLK, CAN_BAUDRATE, FALSE);
	NCAN_FIFOInterrupt(TRUE);
	NCAN_FilterInit(0, 0, 0);
}
//------------------------------------------------------------------------------

void UART_Config()
{
	USART_Init(USART1, SYSCLK, USART_BAUDRATE);
	USART_Recieve_Interupt(USART1, 0, true);
}
//------------------------------------------------------------------------------

void ADC_Init()
{
	RCC_ADC_Clk_EN(ADC_12_ClkEN);
	ADC_Calibration(ADC1);
	ADC_SoftTrigConfig(ADC1);
	ADC_ChannelSet_SampleTime(ADC1, 1, ADC_SMPL_TIME_7_5);
	ADC_Enable(ADC1);
}
//------------------------------------------------------------------------------

void DAC_Config()
{
	DACx_Clk_Enable(DAC_1_ClkEN);
	DACx_Reset();
	DAC_Trigger_Config(TRIG1_TIMER6, TRIG1_ENABLE);
	DAC_Buff(BUFF1, false);
	DACx_Enable(DAC1ENABLE);
}
// -----------------------------------------------------------------------------

void Timer6_Config()
{
	TIM_Clock_En(TIM_6);
	TIM_Config(TIM6, SYSCLK, TIMER6_uS);
	TIM_Start(TIM6);
}
//------------------------------------------------------------------------------

void Timer3_Config()
{
	TIM_Clock_En(TIM_3);
	TIM_Config(TIM3, SYSCLK, TIMER3_uS);
	TIM_Interupt(TIM3, 1, true);
	TIM_Start(TIM3);
}
//------------------------------------------------------------------------------

void Timer4_Config()
{
	TIM_Clock_En(TIM_4);
	TIM_Config(TIM4, SYSCLK, TIMER4_uS);
	TIM_Interupt(TIM4, 0, true);
	TIM_Start(TIM4);
}
//------------------------------------------------------------------------------

void WatchDog_Config()
{
	IWDG_Config();
	IWDG_ConfigureFastUpdate();
}
//------------------------------------------------------------------------------

void EI_Config()
{
	EXTI_Config(EXTI_PA, EXTI_5, FALL_TRIG, 0);
	EXTI_EnableInterrupt(EXTI9_5_IRQn, 0, true);
}
//------------------------------------------------------------------------------

void InitializeController(Boolean GoodClock)
{
	CONTROL_Init();
}
// -----------------------------------------------------------------------------

﻿// ----------------------------------------
// Board parameters
// ----------------------------------------

#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

// Board includes
#include "ZwRCC.h"
#include "ZwGPIO.h"
#include "ZwNCAN.h"
#include "ZwSCI.h"
#include "ZwTIM.h"
#include "ZwIWDG.h"
#include "ZwNFLASH.h"

// Blinking LED settings
#define LED_BLINK_PORT		GPIOB
#define LED_BLINK_PIN		Pin_7

// Определения для выходных портов
GPIO_PortPinSettingMacro GPIO_MEASURE_V 	= {GPIOA, Pin_0};	// Напряжение на выходе обратноходного ИП
GPIO_PortPinSettingMacro GPIO_DAC_V 		= {GPIOA, Pin_4};	// ЦАП - задание

GPIO_PortPinSettingMacro GPIO_OUT_B0 	 	= {GPIOA, Pin_1};
GPIO_PortPinSettingMacro GPIO_OUT_B1 	 	= {GPIOA, Pin_2};
GPIO_PortPinSettingMacro GPIO_PULSE_EN 	 	= {GPIOA, Pin_3};
GPIO_PortPinSettingMacro GPIO_START_PULSE 	= {GPIOA, Pin_6};
GPIO_PortPinSettingMacro GPIO_FAN		 	= {GPIOA, Pin_15};
GPIO_PortPinSettingMacro GPIO_LAMP	 	 	= {GPIOB, Pin_3};
GPIO_PortPinSettingMacro GPIO_PS_BOARD 	 	= {GPIOB, Pin_4};
GPIO_PortPinSettingMacro GPIO_SW_BOARD 	 	= {GPIOB, Pin_5};
GPIO_PortPinSettingMacro GPIO_LED1	 	 	= {GPIOB, Pin_7};
GPIO_PortPinSettingMacro GPIO_LED2	 	 	= {GPIOB, Pin_6};

// Определения для портов альтернативных функций
GPIO_PortPinSettingMacro GPIO_ALT_UART1_TX	= {GPIOA, Pin_9};
GPIO_PortPinSettingMacro GPIO_ALT_UART1_RX	= {GPIOA, Pin_10};
GPIO_PortPinSettingMacro GPIO_ALT_CAN1_TX	= {GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_CAN1_RX	= {GPIOA, Pin_11};

#endif // __BOARD_CONFIG_H

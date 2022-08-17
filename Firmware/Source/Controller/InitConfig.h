#ifndef __INITCONFIG_H
#define __INITCONFIG_H

#include "Board.h"

//Functions
//

Boolean INITCFG_ConfigSystemClock();
void INITCFG_ConfigEI();
void INITCFG_ConfigIO();
void INITCFG_ConfigUART();
void INITCFG_ConfigADC();
void INITCFG_ConfigDMA();
void INITCFG_ConfigTimer3();
void INITCFG_ConfigTimer6();
void INITCFG_ConfigTimer7();
void INITCFG_ConfigWatchDog();
void INITCFG_ConfigPWM();
void INITCFG_ConfigDAC();

#endif //__INITCONFIG_H

#ifndef __CAP_MON_H
#define __CAP_MON_H

// Include
#include "stdinc.h"

// Functions
//
void DRIVER_SetMode(bool Activate);
void DRIVER_ControlRoutine();

void DRIVER_CacheVariables();
void DRIVER_GatePrepare();

void FlybackPWMSet(uint16_t PWM_Value);
void BrakePWMSet(uint16_t PWM_Value);

#endif // __CAP_MON_H

// -----------------------------------------
// Logic controller
// ----------------------------------------

#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"


// Functions
//
void CONTROL_InterruptProcess();
void SoftwareStopPulse();


// Variables
//
extern volatile Int64U CONTROL_TimeCounter;


#endif // __CONTROLLER_H

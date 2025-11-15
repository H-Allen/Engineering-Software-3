#ifndef __GPIO_INIT_H_
#define __GPIO_INIT_H_

#include "xgpio.h"		// Added for xgpio object definitions

XStatus initGpio(void);

extern XGpio SLIDE_SWITCHES;
extern XGpio VGA_REGION;
extern XGpio VGA_COLOUR;

#endif

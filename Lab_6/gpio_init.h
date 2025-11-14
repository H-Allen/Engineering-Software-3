#ifndef __GPIO_INIT_H_ // Include Guard to prevent this header from being included multiple times.
#define __GPIO_INIT_H_

#include "xgpio.h" // Include the header file for the Xilinx GPIO driver library.

// Function prototype for the GPIO initialization function.
// It is defined in gpio_init.c.
XStatus initGpio(void);

// Use the 'extern' keyword to declare global variables.
// These variables are defined in gpio_init.c, and 'extern' makes them
// accessible to any other source file that includes this header.
// Each variable is an instance of the XGpio driver struct.

extern XGpio SEG7_HEX_OUT;  // For the GPIO connected to the 7-segment display's segment lines (a,b,c,d,e,f,g,dp).
extern XGpio SEG7_SEL_OUT;  // For the GPIO that selects which of the 4 digits is active.
extern XGpio P_BTN_LEFT;    // For the GPIO connected to the left push-button.
extern XGpio P_BTN_RIGHT;   // For the GPIO connected to the right push-button.
extern XGpio LED_OUT;       // For the GPIO connected to the LEDs.
extern XGpio SLIDE_SWITCHES;// For the GPIO connected to the slide switches.

#endif // End of the include guard.

#include <stdio.h>
#include "platform.h"
#include "xil_types.h"
#include "seg7_display.h"
#include "gpio_init.h"

int main() {
    int status;
    u16 counter = 0;
    u16 switchCounter = 0;
    u8 displayMode = 0;

    init_platform();

    status = initGpio();
    if(status != XST_SUCCESS) {
        print("GPIO Initialization Failed\r\n");
        cleanup_platform();
        return 0;
    }

    //Set up the interrupt system
    status = setupInterruptSystem();
    if(status != XST_SUCCESS) {
        print("Interrupt Setup Failed\r\n");
        cleanup_platform();
        return 0;
    }

    print("System ready!\r\n");

    while(1)
    {
        if (displayMode == 0) 
        {
            displayNumber(counter);
        }
        else 
        {
            displayNumber(switchCounter);
        }

        if(XGpio_DiscreteRead(&P_BTN_LEFT, 1) == 1) {
            //A simple software delay loop for button debouncing
            //A 'volatile prevents the compiler from optimising this loop away

            for (volatile int i = 0; i < 100000; i++);

            //After the delay, check the button state again
            if(XGpio_DiscreteRead(&P_BTN_LEFT, 1) == 1)
            {
                //If the counter is 999 wrap it around to 0 otherwise increment it
                counter = (counter >= 9999) ? 0 : counter + 1;
                //Switch display mode to show the counter
                displayMode = 0;
                //Wait for button release
                while (XGpio_DiscreteRead(&P_BTN_LEFT, 1) == 1);
            }
        }
        if(XGpio_DiscreteRead(&P_BTN_RIGHT, 1) == 1) {
            //A simple software delay loop for button debouncing
            //A 'volatile prevents the compiler from optimising this loop away

            for (volatile int i = 0; i < 100000; i++);

            //After the delay, check the button state again
            if(XGpio_DiscreteRead(&P_BTN_RIGHT, 1) == 1)
            {
                //Read the slide switch value and apply a bitmasl 
                //This keeps the lower 4 bits, resulting in a value from 0 to 15
                u16 switchVal = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1) & 0x0F;
                
                //Add the switch value to the switchCounter, wrapping around at 9999
                switchCounter = (switchCounter + switchVal) % 10000;

                //Switch display mode to show the slide switch value
                displayMode = 1;

                //Wait for button release
                while (XGpio_DiscreteRead(&P_BTN_RIGHT, 1) == 1);
            }
        }
    }
    cleanup_platform();
    return 0;
}
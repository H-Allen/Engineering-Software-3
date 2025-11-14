#include <stdio.h>
#include "platform.h"
#include "xil_types.h"
#include "seg7_display.h"
#include "gpio_init.h"

int main() {
    init_platform();
    int status;

    status = initGpio();
    if(status != XST_SUCCESS) {
        print("GPIO Initialization Failed\r\n");
        cleanup_platform();
        return 0;
    }

    //Set up the interrupt system
    status = setUpInterruptSystem();
    if(status != XST_SUCCESS) {
        print("Interrupt Setup Failed\r\n");
        cleanup_platform();
        return 0;
    }

    u16 pushBtnLeftIn = 0;
    u16 counter = 0;

    while(1)
    {
        displayNumber(counter);
        pushBtnLeftIn = XGpio_DiscreteRead(&P_BTN_LEFT, 1);
        if(pushBtnLeftIn == 1) {
            while (pushBtnLeftIn == 1) {
                pushBtnLeftIn = XGpio_DiscreteRead(&P_BTN_LEFT, 1);
                displayNumber(counter);
            }
            counter++;
        }
    }
    
    cleanup_platform();
    return 0;
}
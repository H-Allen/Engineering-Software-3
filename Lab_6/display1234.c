#include <stdio.h>
#include "platform.h"
#include "xil_types.h"
#include "seg7_display.h"
#include "gpio_init.h"

u16 slideSwitchIn = 0;

int main()
{
    init_platform();
    int status;

    //initialise the GPIOs
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

    while (1)
    {
        displayNumber(1234);
    }
    cleanup_platform();
    return 0;
}
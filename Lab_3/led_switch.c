#include <stdio.h>
#include "platform.h"
#include "xgpio.h"

XStatus initGpio(void);
XGpio SLIDE_SWITCHES;
XGpio LED_OUT;
u32 XGpio_DiscreteRead(XGpio *InstancePtr, unsigned Channel);
void XGpio_DiscreteWrite(XGpio *InstancePtr, unsigned Channel, u32 Mask);

int main()
{
    init_platform();
    XStatus Status;
    u16 slideSwitchIn;

    Status = initGpio();
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    while (1) {
        slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
        XGpio_DiscreteWrite(&LED_OUT, 1, slideSwitchIn);
    }

    cleanup_platform();
    return 0;
}

XStatus initGpio(void) {
    XStatus Status;

    Status = XGpio_Initialize(&SLIDE_SWITCHES, 7);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    Status = XGpio_Initialize(&LED_OUT, 2);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}
#include <stdio.h>
#include "platform.h"
#include "xil_types.h"
#include "xgpio.h"

int setUpnterruptSystem();
void hwTimerISR(void *CallbackRef);
XStatus initGpio(void);
volatile u8 shiftLeft = TRUE;

XGpio LED_OUT;

volatile u16 ledValue = 0x0001;

int main() {
    init_platform();

    XStatus status;
    status = initGpio();
    status = setUpInterruptSystem();

    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    while(1) {
        XGpio_DiscreteWrite(&LED_OUT, 1, ledValue);
    }
    cleanup_platform();
    return 0;
}

XStatus initGpio(void) {
    XStatus Status;

    Status = XGpio_Initialize(&LED_OUT, 2);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }


    return XST_SUCCESS;
}

void hwTimerISR(void *CallbackRef)
{
    /*
     * Hardware Timer ISR - called every 0.1 seconds
     * Controls LED shifting direction based on position
     */

    if (shiftLeft) {  // Currently shifting left
        if (ledValue < 0x8000) {  // Check if LED hasn't reached leftmost position (bit 15)
            ledValue = ledValue << 1;  // Shift LED one position to the left
        }
        else {  // LED has reached leftmost position
            shiftLeft = FALSE;  // Change direction to right
            ledValue = ledValue >> 1;  // Shift LED one position to the right
        }
    }
    else {  // Currently shifting right
        if (ledValue > 0x0001) {  // Check if LED hasn't reached rightmost position (bit 0)
            ledValue = ledValue >> 1;  // Shift LED one position to the right
        }
        else {  // LED has reached rightmost position
            shiftLeft = TRUE;  // Change direction to left
            ledValue = ledValue << 1;  // Shift LED one position to the left
        }
    }

    return;
}
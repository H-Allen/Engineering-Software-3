#include <stdio.h>
#include "platform.h"
#include "xil_types.h"
#include "xgpio.h"

int setUpnterruptSystem();
void hwTimerISR(void *CallbackRef);
XStatus initGpio(void);
u16 rotateLeft(u16 value, u8 shift);

XGpio LED_OUT;

volatile u16 ledValue = 0x0001;
//declare counter variable
volatile u8 counter = 27;

int main() {
    init_platform();

    XStatus status;
    status = initGpio();
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    status = setUpInterruptSystem();
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    u8 limit = 27;
    counter = limit;

    XGpio_DiscreteWrite(&LED_OUT, 1, ledValue);
    while(1) {
        if (counter == 0) {
            ledValue = rotateLeft(ledValue, 1);
            XGpio_DiscreteWrite(&LED_OUT, 1, ledValue);
            limit = limit / 3;
            if (limit == 0) {
                limit = 27;
            }
            counter = limit;
        }
    }
    cleanup_platform();
    return 0;
}

void hwTimerISR(void *CallbackRef)
{
    /*
     * Hardware Timer ISR - called every 0.1 seconds
     * Decrements the counter for LED rotation timing
     */

    if (counter > 0) {
        counter--;
    }
}

XStatus initGpio(void) {
    XStatus Status;

    Status = XGpio_Initialize(&LED_OUT, 2);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}
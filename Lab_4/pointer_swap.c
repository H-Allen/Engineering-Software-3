#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "platform.h"
#include "sleep.h"

//GPIO device configuration
#define LED_DEVICE_ID 2
#define LED_CHANNEL 1

XGpio GpioLed; // GPIO instance for LEDs

void swap_with_pointers(int *a, int *b);
void swap_no_temp(int *a, int *b);
vod swap_arithmetic(int *a, int *b);

int main() {
    init_platform();

    int status;
    int x = 15; //binary 00001111
    int y = 27; //binary 00011011

    status = XGpio_Initialize(&GpioLed, LED_DEVICE_ID);
    if (status != XST_SUCCESS) {
        xil_printf("GPIO Initialization Failed\r\n");
        return XST_FAILURE;
    }

    XGpio_SetDataDirection(&GpioLed, LED_CHANNEL, 0x0); // Set as output

    xil_printf("=== Swap with Pointers ===\r\n");

    //Method 1: Traditional swap:
    xil_printf("Before Swap with Pointers: x = %d, y = %d\r\n", x, y);
    XGpio_DiscreteWrite(&GpioLed, LED_CHANNEL, x);
    sleep(2);

    XGpio_DiscreteWrite(&GpioLed, LED_CHANNEL, x);
    sleep(2);

    swap_with_pointers(&x, &y);
    xil_printf("After Swap with Pointers: x = %d, y = %d\r\n", x, y);
    XGpio_DiscreteWrite(&GpioLed, LED_CHANNEL, x);
    sleep(2);

    XGpio_DiscreteWrite(&GpioLed, LED_CHANNEL, y);
    sleep(2);

    //Method 2: XOR swap:
    int a = 100, b = 200;
    xil_printf("Before XOR Swap: a = %d, b = %d\r\n", a, b);
    swap_no_temp(&a, &b);
    xil_printf("After XOR Swap: a = %d, b = %d\r\n", a, b);

    //Method 3: Arithmetic swap:
    int p = 50, q = 75;
    xil_printf("Before Arithmetic Swap: p = %d, q = %d\r\n", p, q);
    swap_arithmetic(&p, &q);
    xil_printf("After Arithmetic Swap: p = %d, q = %d\r\n", p, q);

    //Continuous Display Loop
    Xil_printf("Entering continuous display loop. Press Ctrl+C to exit.\r\n");
    while (1) {
        XGpio_DiscreteWrite(&GpioLed, LED_CHANNEL, x);
        sleep(1);
        XGpio_DiscreteWrite(&GpioLed, LED_CHANNEL, y);
        sleep(1);
    }

    cleanup_platform();
    return 0;
}

void swap_with_pointers(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swap_no_temp(int *a, int *b) {
    if (*a != *b) { // Avoid zeroing out when both are same
        *a = *a ^ *b; //Step 1: a becomes (a XOR b)
        *b = *a ^ *b; //Step 2: b becomes (a XOR b) XOR b = original a
        *a = *a ^ *b; //Step 3: a becomes (a XOR b) XOR a = original b
    }
}

void swap_arithmetic(int *a, int *b) {
    if (*a != *b) { // Avoid overflow when both are same
        *a = *a + *b; //Step 1: a becomes (a + b)
        *b = *a - *b; //Step 2: b becomes (a + b) - b = original a
        *a = *a - *b; //Step 3: a becomes (a + b) - a = original b
    }
}
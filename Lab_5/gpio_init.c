#include "gpio_init.h" // Include the corresponding header file to get the variable declarations.

// Define the global GPIO driver instances that were declared with 'extern' in the header file.
// Memory is allocated for these structures here.
XGpio SEG7_HEX_OUT;
XGpio SEG7_SEL_OUT;
XGpio P_BTN_LEFT;
XGpio P_BTN_RIGHT;
XGpio LED_OUT;
XGpio SLIDE_SWITCHES;

/**
 * @brief Initializes all GPIO peripherals used in the project.
 * @return XST_SUCCESS if all initializations succeed,
 * XST_FAILURE if any initialization fails.
 */
XStatus initGpio(void)
{
    XStatus status; // Variable to hold the return status of function calls for error checking.

    // Initialize the GPIO for the 7-segment display's segment data lines.
    // The second argument, 0, is the unique Device ID assigned in the hardware design (e.g., Vivado).
    status = XGpio_Initialize(&SEG7_HEX_OUT, 0);
    if (status != XST_SUCCESS) return XST_FAILURE; // If initialization fails, return immediately.

    // Initialize the GPIO for the 7-segment display's digit selection lines. Device ID is 1.
    status = XGpio_Initialize(&SEG7_SEL_OUT, 1);
    if (status != XST_SUCCESS) return XST_FAILURE;

    // Initialize the GPIO for the LEDs. Device ID is 2.
    status = XGpio_Initialize(&LED_OUT, 2);
    if (status != XST_SUCCESS) return XST_FAILURE;

    // Initialize the GPIO for the left push-button. Device ID is 4.
    status = XGpio_Initialize(&P_BTN_LEFT, 4);
    if (status != XST_SUCCESS) return XST_FAILURE;

    // Initialize the GPIO for the right push-button. Device ID is 5.
    status = XGpio_Initialize(&P_BTN_RIGHT, 5);
    if (status != XST_SUCCESS) return XST_FAILURE;

    // Initialize the GPIO for the slide switches. Device ID is 7.
    status = XGpio_Initialize(&SLIDE_SWITCHES, 7);
    if (status != XST_SUCCESS) return XST_FAILURE;

    // If all initializations were successful, return success.
    return XST_SUCCESS;
}

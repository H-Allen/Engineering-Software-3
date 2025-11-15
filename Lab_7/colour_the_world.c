#include <stdio.h>
#include "platform.h"
#include "xil_types.h"
#include "gpio_init.h"

u32 XGpio_DiscreteRead(XGpio *InstancePtr, unsigned Channel);
u8 i = 0;

int main() 
{
    init_platform();
    
    int status;

    status = initGpio();
    if (status != XST_SUCCESS) {
        // Print error message if GPIO initialization fails
        printf("GPIO Initialization Failed\n");
        cleanup_platform();
        return 0;
    }
    else
    {
        print("GPIO Initialization Successful\n");
    }

    //Declare and define requried variables
    // slideSwitchIn: Stores the 16 bit value read from the physical slide switches
    u16 slideSwitchIn. = 0x000;
    // region: stores the 9 bit value to control which VGA region is active
    u16 region = 0x000;

    // Main program loop - runs indefinitely
    while(1)
    {
        slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1); // Read the value from the slide switches
        XGpio_DiscreteWrite(&VGA_COLOUR, 1, slideSwitchIn); // Write the value to the VGA colour output
        u16 region_selector = slideSwitchIn >> 12; // Get the top 4 bits to select the region

        switch (region_selector)
        {
            case 0:
                region = 0b000000001; // Region 0
                break;
            case 1:
                region = 0b000000010; // Region 1
                break;
            case 2:
                region = 0b000000100; // Region 2
                break;
            case 3:
                region = 0b000001000; // Region 3
                break;
            case 4:
                region = 0b000010000; // Region 4
                break;
            case 5:
                region = 0b000100000; // Region 5
                break;
            case 6:
                region = 0b001000000; // Region 6
                break;
            case 7:
                region = 0b010000000; // Region 7
                break;
            case 8: 
                region = 0b100000000; // Region 8
                break;
            default:
                //If region_selector is 10 or higher the region variable remains unchanged from its previous value
                break;
        }

        XGpio_DiscreteWrite(&VGA_REGION, 1, region); // Write the region value to the VGA region output
    }

    cleanup_platform();
    return 0;
}
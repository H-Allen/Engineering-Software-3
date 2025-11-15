#include <stdio.h>
#include "platform.h"
#include "xil_types.h"
#include "gpio_init.h"
#include "adv_colour.h"
#include "limits.h"

//Global Variables
u16 i = 0;           //Index for REGION_PATH array (tracks which region to move to next)
u16 region = 0;      //Holds the one-hot encoded value of the currently active region
u16 colour = 0xF00;  //Fixed colour value in hexadecimal 
volatile u8 j = 0;   //Counter used inside the timer interrupt to regulate movement speed
                     //Declared volatile as it is modified in an ISR (avoids compiler optimisation issues)

/*
    REGION_PATH: Defines the order of regions to follow on the CGA 3*3 grid.
    The VGA display is divided into 9 regions as follows:
        0 | 1 | 2
       -----------
        3 | 4 | 5
       -----------
        6 | 7 | 8
    The array values represent region indicied in th eorder the sjhoudl be activated
    In this version the chose sequence is designed to draw a G shape on the 3*3 grid
*/          

static const u8 REGION_PATH[9] = {2, 1, 0, 7, 6, 5, 4, 3, 8};

int main() {
    init_platform();        //Initialise the hardware platform

    //Step 1: Initislise GPIOs
    int status = initGpio();
    if (status != XST_SUCCESS) {
        print("GPIO initialisation failed!\n");
        cleanup_platform();
        return 0;
    }

    //Step 2: Set up the interrupt system (connects the timer interrupt to hwTimerISR)
    status = setUpInterruptSystem();
    if (status != XST_SUCCESS) {
        print("Interrupt system setup failed!\n");
        cleanup_platform();
        return 0;
    }

    //Main loop
    while (1) {
        //check if the "move" flag has been set by the timer ISR
        if (move) {
            move = FALSE //Reset the flag so movement only happens once per interrupt cycle

            //Get the next region index from REGION_PATH and convert it to one-hot
            region = (u16) (1u << REGION_PATH[i]);

            //Increment the index to move to the next step in the path
            i++
            //Wrap around when reaching the end of REGION_PATH
            if (i >= (sizeof(REGION_PATH)/ sizeof(REGION_PATH[0]))) i = 0;
        }
        //Write the fixed colour value to VGA_COLOUR (always red in this example)
        XGpio_DiscreteWrite(&VGA_COLOUR, 1, colour);

        //Write the current region mask to VGA_REGION to activate the selected region
        XGpio_DiscreteWrite(&VGA_REGION, 1, region);
    }

    cleanup_platform();
    return 0;
}

/*
    hwTimerISR: Hardware Timer Interrupt Service Routine
    This function is automatically called each time the timer generates and interrupt.

    Purpose:
    - count timer "ticks" using the variable j.
    - Only after j reaches a threshold (here, 10) do we set the move flag to ture
    - this effectively slows down the region movement (otherwise it would be too fast)
*/

void hwTimerISR(void *CallbackRef)
{
    if (j >= 10)
    {
        j = 0;
        move = TRUE;
    }
    else
    {
        j++;
    }
}
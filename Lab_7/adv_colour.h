#ifndef ADV_COLOUR_H_
#define ADV_COLOUR_H_

#include "xil_types.h"   // Xilinx fixed-width types (e.g., u8, u16, u32)

/*
 * 'move' is a global flag that the timer ISR sets when it is time
 * to advance the animation (e.g., move to the next VGA region).
 *
 * - 'extern' means the storage is defined in a .c file (adv_colour.c), not here.
 * - 'volatile' prevents the compiler from optimizing reads/writes away,
 *   because 'move' can change asynchronously inside an interrupt.
 *
 * Typical usage:
 *   In the main loop:
 *      if (move) { move = 0; ...advance state... }
 *   In the ISR:
 *      move = 1;   // signal the main loop to step
 */
extern volatile u8 move;

/*
 * setUpInterruptSystem
 * --------------------
 * Initialize and start the interrupt controller, connect hwTimerISR as the
 * handler for the hardware timer interrupt, and enable CPU exceptions.
 *
 * Return value:
 *   XST_SUCCESS (0) on success, XST_FAILURE (non-zero) on failure.
 *
 * The function is typically implemented in a separate source file that
 * configures XIntc/XScuGic and CPU exception handling (e.g., xinterrupt*.c).
 */
int setUpInterruptSystem(void);

/*
 * hwTimerISR
 * ----------
 * Hardware Timer Interrupt Service Routine.
 * This routine is invoked whenever the timer fires. It should execute quickly:
 *   - update a small counter or flag (e.g., 'move')
 *   - avoid long computations / blocking operations
 *
 * 'CallbackRef' can be used to pass a device/context pointer if needed.
 */
void hwTimerISR(void *CallbackRef);

#endif /* ADV_COLOUR_H_ */

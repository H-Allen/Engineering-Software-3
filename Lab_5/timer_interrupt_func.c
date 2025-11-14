#include "seg7_display.h" // Include header to get the prototype for displayDigit().

/**
 * @brief The Interrupt Service Routine (ISR) for the hardware timer.
 * @param CallbackRef A reference passed during interrupt setup, not used here.
 *
 * This function is automatically called by the processor whenever the timer interrupt occurs.
 * It should be as short and fast as possible.
 */
void hwTimerISR(void *CallbackRef)
{
	// Each time the timer interrupt fires, call the displayDigit() function.
	// This refreshes one digit on the 7-segment display.
	displayDigit();
}

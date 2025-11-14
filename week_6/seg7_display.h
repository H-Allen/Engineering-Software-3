#ifndef __SEG7_DISPLAY_H_ // Include Guard.
#define __SEG7_DISPLAY_H_

#include "xgpio.h" // Include GPIO driver header.

// --- MACRO DEFINITIONS ---

// Define the segment patterns for a Common Anode 7-segment display.
// A '0' bit turns a segment ON, and a '1' bit turns it OFF.
// The bits correspond to segments (dp, g, f, e, d, c, b, a).
#define DIGIT_BLANK     0xFF // All segments OFF.
#define DIGIT_ZERO      0xC0 // Pattern for '0'.
#define DIGIT_ONE       0xF9 // Pattern for '1'.
#define DIGIT_TWO       0xA4 // Pattern for '2'.
#define DIGIT_THREE     0xB0 // Pattern for '3'.
#define DIGIT_FOUR      0x99 // Pattern for '4'.
#define DIGIT_FIVE      0x92 // Pattern for '5'.
#define DIGIT_SIX       0x82 // Pattern for '6'.
#define DIGIT_SEVEN     0xF8 // Pattern for '7'.
#define DIGIT_EIGHT     0x80 // Pattern for '8'.
#define DIGIT_NINE      0x90 // Pattern for '9'.
#define DIGIT_DASH      0xBF // Pattern for '-'.

// Define logical numbers for the special characters for easier use in code.
#define NUMBER_BLANK    10
#define NUMBER_DASH     11

// Define the digit selection patterns. This is for an active-low common anode display.
// A '0' selects (enables) the corresponding digit.
#define EN_FIRST_SEG    0b0111 // Enable digit 1 (left-most).
#define EN_SECOND_SEG   0b1011 // Enable digit 2.
#define EN_THIRD_SEG    0b1101 // Enable digit 3.
#define EN_FOURTH_SEG   0b1110 // Enable digit 4 (right-most).

// --- FUNCTION PROTOTYPES ---

// Declares the function to set up the interrupt system (implementation likely in xinterruptES3.c).
int setUpInterruptSystem(void);

// Declares the Interrupt Service Routine (ISR) for the hardware timer.
void hwTimerISR(void *CallbackRef);

// Declares the function to display a 16-bit unsigned number.
void displayNumber(u16 number);

// Declares the function to split a number into its individual digits.
void calculateDigits(u16 number);

// Declares the function that drives the GPIOs to show a single digit.
void displayDigit(void);

#endif // End of include guard.

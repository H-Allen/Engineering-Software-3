#include "adv_colour.h"  // brings in the extern declaration and type aliases

/*
 * Define the storage for the global 'move' flag.
 *
 * Why 'volatile'?
 *   The flag is written by an ISR (asynchronous to main) and read in the main loop.
 *   'volatile' guarantees each read observes the most recent write and the compiler
 *   does not cache the value in a register across loop iterations.
 *
 * Typical lifecycle:
 *   - ISR:   move = 1;          // signal "step the animation"
 *   - main:  if (move) { move = 0; ...update state... }
 */
volatile u8 move = 0;

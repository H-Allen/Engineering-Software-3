#include "xil_types.h"  // Added for integer type definitions
#include "limits.h"     // Added for CHAR_BIT definition

u16 rotateLeft(u16 value, u8 shift)
{
    return (value << shift) | (value >> (sizeof(value) * CHAR_BIT - shift));
}

u16 rotateRight(u16 value, u8 shift)
{
    return (value >> shift) | (value << (sizeof(value) * CHAR_BIT - shift));
}
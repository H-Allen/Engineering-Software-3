#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

s32 adder (s32 num1, s32 num2);
s32 subtractor (s32 minuend, s32 subtrahend);
s32 multiplier (s32 multiplicand, s32 multiplier);
s32 divider (s32 dividend, s32 divisor);
u32 modulator(u32 dividend, u32 divisor);
void xil_printf(const char *ctrl1, ...);

int main()
{
    init_platform();

    s32 add_results;
    s32 sub_results;
    s32 mul_results;
    s32 div_results;
    s32 mod_results;

    add_results = adder(15, 10);
    sub_results = subtractor(15, 10);
    mul_results = multiplier(15, 10);
    div_results = divider(15, 10);
    mod_results = modulator(15, 10);

    xil_printf("Addition Result: %d\n", add_results);
    xil_printf("Subtraction Result: %d\n", sub_results);
    xil_printf("Multiplication Result: %d\n", mul_results);
    xil_printf("Division Result: %d\n", div_results);
    xil_printf("Modulus Result: %d\n", mod_results);

    cleanup_platform();
    return 0;
}

s32 adder (s32 num1, s32 num2) {
    return num1 + num2;
}

s32 subtractor (s32 minuend, s32 subtrahend) {
    return minuend - subtrahend;
}

s32 multiplier (s32 multiplicand, s32 multiplier) {
    return multiplicand * multiplier;
}

s32 divider (s32 dividend, s32 divisor) {
    return dividend / divisor;
}

u32 modulator(u32 dividend, u32 divisor) {
    return dividend % divisor;
}
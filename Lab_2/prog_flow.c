#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_types.h"
#include "xuart_receiver.c"

s32 adder(s32 augend, s32 addend);
s32 subtractor(s32 minuend, s32 subtrahend);
s32 multiplicator(s32 multiplicand, s32 multiplier);
s32 divider(s32 dividend, s32 divisor);
u32 modulator(u32 dividend, u32 divisor);
void xil_printf(const char *ctrl1,...);


int main()
{
    init_platform();

    char arith_op_type;
    while(1)
    {
    	xil_printf("Enter an operation type and press enter: \n\r");
    	arith_op_type = uartReceive();

        switch(arith_op_type) {
            case '0':
                s32 add_result = adder(20, 23);
                xil_printf("Addition result: %d \n\r", add_result);
                break;
            case '1':
                s32 sub_result = subtractor(1, 19);
                xil_printf("Subtraction result: %d \n\r", sub_result);
                break;
            case '2':
                s32 mul_result = multiplicator(-2, 8);
                xil_printf("Multiplication result: %d \n\r", mul_result);
                break;
            case '3':
                s32 div_result = divider(20, 10);
                xil_printf("Division result: %d \n\r", div_result);
                break;
            case '4':
                s32 mod_result = modulator(5, 2);
                xil_printf("Modulo result: %d \n\r", mod_result);
                break;
            default:
                xil_printf("Error! The operation type (%c) is wrong!\n\r", arith_op_type);
                break;

        }
    }
    cleanup_platform();
    return 0;
}

s32 adder(s32 augend, s32 addend)
{
	s32 sum;
	sum = augend + addend;
	return sum;
}

s32 subtractor(s32 minuend, s32 subtrahend)
{
	s32 difference;
	difference = minuend - subtrahend;
	return difference;
}

s32 multiplicator(s32 multiplicand, s32 multiplier)
{
	s32 product = multiplicand * multiplier;
	return product;
}

s32 divider(s32 dividend, s32 divisor)
{
	s32 quotient = dividend / divisor;
	return quotient;
}

u32 modulator(u32 dividend, u32 divisor)
{
	u32 modulus = dividend % divisor;
	return modulus;
}

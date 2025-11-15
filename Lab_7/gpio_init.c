#include "gpio_init.h"

XGpio VGA_COLOUR;
XGpio VGA_REGION;
XGpio SLIDE_SWITCHES;

XStatus initGpio(void)
{
	XStatus status;
	status = XGpio_Initialize(&SLIDE_SWITCHES, 7);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	status = XGpio_Initialize(&VGA_COLOUR, 8);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	status = XGpio_Initialize(&VGA_REGION, 10);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

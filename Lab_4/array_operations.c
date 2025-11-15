#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "sleep.h"
#include <string.h>

//Hardware Configuration
#define GPIO_DEVICE_ID 2
#define MAX_SIDE 6

XGpio GpioLed;
void display_array(int *arr, int size);
void display_binary_on_leds(int value);
void bubble_sort_descending(int *arr, int size);
int find_max(int *arr, int size);

int main() {
    init_platform();

    int status;

    int array[MAX_SIZE] = {34, 12, 5, 67, 23, 89};
    status = XGpio_Initialize(&GpioLed, GPIO_DEVICE_ID);

    if (status != XST_SUCCESS) {
        xil_printf("GPIO Initialization Failed\r\n");
        return XST_FAILURE;
    }

    //configure all LEDs as outputs
    XGpio_SetDataDirection(&GpioLed, 1, 0x0); // Set as output

    xil_printf("===Array Operations With Pointers===\r\n");
    xil_printf("Note: Values are displayed on LEDs in binary format.\r\n");
    xil_printf("Each LED represents a bit of a number\r\n");

    xil_printf("Original Array:\r\n");
    display_array(array, MAX_SIZE);
    xil_printf("\r\n");

    //Function 1: Find Maximum Value
    xil_printf("Finding Maximum Value:\r\n");
    //call find mac function to locate largest value
    int max_value = find_max(array, MAX_SIZE);
    xil_printf("Maximum Value Found: %d\r\n", max_value);
    display_binary_on_leds(max_value); // Show binary representation on LEDs

    XGpio_DiscreteWrite(&GpioLed, 1, max_value);
    xil_printf("Maximum value displayed on LEDs.\r\n");
    sleep(3); // Pause to view result
    xil_printf("\r\n");

    //Function 2: Bubble Sort in Descending Order
    xil_printf("Sorting Array in Descending Order:\r\n");
    bubble_sort_descending(array, MAX_SIZE);
    xil_printf("Sorted Array:\r\n");
    display_array(array, MAX_SIZE);
    xil_printf("\r\n");

    xil_printf("Display each sorted value on LEDs:\r\n");
    for (int i = 0; i < MAX_SIZE; i++) {
        int current_value = *(array + i); //get element using pointer arithmetic
        xil_printf("Value %d: %d\r\n", i, current_value);
        display_binary_on_leds(current_value);

        //Write current_value to LEDs
        XGpio_DiscreteWrite(&GpioLed, 1, current_value);
        sleep(2); // Pause to view each value
    }

    xil_printf("Array operations completed.\r\n");

    //Continuously cycling through sorted values on LEDs...
    xil_printf("Cycling through sorted values on LEDs...\r\n");
    int index = 0;
    while (1) {
        int current_value = *(array + index);
        XGpio_DiscreteWrite(&GpioLed, 1, current_value);
        xil_printf("Displaying Value %d: %d\r\n", index, current_value);
        display_binary_on_leds(current_value);

        //Increment index and wrap around
        index = (index + 1) % MAX_SIZE;
        sleep(2); // Pause before next value
    }
    cleanup_platform();
    return 0;
}

void display_array(int *arr, int size) {
    xil_printf("Array Elements:\r\n");
    for (int i = 0; i < size; i++) {
        xil_printf("Element %d: %d\r\n", i, *(arr + i)); // Accessing via pointer arithmetic
    }
    xil_printf("\r\n");
}

void display_binary_on_leds(int value) {
    xil_printf("Binary Representation on LEDs: ");
    for (int i = 15; i >= 0; i--) {
        if (i == 7) xil_printf(" "); // add space between high and low byte
        xil_printf("%d", (value >> i) & 1); //Right shift and mask to get bit value 
    }
    xil_printf("(LEDs show lower bits) \r\n");
}

void bubble_sort_descending(int *arr, int size) {
    //Outer loop: n-1 passes though the array
    for (int i = 0; i < size - 1; i++) {
        //Inner loop: compare adjacent elements up to unsorted portion
        for (int j = 0; j < size - i - 1; j++) {
            //Compare current element with next using pointer arithmetic
            if (*(arr + j) < *(arr + j + 1)) { //If current < next, swap (for descending)
                // Three step swap using temp variable (couls also use functions from prev excersises)
                int temp = *(arr + j);
                *(arr + j) = *(arr + j + 1);
                *(arr + j + 1) = temp;
            }
        }
    }
}

int find_max(int *arr, int size) {
    int max_value = *arr; //Initialize max with first element
    //Travere remaining elements starting from index 1
    for (int i = 1; i < size; i++) {
        if (*(arr + i) > max_value) { //Compare current element with max
            max_value = *(arr + i); //Update max if current is larger
        }
    }
    return max_value;
}
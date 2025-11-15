#include <stdio.h>
#include <stdlib.h>

int main() {
    //Basic Pointer Operations
    char b = 5;
    char *address = &b;

    printf("Value of b: %d\n", b);
    printf("Address of b: %p\n", &b);
    printf("Value of address pointer: %p\n", address);

    *address = 7;

    printf("New value of b after modifying through pointer: %d\n", b);
    printf("Address stored in address pointer: %c\n", *address);

    //Multiple pointer operations:
    int x = 100;
    int y = 200;
    int *p1 = &x;
    int *p2 = &y;

    printf("Before swapping: x = %d, y = %d\n", x, y);
    printf("Addresses: p1 = %p, p2 = %p\n", p1, p2);
    
    // Pointer arithmetic
    int arr[5] = {10, 20, 30, 40, 50};
    int *ptr = arr;

    printf("Array elements using pointer arithmetic:\n");
    for (int i = 0; i < 5; i++) {
        printf("Element %d: %d\n", i, *(ptr + i));
    }
    
    return 0;
}
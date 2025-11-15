#include "xil_printf.h"   // Xilinx printf function for UART output
#include "platform.h"      // Platform initialization/cleanup functions
#include "xparameters.h"   // Hardware parameter definitions

#define TEST_SIZE 1000     // Test array size: 1000 elements

volatile unsigned int timer_ticks = 0;  // Loop iteration counter
int sum_basic(int *arr, int size);      // Basic summation (no optimization)
int sum_unrolled_2x(int *arr, int size); // 2x loop unrolling summation
int sum_unrolled_4x(int *arr, int size); // 4x loop unrolling summation
int sum_unrolled_8x(int *arr, int size); // 8x loop unrolling summation
void benchmark_sum();                   // Main benchmark execution function


int main() {
    // ========= Platform Initialization =========
    init_platform();   // Initialize hardware platform

    // ========= Display Program Title =========
    xil_printf(" LOOP UNROLLING OPTIMIZATION DEMO\r\n");

    // ========= Run Benchmark Test =========
    benchmark_sum();   // Execute complete benchmark suite

    // ========= Display Completion Message =========
    xil_printf("\r\n[INFO] Test completed. Reset to run again.\r\n");

    // ========= Platform Cleanup =========
    cleanup_platform();   // Clean up platform resources

    // Program completed successfully
    return 0;
}

int sum_basic(int *arr, int size) {
    int sum = 0;  // Initialize accumulator
    for (int i = 0; i < size; i++) {  // Standard for loop
        sum += arr[i];  // Add current element
        timer_ticks++;  // Increment counter to track iterations
    }
    return sum;  // Return total sum
}

int sum_unrolled_2x(int *arr, int size) {
    int sum = 0;  // Initialize accumulator
    int i;  // Loop variable declared outside for remainder handling

    // Process 2 elements per iteration
    // Main loop: processes 2 elements at once
    for (i = 0; i < size - 1; i += 2) {  // Increment by 2 each iteration
        sum += arr[i] + arr[i+1];  // Accumulate 2 elements at once
        timer_ticks++;  // Count as 1 iteration (processing 2 elements)
    }

    // Handle remainder
    // Remainder loop: processes leftover elements (if size is odd)
    for (; i < size; i++) {  // Continue from previous loop index
        sum += arr[i];  // Process final element
        timer_ticks++;
    }
    return sum;
}

int sum_unrolled_4x(int *arr, int size) {
    int sum = 0;
    int i;

    // Process 4 elements per iteration
    // Main loop: processes 4 elements at once
    for (i = 0; i < size - 3; i += 4) {   // Increment by 4, ensure at least 4 elements available
        sum += arr[i] + arr[i+1] + arr[i+2] + arr[i+3]; // Accumulate 4 elements
        timer_ticks++; // Count as 1 iteration (processing 4 elements)
    }

    // Handle remainder
    // Remainder loop: processes 0-3 leftover elements
    for (i; i < size; i++) {
        sum += arr[i];
        timer_ticks++;
    }

    return sum;
}

int sum_unrolled_8x(int *arr, int size) {
    int sum = 0;
    int i;

    // Process 8 elements per iteration
    // Main loop: processes 8 elements at once
    for (i = 0; i < size - 7; i += 8) {   // Increment by 8, ensure at least 8 elements available
        sum += arr[i] + arr[i+1] + arr[i+2] + arr[i+3] + arr[i+4] + arr[i+5] + arr[i+6] + arr[i+7]; // Accumulate 8 elements
        timer_ticks++; // Count as 1 iteration (processing 8 elements)
    }

    // Handle remainder
    // Remainder loop: processes 0-7 leftover elements
    for (i; i < size; i++) {
        sum += arr[i];
        timer_ticks++;
    }

    return sum;
}

void benchmark_sum() {
    // ========== Variable Declarations ==========
    static int test_array[TEST_SIZE];  // Static array to avoid stack overflow
    unsigned int basic_ops, unroll2_ops, unroll4_ops, unroll8_ops;  // Iteration counts
    int sum1, sum2, sum3, sum4;  // Computed sums from each method

    // ========== Initialise Test Data ==========
    // Initialise array with pattern: arr[i] = i % 100
    // Content: 0,1,2,...99, 0,1,2,...99 (repeats 10 times)
    // Expected sum: (0+1+...+99) * 10 = 4950 x 10 = 49,500
    for (int i = 0; i < TEST_SIZE; i++) {
        test_array[i] = i % 100;
    }

    // ========== Output Test Description ==========
    xil_printf(
        "UNDERSTANDING LOOP UNROLLING\r\n"
        "\r\nloop unrolling processes multiple elements per iteration\r\n"
        "to reduce loop overhead (i++, comparison, branch).\r\n"
        "\r\nTest: Array of %d elements (values 0-99)\r\n", TEST_SIZE);
    xil_printf("Methods: Basic, 2x, 4x, 8x unrolling\r\n");
    xil_printf("RUNNING BENCHMARK\r\n");

    // ========== Test 1: Basic Implementation ==========
    xil_printf("[1] BASIC... ");
    timer_ticks = 0;  // Reset counter
    sum1 = sum_basic(test_array, TEST_SIZE);  // Execute basic summation
    basic_ops = timer_ticks;  // Save iteration count
    xil_printf("Done (%u iterations)\r\n", basic_ops);  // Display result

    // ========== Test 2: 2x Unrolling ==========
    xil_printf("[2/4] 2x UNROLLED... ");
    timer_ticks = 0;  // Reset counter
    sum2 = sum_unrolled_2x(test_array, TEST_SIZE);  // Execute 2x unrolled summation
    unroll2_ops = timer_ticks;  // Save iteration count
    xil_printf("Done (%u iterations)\r\n", unroll2_ops);

    // ========== Test 3: 4x Unrolling ==========
    xil_printf("[3/4] 4x UNROLLED... ");
    timer_ticks = 0;  // Reset counter
    sum3 = sum_unrolled_4x(test_array, TEST_SIZE);  // Execute 4x unrolled summation
    unroll4_ops = timer_ticks;  // Save iteration count
    xil_printf("Done (%u iterations)\r\n", unroll4_ops);

    // ========== Test 4: 8x Unrolling ==========
    xil_printf("[4/4] 8x UNROLLED... ");
    timer_ticks = 0;  // Reset counter
    sum4 = sum_unrolled_8x(test_array, TEST_SIZE);  // Execute 8x unrolled summation
    unroll8_ops = timer_ticks;  // Save iteration count
    xil_printf("Done (%u iterations)\r\n", unroll8_ops);

    // ========== Results Output ==========
    xil_printf("RESULTS\r\n");

    // ========== Correctness Verification ==========
    /* Verify that all methods produce identical results
       This is critical: optimization should never change program correctness */
    xil_printf("\r\nCorrectness Check --\r\n");
    xil_printf("Basic: %d, 2x: %d, 4x: %d, 8x: %d\r\n", sum1, sum2, sum3, sum4);

    // Compare all results for equality
    if (sum1 == sum2 && sum2 == sum3 && sum3 == sum4) {
        xil_printf("Status: [PASS] All methods identical\r\n"); // Pass: all results match
    } else {
        xil_printf("Status: [FAIL] Mismatch detected!\r\n"); // Fail: results differ
    }

    // ========== Iteration Count Analysis ==========
    /* Display iteration count for each method and percentage relative to baseline */
    xil_printf("\r\n-- Iteration Count ---\r\n");
    xil_printf("Method              Iterations  %% of Basic\r\n");
    xil_printf("-------------------- ----------- ---------\r\n");
    xil_printf("Basic               %4u    100%%\r\n", basic_ops); // Baseline: 100%

    // Calculate percentage: (unrolled_iterations * 100) / baseline_iterations
    xil_printf("2X Unroll           %4u    %5u%%\r\n", unroll2_ops, (unroll2_ops * 100) / basic_ops); // Expected: ~50%
    xil_printf("4X Unroll           %4u    %5u%%\r\n", unroll4_ops, (unroll4_ops * 100) / basic_ops); // Expected: ~25%
    xil_printf("8X Unroll           %4u    %5u%%\r\n", unroll8_ops, (unroll8_ops * 100) / basic_ops); // Expected: ~12%

    // ========== Speedup Factor Calculation ==========
    /* Calculate speedup factor for each unrolling method
       Formula: speedup = baseline_iterations / unrolled_iterations
       Multiply by 100 to preserve 2 decimal places in integer arithmetic */
    unsigned int reduction_2x = (basic_ops * 100) / unroll2_ops; // Expected: 200 (2.00X)
    unsigned int reduction_4x = (basic_ops * 100) / unroll4_ops; // Expected: 400 (4.00X)
    unsigned int reduction_8x = (basic_ops * 100) / unroll8_ops; // Expected: 800 (8.00X)

    xil_printf("\r\n-- Speedup --\r\n");
    xil_printf("2x: %u.%02ux, 4x: %u.%02ux, 8x: %u.%02ux\r\n",
        reduction_2x/100, reduction_2x%100,  // 2x speedup factor
        reduction_4x/100, reduction_4x%100,  // 4x speedup factor
        reduction_8x/100, reduction_8x%100); // 8x speedup factor


    xil_printf(" DEMONSTRATION COMPLETE\r\n");
}
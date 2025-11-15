#include "xil_printf.h"     // Xilinx printf function for UART output
#include "platform.h"        // Platform initialization/cleanup functions
#include "xparameters.h"     // Hardware parameter definitions

// ========= Macro Definitions =========
#define ARRAY_SIZE 500        // Test array size: 500 elements

// ========= Global Variables =========
static volatile unsigned int cycle_count = 0;  // Cycle counter

void transform_basic(int *input, int *output, int size);
// Basic sequential transformation (no pipelining)
// Baseline implementation for correctness comparison
// Each iteration: Load -> Compute -> Store (no overlap)

void transform_pipelined(int *input, int *output, int size);
// Simple 3-stage software pipeline
// Overlaps Load, Compute, and Store operations
// Demonstrates basic prologue-kernel-epilogue structure

void transform_advanced_pipeline(int *input, int *output, int size);
// Advanced 4-stage pipeline
// Splits computation into two separate stages
// More complex pipeline with better potential overlap

void complex_transform_basic(int *input, int *output, int size);
// Complex sequential computation baseline
// Multi-step operation: Square -> Add -> Divide -> Mask
// Sequential execution for comparison

void complex_transform_pipelined(int *input, int *output, int size);
// Complex 5-stage pipeline
// Overlaps all computation steps across iterations
// Demonstrates advanced pipelining for multi-step operations

void benchmark_pipelining();
// Main benchmark function
// Executes all transformations and verifies correctness
// Displays results via UART console

int main()
{
    init_platform();

    xil_printf("\r\n=== Software Pipelining Demo === \r\n");
    xil_printf("Starting automated benchmark... \r\n\r\n");

    benchmark_pipelining();

    xil_printf("\r\nBenchmark Completed\r\n");

    cleanup_platform();
    return 0;
}

void transform_basic(int *input, int *output, int size) {
    for (int i = 0; i < size; i++) {
        int temp = input[i];          // Load: Read from input array
        int result = temp * 2 + 5;    // Compute: Multiply by 2 and add 5
        output[i] = result;           // Store: Write to output array
    }
}

void transform_pipelined(int *input, int *output, int size) {
    if (size == 0) return;           // Handle empty array

    // Prologue: Fill pipeline
    // Initialize first iteration to start pipeline
    int temp0 = input[0];            // Load first element
    int result0 = temp0 * 2 + 5;     // Compute first result

    // Kernel: Steady state with overlapping operations
    // All pipeline stages active simultaneously
    for (int i = 1; i < size; i++) {
        int temp1 = input[i];         // Load next (iteration i)
        int result1 = temp1 * 2 + 5;  // Compute next (iteration i)
        output[i-1] = result0;        // Store previous (iteration i-1)

        // Advance pipeline
        // Move data forward through pipeline stages
        temp0 = temp1;
        result0 = result1;
    }

    // Epilogue: Drain pipeline
    // Complete final store operation
    output[size-1] = result0;
}

void transform_advanced_pipeline(int *input, int *output, int size) {
    if (size <= 1) {                // Handle edge cases
        if (size == 1) output[0] = input[0] * 2 + 5;
        return;
    }

    // Prologue: Initialize all stages
    int stage0_data = input[0];     // Load first
    int stage1_data = stage0_data * 2;  // Compute1 first
    int stage2_data = stage1_data + 5;  // Compute2 first

    // Kernel: All stages active
    for (int i = 1; i < size; i++) {
        int new_load = input[i];     // Load iteration i
        int new_compute1 = new_load * 2;  // Compute1 iteration i
        int new_compute2 = stage1_data + 5;  // Compute2 iteration i-1
        output[i-1] = stage2_data;   // Store iteration i-2

        // Advance pipeline
        // Each stage receives data from previous stage
        stage0_data = new_load;
        stage1_data = new_compute1;
        stage2_data = new_compute2;
    }

    // Epilogue
    // Store final result
    output[size-1] = stage2_data;
}

void complex_transform_basic(int *input, int *output, int size) {
    for (int i = 0; i < size; i++) {
        int temp = input[i];         // Load input
        int step1 = temp * temp;     // Square
        int step2 = step1 + temp;   // Add original
        int step3 = step2 >> 2;     // Divide by 4
        output[i] = step3 & 0xFF;   // Mask to byte and store
    }
}

void complex_transform_pipelined(int *input, int *output, int size) {
    if (size == 0) return;     // Handle empty array

    // Prologue
    // Fill all pipeline stages
    int stage0 = input[0];      // Load first
    int stage1 = stage0 * stage0;  // Square first
    int stage2 = stage1 + stage0;  // Add first
    int stage3 = stage2 >> 2;      // Divide first

    // Kernel: Five iterations overlapping
    // Each stage processes different iteration
    for (int i = 1; i < size; i++) {
        int new_load = input[i];      // Load iteration i
        int new_square = new_load * new_load;  // Square iteration i
        int new_add = stage1 + stage0;   // Add iteration i-1
        int new_div = stage2 >> 2;      // Divide iteration i-2
        output[i-1] = stage3 & 0xFF;   // Store iteration i-3

        // Advance all stages
        // Data flows through all stages
        stage0 = new_load;
        stage1 = new_square;
        stage2 = new_add;
        stage3 = new_div;
    }

    // Epilogue
    // Complete final store
    output[size-1] = stage3 & 0xFF;
}

void benchmark_pipelining() {
// Static arrays avoid stack overflow
// Total memory: 5 arrays x 500 elements x 4 bytes = 10 KB
static int input_array[ARRAY_SIZE];
static int output_basic[ARRAY_SIZE];
static int output_pipeline[ARRAY_SIZE];
static int output_advanced[ARRAY_SIZE];
static int output_complex[ARRAY_SIZE];

// Initialize input: pattern (i * 7 + 3) % 100
// Provides varied test data (values 0-99)
for (int i = 0; i < ARRAY_SIZE; i++) {
    input_array[i] = (i * 7 + 3) % 100;
}

xil_printf("*** Software Pipelining Demo ===\r\n");
xil_printf("Array size: %d elements\r\n", ARRAY_SIZE);
xil_printf("\r\nRunning transformations...\r\n");

// Execute all transformations
transform_basic(input_array, output_basic, ARRAY_SIZE);
xil_printf("Basic Transform: Complete\r\n"); // Sequential baseline
transform_pipelined(input_array, output_pipeline, ARRAY_SIZE);
xil_printf("Simple Pipeline: Complete\r\n"); // 3-stage pipeline
transform_advanced_pipeline(input_array, output_advanced, ARRAY_SIZE);
xil_printf("Advanced Pipeline: Complete\r\n"); // 4-stage pipeline
complex_transform_pipelined(input_array, output_complex, ARRAY_SIZE);
xil_printf("Complex Pipeline: Complete\r\n"); // 5-stage pipeline

// Verify correctness
xil_printf("\r\n=== Correctness Verification ===\r\n");
xil_printf("Checking first 5 elements:\r\n");

int errors = 0; // Error counter
for (int i = 0; i < 5 && i < ARRAY_SIZE; i++) {
// Display values for manual inspection
    xil_printf(" [%d] Input=%d, Basic=%d, Pipeline=%d, Advanced=%d\r\n",
    input_array[i], output_basic[i], output_pipeline[i], output_advanced[i]);

// Check if pipelined result matches basic result
    if (output_basic[i] != output_pipeline[i]) {
        errors++; // Count mismatches
    }
}

// Display verification result
if (errors == 0) {
    xil_printf("\r\nPipeline results MATCH basic implementation\r\n");
} else {
    xil_printf("\r\nPipeline results DIFFer from basic implementation\r\n");
}

// Remind user about timing limitations
xil_printf("\r\nNote: Install timer hardware for cycle-accurate timing\r\n");
}
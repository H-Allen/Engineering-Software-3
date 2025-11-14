#include <stdio.h>
#include "xil_types.h"
#include "seg7_display.h"
#include "gpio_init.h"

// --- GLOBAL VARIABLES ---
u8 digitDisplayed = FALSE; // A flag for synchronization between the main loop and the ISR.
                           // The ISR sets it to TRUE after displaying a digit.
u8 digits[4];              // An array to store the individual digits of the number to be displayed.
u8 numOfDigits;            // The number of active digits in the number (e.g., 3 for the number 123).
u8 digitToDisplay;         // The actual digit value (0-9) to be shown in the current cycle.
u8 digitNumber;            // The position (1-4) of the digit to be displayed in the current cycle.

/**
 * @brief Displays a number up to 9999 on the 4-digit display.
 * @param number The 16-bit unsigned integer to display.
 */
void displayNumber(u16 number)
{
    u8 count;

    if (number <= 9999) // Check if the number is within the displayable range.
    {
        calculateDigits(number); // Split the number into individual digits and store them in the 'digits' array.
        count = 4; // Start displaying from the right-most digit (digit 4).

        // Loop to display each valid digit of the number.
        while (count > 4 - numOfDigits)
        {
            digitToDisplay = digits[count-1]; // Set the digit value to be displayed.
            digitNumber = count;              // Set the digit position to be enabled.
            count--;

            // This is a busy-wait loop. It waits for the ISR to finish displaying the previous digit.
            // The ISR will set digitDisplayed to TRUE.
            while (digitDisplayed == FALSE);

            // Reset the flag, indicating that we are now preparing the next digit.
            digitDisplayed = FALSE;
        }
    }
    else // If the number is out of range.
    {
        // Display "----" to indicate an overflow or error.
        count = 1;
        while (count < 5)
        {
            digitToDisplay = NUMBER_DASH; // Set the character to a dash.
            digitNumber = count;          // Select each digit position in turn.
            count++;
            while (digitDisplayed == FALSE); // Wait for the ISR.
            digitDisplayed = FALSE;          // Reset the flag.
        }
    }
}

/**
 * @brief Splits a u16 number into four separate digits.
 * @param number The number to split (0-9999).
 */
void calculateDigits(u16 number)
{
    u8 fourthDigit, thirdDigit, secondDigit, firstDigit;

    // Use integer division and modulo arithmetic to extract each digit.
    if (number > 999) // 4-digit number.
    {
        numOfDigits = 4;
        fourthDigit  = number % 10;
        thirdDigit   = (number / 10) % 10;
        secondDigit  = (number / 100) % 10;
        firstDigit   = number / 1000;
    }
    else if (number > 99) // 3-digit number.
    {
        numOfDigits = 3;
        fourthDigit  = number % 10;
        thirdDigit   = (number / 10) % 10;
        secondDigit  = number / 100;
        firstDigit   = 0; // The most significant digit is not used.
    }
    else if (number > 9) // 2-digit number.
    {
        numOfDigits = 2;
        fourthDigit  = number % 10;
        thirdDigit   = number / 10;
        secondDigit  = 0;
        firstDigit   = 0;
    }
    else // 1-digit number.
    {
        numOfDigits = 1;
        fourthDigit  = number % 10;
        thirdDigit   = 0;
        secondDigit  = 0;
        firstDigit   = 0;
    }

    // Store the calculated digits in the global array.
    digits[0] = firstDigit;
    digits[1] = secondDigit;
    digits[2] = thirdDigit;
    digits[3] = fourthDigit;
}

/**
 * @brief Drives the GPIOs to display a single digit at its specified position.
 * This function is designed to be called rapidly by a timer interrupt.
 */
void displayDigit(void)
{
    // Use a switch statement to write the correct segment pattern to the segment GPIO port
    // based on the global 'digitToDisplay' variable.
    switch (digitToDisplay)
    {
        case NUMBER_BLANK:
            XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_BLANK);
            break;
        case 0: XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_ZERO); break;
        case 1: XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_ONE); break;
        case 2: XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_TWO); break;
        case 3: XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_THREE); break;
        case 4: XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_FOUR); break;
        case 5: XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_FIVE); break;
        case 6: XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_SIX); break;
        case 7: XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_SEVEN); break;
        case 8: XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_EIGHT); break;
        case 9: XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_NINE); break;
        case NUMBER_DASH:
            XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_DASH);
            break;
        default:
            break;
    }

    // Write the correct selection pattern to the digit selector GPIO port
    // based on the global 'digitNumber' variable to enable the correct digit.
    if (digitNumber == 1)
        XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_FIRST_SEG);
    else if (digitNumber == 2)
        XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_SECOND_SEG);
    else if (digitNumber == 3)
        XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_THIRD_SEG);
    else if (digitNumber == 4)
        XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_FOURTH_SEG);

    // Set the flag to TRUE to signal to the main loop (in displayNumber)
    // that the display has been updated and it can prepare the next digit.
    digitDisplayed = TRUE;
}

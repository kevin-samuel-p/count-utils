/**
 *      *Increasing Digits
 *      Given a number with its digits in non-decreasing order, find the next such number
 * 
 *      ___Working__Procedure___
 *      ->  The executable is called with a single parameter, namely, the number.
 *      ->  The inputted number is first evaluated to check whether it fits the input rules.
 *      ->  After ensuring the inputted number has digits in non-decreasing order, it is sent through nextNumber().
 *      ->  nextNumber() returns the stringified version of the next such number with digits in non-decreasing order.
 * 
 *      ___Input__Requirements___
 *      ->  The inputted number must be a positive integer.
 *      ->  The input string must have all digits in non-decreasing order.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>


// Input verification function
bool hasIncreasingDigits(char *number) {
    char prev = '\0';
    for (int i = 0; number[i] != '\0'; i++) {
        if (prev > number[i]) return false;
        prev = number[i];
    }
    return true;
}


// Backtracking lexicographic generator
char *nextNumber(char *number) {

    // Check whether input number's digits are in non-decreasing order
    if (!hasIncreasingDigits(number)) {
        printf("Bad Input - Digits are not in non-decreasing order");
        return NULL;
    }

    int n = strlen(number);
    char *buffer = (char *)malloc((n + 2) * sizeof(char));
    strcpy(buffer, number);

    char lastDigit = buffer[n - 1];
    int depth = n - 1;

    while (depth >= 0 && depth < n) {
        if (lastDigit == '9') {
            lastDigit = buffer[--depth];
        } else {
            buffer[depth++] = lastDigit + 1;
        }
    }

    if (depth == -1) {
        for (int i = 0; i < n + 1; i++) {
            buffer[i] = '1';
        } 
        buffer[n + 1] = '\0';
    }

    return buffer;
}


int main(int argc, char *argv[]) {
    /**
     *      ___Command__Line__Syntax___
     *          .\increasing.exe <number>
     */
    if (argc < 2) {
        printf("Invalid Syntax: .\\increasing.exe <number>");
        return -1;
    }

    if (argv[1][0] == '-') {
        printf("Bad Input - Negative numbers are not allowed");
        return -1;
    }

    char *output;
    clock_t startTime, endTime;

    startTime = clock();

    int offset;                 // Variable for skipping leading zeroes
    int n = strlen(argv[1]);
    for (
        offset = 0; 
        argv[1][offset] == '0' && offset < n - 1; 
        offset++
    );

    // Validate input
    for (int i = 0; i + offset < n; i++) {
        if (!isdigit(argv[1][i + offset])) {
            printf("Bad Input - Invalid number");
            return -1;
        }
    }

    output = nextNumber(argv[1] + offset);
    if (!output) return -1;

    endTime = clock();

    if (argc > 2) {
        printf("WARNING: Extra arguments will be ignored...\n\n");
    }

    printf("%s\n", output);
    free(output);

    double timeUsed = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.4lf seconds", timeUsed);

    return 0;
}
/**
 *      Non-repeating numbers
 *      Given a number, find the next number with no repeating digits.
 * 
 *      ___Working__Procedure___ 
 *      ->  Input is sanitized to check for rogue input or invalid numbers.
 *      ->  After validating input, number is fed to nextNumber() function, which returns the next number.
 *      ->  nextNumber() calls backtrack() if given input is feasible.
 *      ->  backtrack() recursively constructs the next viable number.
 * 
 *      ___Input__Requirements___
 *      ->  The inputted number should be an integer in the range [1, 9876543210).
 *      ->  The inputted number should not have repeating digits.
 *      ->  User input should be sanitized to prevent non-numeric inputs.
 */

 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <errno.h>


bool isRepeating(long long);
long long backtrack(long long, int, long long, int);
long long nextNumber(long long);


//  Checks whether a given number has repeating/recurring digits or not.
bool isRepeating(long long number) {
    
    int mask = 0;   // Bitmask used as a set to keep track of digits      

    long long copy = number;
    while (copy > 0) {
        int rem = copy % 10;

        if ((mask & (1 << rem)) != 0) 
            return true;

        mask |= (1 << rem);
        copy /= 10;
    }

    return false;
}


//  Finds next number with non-repeating digits, given a number with non-repeating digits.
long long nextNumber(long long number) {

    if (number == 0) {
        printf("Bad Input: Input a positive number");
        return -1;
    }
    
    if (number >= 9876543210) {
        printf("Bad Input: Range's maximum limit exceeded");
        return -1;
    }

    if (isRepeating(number)) {
        printf("Bad Input: Input has repeating digits");
        return -1;
    }

    long long rev = 0;      // Digit stack
    int digitCount = 0;     // Stack length tracked using digit count

    long long copy = number + 1;
    while (copy > 0) {
        int rem = copy % 10;
        rev = (rev * 10) + rem;
        digitCount++;
        copy /= 10;
    }

    rev *= 10;      // Appending a zero to stack
    digitCount++;   // Just in case of underflow during lexicographic DFS

    return backtrack(0L, 0, rev, digitCount);
}


long long backtrack(long long number, int mask, long long stack, int stackLen) {
    if (stackLen == 0) return number;

    int currDigit = stack % 10;
    for (int offset = 0; offset + currDigit <= 9; offset++) {
        long long trial;
        if ((mask & (1 << (offset + currDigit))) == 0 && 
            ((trial = backtrack(
                number * 10 + offset + currDigit,  
                (currDigit + offset == mask) ? mask : mask | (1 << (offset + currDigit)),
                stack / 10,
                stackLen - 1
            )) != -1)
        ) { return trial; }
        stack = 0;
    }

    return -1;
}


int main(int argc, char *argv[]) {
    /**
     *      ___Command__Line__Syntax___
     *          .\norep.exe <number>
     */

    if (argc < 2) {
        printf("Invalid Syntax:\n.\\norep.exe <number>");
        return -1;
    }

    int n = strlen(argv[1]);
    long long input, output;
    char *endChar;
    clock_t startTime, endTime;
    errno = 0;

    startTime = clock();

    input = strtoll(argv[1], &endChar, 10);
    if (*endChar != '\0') {
        printf("Bad Input - Invalid number");
        return -1;
    }

    if (errno == ERANGE || input >= 9876543210) {
        printf("Bad Input - Number too large");
        return -1;
    }

    if (input < 0) {
        printf("Bad Input - Negative numbers not allowed");
        return -1;
    }

    output = nextNumber(input);
    if (output == -1) return -1;

    if (argc > 2) {
        printf("WARNING: Extra arguments will be ignored...\n\n");
    }

    printf("%lld\n", output);
    endTime = clock();

    double timeUsed = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.4lf seconds", timeUsed);

    return 0;
}
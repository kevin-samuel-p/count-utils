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
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>


bool isRepeating(long);
long backtrack(long, int, long, int);
long nextNumber(long);


//  Checks whether a given number has repeating/recurring digits or not.
bool isRepeating(long number) {
    
    int mask = 0;   // Bitmask used as a set to keep track of digits      

    long copy = number;
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
long nextNumber(long number) {

    if (number <= 0) {
        printf("Bad Input: Input a positive number\n");
        return -1;
    }
    
    if (number >= 9876543210) {
        printf("Bad Input: Range's maximum limit exceeded\n");
        return -1;
    }

    if (isRepeating(number)) {
        printf("Bad Input: Input has repeating digits\n");
        return -1;
    }

    long rev = 0;       // Digit stack
    int digitCount = 0; // Stack length tracked using digit count

    long copy = number + 1;
    while (copy > 0) {
        int rem = copy % 10;
        rev = (rev * 10) + rem;
        digitCount++;
        copy /= 10;
    }

    return backtrack(0L, 0, rev, digitCount);
}


long backtrack(long number, int mask, long stack, int stackLen) {
    if (stackLen == 0) return number;

    int currDigit = stack % 10;
    for (int offset = 0; offset + currDigit <= 9; offset++) {
        int trial;
        if ((mask & (1 << (offset + currDigit))) == 0 && 
            ((trial = backtrack(
                number * 10 + offset + currDigit,  
                mask | (1 << (offset + currDigit)),
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
        printf("Invalid Syntax:\n.\\norep.exe <number>\n");
        return -1;
    }

    int n = strlen(argv[1]);
    long num = 0L;
    clock_t startTime, endTime;

    startTime = clock();

    for (int i = 0; i < n; i++) {
        if (!isdigit(argv[1][i])) {
            printf("Bad Input - Invalid number\n");
            return -1;
        }
        
        if (num >= 9876543210) {
            printf("Bad Input - Number too large\n");
            return -1;
        }
        num = (num * 10) + (argv[1][i] - '0');
    }

    long nextNum = nextNumber(num);
    if (nextNum == -1) return -1;

    if (argc > 2) {
        printf("WARNING: Extra arguments will be ignored...\n\n");
    }

    printf("%ld -> %ld\n", num, nextNumber(num));
    endTime = clock();

    double timeUsed = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.4lf seconds", timeUsed);

    return 0;
}
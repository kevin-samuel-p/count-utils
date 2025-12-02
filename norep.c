/**
 *      Non-repeating numbers
 *      Given a number, find the next number with no repeating digits.
 * 
 *      ___Working__Procedure___ 
 *      ->  A sanitizer function verifies command line input to check for rogue or invalid input.
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


void sanitizer(char*);
long backtrack(long, int, long, int);
void testBackTrack(long);


long nextNumber(long number) {
    /**
     *  Validate the given input.
     *  The given input should not contain any repeating digits.
     */

    if (number <= 0) {
        printf("Bad Input: Input a positive number\n");
        return -1;
    }
    
    if (number >= 9876543210) {
        printf("Bad Input: Range's maximum limit exceeded\n");
        return -1;
    }

    int mask = 0;       // Bitmask used as a set
    long rev = 0;       // Bitset used as a stack
    int digitCount = 0; // Stack length tracked using digit count

    // Check for invalid input
    long copy = number;
    while (copy > 0) {
        int rem = copy % 10;

        if ((mask & (1 << rem)) != 0) {
            printf("Bad Input: Input has repeating digits\n");
            return -1;
        }

        mask |= (1 << rem);
        rev = (rev * 10) + rem;
        digitCount++;
        copy /= 10;
    }

    return backtrack(0, 0, rev, digitCount);
}


long backtrack(long number, int mask, long stack, int stackLen) {
    if (stackLen == 0) return number;

    int currDigit = stack % 10;
    for (int offset = 0; offset + currDigit <= 9; offset++) {
        if (stackLen == 1 && offset == 0) continue;

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


void sanitizer(char* input) {
    int n = strlen(input);

    if (n > 11) {
        printf("Bad Input: Input too large\n");
        return;
    }

    for (int i = 0; i < n - 1; i++) {
        if (!isdigit(input[i])) {
            printf("Bad Input: Enter a proper integer\n");
            return;
        }
    }

    long x;
    sscanf(input, "%ld", &x);

    testBackTrack(x);
}


void testBackTrack(long num) {
    /**
     *  XXX: Currently testing backtrack()
     */

    clock_t startTime, endTime;
    
    startTime = clock();
    // printf("%d -> %ld\n", 12580, nextNumber(12580));
    // printf("%d -> %ld\n", 43298, nextNumber(43298));
    // printf("%ld -> %ld\n", 109876, nextNumber(109876));
    // printf("%ld -> %ld\n", 1230123010, nextNumber(1230123010));
    // printf("%d -> %ld\n", -5, nextNumber(-5));
    // printf("%d -> %ld\n", 111, nextNumber(111));
    printf("%ld -> %ld", num, nextNumber(num));
    endTime = clock();

    double timeUsed = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.4lf seconds", timeUsed);
}


int main(void) {
    char input[100];
    printf("Enter number: ");
    fgets(input, 99, stdin);
    input[99] = '\0';
    sanitizer(input);
    return 0;
}
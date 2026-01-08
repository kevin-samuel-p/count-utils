/**
 *      Repeating Numbers
 *      Given a number, find the next number such that at least one of its digits recur.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>


// Verifies whether a number has a least one repeating digit
bool isRepeating(unsigned long long number) {

    // By pigeonhole principle, number with more than ten digits contains at least one repeating digit
    if (number > 9876543210) return true;

    int bitSet = 0;     // Bitset to track digit occurrences
    while (number != 0) {
        if ((bitSet & (1 << (number % 10))) != 0) return true;
        bitSet |= (1 << (number % 10));
        number /= 10;
    }
    return false;
}


//  Returns next number in sequence with at least one repeating digit
unsigned long long nextNumber(unsigned long long number) {
    
    // For unsigned function, returning 0 indicates early return
    if (!isRepeating(number)) {
        printf("Bad Input - Input does not have repeating digits\n");
        return 0;   
    }

    if (number == ULLONG_MAX) {
        printf("Bad Input - Maximum allowed number\n");
        return 0;
    }

    while (
        !isRepeating(++number)  // Empty while loop runs in the conditional using increment operator
    );

    return number;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Invalid Syntax: .\\yesrep.exe <number>");
        return -1;
    }

    unsigned long long input, output;
    char *endChar;
    clock_t startTime, endTime;

    startTime = clock();

    if (argv[1][0] == '-') {
        printf("Bad Input - Negative numbers not allowed\n");
        return -1;
    }

    input = strtoull(argv[1], &endChar, 10);
    if (*endChar != '\0') {
        printf("Bad Input - Invalid number\n");
        return -1;
    }

    output = nextNumber(input);
    if (output == 0) return -1;

    endTime = clock();
    
    if (argc > 2) {
        printf("WARNING: Extra arguments will be ignored...\n\n");
    }
    printf("%llu\n", output);

    double timeUsed = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.4lf seconds", timeUsed);

    return 0;
}
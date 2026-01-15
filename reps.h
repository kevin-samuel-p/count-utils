/**
 *      Repetitions
 *      Utilities that deal with patterns based on digit repetitions
 */


#include <stdio.h>
#include <stdbool.h>
#include <limits.h>


bool is_repeating(unsigned long long);
long long backtrack(unsigned long long, int, unsigned long long, int);
long long next_non_repeating_number(unsigned long long);
unsigned long long next_repeating_number(unsigned long long);


// Checks whether a given number has any repeating/recurring digits
bool is_repeating(unsigned long long number) 
{
    if (number > 9876543210) return true;   // Pigeonhole principle

    int set = 0;    // Bitset to keep track of digits
    while (number != 0)
    {
        if ((set & (1 << (number % 10))) != 0) 
            return true;

        set |= (1 << (number % 10));
        number /= 10;
    }

    return false;    
}


// Returns next number in sequence with at least one repeating digit
unsigned long long next_repeating_number(unsigned long long number)
{
    if (number == ULLONG_MAX)
    {
        printf("Bad Input - Maximum allowed number");
        return 0;
    }

    while (
        !is_repeating(++number)
    );

    return number;
}


// Finds next number with non-repeating digits
long long next_non_repeating_number(unsigned long long number)
{
    if (number >= 9876543210)
    {
        printf("Bad Input: Maximum limit exceeded");
        return 1;
    }
    
    unsigned long long rev = 0;      // Digit stack
    int digitCount = 0;     // Stack length tracked using digit count

    unsigned long long copy = number + 1;
    while (copy > 0)
    {
        int rem = copy % 10;
        rev = (rev * 10) + rem;
        digitCount++;
        copy /= 10;
    }

    rev *= 10;      // Appending a zero to stack
    digitCount++;   // Just in case of underflow during lexicographic DFS

    return backtrack(0ULL, 0, rev, digitCount);
}


// Helper function: lexicographic DFS
long long backtrack(
    unsigned long long number, 
    int set, 
    unsigned long long stack, 
    int stackLen
) {
    if (stackLen == 0) return number;

    int currDigit = stack % 10;
    for (int offset = 0; offset + currDigit <= 9; offset++)
    {
        unsigned long long trial;
        if ((set & (1 << (offset + currDigit))) == 0 && (
            (trial = backtrack(
                number * 10 + offset + currDigit, 
                (currDigit + offset == set) ? set : set | (1 << (offset + currDigit)), 
                stack / 10, 
                stackLen - 1
            )) != -1)
        ) { return trial; }

        stack = 0;
    }

    return -1;
}
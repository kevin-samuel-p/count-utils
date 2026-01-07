/**
 *      Numbers Within Numbers (NWN) with N Numbers
 * 
 *      Numbers are represented by period-separated numerals (similar to the format of an IPv4 address).
 *      Each digit encloses the succeeding digit, and the number is incremented by increasing the last digit until it exceeds its predecessor.
 *      It follows a custom place value system with a changing radix based on the initial numeral.
 * 
 *      Encapsulated numbers must reach their predecessors before getting reset.
 * 
 *      Example sequence:
 *      1
 *      2.1
 *      2.2
 *      3.1.1
 *      3.2.1
 *      3.2.2
 *      3.3.1
 *      3.3.2
 *      3.3.3
 *      4.1.1.1
 *      4.2.1.1
 *      4.2.2.1
 *      4.2.2.2
 *      4.3.1.1
 *      4.3.2.1
 *      .......
 *      .......
 *      .......
 *      4.4.4.4
 *      5.1.1.1.1
 * 
 *      ___Working__Procedure___
 *      ->  User inputs number through CLI call.
 *      ->  Number is parsed using a tokenizer to split at periods, and each number is processed into an array.
 *      ->  Rules are followed to ensure proper number formatting and to escape early in case of improper format.
 *      ->  Number is incremented according to incrementation rules.
 *      ->  Array is rejoined as a string and displayed.
 * 
 *      ___Input__Requirements___
 *      ->  User must follow proper number format.
 *      ->  Number partitions should follow non-increasing order.
 *      ->  No spaces should be present in the number's string.
 *      ->  Non-positive values will not be allowed.
 *      ->  The number of partitions in the string will strictly be equal to the value of the first partition.
 *      ->  Supported range of numeral values is [1, LONG_MAX) (exclusive upper bound for increment feasibility).
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <time.h>


static long *arr;
static long size;


void increment() {
    long depth = size - 1;
    for (; depth > 0; depth--) {
        if (++arr[depth] > arr[depth - 1]) {
            arr[depth] = 1;
            continue;
        } break;
    }

    if (depth == 0) {
        arr = (long *)realloc(arr, (size + 1) * sizeof(long));
        arr[0] = size + 1;
        arr[size++] = 1L;
    }
}


char *stringify() {
    char *string = (char *)malloc(15 * (size_t) size * sizeof(char));
    string[0] = '\0';

    char *number = (char *)malloc(15 * sizeof(char));
    for (int i = 0; i < size; i++) {
        if (i != 0) strcat(string, ".");

        sprintf(number, "%ld", arr[i]);
        strcat(string, number);
    }
    free(number);
    
    return string;
}


int main(int argc, char *argv[]) {
    /**
     *      ___Command__Line__Syntax____
     *          .\nwnception.exe <numeral>
     * 
     *      ___Numeral__Format___
     *          a.b.c.d.e
     * 
     *      a - Number of numerals
     *      b - Secondary numeral
     *      c - Tertiary numeral
     *      d - Quarternary numeral
     *      and so on.
     */

    if (argc < 2) {
        printf("Invalid Syntax: %s <numeral>", argv[0]);
        return -1;
    }

    char *endPtr, *token, *next;
    long arrPtr = 0;
    errno = 0;
    clock_t startTime, endTime;

    startTime = clock();

    token = strtok(argv[1], ".");
    size = strtol(token, &endPtr, 10);

    if (*endPtr != '\0') {
        printf("Bad Input - Non-numeral partition in string\n");
        return -1;
    } else if (size <= 0) {
        printf("Bad Input - Non-positive numerals not allowed\n");
        return -1;
    } else if (errno == ERANGE || size == LONG_MAX) {
        printf("Bad Input - Numeral too large\n");
        return -1;
    }

    arr = (long *)malloc(size * sizeof(long));
    arr[arrPtr++] = size;

    while ((token = strtok(NULL, ".")) != NULL && arrPtr < size) {
        arr[arrPtr] = strtol(token, &endPtr, 10);

        if (*endPtr != '\0') {
            printf("Bad Input - Non-numeral partition in string\n");
            free(arr);
            return -1;
        } else if (arr[arrPtr] <= 0) {
            printf("Bad Input - Non-positive numerals not allowed\n");
            free(arr);
            return -1;
        } else if (errno == ERANGE || arr[arrPtr] == LONG_MAX) {
            printf("Bad Input - Numeral too large\n");
            free(arr);
            return -1;
        } else if (arr[arrPtr] > arr[arrPtr - 1]) {
            printf("Bad Input - Numeral partitions are not in non-increasing order\n");
            free(arr);
            return -1;
        } 
        ++arrPtr;
    }

    if (arrPtr != size) {
        printf("Bad Input - Too few partitions for %d numerals\n", size);
        free(arr);
        return -1;
    } else if (token != NULL) {
        printf("Bad Input - Too many partitions for %d numerals\n", size);
        free(arr);
        return -1;
    }

    increment();
    next = stringify();

    endTime = clock();

    if (argc > 2) {
        printf("WARNING: Extra arguments will be ignored...\n\n");
    }

    printf("%s\n", next);
    free(next);
    free(arr);

    double timeUsed = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.4lf seconds", timeUsed);

    return 0;
}
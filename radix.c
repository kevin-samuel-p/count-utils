/**
 *      Number Systems
 *      Utilities for conversions among number systems
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>


enum Radix {
    BINARY = 2,
    OCTAL = 8,
    DECIMAL = 10,
    HEXADECIMAL = 16
};


// Conversion Functions/Sanitization Functions
// NOTE: Self-conversion can be used for input sanitization.
char *convertBinary(char *, enum Radix);
char *convertDecimal(char *, enum Radix);
char *convertHexadecimal(char *, enum Radix);
char *convertOctal(char *, enum Radix);

// Sequence Function - Get next number in sequence by specifying sanitized input with radix of the number
char *nextNumber(char *, enum Radix);


//  For inputted binary strings, ensure string length doesn't exceed 64 + 1
//  For inputted decimal strings, ensure string length doesn't exceed 20 + 1
//  For inputted hexadecimal strings, ensure string length doesn't exceed 16 + 1
//  For inputted octal strings, ensure string length doesn't exceed 22 + 1

//  BORDERLINE CASES
//  For inputted decimal strings of length 20 + 1, ensure string does not exceed 18446744073709551615
//  For inputted octal strings of length 22 + 1, ensure string does not exceed 17777777777777777777777


char *convertBinary(char *binary, enum Radix radix) {
    int n = strlen(binary);
    if (n > 64) {
        printf("Bad Input: Number too large\n");
        return NULL;
    }

    unsigned long long number = 0ULL;
    for (int i = n-1; i >= 0; i--) {
        if (binary[i] != '0' && binary[i] != '1') {
            printf("Bad Input: Invalid binary number\n");
            return NULL;
        }
        number = (number << 1) + (binary[i] - '0');
    }

    char *numString;
    switch(radix) {
        case BINARY:
            return binary;
        
        case DECIMAL:
            numString = (char *)malloc(21 * sizeof(char));
            sprintf(numString, "%llu", number);
        break;

        case HEXADECIMAL:
            numString = (char *)malloc(17 * sizeof(char));
            sprintf(numString, "%llX", number);
        break;

        case OCTAL:
            numString = (char *)malloc(23 * sizeof(char));
            sprintf(numString, "%llo", number);
        break;
    }

    return numString;
}


char *convertDecimal(char *decimal, enum Radix radix) {
    // Validate input
    int n = strlen(decimal);

    if (n > 20 || (n == 20 && strcmp(decimal, "18446744073709551615") > 0)) {
        printf("Bad Input: Number too large\n");
        return NULL;
    }

    unsigned long long number = 0ULL;
    for (int i = 0; i < n; i++) {
        if (!isdigit(decimal[i])) {
            printf("Bad Input: Invalid decimal number'n");
            return NULL;
        }
        number = (number * 10) + (decimal[i] - '0');
    }

    char *numString;
    switch(radix) {
        case BINARY:
            numString = (char *)malloc(65 * sizeof(char));
            int len = 0;

            // Construct in reverse order and then reverse string
            while (number > 0) {
                numString[len++] = '0' + (number & 1);
                number >>= 1;
            }
            numString[len] = '\0';

            // Reverse string order
            for (int left = 0, right = len - 1; left < right; left++, right--) {
                char temp = numString[left];
                numString[left] = numString[right];
                numString[right] = temp;
            }
        break;

        case DECIMAL:
            numString = (char *)malloc((n + 1) * sizeof(char));
            sprintf(numString, "%lld", number);

        case HEXADECIMAL:
            numString = (char *)malloc(17 * sizeof(char));
            sprintf(numString, "%llX", number);
        break;

        case OCTAL:
            numString = (char *)malloc(23 * sizeof(char));
            sprintf(numString, "%llo", number);
        break;
    }

    return numString;
}


char *convertHexadecimal(char *hexadecimal, enum Radix radix) {
    int n = strlen(hexadecimal);
    if (n > 16) {
        printf("Bad Input: Number too large\n");
        return NULL;
    }

    unsigned long long number = 0ULL;
    for (int i = 0; i < n; i++) {
        if (isdigit(hexadecimal[i])) {
            number = (number << 4) + (hexadecimal[i] - '0');
        } else switch(hexadecimal[i]) {
            case 'A':
            case 'a':
                number = (number << 4) + 10;
            break;

            case 'B':
            case 'b':
                number = (number << 4) + 11;
            break;

            case 'C':
            case 'c':
                number = (number << 4) + 12;
            break;

            case 'D':
            case 'd':
                number = (number << 4) + 13;
            break;

            case 'E':
            case 'e':
                number = (number << 4) + 14;
            break;

            case 'F':
            case 'f':
                number = (number << 4) + 15;
            break;

            default:
                printf("Bad Input - Invalid hexadecimal number\n");
                return NULL;
        }
    }

    char *numString;
    switch(radix) {
        case BINARY:
            numString = (char *)malloc((4*n + 1) * sizeof(char));
            numString[0] = '\0';

            for (int i = 0; i < n; i++) {
                switch(hexadecimal[i]) {
                    case '0':
                        if (numString[0] != '\0') strcat(numString, "0000");
                    break;

                    case '1':
                        strcat(numString, (numString[0] == '\0') ? "1" : "0001");
                    break;

                    case '2':
                        strcat(numString, (numString[0] == '\0') ? "10" : "0010");
                    break;

                    case '3':
                        strcat(numString, (numString[0] == '\0') ? "11" : "0011");
                    break;

                    case '4':
                        strcat(numString, (numString[0] == '\0') ? "100" : "0100");
                    break;

                    case '5':
                        strcat(numString, (numString[0] == '\0') ? "101" : "0101");
                    break;

                    case '6':
                        strcat(numString, (numString[0] == '\0') ? "110" : "0110");
                    break;

                    case '7':
                        strcat(numString, (numString[0] == '\0') ? "111" : "0111");
                    break;

                    case '8':
                        strcat(numString, "1000");
                    break;

                    case '9':
                        strcat(numString, "1001");
                    break;

                    case 'A':
                    case 'a':
                        strcat(numString, "1010");
                    break;

                    case 'B':
                    case 'b':
                        strcat(numString, "1011");
                    break;

                    case 'C':
                    case 'c':
                        strcat(numString, "1100");
                    break;

                    case 'D':
                    case 'd':
                        strcat(numString, "1101");
                    break;

                    case 'E':
                    case 'e':
                        strcat(numString, "1110");
                    break;

                    case 'F':
                    case 'f':
                        strcat(numString, "1111");
                    break;
                }
            }
        break;

        case DECIMAL:
            numString = (char *)malloc(21 * sizeof(char));
            sprintf(numString, "%lld", number);
        break;

        case HEXADECIMAL:
            numString = (char *)malloc((n + 1) * sizeof(char));
            sprintf(numString, "%llX", number);
        break;

        case OCTAL:
            numString = (char *)malloc(23 * sizeof(char));
            sprintf(numString, "%llo", number);
        break;
    }

    return numString;
}


char *convertOctal(char *octal, enum Radix radix) {
    int n = strlen(octal);
    if (n > 22 || (n == 22 && strcmp(octal, "1777777777777777777777") > 0)) {
        printf("Bad Input - Number too large\n");
        return NULL;
    }

    unsigned long long number = 0ULL;
    for (int i = 0; i < n; i++) {
        if (!isdigit(octal[i]) || (octal[i] - '0') >= 8) {
            printf("Bad Input - Invalid octal number\n");
            return NULL;
        }
        number = (number << 3) + (octal[i] - '0');
    }

    char *numString;
    switch(radix) {
        case BINARY:
            numString = (char *)malloc((3*n + 1) * sizeof(char));
            numString[0] = '\0';

            for (int i = 0; i < n; i++) {
                switch(octal[i]) {
                    case '0':
                        if (numString[0] != '\0') strcat(numString, "000");
                    break;

                    case '1':
                        strcat(numString, (numString[0] == '\0') ? "1" : "001");
                    break;

                    case '2':
                        strcat(numString, (numString[0] == '\0') ? "10" : "010");
                    break;

                    case '3':
                        strcat(numString, (numString[0] == '\0') ? "11" : "011");
                    break;

                    case '4':
                        strcat(numString, "100");
                    break;

                    case '5':
                        strcat(numString, "101");
                    break;

                    case '6':
                        strcat(numString, "110");
                    break;

                    case '7':
                        strcat(numString, "111");
                    break;
                }
            }
        break;

        case DECIMAL:
            numString = (char *)malloc(21 * sizeof(char));
            sprintf(numString, "%lld", number);
        break;

        case HEXADECIMAL:
            numString = (char *)malloc(17 * sizeof(char));
            sprintf(numString, "%llX", number);
        break;

        case OCTAL:
            numString = (char *)malloc((n + 1) * sizeof(char));
            sprintf(numString, "%llo", number);
        break;
    }

    return numString;
}


char *nextNumber(char *number, enum Radix radix) {
    // Ensure input is already sanitized
    int n = strlen(number);
    char *res;
    unsigned long long value;

    switch(radix) {
        case BINARY:
            for (int i = n - 1; i >= 0; i--) {
                switch(number[i]) {
                    case '0':
                        number[i] = '1';
                        return number;

                    case '1':
                        number[i] = '0';
                }
            }

            res = (char *)malloc((n + 2) * sizeof(char));
            res = "1";
            strcat(res, number);

        break;

        case DECIMAL:
        case HEXADECIMAL:
        case OCTAL:

            switch(radix) {
                case DECIMAL:
                    sscanf(number, "%lld", &value);
                break;

                case HEXADECIMAL:
                    sscanf(number, "%llx", &value);
                break;

                case OCTAL:
                    sscanf(number, "%llo", &value);
                break;
            }

            if (value == ULLONG_MAX) {
                printf("Bad Input - Maximum allowed number\n");
                return NULL;
            }

            res = (char *)malloc((n + 2) * sizeof(char));
            
            switch(radix) {
                case DECIMAL:
                    sprintf(res, "%lld", ++value);
                break;

                case HEXADECIMAL:
                    sprintf(res, "%llX", ++value);
                break;

                case OCTAL:
                    sprintf(res, "%llo", ++value);
                break;
            }
        break;    
    }

    return res;
}
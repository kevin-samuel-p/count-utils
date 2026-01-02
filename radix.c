/**
 *      Number Systems
 *      Utilities for conversions among number systems
 * 
 *      ___Working__Procedure___
 *      ->  User can utilize functions with command line syntax (documented @ main function).
 *      ->  Supported functions are conversion between number systems/radices and finding the next number, given the radix.
 *      ->  The conversion functions contain escapes for bad inputs, thus they can be used for input sanitization by self-converting.
 * 
 *      ___Input__Requirements___
 *      ->  Inputted number should be a non-negative integer for all number systems, unsigned for binary.
 *      ->  Inputted number should not exceed ULLONG_MAX (2^65 - 1).
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>


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
//  For inputted octal strings of length 22 + 1, ensure string does not exceed 1777777777777777777777


char *convertBinary(char *binary, enum Radix radix) {
    int n = strlen(binary);
    if (n > 64) {
        printf("Bad Input: Number too large\n");
        return NULL;
    }

    unsigned long long number = 0ULL;
    for (int i = 0; i < n; i++) {
        if (binary[i] != '0' && binary[i] != '1') {
            printf("Bad Input: Invalid binary number\n");
            return NULL;
        }
        number = (number << 1) | (binary[i] - '0');
    }

    char *numString;
    switch(radix) {
        case BINARY:
            numString = (char *)malloc((n + 1) * sizeof(char));

            int k = 0;
            while (binary[k] == '0') k++;
            strcpy(numString, binary + k);
        break;
        
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
            printf("Bad Input: Invalid decimal number\n");
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
            do {
                numString[len++] = '0' + (number & 1);
                number >>= 1;
            } while (number > 0);
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
            sprintf(numString, "%llu", number);
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
            sprintf(numString, "%llu", number);
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
            res = (char *)malloc((n + 2) * sizeof(char));
            strcpy(res, number);

            // Add 1 from the end
            int i = n - 1;
            while (i >= 0 && res[i] == '1') {
                res[i] = '0';
                i--;
            }

            if (i >= 0) {
                res[i] = '1';
            } else {
                // Overflow: prepend '1'
                memmove(res + 1, res, n + 1);
                res[0] = '1';
            }
        break;

        case DECIMAL:
        case HEXADECIMAL:
        case OCTAL:

            switch(radix) {
                case DECIMAL:
                    sscanf(number, "%llu", &value);
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
                    sprintf(res, "%llu", ++value);
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


int main(int argc, char *argv[]) {
    /**
     *      ___Command__Line__Argument__Format___
     *          .\radix.exe <function-flag> [radix-flag] <number> [conversion-flag]
     * 
     *      <> - Required by syntax
     *      [] - Option
     * 
     *      ___Function__Flags___
     *      Necessary argument
     *      -c / -convert : Calls conversion function
     *      -n / -next    : Calls sequence function nextNumber()
     * 
     *      ___Radix__Flags___
     *      OPTION - If omitted, radix is assumed to be decimal by default
     *      -b / -bin / -binary      :  Sets input radix as binary
     *      -d / -dec / -decimal     :  Sets input radix as decimal
     *      -h / -hex / -hexadecimal :  Sets input radix as hexadecimal
     *      -o / -oct / -octal       :  Sets input radix as octal
     * 
     *      ___Number___
     *      Numeral string - Necessary argument
     * 
     *      ___Conversion__Flags___
     *      OPTION - If omitted, radix is assumed to be set input radix by default
     *      Same options as radix flags
     *      Only used for conversion functions, will be ignored in sequence function (as extra parameters)
     */


    if (argc < 3) {
        printf("Invalid Syntax - check docstring for command line syntax\n");
        return -1;  // Returns -1, signalling insufficient arguments
    }


    int x = 2, y;
    char *input, *output;           // Pointers for sanitized input and output strings respectively
    char function;                  // Function flag - stores either c or n
    enum Radix 
        inputRadix = DECIMAL,       // Input radix flag - stores input number's radix
        conversionRadix = DECIMAL;  // Conversion radix flag - stores radix for conversion, if needed
    clock_t startTime, endTime;


    startTime = clock();


    if (
        strcmp(argv[1], "-c") == 0 || 
        strcmp(argv[1], "-convert") == 0
    ) { function = 'c'; }

    else if (
        strcmp(argv[1], "-n") == 0 || 
        strcmp(argv[1], "-next") == 0
    ) { function = 'n'; } 
    
    else {
        printf("Invalid Syntax - check docstring for command line syntax\n");
        return 1;   // Returns index of bad argument
    }


    if (
        strcmp(argv[x], "-b") == 0 || 
        strcmp(argv[x], "-bin") == 0 || 
        strcmp(argv[x], "-binary") == 0
    ) { 
        inputRadix = BINARY;
        conversionRadix = BINARY;
        ++x;
    }   
    else if (
        strcmp(argv[x], "-d") == 0 || 
        strcmp(argv[x], "-dec") == 0 || 
        strcmp(argv[x], "-decimal") == 0
    ) {
        conversionRadix = DECIMAL;
        ++x;
    }    
    else if (
        strcmp(argv[x], "-h") == 0 || 
        strcmp(argv[x], "-hex") == 0 || 
        strcmp(argv[x], "-hexadecimal") == 0
    ) {
        inputRadix = HEXADECIMAL;
        conversionRadix = HEXADECIMAL;
        ++x;
    }  
    else if (
        strcmp(argv[x], "-o") == 0 || 
        strcmp(argv[x], "-oct") == 0 || 
        strcmp(argv[x], "-octal") == 0
    ) {
        inputRadix = OCTAL;
        conversionRadix = OCTAL;
        ++x;
    }


    switch(inputRadix) {
        case BINARY:
            input = convertBinary(argv[x], BINARY);
        break;

        case DECIMAL:
            input = convertDecimal(argv[x], DECIMAL);
        break;

        case HEXADECIMAL:
            input = convertHexadecimal(argv[x], HEXADECIMAL);
        break;

        case OCTAL:
            input = convertOctal(argv[x], OCTAL);
        break;
    }


    if (input == NULL) return x;    // Returns index of input string

    switch(function) {
        case 'c':
            y = x + 1;
            if (argc > y) {
                if (
                    strcmp(argv[y], "-b") == 0 || 
                    strcmp(argv[y], "-bin") == 0 || 
                    strcmp(argv[y], "-binary") == 0
                ) { conversionRadix = BINARY; } 

                else if (
                    strcmp(argv[y], "-d") == 0 || 
                    strcmp(argv[y], "-dec") == 0 || 
                    strcmp(argv[y], "-decimal") == 0
                ) { conversionRadix = DECIMAL; } 
                
                else if (
                    strcmp(argv[y], "-h") == 0 || 
                    strcmp(argv[y], "-hex") == 0 || 
                    strcmp(argv[y], "-hexadecimal") == 0
                ) { conversionRadix = HEXADECIMAL; } 
                
                else if (
                    strcmp(argv[y], "-o") == 0 || 
                    strcmp(argv[y], "-oct") == 0 || 
                    strcmp(argv[y], "-octal") == 0
                ) { conversionRadix = OCTAL; } 
                
                else --y;
            }

            switch(inputRadix) {
                case BINARY:
                    output = convertBinary(input, conversionRadix);
                break;

                case DECIMAL:
                    output = convertDecimal(input, conversionRadix);
                break;

                case HEXADECIMAL:
                    output = convertHexadecimal(input, conversionRadix);
                break;

                case OCTAL:
                    output = convertOctal(input, conversionRadix);
                break;
            }
        break;

        case 'n':
            output = nextNumber(input, inputRadix);
            if (output == NULL) return x;   // Returns index of bad number
            y = x;
        break;
    }

    if (argc > y + 1) {
        printf("WARNING: Extra arguments will be ignored...\n\n");
    }

    switch(function) {
        case 'c':
            switch(inputRadix) {
                case BINARY:
                    printf("Binary Number:\t\t%s\n", input);
                break;

                case DECIMAL:
                    printf("Decimal Number:\t\t%s\n", input);
                break;

                case HEXADECIMAL:
                    printf("Hexadecimal Number:\t%s\n", input);
                break;

                case OCTAL:
                    printf("Octal Number:\t\t%s\n", input);
                break;
            }

            switch(conversionRadix) {
                case BINARY:
                    printf("Binary Number:\t\t%s\n", output);
                break;

                case DECIMAL:
                    printf("Decimal Number:\t\t%s\n", output);
                break;

                case HEXADECIMAL:
                    printf("Hexadecimal Number:\t%s\n", output);
                break;

                case OCTAL:
                    printf("Octal Number:\t\t%s\n", output);
                break;
            }
        break;

        case 'n':
            switch(inputRadix) {
                case BINARY:
                    printf("Next Binary Number:\t\t%s\n", output);
                break;

                case DECIMAL:
                    printf("Next Decimal Number:\t\t%s\n", output);
                break;

                case HEXADECIMAL:
                    printf("Next Hexadecimal Number:\t%s\n", output);
                break;

                case OCTAL:
                    printf("Next Octal Number:\t\t%s\n", output);
                break;
            }
        break;
    }

    endTime = clock();

    double timeUsed = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.4lf seconds", timeUsed);

    free(input);
    free(output);

    return 0;
}
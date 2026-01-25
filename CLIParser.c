#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <wchar.h>

#include "ClipboardFunctions.h"
#include "Inputter.h"
#include "MiscUtils.h"
#include "Runner.h"

#include "emoji.h"
#include "increasing.h"
#include "meme.h"
#include "morse.h"
#include "nihongo.h"
#include "nwnception.h"
#include "radix.h"
#include "reps.h"
#include "reversals.h"
#include "roman.h"
#include "tally.h"


/**
 *      ____________Command__Line__Syntax____________
 *      (argv[0]) <mode> <option> [option-parameters]
 * 
 *      <> - necessary arguments
 *      [] - optional parameters
 * 
 * 
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |      MODE      |    OPTION     |        PARAMS          |                        DESCRIPTION                        |
 *      |----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a normal counting run with binary numbers.      |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  solorun / s  |          NIL           | Initiates a solo counting run with binary numbers.        |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |     binary     |               |   d / dec / decimal    | Converts an inputted binary number to decimal.            |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |  h / hex / hexadecimal | Converts an inputted binary number to hexadecimal.        |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |     o / oct / octal    | Converts an inputted binary number to octal.              |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a normal counting run with decimal numbers.     |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  solorun / s  |          NIL           | Initiates a solo counting run with decimal numbers.       |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      | decimal/normal |               |    b / bin / binary    | Converts an inputted decimal number to octal.             |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |  h / hex / hexadecimal | Converts an inputted decimal number to hexadecimal.       |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |     o / oct / octal    | Converts an inputted decimal number to octal.             |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a normal counting run with emojified numbers.   |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |      emoji     |  solorun / s  |          NIL           | Initiates a solo counting run with emojified numbers.     |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |          NIL           | Converts an inputted number into an emojified number.     |
 *      |                |  emojify / e  |                        |                                                           |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a normal counting run with hexadecimal numbers. |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  solorun / s  |          NIL           | Initiates a solo counting run with hexadecimal numbers.   |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |   hexadecimal  |               |    b / bin / binary    | Converts an inputted hexadecimal number to binary.        |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |    d / dec / decimal   | Converts an inputted hexadecimal number to decimal.       |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |     o / oct / octal    | Converts an inputted hexadecimal number to octal.         |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using numbers with digits in     |
 *      |                |               |                        | non-decreasing order.                                     |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |   increasing   |  solorun / s  |          NIL           | Initiates a solo counting run using numbers with digits   |
 *      |                |               |                        | in non-decreasing order.                                  |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |   check / c   |          NIL           | Checks whether an inputted number's digits are in         |
 *      |                |               |                        | non-decreasing order.                                     |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using Japanese numbers.          |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |    japanese    |  solorun / s  |          NIL           | Initiates a solo counting run using Japanese numbers.     |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |          NIL           | Translates an inputted number to Japanese, and converts   |
 *      |                | translate / t |                        | an inputted Japanese number to its decimal form.          |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a normal counting run with octal numbers.       |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  solorun / s  |          NIL           | Initiates a solo counting run with octal numbers.         |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |      octal     |               |    b / bin / binary    | Converts an inputted octal number to binary.              |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |    d / dec / decimal   | Converts an inputted octal number to decimal.             |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |  h / hex / hexadecimal | Converts an inputted octal number to hexadecimal.         |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |          69            | Initiates a counting run using '69' numbers.              |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          420           | Initiates a counting run using '420' numbers.             |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |         69420          | Initiates a counting run using both '69' numbers and '420'|
 *      |                |               |                        | numbers.                                                  |
 *      |      meme      |---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |          69            | Initiates a solo counting run using '69' numbers.         |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |  solorun / s  |          420           | Initiates a solo counting run using '420' numbers.        |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |         69420          | Initiates a solo counting run using both '69' numbers and |
 *      |                |               |                        | '420' numbers.                                            |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |       m / mirror       | Initiates a counting run using mirrored numbers, taking   |
 *      |                |               |                        | the initial number as mirrored input.                     |
 *      |                |    run / r    |------------------------+-----------------------------------------------------------+
 *      |                |               |       n / normal       | Initiates a counting run using mirrored numbers, taking   |
 *      |                |               |                        | the initial number as normal input.                       |
 *      |     mirror     |---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |       m / mirror       | Initiates a solo counting run using mirrored numbers,     |
 *      |                |               |                        | taking the initial number as mirrored input.              |
 *      |                |  solorun / s  |------------------------+-----------------------------------------------------------+
 *      |                |               |       n / normal       | Initiates a solo counting run using mirrored numbers,     |
 *      |                |               |                        | taking the initial number as normal input.                |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run in Morse code.                   |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |      morse     |  solorun / s  |          NIL           | Initiates a solo counting run in Morse code.              |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |          NIL           | Converts an inputted Morse code to its numerical value,   |
 *      |                |               |                        | and an inputted number to Morse code.                     |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using numbers that have no       |
 *      |                |               |                        | recurring digits.                                         |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |      norep     |  solorun / s  |          NIL           | Initiates a solo counting run using numbers that have no  |
 *      |                |               |                        | recurring digits.                                         |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |   check / c   |          NIL           | Checks whether an inputted number contains no recurring   |
 *      |                |               |                        | digits.                                                   |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using Numbers within Numbers.    |
 *      |       nwn      |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  solorun / s  |          NIL           | Initiates a solo counting run using Numbers within        |
 *      |                |               |                        | numbers.                                                  |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using Numbers within Numbers     |
 *      |                |               |                        | within Numbers.                                           |
 *      |      nwnwn     |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  solorun / s  |          NIL           | Initiates a solo counting run using Numbers within        |
 *      |                |               |                        | Numbers within Numbers.                                   |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using Numbers within Numbers     |
 *      |                |               |                        | with N Numbers.                                           |
 *      |     nwnwnn     |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  solorun / s  |          NIL           | Initiates a solo counting run using Numbers within        |
 *      |                |               |                        | Numbers with N Numbers.                                   |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using palindromic numbers.       |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |   palindrome   |  solorun / s  |          NIL           | Initiates a solo counting run using palindromic numbers.  |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |   check / c   |          NIL           | Checks whether an inputted number is a palindrome.        |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |    run / r    |          NIL           | Initiates a counting run using numbers that have at least |
 *      |                |               |                        | one repeating digit.                                      |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |       rep      |  solorun / s  |          NIL           | Initiates a solo counting run using numbers that have at  |
 *      |                |               |                        | least one repeating digit.                                |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |   check / c   |          NIL           | Checks whether an inputted number contains at least one   |
 *      |                |               |                        | repeating digit.                                          |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |                        | Initiates a counting run with Roman Numerals using their  |
 *      |                |               |     i / idiomatic      | idiomatic representations (inclusion of M as a recurring  |
 *      |                |               |                        | numeral in each chunk).                                   |
 *      |                |    run / r    |------------------------+-----------------------------------------------------------+
 *      |                |               |                        | Initiates a counting run with Roman Numerals using their  |
 *      |                |               |        l / legal       | legal semantically correct standard representations       |
 *      |                |               |                        | (clustering groups of thousand by a single combining      |
 *      |                |               |                        | overline).                                                |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |      roman     |               |                        | Initiates a solo counting run with Roman Numerals using   |
 *      |                |               |     i / idiomatic      | their idiomatic representations (inclusion of M as a      |
 *      |                |               |                        | recurring numeral in each chunk).                         |
 *      |                |  solorun / s  |------------------------+-----------------------------------------------------------+
 *      |                |               |                        | Initiates a solo counting run with Roman Numerals using   |
 *      |                |               |        l / legal       | their legal semantically correct standard representations |
 *      |                |               |                        | (clustering groups of thousand by a single combining      |
 *      |                |               |                        | overline).                                                |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |  convert / c  |          NIL           | Converts an inputted Roman number to its numerical value, |
 *      |                |               |                        | and an inputted number to its Roman numeral equivalent.   |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |         b / big        | Initiates a counting run using tally marks with big text  |
 *      |                |               |                        | formatting.                                               |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |       c / compact      | Initiates a counting run using tally marks with compact   |
 *      |                |               |                        | formatting (removal of newline characters).               |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |                        | Initiates a counting run using tally marks with default   |
 *      |                |               |       d / default      | formatting (max 50 tallies per line). Default selected    |
 *      |                |               |                        | parameter if options are omitted from command.            |
 *      |                |    run / r    |------------------------+-----------------------------------------------------------+
 *      |                |               |        g / giant       | Initiates a counting run using tally marks with giant     |
 *      |                |               |                        | text formatting.                                          |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |       i / impact       | Initiates a counting run using tally marks with impact    |
 *      |                |               |                        | formatting (slightly big and bold text).                  |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |        t / tiny        | Initiates a counting run using tally marks with tiny text |
 *      |                |               |                        | formatting.                                               |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |         b / big        | Initiates a solo counting run using tally marks with big  |
 *      |      tally     |               |                        | text formatting.                                          |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |       c / compact      | Initiates a solo counting run using tally marks with      |
 *      |                |               |                        | compact formatting (removal of newline characters).       |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |                        | Initiates a solo counting run using tally marks with      |
 *      |                |               |       d / default      | default formatting (max 50 tallies per line). Default     |
 *      |                |               |                        | selected parameter if options are omitted from command.   |
 *      |                |  solorun / s  |------------------------+-----------------------------------------------------------+
 *      |                |               |        g / giant       | Initiates a solo counting run using tally marks with      |
 *      |                |               |                        | giant text formatting.                                    |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |       i / impact       | Initiates a solo counting run using tally marks with      |
 *      |                |               |                        | impact formatting (slightly big and bold text).           |
 *      |                |               |------------------------+-----------------------------------------------------------+
 *      |                |               |        t / tiny        | Initiates a solo counting run using tally marks with tiny |
 *      |                |               |                        | text formatting.                                          |
 *      |                |---------------+------------------------+-----------------------------------------------------------+
 *      |                |               |                        | Converts an inputted number to its tally mark form        |
 *      |                |  convert / c  |          NIL           | (incorporating tally mark strikethrough according to      |
 *      |                |               |                        | Discord markdown rules), and counts tally marks to assess |
 *      |                |               |                        | its true numerical value.                                 |
 *      +----------------+---------------+------------------------+-----------------------------------------------------------+
 */


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf(
            "Invalid Syntax: %s <mode> <option> [option-parameters]\n\n"
            "Please review documentation for more information on the command line syntax...\n"
        );
        return 1;
    }

    char option, param;     // Used for parsing the CLI call

    char 
        *n,         // Pointer for number from user input
        *s_n,       // Pointer for sanitized number (in case of input sanitization functions)
        *o_n,       // Pointer for output number (can be reused)

        *i,         // Pointer for general user input (for modules that take non-numerical strings as input)
        *o          // Pointer for general user output (for modules that return non-numerical strings as output)
    ;

    wchar_t 
        *i_w,       // Pointer for wide input strings
        *o_w        // Pointer for wide output strings
    ;

    if (!create_temp_file())
    {
        printf(
            "Error: Could not create input file.\n"
            "Please make sure you are running this tool as an administrator.\n"
        );
        return 1;
    }

    /* -------- BINARY MODE -------- */
    if (strcmp(argv[1], "binary") == 0)
    {
        if (
            strcmp(argv[2], "r")  == 0 || 
            strcmp(argv[2], "run") == 0
        ) {
            option = 'r';       // Run
        }
        else if (
            strcmp(argv[2], "s") == 0 || 
            strcmp(argv[2], "solorun") == 0
        ) {
            option = 's';       // Solorun
        }
        else if (
            strcmp(argv[2], "c") == 0 ||
            strcmp(argv[2], "convert") == 0
        ) {
            option = 'c';       // Convert

            // Parse parameter
            if (
                strcmp(argv[3], "d") == 0 ||
                strcmp(argv[3], "dec") == 0 ||
                strcmp(argv[3], "decimal") == 0
            ) {
                param = 'd';    // Convert to decimal
            }
            else if (
                strcmp(argv[3], "h") == 0 ||
                strcmp(argv[3], "hex") == 0 ||
                strcmp(argv[3], "hexadecimal") == 0
            ) {
                param = 'h';    // Convert to hexadecimal
            }
            else if (
                strcmp(argv[3], "o") == 0 ||
                strcmp(argv[3], "oct") == 0 ||
                strcmp(argv[3], "octal") == 0
            ) {
                param = 'o';    // Convert to octal
            }
            else
            {
                printf(
                    "Invalid Syntax - %s is not a valid parameter for binary conversion.\n"
                    "Please refer to the official documentation for the supported bases, "
                    "or use the help option.\n", 
                    argv[3]
                );
                return 1;
            }
        }
        else
        {
            printf(
                "Invalid Syntax - %s is not a valid option for binary mode.\n"
                "Please refer to official documentation to understand the available options, "
                "or use the help option.\n", 
                argv[2]
            );
            return 1;
        }

        if (!await_user_input())
        {
            printf(
                "Error: Could not open Notepad.exe.\n"
                "Input parsing is not available at this time, please try again later.\n"
            );
            return 1;
        }

        n = read_temp_file_utf8();
        if (!n)
            return 1;

        s_n = convert(n, BINARY, BINARY);   // Self-conversion to sanitize
        if (!s_n)
        {
            printf("Please enter a valid number.\n");
            free(n);
            return 1;
        }

        if (option == 'c')
        {
            if (argc > 4)
            {
                printf("Warning - Extra arguments will be ignored...\n");
            }

            switch(param)
            {
                case 'd':
                    o_n = convert(s_n, BINARY, DECIMAL);
                break;

                case 'h':
                    o_n = convert(s_n, BINARY, HEXADECIMAL);
                break;

                case 'o':
                    o_n = convert(s_n, BINARY, OCTAL);
                break;
            }

            if (!o_n)
            {
                free(s_n);
                free(n);
                return 1;
            }

            printf("\n%s\n", o_n);
            if (copy_utf8_to_clipboard(o_n))
                printf("Copied value to clipboard!\n");

            free(o_n);
            free(s_n);
            free(n);
        }
        else
        {
            if (argc > 3)
            {
                printf("Warning - Extra arguments will be ignored...\n");
            }

            o_n = next_number(s_n, BINARY);
        }
    }

    /* -------- DECIMAL MODE -------- */
    else if (
        strcmp(argv[1], "decimal") == 0 || 
        strcmp(argv[1], "normal") == 0
    ) { 
        if (
            strcmp(argv[2], "r")  == 0 || 
            strcmp(argv[2], "run") == 0
        ) {
            option = 'r';       // Run
        }
        else if (
            strcmp(argv[2], "s") == 0 || 
            strcmp(argv[2], "solorun") == 0
        ) {
            option = 's';       // Solorun
        }
        else if (
            strcmp(argv[2], "c") == 0 ||
            strcmp(argv[2], "convert") == 0
        ) {
            option = 'c';       // Convert

            // Parse parameter
            if (
                strcmp(argv[3], "b") == 0 ||
                strcmp(argv[3], "bin") == 0 ||
                strcmp(argv[3], "binary") == 0
            ) {
                param = 'b';    // Convert to binary
            }
            else if (
                strcmp(argv[3], "h") == 0 ||
                strcmp(argv[3], "hex") == 0 ||
                strcmp(argv[3], "hexadecimal") == 0
            ) {
                param = 'h';    // Convert to hexadecimal
            }
            else if (
                strcmp(argv[3], "o") == 0 ||
                strcmp(argv[3], "oct") == 0 ||
                strcmp(argv[3], "octal") == 0
            ) {
                param = 'o';    // Convert to octal
            }
            else
            {
                printf(
                    "Invalid Syntax - %s is not a valid parameter for decimal conversion.\n"
                    "Please refer to the official documentation for the supported bases, "
                    "or use the help option.\n", 
                    argv[3]
                );
                return 1;
            }
        }
        else
        {
            printf(
                "Invalid Syntax - %s is not a valid option for decimal mode.\n"
                "Please refer to official documentation to understand the available options, "
                "or use the help option.\n", 
                argv[2]
            );
            return 1;
        }

        if (!await_user_input())
        {
            printf(
                "Error: Could not open Notepad.exe.\n"
                "Input parsing is not available at this time, please try again later.\n"
            );
            return 1;
        }

        n = read_temp_file_utf8();
        if (!n)
            return 1;

        s_n = convert(n, DECIMAL, DECIMAL);   // Self-conversion to sanitize
        if (!s_n)
        {
            printf("Please enter a valid number.\n");
            free(n);
            return 1;
        }

        if (option == 'c')
        {
            if (argc > 4)
            {
                printf("Warning - Extra arguments will be ignored...\n");
            }

            switch(param)
            {
                case 'b':
                    o_n = convert(s_n, DECIMAL, BINARY);
                break;

                case 'h':
                    o_n = convert(s_n, DECIMAL, HEXADECIMAL);
                break;

                case 'o':
                    o_n = convert(s_n, DECIMAL, OCTAL);
                break;
            }

            if (!o_n)
            {
                free(s_n);
                free(n);
                return 1;
            }

            printf("\n%s\n", o_n);
            if (copy_utf8_to_clipboard(o_n))
                printf("Copied value to clipboard!\n");

            free(o_n);
            free(s_n);
            free(n);
        }
        else
        {
            if (argc > 3)
            {
                printf("Warning - Extra arguments will be ignored...\n");
            }

            // Prepare args for function call
            enum Radix base = DECIMAL;
            struct Arg list[] = 
            {
                ARG(ARG_CONST_CHAR_PTR, s_n),
                ARG(ARG_INT, &base)
            };

            call.args_list = list;
            call.incrementer_function = (void *)next_number;
            call.formatter_function = NULL;
            call.mode = RADIX_MODE;
            
            // TODO: Call Dispatcher/Runner
        }
    }

    /* -------- EMOJI MODE -------- */
    else if (strcmp(argv[1], "emoji") == 0)
    {
        if (
            strcmp(argv[2], "r") == 0 ||
            strcmp(argv[2], "run") == 0
        ) {
            option = 'r';   // Run
        }
        else if (
            strcmp(argv[2], "s") == 0 ||
            strcmp(argv[2], "solorun") == 0
        ) {
            option = 's';   // Solorun
        } else if (
            strcmp(argv[2], "c") == 0 ||
            strcmp(argv[2], "e") == 0 ||
            strcmp(argv[2], "convert") == 0 ||
            strcmp(argv[2], "emojify") == 0
        ) {
            option = 'c';   // Convert
        }
        else
        {
            printf(
                "Invalid Syntax - %s is not a valid option for emoji mode.\n"
                "Please refer to official documentation to understand the available options, "
                "or use the help option.\n",
                argv[2]
            );
            return 1;
        }

        if (!await_user_input())
        {
            printf(
                "Error: Could not open Notepad.exe.\n"
                "Input parsing is not available at this time, please try again later.\n"
            );
            return 1;
        }

        n = read_temp_file_utf8();
        if (!n)
            return 1;

        if (!is_valid_number(n))
        {
            printf("Bad Input - Invalid number.\n");
            free(n);
            return 1;
        }

        s_n = strip_leading_zeroes(n);
        if (!s_n)
        {
            free(n);
            return 1;
        }

        if (argc > 3)
        {
            printf("Warning - Extra arguments will be ignored...\n");
        }

        if (option == 'c')
        {
            o_n = number_to_emoji(s_n);
            if (!o_n)
            {
                free(s_n);
                free(n);
                return 1;
            }

            printf("\n%s\n", o_n);
            if (copy_utf8_to_clipboard(o_n))
                printf("Copied value to clipboard!\n");

            free(o_n);
            free(s_n);
            free(n);
        }
        else
        {
            // Prepare args for function call
            struct Arg list[] = { ARG(ARG_CHAR_PTR, s_n) };

            call.args_list = list;
            call.formatter_function = number_to_emoji;
            call.incrementer_function = increment_numstring;
            call.mode = EMOJI_MODE;

            // TODO: Figure out what to do with incrementer function and emojification function
        }
    }

    /* -------- HEXADECIMAL MODE -------- */
    else if (strcmp(argv[1], "hexadecimal") == 0)
    {
        if (
            strcmp(argv[2], "r") == 0 ||
            strcmp(argv[2], "run") == 0
        ) {
            option = 'r';       // Run
        }
        else if (
            strcmp(argv[2], "s") == 0 ||
            strcmp(argv[2], "solorun") == 0
        ) {
            option = 's';       // Solorun
        }
        else if (
            strcmp(argv[2], "c") == 0 ||
            strcmp(argv[2], "convert") == 0
        ) {
            option = 'c';       // Convert

            // Parse parameter
            if (
                strcmp(argv[3], "b") == 0 ||
                strcmp(argv[3], "bin") == 0 ||
                strcmp(argv[3], "binary") == 0
            ) {
                param = 'b';    // Convert to binary
            }
            else if (
                strcmp(argv[3], "d") == 0 ||
                strcmp(argv[3], "dec") == 0 ||
                strcmp(argv[3], "decimal") == 0
            ) {
                param = 'd';    // Convert to decimal
            }
            else if (
                strcmp(argv[3], "o") == 0 ||
                strcmp(argv[3], "oct") == 0 ||
                strcmp(argv[3], "octal") == 0
            ) {
                param = 'o';    // Convert to octal
            }
            else
            {
                printf(
                    "Invalid Syntax - %s is not a valid parameter for hexadecimal conversion.\n"
                    "Please refer to the official documentation for the supported bases, "
                    "or use the help option.\n", 
                    argv[3]
                );
                return 1;
            }
        }
        else
        {
            printf(
                "Invalid Syntax - %s is not a valid option for hexadecimal mode.\n"
                "Please refer to official documentation to understand the available options, "
                "or use the help option.\n",
                argv[2]
            );
            return 1;
        }

        if (!await_user_input())
        {
            printf(
                "Error: Could not open Notepad.exe.\n"
                "Input parsing is not available at this time, please try again later.\n"
            );
            return 1;
        }

        n = read_temp_file_utf8();
        if (!n)
            return 1;

        s_n = convert(n, HEXADECIMAL, HEXADECIMAL);     // Self-conversion to sanitize
        if (!s_n)
        {
            printf("Please enter a valid number.\n");
            free(n);
            return 1;
        }

        if (option == 'c')
        {
            if (argc > 4)
            {
                printf("Warning - Extra arguments will be ignored...\n");
            }

            switch(param)
            {
                case 'b':
                    o_n = convert(s_n, HEXADECIMAL, BINARY);
                break;

                case 'd':
                    o_n = convert(s_n, HEXADECIMAL, DECIMAL);
                break;

                case 'o':
                    o_n = convert(s_n, HEXADECIMAL, OCTAL);
                break;
            }

            if (!o_n)
            {
                free(s_n);
                free(n);
                return 1;
            }

            printf("\n%s\n", o_n);
            if (copy_utf8_to_clipboard(o_n))
                printf("Copied value to clipboard!\n");

            free(o_n);
            free(s_n);
            free(n);
        }
        else
        {
            if (argc > 3)
            {
                printf("Warning - Extra arguments will be ignored...\n");
            }

            // Prepare args for function call
            enum Radix base = HEXADECIMAL;
            struct Arg list[] =
            {
                ARG(ARG_CONST_CHAR_PTR, s_n),
                ARG(ARG_INT, &base)
            };

            call.args_list = list;
            call.incrementer_function = (void *)next_number;
            call.formatter_function = NULL;
            call.mode = RADIX_MODE;

            // TODO: Call Dispatcher/Runner
        }
    }
    else if (strcmp(argv[1], "increasing") == 0)
    {}
    else if (strcmp(argv[1], "japanese") == 0)
    {}

    /* -------- OCTAL MODE -------- */
    else if (strcmp(argv[1], "octal") == 0)
    {
        if (
            strcmp(argv[2], "r")  == 0 || 
            strcmp(argv[2], "run") == 0
        ) {
            option = 'r';       // Run
        }
        else if (
            strcmp(argv[2], "s") == 0 || 
            strcmp(argv[2], "solorun") == 0
        ) {
            option = 's';       // Solorun
        }
        else if (
            strcmp(argv[2], "c") == 0 ||
            strcmp(argv[2], "convert") == 0
        ) {
            option = 'c';       // Convert

            // Parse parameter
            if (
                strcmp(argv[3], "b") == 0 ||
                strcmp(argv[3], "bin") == 0 ||
                strcmp(argv[3], "binary") == 0
            ) {
                param = 'b';    // Convert to binary
            }
            else if (
                strcmp(argv[3], "d") == 0 ||
                strcmp(argv[3], "dec") == 0 ||
                strcmp(argv[3], "decimal") == 0
            ) {
                param = 'd';    // Convert to decimal
            }
            else if (
                strcmp(argv[3], "h") == 0 ||
                strcmp(argv[3], "hex") == 0 ||
                strcmp(argv[3], "hexadecimal") == 0
            ) {
                param = 'h';    // Convert to hexadecimal
            }
            else
            {
                printf(
                    "Invalid Syntax - %s is not a valid parameter for octal conversion.\n"
                    "Please refer to the official documentation for the supported bases, "
                    "or use the help option.\n", 
                    argv[3]
                );
                return 1;
            }
        }
        else
        {
            printf(
                "Invalid Syntax - %s is not a valid option for octal mode.\n"
                "Please refer to official documentation to understand the available options, "
                "or use the help option.\n", 
                argv[2]
            );
            return 1;
        }

        if (!await_user_input())
        {
            printf(
                "Error: Could not open Notepad.exe.\n"
                "Input parsing is not available at this time, please try again later.\n"
            );
            return 1;
        }

        n = read_temp_file_utf8();
        if (!n)
            return 1;

        s_n = convert(n, OCTAL, OCTAL);   // Self-conversion to sanitize
        if (!s_n)
        {
            printf("Please enter a valid number.\n");
            free(n);
            return 1;
        }

        if (option == 'c')
        {
            if (argc > 4)
            {
                printf("Warning - Extra arguments will be ignored...\n");
            }

            switch(param)
            {
                case 'b':
                    o_n = convert(s_n, OCTAL, BINARY);
                break;

                case 'd':
                    o_n = convert(s_n, OCTAL, DECIMAL);
                break;

                case 'h':
                    o_n = convert(s_n, OCTAL, HEXADECIMAL);
                break;
            }

            if (!o_n)
            {
                free(s_n);
                free(n);
                return 1;
            }

            printf("\n%s\n", o_n);
            if (copy_utf8_to_clipboard(o_n))
                printf("Copied value to clipboard!\n");

            free(o_n);
            free(s_n);
            free(n);
        }
        else
        {
            if (argc > 3)
            {
                printf("Warning - Extra arguments will be ignored...\n");
            }

            // Prepare args for function call
            enum Radix base = OCTAL;
            struct Arg list[] = 
            {
                ARG(ARG_CONST_CHAR_PTR, s_n),
                ARG(ARG_INT, &base)
            };

            call.args_list = list;
            call.incrementer_function = (void *)next_number;
            call.formatter_function = NULL;
            call.mode = RADIX_MODE;
            
            // TODO: Call Dispatcher/Runner
        }
    }
    else if (strcmp(argv[1], "meme") == 0)
    {}
    else if (strcmp(argv[1], "mirror") == 0)
    {}
    else if (strcmp(argv[1], "morse") == 0)
    {}
    else if (strcmp(argv[1], "norep") == 0)
    {}
    else if (strcmp(argv[1], "nwn") == 0)
    {}
    else if (strcmp(argv[1], "nwnwn") == 0)
    {}
    else if (strcmp(argv[1], "nwnwnn") == 0)
    {}
    else if (strcmp(argv[1], "palindrome") == 0)
    {}
    else if (strcmp(argv[1], "rep") == 0)
    {}
    else if (strcmp(argv[1], "roman") == 0)
    {}
    else if (strcmp(argv[1], "tally") == 0)
    {}
    else
    {
        printf(
            "Invalid Syntax - %s is not a valid use mode.\n"
            "To learn more, please refer to the official documentation of this tool.\n",
            argv[1]
        );
        return 1;
    }
}
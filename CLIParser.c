#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include "ClipboardFunctions.h"
#include "Inputter.h"
#include "MiscUtils.h"

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
 *      |                |               |                        | Numbers within Numbers.
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

    char option;
    char param[4] = {0};

    char 
        *n,         // Pointer for number from user input
        *s_n,       // Pointer for sanitized number (in case of input sanitization functions)
        *n_n,       // Pointer for next number in sequence (can be reused)

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

    if (strcmp(argv[1], "binary") == 0)
    {
        if (!await_user_input())
        {
            printf(
                "Error: Could not open Notepad.exe.\n"
                "Input parsing is not available at this time, please try again later.\n"
            );
            return 1;
        }

        n = read_temp_file_utf8();

        
    }
}
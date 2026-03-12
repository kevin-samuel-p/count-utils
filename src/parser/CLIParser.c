#include <stdio.h>
#include <stdlib.h>

#include "ModeSelector.h"
#include "OptionSelector.h"

#include "CustomActions.h"
#include "HelpActions.h"
#include "Inputter.h"
#include "MiscUtils.h"
#include "ParamParser.h"
#include "RunActions.h"

int main(int argc, char *argv[])
{
    int exit_status = 1;

    enum CountMode mode;
    char option, param;
    char *input = NULL;
    unsigned long long *val = NULL;
    bool isNumber;

    printf("\n\n");       // Convenient spacing, easier on the eyes

    // If no args, print info
    if (argc == 1)
    {
        if (!read_docs(0))
            goto Done;
        goto Success;
    }

    // Find mode
    mode = find_mode(argv[1]);
    if (!~mode)
    {
        printf(
            "Invalid Syntax - Mode %s does not exist.\n"
            "For supported modes, type\n"
            "    count help\n",
            argv[1]
        );
        goto Done;
    }

    if (argc == 2)
    {
        if (!read_docs(mode))
            goto Done;
        goto Success;
    }

    // Help mode
    if (mode == MODE_HELP)
    {
        enum CountMode helpTopic = find_mode(argv[2]);
        if (!~helpTopic || helpTopic == MODE_HELP)
        {
            printf(
                "Invalid Syntax - Command `count help %s` does not exist.\n"
                "For supported commands, type\n"
                "    count help\n",
                argv[2]
            );
            goto Done;
        }

        if (argc > 3)
            printf("Warning - Extra arguments will be ignored...\n\n");
            
        if (!read_docs(helpTopic))
            goto Done;

        goto Success;
    }

    // If not help mode, then counting mode
    // Find option
    option = is_run_option(argv[2]);

    if (!option)
        option = is_custom_option(argv[2], mode);

    if (!option)
    {
        printf(
            "Invalid Syntax - Command `count %s %s` does not exist.\n"
            "For supported commands, type\n"
            "    count help %s\n",
            argv[1], argv[2], argv[1]
        );
        goto Done;
    }

    // Check for parameter
    bool takes_param = param_satisfiability(mode, option);
    if (takes_param)
    {
        if (argc < 4 && mode != MODE_ROMAN && mode != MODE_TALLY)
        {
            printf(
                "Invalid Syntax - Expected fourth parameter.\n"
                "    count %s %s <param>\n"
                "For more information, type `count help %s`\n",
                argv[1], argv[2], argv[1]
            );
            goto Done;
        }

        param = parse_param((argc >= 4) ? argv[3] : NULL, mode);
        if (!param)
        {
            printf(
                "Invalid Syntax - Command `count %s %s %s` does not exist.\n"
                "For supported commands, type\n"
                "    count help %s\n",
                argv[1], argv[2], (argc >= 4) ? argv[3] : NULL, argv[1]
            );
            goto Done;
        }
    }

    if ((takes_param) ? argc > 4 : argc > 3)
        printf("Warning - Extra arguments will be ignored...\n\n");

    // Create temporary input file
    if (!create_temp_file())
    {
        printf(
            "Error - Could not create input file.\n"
            "Please make sure you are running this tool as an administrator.\n"
        );
        goto Done;
    }

    if (!await_user_input())
    {
        printf(
            "Error - Could not open default file editor.\n"
            "Please check whether your computer has a default file editor.\n"
        );
        goto Done;
    }

    // Read user input
    input = read_temp_file_utf8();
    if (!input)
        goto Done;

    // Perform action
    switch(mode)
    {
        case MODE_ALPHA:
            sanitize(&input, false);
            if (!input) goto Done;
            if (!run_alpha(input, option)) goto Done;
        break;

        case MODE_BINARY:
        case MODE_DECIMAL:
        case MODE_HEXADECIMAL:
        case MODE_OCTAL:
        case MODE_TERNARY:
            sanitize(&input, true);
            if (!input || !is_convertible(input, mode)) goto Done;
            if (option == 'c' && !convert_base(input, mode, param)) goto Done;
            if (option != 'c' && !run_radix(input, option, mode)) goto Done;
        break;

        case MODE_EMOJI:
            if (!sanitize(&input, true))
            {
                if (!input) goto Done;
                printf("Bad Input - Invalid number.\n");
                goto Done;
            }
            if (option == 'c' && !convert_emoji(input)) goto Done; 
            if (option != 'c' && !run_emoji(input, option)) goto Done;
        break;

        case MODE_FACTORIZATION:
            isNumber = sanitize(&input, false);
            if (option == 'c' && !convert_factorization(input, isNumber)) goto Done;
            if (option != 'c' && !run_factorization(input, option)) goto Done;
        break;

        case MODE_INCREASING:
            if (!sanitize(&input, true)) 
            {
                if (!input) goto Done;
                printf("Bad Input - Invalid number.\n");
                goto Done;
            }
            if (option == 'c') check_increasing(input);
            if (option != 'c' && !run_increasing(input, option)) goto Done;
        break;

        case MODE_JAPANESE:
            isNumber = sanitize(&input, true);
            if (!input) goto Done;
            if (option == 'c' && !convert_japanese(input, isNumber)) goto Done;
            if (option != 'c' && !run_japanese(input, option, isNumber)) goto Done;
        break;

        case MODE_MEME:
            if (!sanitize(&input, true))
            {
                if (!input) goto Done;
                printf("Bad Input - Invalid number.\n");
                goto Done;
            }
            if (!run_meme(input, option, param)) goto Done;
        break;

        case MODE_MIRROR:
            if (!sanitize(&input, param == 'n')) 
            {
                if (!input) goto Done;
                printf("Bad Input - Invalid number.\n");
                goto Done;
            }
            if (!run_mirror(input, option, param)) goto Done;
        break;

        case MODE_MORSE:
            isNumber = sanitize(&input, true);
            if (!input) goto Done;
            if (option == 'c' && !convert_morse(input, isNumber)) goto Done;
            if (option != 'c' && !run_morse(input, option, isNumber)) goto Done;
        break;

        case MODE_NOREP:
            if (!sanitize(&input, true)) 
            {
                if (!input) goto Done;
                printf("Bad Input - Invalid number.\n");
                goto Done;
            }
            string_to_number(input, &val);
            if (!val) goto Done;

            if (option == 'c') check_norep(*val);
            if (option != 'c' && !run_norep(*val, option)) goto Done;
        break;

        case MODE_NWN:
        case MODE_NWNWN:
        case MODE_NWNWNN:
            sanitize(&input, false);
            if (!input) goto Done;
            if (!run_nwns(input, option, mode)) goto Done;
        break;

        case MODE_PALINDROME:
            if (!sanitize(&input, true)) 
            {
                if (!input) goto Done;
                printf("Bad Input - Invalid number.\n");
                goto Done;
            }
            if (option == 'c') check_palindrome(input);
            if (option != 'c' && !run_palindrome(input, option)) goto Done;
        break;

        case MODE_REP:
            if (!sanitize(&input, true)) 
            {
                if (!input) goto Done;
                printf("Bad Input - Invalid number.\n");
                goto Done;
            }
            string_to_number(input, &val);
            if (!val) goto Done;

            if (option == 'c') check_rep(*val);
            if (option != 'c' && !run_rep(*val, option)) goto Done;
        break;

        case MODE_ROMAN:
            isNumber = sanitize(&input, true);
            if (!input) goto Done;
            if (option == 'c' && !convert_roman(input, isNumber, param)) goto Done;
            if (option != 'c' && !run_roman(input, option, param, isNumber)) goto Done;
        break;

        case MODE_TALLY:
            isNumber = sanitize(&input, true);
            if (!input) goto Done;
            if (option == 'c' && !convert_tally(input, isNumber)) goto Done;
            if (option != 'c' && !run_tally(input, option, param, isNumber)) goto Done;
        break;
    }

Success:
    --exit_status;

Done:
    printf("\n\n");     // Convenient spacing, easier on the eyes

    delete_temp_file();
    free(input);
    free(val);

    return exit_status;
}
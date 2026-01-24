#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>
#include <windows.h>


enum RunMode
{
    EMOJI_MODE,
    INCREASING_MODE,
    JAPANESE_MODE,
    RADIX_MODE,
    MEME_MODE,
    MIRROR_MODE,
    MORSE_MODE,
    NOREP_MODE,
    NWN_MODE,
    NWNWN_MODE,
    NWNWNN_MODE,
    PALINDROME_MODE,
    REP_MODE,
    ROMAN_MODE,
    TALLY_MODE
};

enum ArgType
{
    ARG_INT,
    ARG_ULLONG,
    ARG_CHAR,
    ARG_CHAR_PTR,
    ARG_CONST_CHAR_PTR,
    ARG_CHAR_DOUBLE_PTR,
    ARG_WCHAR_PTR,
    ARG_CONST_WCHAR_PTR
};

struct Arg
{
    enum ArgType type;
    void *arg_ptr;
};

struct Func_Call
{
    enum RunMode mode;          // Running mode enum to match function signature(s)
    void *formatter_function;   // Function pointer to formatting function (for some modules)
    void *incrementer_function; // Function pointer to main incrementer function 
    struct Arg *args_list;      // List of arguments for both functions in sequence
};


#define ARG(type_enum, value_ptr)                                   \
    (struct Arg){ type_enum, (void *)(value_ptr) }

#define ARG_AS(type, ptr) (*(type *)(ptr))

#define DISPATCH_ARG_ASSIGN(dst, arg)                               \
    do {                                                            \
        switch ((arg).type) {                                       \
            case ARG_INT:                                           \
                (dst) = ARG_AS(int, (arg).arg_ptr);                 \
                break;                                              \
            case ARG_ULLONG:                                        \
                (dst) = ARG_AS(unsigned long long, (arg).arg_ptr);  \
                break;                                              \
            case ARG_CHAR:                                          \
                (dst) = ARG_AS(char, (arg).arg_ptr);                \
                break;                                              \
            case ARG_CHAR_PTR:                                      \
                (dst) = (char *)(arg).arg_ptr;                      \
                break;                                              \
            case ARG_CONST_CHAR_PTR:                                \
                (dst) = (const char *)(arg).arg_ptr;                \
                break;                                              \
            case ARG_WCHAR_PTR:                                     \
                (dst) = (wchar_t *)(arg).arg_ptr;                   \
                break;                                              \
            case ARG_CONST_WCHAR_PTR:                               \
                (dst) = (const wchar_t *)(arg).arg_ptr;             \
                break;                                              \
            case ARG_CHAR_DOUBLE_PTR:                               \
                (dst) = (char **)(arg).arg_ptr;                     \
                break;                                              \
            default:                                                \
                fprintf(stderr, "Unsupported ArgType\n");           \
                break;                                              \
        }                                                           \
    } while (0)

// #define DISPATCH_RETURN(ret_type, value)                            \
//     do {                                                            \
//         switch (ret_type) {                                         \
//             case RET_INT:                                           \
//                 return (void *)(intptr_t)(value);                   \
//             case RET_BOOL:                                          \
//                 return (void *)(intptr_t)(value);                   \
//             case RET_CHAR_PTR:                                      \
//                 return (void *)(value);                             \
//             case RET_WCHAR_PTR:                                     \
//                 return (void *)(value);                             \
//             case RET_VOID:                                          \
//                 return NULL;                                        \
//             default:                                                \
//                 return NULL;                                        \
//         }                                                           \
//     } while (0)


void *dispatcher(struct Func_Call *call)
{
    switch (call->mode)
    {
        case EMOJI_MODE:
        {
            char *arg0;

            DISPATCH_ARG_ASSIGN(arg0, call->args_list[0]);

            // Casting generic function pointers to respective signatures
            void (*incr)(char **) = call->incrementer_function;
            char *(*form)(const char *) = call->formatter_function;

            incr(&arg0);    // Alters arg0
            if (!arg0)
                return NULL;

            char *res = form(arg0);
            return (void *)res;
        }

        case RADIX_MODE:
        {
            const char *arg0;
            int arg1;

            DISPATCH_ARG_ASSIGN(arg0, call->args_list[0]);
            DISPATCH_ARG_ASSIGN(arg1, call->args_list[1]);

            // Cast the generic function pointer to actual signature
            char *(*incr)(const char *, int) = call->incrementer_function;

            char *res = incr(arg0, arg1);
            return (void *)res;
        }

        case RET_VOID:
        {
            // Example for void functions
            int arg0;
            if (call->arg_count > 0) DISPATCH_ARG_ASSIGN(arg0, call->args_list[0]);

            void (*func)(int) = call->func;
            func(arg0);

            DISPATCH_RETURN(RET_VOID, 0);
        }

        default:
            fprintf(stderr, "Unsupported return type\n");
            return NULL;
    }
}

    
/**
 *  Run Modes:
 *      r - run (normal duos run)
 *      s - solorun
 */
bool runner();
#ifndef RUNNER_H
#define RUNNER_H

enum RunMode
{
    EMOJI_MODE,
    INCREASING_MODE,
    JAPANESE_MODE,
    MEME_MODE,
    MIRROR_MODE,
    MORSE_MODE,
    NOREP_MODE,
    NWN_MODE,
    NWNWN_MODE,
    NWNWNN_MODE,
    PALINDROME_MODE,
    RADIX_MODE,
    REP_MODE,
    ROMAN_MODE,
    TALLY_MODE
};

// enum ArgType
// {
//     ARG_INT,
//     ARG_ULLONG,
//     ARG_CHAR,
//     ARG_CHAR_PTR,
//     ARG_CHAR_DOUBLE_PTR,
//     ARG_WCHAR_PTR
// };

// struct Arg
// {
//     enum ArgType type;
//     void *arg_ptr;
// };

struct Func_Call
{
    enum RunMode mode;          // Running mode enum to match function signature(s)

    union
    {
        void *formatter;        // Function pointer to formatting function (for some modules)
        void *incrementer;      // Function pointer to main incrementer function 
    } 
    func;                       // Union to store function pointers

    union
    {
        unsigned long long num_ullong;
        long long num_llong;
        char *num_char_ptr;
        wchar_t *num_wchar_ptr;
    } arg;                      // Union to store main starting argument

    char *extra_args;           // constant string pointer for enums (ints) or chars passed as extra args
};


// #define ARG(type_enum, value_ptr)                                   \
//     (struct Arg){ type_enum, (void *)(value_ptr) }

// #define ARG_AS(type, ptr) (*(type *)(ptr))

// #define DISPATCH_ARG_ASSIGN(dst, arg)                               \
//     do {                                                            \
//         switch ((arg).type) {                                       \
//             case ARG_INT:                                           \
//                 (dst) = ARG_AS(int, (arg).arg_ptr);                 \
//                 break;                                              \
//             case ARG_ULLONG:                                        \
//                 (dst) = ARG_AS(unsigned long long, (arg).arg_ptr);  \
//                 break;                                              \
//             case ARG_CHAR:                                          \
//                 (dst) = ARG_AS(char, (arg).arg_ptr);                \
//                 break;                                              \
//             case ARG_CHAR_PTR:                                      \
//                 (dst) = (char *)(arg).arg_ptr;                      \
//                 break;                                              \                                          \
//             case ARG_WCHAR_PTR:                                     \
//                 (dst) = (wchar_t *)(arg).arg_ptr;                   \
//                 break;                                              \
//             case ARG_CHAR_DOUBLE_PTR:                               \
//                 (dst) = (char **)(arg).arg_ptr;                     \
//                 break;                                              \
//             default:                                                \
//                 fprintf(stderr, "Unsupported ArgType\n");           \
//                 break;                                              \
//         }                                                           \
//     } while (0)


#endif /* RUNNER_H */
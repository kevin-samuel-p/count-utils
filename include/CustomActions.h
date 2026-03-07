#ifndef CUSTOMACTIONS_H
#define CUSTOMACTIONS_H

#include <stdbool.h>
#include "ModeSelector.h"

bool is_convertible(const char *, enum CountMode);
bool convert_base(const char *, enum CountMode, char);
bool convert_emoji(const char *);
bool convert_japanese(const char *, bool);
bool convert_morse(const char *, bool);
bool convert_roman(const char *, bool, char);
bool convert_tally(const char *, bool);

void check_increasing(const char *);
void check_norep(unsigned long long);
void check_palindrome(const char *);
void check_rep(unsigned long long);

#endif
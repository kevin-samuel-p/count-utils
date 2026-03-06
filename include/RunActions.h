#ifndef RUNACTIONS_H
#define RUNACTIONS_H

#include <stdbool.h>
#include "ModeSelector.h"

bool run_radix(const char *, char, enum CountMode);
bool run_emoji(const char *, char);
bool run_increasing(const char *, char);
bool run_japanese(const char *, char, bool);
bool run_meme(const char *, char, char);
bool run_mirror(const char *, char, char);
bool run_morse(const char *, char, bool);
bool run_norep(unsigned long long, char);
bool run_nwns(const char *, char, enum CountMode);
bool run_palindrome(const char *, char);
bool run_rep(unsigned long long, char);
bool run_roman(const char *, char, char, bool);
bool run_tally(const char *, char, char, bool);

#endif
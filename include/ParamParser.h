#ifndef PARAMPARSER_H
#define PARAMPARSER_H

#include "ModeSelector.h"

char parse_radix_param(const char *, enum CountMode);
char parse_meme_param(const char *);
char parse_mirror_param(const char *);
char parse_roman_param(const char *);
char parse_tally_param(const char *);

#endif
#ifndef PARAMPARSER_H
#define PARAMPARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include "ModeSelector.h"

bool param_satisfiability(enum CountMode, char);
char parse_param(const char *, enum CountMode);

#endif
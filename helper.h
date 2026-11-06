#ifndef HELPER_H

#define HELPER_H

#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

bool string_is_digit(char *str);
bool string_is_puncdig(char *str);

size_t get_filesize(FILE *source);

bool assign_buffer_literal(char *string_literal, char *buffer);

#endif
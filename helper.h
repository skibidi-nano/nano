#ifndef HELPER_H

#define HELPER_H

#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

bool string_is_digit(char *str);
bool string_is_puncdig(char *str);
bool string_is_sing_char(const char *str);

size_t get_filesize(FILE *source);

char* assign_buffer_literal(char *buffer);

uint64_t hash_djb2(const char *str);

#endif
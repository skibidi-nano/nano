#ifndef MAIN_H

#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define NAME_BUFFER_SIZE 512
#define TOKEN_LITERAL_SIZE 256
#define TEMPORARY_TOKEN_ARR_SIZE_WILL_BE_DYNAMIC_SOON 2048

extern char **to_free;
extern int tf_index;

#endif
#include "main.h"
#include "helper.h"

int lexer(FILE *target);
int tokenize(char *file);
token compare_literals(char* literal, char literal_extra);

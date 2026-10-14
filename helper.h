#include <stdio.h>

typedef enum
{
    KEYWORD,
    IDENTIFIER,
    ASSIGNMENT_OP,
    INT_LITERAL,
    STRING,
    N_BRACE_OP,
    C_BRACE_OP,
    B_BRACE_OP,
    N_BRACE_CL,
    C_BRACE_CL,
    B_BRACE_CL,
    PLUS,
    MINUS,
    SLASH,
    AMPERSAND,
}
label;

typedef struct
{
    label token_type;
    char *string_literal;
    int line;
    int column;
}
token;

typedef struct
{
    int one;
    int two;
    int buffer;
}
index_type;

bool string_is_alnum(char *str);

int get_filesize(FILE *source);
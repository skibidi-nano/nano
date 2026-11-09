#ifndef LEXER_H

#define LEXER_H

#include "main.h"
#include "helper.h"

typedef enum
{
    STRING,
    PUNCT,
    CHARS,
    NUMBER,
}
type;

typedef enum
{
    /* KEYWORDS */
    KW_INT,
    KW_CHAR,
    KW_BOOL,
    KW_FLOAT,
    KW_WHILE,
    KW_FOR,
    KW_IF,
    KW_ELSE,
    //////////////


    /* PUNCTUATION */
    PUNCT_LPAREN,
    PUNCT_RPAREN,
    PUNCT_LBRACKET,
    PUNCT_RBRACKET,
    PUNCT_LBRACE,
    PUNCT_RBRACE,
    PUNCT_SCOLON,
    PUNCT_COLON,
    PUNCT_COMMA,
    PUNCT_FULLSTOP,
    //////////////


    /* OPERATORS */

    /* ASSIGN */
    OP_ASSIGN,

    /* ARITHMETIC */
    OP_MINUS,
    OP_PLUS,
    OP_MULT,
    OP_DIV,
    OP_MOD,

    /* CMP */
    OP_EQ,
    OP_NOT_EQ,
    OP_GT,
    OP_LT,
    OP_GE,
    OP_LE,

    /* LOGIC */
    OP_AND,
    OP_OR,
    OP_NOT,
    //////////////

    
    /* LITERALS */
    LITERAL_INT,
    LITERAL_FLOAT,
    LITERAL_STRING,
    LITERAL_CHAR,
    //////////////

    SIGNAL_EOF,
    ERROR,
    


    /* IDENTIFIER */
    IDENTIFIER
    //////////////
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

int lexer(token *tokens, FILE *target);

token scan_string(void);
token scan_punct(void);
token scan_char(void);
token scan_num(void);

label match_types(char *input, type type);

/* FOR DEBUGGING*/

static const char *label_names[] = {
    /* KEYWORDS */
    "KW_INT",
    "KW_CHAR",
    "KW_BOOL",
    "KW_FLOAT",
    "KW_WHILE",
    "KW_FOR",
    "KW_IF",
    "KW_ELSE",

    /* PUNCTUATION */
    "PUNCT_LPAREN",
    "PUNCT_RPAREN",
    "PUNCT_LBRACKET",
    "PUNCT_RBRACKET",
    "PUNCT_LBRACE",
    "PUNCT_RBRACE",
    "PUNCT_SCOLON",
    "PUNCT_COLON",
    "PUNCT_COMMA",
    "PUNCT_FULLSTOP",

    /* ASSIGN */
    "OP_ASSIGN",

    /* ARITHMETIC */
    "OP_MINUS",
    "OP_PLUS",
    "OP_MULT",
    "OP_DIV",
    "OP_MOD",

    /* CMP */
    "OP_EQ",
    "OP_NOT_EQ",
    "OP_GT",
    "OP_LT",
    "OP_GE",
    "OP_LE",

    /* LOGIC */
    "OP_AND",
    "OP_OR",
    "OP_NOT",

    /* LITERALS */
    "LITERAL_INT",
    "LITERAL_FLOAT",
    "LITERAL_STRING",
    "LITERAL_CHAR",

    "SIGNAL_EOF",
    "ERROR",

    /* IDENTIFIER */
    "IDENTIFIER",
};

#endif
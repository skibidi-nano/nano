#ifndef LEXER_H

#define LEXER_H

#include "main.h"
#include "helper.h"

#include <stdint.h>

enum tokenhash: uint64_t 
{
    /* KEYWORDS */
    HASH_INT      = 193495088ULL,    // int
    HASH_CHAR     = 6385115235ULL,   // char
    HASH_BOOL     = 6385087377ULL,   // bool
    HASH_FLOAT    = 210712519067ULL, // float
    HASH_WHILE    = 210732529790ULL, // while
    HASH_FOR      = 193491852ULL,    // for
    HASH_IF       = 5863476ULL,      // if
    HASH_ELSE     = 6385192046ULL,   // else

    /* PUNCTUATION */
    HASH_LPAREN   = 177613ULL,       // (
    HASH_RPAREN   = 177614ULL,       // )
    HASH_LBRACKET = 177664ULL,       // [
    HASH_RBRACKET = 177666ULL,       // ]
    HASH_LBRACE   = 177696ULL,       // {
    HASH_RBRACE   = 177698ULL,       // }
    HASH_SEMCOL   = 177632ULL,       // ;
    HASH_COL      = 177631ULL,       // :
    HASH_COMMA    = 177617ULL,       // ,
    HASH_FSTOP    = 177619ULL,       // .

    /* OPERATORS */

    /* ASSIGN */
    HASH_ASSIGN       = 177634ULL,       // =

    /* ARITHMETIC */
    HASH_SUBT     = 177618ULL,       // -
    HASH_ADD      = 177616ULL,       // +
    HASH_MULT     = 177615ULL,       // *
    HASH_DIV      = 177620ULL,       // /
    HASH_MOD      = 177610ULL,       // %

    /* CMP/LOGIC */
    HASH_EQ      = 5861983ULL,      // ==
    HASH_UNEQ     = 5861059ULL,      // !=
    HASH_GT       = 177635ULL,       // >
    HASH_LT       = 177633ULL,       // <
    HASH_GTEQ     = 5862016ULL,      // >=
    HASH_LTEQ     = 5861950ULL,      // <=
    HASH_AND      = 5861201ULL,      // &&
    HASH_OR       = 5864125ULL,      // ||
    HASH_NOT      = 177606ULL        // !
};


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
    OP_SUBT,
    OP_ADD,
    OP_MULT,
    OP_DIV,
    OP_MOD,

    /* CMP */
    OP_EQ,
    OP_UNEQ,
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

label hash_string_label(const char *str);
label fallback_label (const char *str);

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
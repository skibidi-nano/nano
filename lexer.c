#include "lexer.h"

static char *file;
static size_t bytes_read;
static size_t file_index;
static int column = 0;

int lexer(token *tokens, FILE *target)
{
    size_t filesize = get_filesize(target); 
    
    file = malloc(filesize + 1);
    if (file == NULL) 
    {
        return 1;
    }
    bytes_read = fread(file, 1, filesize, target);
    file[bytes_read] = '\0';

    int line = 1;
    column = 0;
    int token_index = 0;
    file_index = 0;

    bool slsl_comment = false;
    bool slst_comment = false;

    while (file_index < bytes_read)
    {
        column++;

        #if VOL_DEBUG
            volatile unsigned char c = file[file_index];
        #else
            unsigned char c = file[file_index];
        #endif

        if ((file_index + 1) >= filesize + 1)
        {
            free(file);
            return -2;
        }
        #if VOL_DEBUG
            volatile unsigned char peek = file[file_index + 1];
        #else
            unsigned char peek = file[file_index + 1];
        #endif
        

        if (c == '\0')
        {
            tokens[token_index].token_type = SIGNAL_EOF;
            tokens[token_index].column = column;
            tokens[token_index].line = line;
            break;
        }
        else if (c == '\n')
        {
            line++;
            column = 0;
            if (slsl_comment)
            {
                slsl_comment = false;
            }
            if ((file_index + 1) >= filesize + 1)
            {
                
                free(file);
                return -2;
            }
            file_index++;
            continue;
        }
        else if (c  == ' ' || c == '\t')
        {
            if ((file_index + 1) >= filesize + 1)
            {
                free(file);
                return -2;
            }
            file_index++;
            continue;
        }
        else if ((c == '/' && peek == '/') || slsl_comment)
        {
            if (slsl_comment)
            {
                if ((file_index + 1) >= filesize + 1)
                {
                    free(file);
                    return -2;
                }
                file_index++;
                continue;
            }
            slsl_comment = true;
        }
        else if ((c == '/' && peek == '*') || slst_comment)
        {
            if ((c == '*' && peek == '/'))
            {
                slst_comment = false;
                if ((file_index + 2) >= filesize + 1)
                {
                    free(file);
                    return -2;
                }
                file_index += 2;
                continue;
            }
            else if (slst_comment)
            {
                if ((file_index + 1) >= filesize + 1)
                {
                    free(file);
                    return -2;
                }
            }
            slst_comment = true;
            file_index++;
            continue;
        }
        else if (c == '"')
        {
            tokens[token_index] = scan_string();
        }
        else if (ispunct(c))
        {
            tokens[token_index] = scan_punct();
        }
        else if (isalpha(c))
        {
            tokens[token_index] = scan_char();
        }
        else if (isdigit(c))
        {
            tokens[token_index] = scan_num();
        }
        else
        {
            tokens[token_index].token_type = ERROR;
            tokens[token_index].column = column;
            tokens[token_index].line = line; 
            free(file);
            return token_index;
        }

        tokens[token_index].column = column;
        tokens[token_index].line = line;

        if ((token_index + 1) >= TEMPORARY_TOKEN_ARR_SIZE_WILL_BE_DYNAMIC_SOON)
        {
            free(file);
            return -3;
        }
        token_index++;
    }

    free(file);

    return token_index;
}

token scan_string(void)
{
    char buffer[TOKEN_LITERAL_SIZE];
    int buffer_index = 0;
    bool string_brk = false;
    token current_token = {};

    while (true)
    {
        buffer[buffer_index] = file[file_index];

        if ((file_index + 1) > bytes_read)
        {
            current_token.token_type = ERROR;
            break;
        }
        file_index++;
        column++;

        if (buffer[buffer_index] == '"')
        {
            if (string_brk)
            {
                current_token.token_type = LITERAL_STRING;
                break;
            }
            string_brk = true;
        }
        else if (buffer[buffer_index] == '\0')
        {
            current_token.token_type = ERROR;
            break;
        }

        if ((buffer_index + 1) >= TOKEN_LITERAL_SIZE)
        {
            current_token.token_type = ERROR;
            break;
        }
        buffer_index++;
    }

    buffer[buffer_index] = '\0';

    current_token.string_literal = assign_buffer_literal(buffer);

    return current_token;
}

token scan_punct(void)
{
    char buffer[TOKEN_LITERAL_SIZE];
    int buffer_index = 0;
    int peek = file_index + 1;
    token current_token = {};

    while (true)
    {
        buffer[buffer_index] = file[file_index];
        file_index++;
        column++;

        if (file_index + 1 < bytes_read && !ispunct(file[file_index]) && !ispunct(file[peek]))
        {

            buffer[buffer_index] = file[peek];
            buffer[buffer_index + 1] = '\0';
            if (hash_string_label(buffer) != IDENTIFIER)
            {
                current_token.token_type = hash_string_label(buffer);
            }
            else
            {
                buffer[buffer_index] = '\0';
                current_token.token_type = hash_string_label(buffer);
            }

            break;
        }

        if ((buffer_index + 1) >= TOKEN_LITERAL_SIZE)
        {
            current_token.token_type = ERROR;
            break;
        }
        buffer_index++;
        peek++;
    }

    current_token.string_literal = assign_buffer_literal(buffer);

    return current_token;
}

token scan_char(void)
{
    char buffer[TOKEN_LITERAL_SIZE];
    int buffer_index = 0;
    token current_token = {};

    while (true)
    {
        buffer[buffer_index] = file[file_index];
        column++;

        if (!isalnum(buffer[buffer_index]) && buffer[buffer_index] != '_')
        {
            buffer[buffer_index] = '\0';
            current_token.token_type = hash_string_label(buffer);
            break;
        }

        if ((file_index + 1) > bytes_read)
        {
            current_token.token_type = ERROR;
            break;
        }
        file_index++;
        if ((buffer_index + 1) >= TOKEN_LITERAL_SIZE)
        {
            current_token.token_type = ERROR;
            break;
        }
        buffer_index++;
    }

    current_token.string_literal = assign_buffer_literal(buffer);

    return current_token;
}

token scan_num(void)
{
    char buffer[TOKEN_LITERAL_SIZE];
    int buffer_index = 0;
    token current_token = {};

    while (true)
    {
        buffer[buffer_index] = file[file_index];
        column++;

        if (buffer[buffer_index] == '.')
        {
            if (current_token.token_type == LITERAL_FLOAT)
            {
                current_token.token_type = ERROR;
                break;
            }
            else
            {
                current_token.token_type = LITERAL_FLOAT;
            }            
        }
        else if(!isdigit(buffer[buffer_index]))
        {
            if(current_token.token_type != LITERAL_FLOAT)
            {
                current_token.token_type = LITERAL_INT;
            }
            
            break;
        }

        if ((file_index + 1) > bytes_read)
        {
            current_token.token_type = ERROR;
            break;
        }
        file_index++;
        if ((buffer_index + 1) >= TOKEN_LITERAL_SIZE)
        {
            current_token.token_type = ERROR;
            break;
        }
        buffer_index++;
    }

    buffer[buffer_index] = '\0';
    
    current_token.string_literal = assign_buffer_literal(buffer);

    return current_token;
}

label hash_string_label(const char *str) 
{
    label current_label;

    switch (hash_djb2(str))
    {
        case HASH_INT:
            if(strcmp("int", str) == 0)
            {
                current_label = KW_INT;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_CHAR:
            if(strcmp("char", str) == 0)
            {
                current_label = KW_CHAR;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_BOOL:
            if(strcmp("bool", str) == 0)
            {
                current_label = KW_BOOL;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_FLOAT:
            if(strcmp("float", str) == 0)
            {
                current_label = KW_FLOAT;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_WHILE:
            if(strcmp("while", str) == 0)
            {
                current_label = KW_WHILE;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_FOR:
            if(strcmp("for", str) == 0)
            {
                current_label = KW_FOR;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_IF:
            if(strcmp("if", str) == 0)
            {
                current_label = KW_IF;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_ELSE:
            if(strcmp("else", str) == 0)
            {
                current_label = KW_ELSE;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;

        /* PUNCTUATION */
        case HASH_LPAREN:
            if(strcmp("(", str) == 0)
            {
                current_label = PUNCT_LPAREN;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_RPAREN:
            if(strcmp(")", str) == 0)
            {
                current_label = PUNCT_RPAREN;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_LBRACKET:
            if(strcmp("[", str) == 0)
            {
                current_label = PUNCT_LBRACKET;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_RBRACKET:
            if(strcmp("]", str) == 0)
            {
                current_label = PUNCT_RBRACKET;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_LBRACE:
            if(strcmp("{", str) == 0)
            {
                current_label = PUNCT_LBRACE;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_RBRACE:
            if(strcmp("}", str) == 0)
            {
                current_label = PUNCT_RBRACE;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_SEMCOL:
            if(strcmp(";", str) == 0)
            {
                current_label = PUNCT_SCOLON;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_COL:
            if(strcmp(":", str) == 0)
            {
                current_label = PUNCT_COLON;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_COMMA:
            if(strcmp(",", str) == 0)
            {
                current_label = PUNCT_COMMA;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_FSTOP:
            if(strcmp(".", str) == 0)
            {
                current_label = PUNCT_FULLSTOP;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;

        /* OPERATORS */

        /* ASSIGN */
        case HASH_ASSIGN:
            if(strcmp("=", str) == 0)
            {
                current_label = OP_ASSIGN;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;

        /* ARITHMETIC */
        case HASH_SUBT:
            if(strcmp("-", str) == 0)
            {
                current_label = OP_SUBT;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_ADD:
            if(strcmp("+", str) == 0)
            {
                current_label = OP_ADD;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_MULT:
            if(strcmp("*", str) == 0)
            {
                current_label = OP_MULT;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_DIV:
            if(strcmp("/", str) == 0)
            {
                current_label = OP_DIV;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_MOD:
            if(strcmp("\%", str) == 0)
            {
                current_label = OP_MOD;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;

        /* CMP/LOGIC */
        case HASH_EQ:
            if(strcmp("==", str) == 0)
            {
                current_label = OP_EQ;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_UNEQ:
            if(strcmp("!=", str) == 0)
            {
                current_label = OP_UNEQ;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_GT:
            if(strcmp(">", str) == 0)
            {
                current_label = OP_GT;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_LT:
            if(strcmp("<", str) == 0)
            {
                current_label = OP_LT;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_GTEQ:
            if(strcmp(">=", str) == 0)
            {
                current_label = OP_GE;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_LTEQ:
            if(strcmp("<=", str) == 0)
            {
                current_label = OP_LE;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_AND:
            if(strcmp("&&", str) == 0)
            {
                current_label = OP_AND;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_OR:
            if(strcmp("||", str) == 0)
            {
                current_label = OP_OR;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        case HASH_NOT:
            if(strcmp("!", str) == 0)
            {
                current_label = OP_NOT;
            }
            else
            {
                current_label = fallback_label(str);
            }
            break;
        default:
            current_label = fallback_label(str);
    }
    
    return current_label; 
}

label fallback_label(const char *str)
{
    if (string_is_sing_char(str))
    {
        return LITERAL_CHAR;
    }
    else
    {
        return IDENTIFIER;
    }
}
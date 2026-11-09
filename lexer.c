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
            if (match_types(buffer, PUNCT) != ERROR)
            {
                current_token.token_type = match_types(buffer, PUNCT);
            }
            else
            {
                buffer[buffer_index] = '\0';
                current_token.token_type = match_types(buffer, PUNCT);
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
            current_token.token_type = match_types(buffer, CHARS);
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

label match_types(char *input, type type)
{
    switch (type)
    {
        case PUNCT:
            if (!strcmp(input, "("))
            {
                return PUNCT_LPAREN;
            }
            else if (!strcmp(input, ")"))
            {
                return PUNCT_RPAREN;
            }
            else if (!strcmp(input, "["))
            {
                return PUNCT_LBRACKET;
            }
            else if (!strcmp(input, "]"))
            {
                return PUNCT_RBRACKET;
            }
            else if (!strcmp(input, "{"))
            {
                return PUNCT_LBRACE;
            }
            else if (!strcmp(input, "}"))
            {
                return PUNCT_RBRACE;
            }
            else if (!strcmp(input, ":"))
            {
                return PUNCT_COLON;
            }
            else if (!strcmp(input, ";"))
            {
                return PUNCT_SCOLON;
            }
            else if (!strcmp(input, ","))
            {
                return PUNCT_COMMA;
            }
            else if (!strcmp(input, "."))
            {
                return PUNCT_FULLSTOP;
            }
            else if (!strcmp(input, "="))
            {
                return OP_ASSIGN;
            }   
            else if (!strcmp(input, "=="))
            {
                return OP_EQ;
            }
            else if (!strcmp(input, "!="))
            {
                return OP_NOT_EQ;
            }
            else if (!strcmp(input, ">"))
            {
                return OP_GT;
            }
            else if (!strcmp(input, "<"))
            {
                return OP_LT;
            }
            else if (!strcmp(input, ">="))
            {
                return OP_GE;
            }
            else if (!strcmp(input, "<="))
            {
                return OP_LE;
            }
            else if (!strcmp(input, "+"))
            {
                return OP_PLUS;
            }
            else if (!strcmp(input, "-"))
            {
                return OP_MINUS;
            }
            else if (!strcmp(input, "*"))
            {
                return OP_MULT;
            }
            else if (!strcmp(input, "/"))
            {
                return OP_DIV;
            }
            else if (!strcmp(input, "\%"))
            {
                return OP_MOD;
            }
            else if (!strcmp(input, "&&"))
            {
                return OP_AND;
            }
            else if (!strcmp(input, "||"))
            {
                return OP_OR;
            }
            else if (!strcmp(input, "!"))
            {
                return OP_NOT;
            }

            break;


        case CHARS:

            if (strlen(input) == 1)
            {
                return LITERAL_CHAR;
            }
            else if(!strcmp(input, "int"))
            {
                return KW_INT;
            }
            else if(!strcmp(input, "bool"))
            {
                return KW_BOOL;
            }
            else if(!strcmp(input, "char"))
            {
                return KW_CHAR;
            }
            else if(!strcmp(input, "float"))
            {
                return KW_FLOAT;
            }
            else if(!strcmp(input, "if"))
            {
                return KW_IF;
            }
            else if(!strcmp(input, "else"))
            {
                return KW_ELSE;
            }
            else if(!strcmp(input, "while"))
            {
                return KW_WHILE;
            }
            else if(!strcmp(input, "for"))
            {
                return KW_FOR;
            }
            else if(string_is_digit(input))
            {
                break;
            }
            else if(string_is_puncdig(input))
            {
                break;
            }

            return IDENTIFIER;
        default:
            break;
    }

    return ERROR;
}
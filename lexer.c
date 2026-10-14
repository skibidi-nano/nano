#include "lexer.h"

int lexer(FILE *target)
{
    size_t filesize = get_filesize(target); 
    
    char *file = malloc(filesize + 1);
    if (file == NULL) 
    {
        return -1; 
    }
    size_t bytes_read = fread(file, 1, filesize, target);
    file[bytes_read] = '\0';


    if(tokenize(file) = false)
    {
        return 0;
    }

    free(file);
    return 1;
}

int tokenize(char *file)
{
    int filesize = get_filesize(file);

    index_type index;
    index.one = 0;
    index.two = 1;
    index.buffer;

    int line = 0;
    int column = 0;

    char buffer[TOKEN_LITERAL_SIZE];

    for (int i = 0; i < filesize; i++)
    {
        buffer[index.buffer] = file[index.one];
        index.buffer++;
        


        if(index.two < filesize)
        {
            buffer[index.buffer] = file[index.two];
            index.buffer++; 
        }

        
        

        index.one += 2;
        index.two += 2;
        column++;
    }
    

    return 1;
}

token compare_literals(char* literal, char literal_extra)
{
    literal[sizeof(literal)] = '\0';

    token string;

    if (strlen(literal) == 1)
    {
        if (literal == '=')
        {
            string.token_type = ASSIGNMENT_OP;
            string.string_literal = '=';
        }
        else if (literal == '(')
        {
            string.token_type = N_BRACE_OP;
            string.string_literal = '(';
        }
        else if (literal == ')')
        {
            string.token_type = N_BRACE_CL;
            string.string_literal = ')';
        }
    }
    else if (string_is_alnum(literal))
    {
        

        
        static int amnt_of_keywords = sizeof(keywords) / sizeof(keywords[0]);
        for (int i = 0; i < amnt_of_keywords; i++)
        {
            if (!strcmp(literal, keywords[i]))
            {
                if (isalpha(literal_extra))
                {
                    string.token_type = IDENTIFIER;
                }
                if (isdigit(literal_extra))
                {
                    string.token_type = KEYWORD;
                }
                
                strcpy(string.string_literal, literal);
                break;
            }
        }
    }

    return string;
        
}
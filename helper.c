#include "helper.h"

bool string_is_digit(char *str)
{
    if (*str == '\0')
    {
        return false;
    }

    while (*str) 
    {
        if (!isdigit((unsigned char)*str)) 
        {
            return false;
        }
        str++;
    }
    return true;
}

bool string_is_puncdig(char *str)
{
    if (*str == '\0')
    {
        return false;
    }

    while (*str) 
    {
        if (!isdigit((unsigned char)*str) && *str != '_') 
        {
            return false;
        }
        str++;
    }
    return true;
}

size_t get_filesize(FILE *source)
{
    if (fseek(source, 0L, SEEK_END) != 0) 
    {
        return -1;
    }

    size_t size = ftell(source);
    rewind(source); 

    return size;
}

char* assign_buffer_literal(char *buffer)
{
    char *string_literal = malloc(strlen(buffer + 1));
    if (string_literal == NULL) 
    {
        return NULL;
    }

    strcpy(string_literal, buffer);

    return string_literal;
}
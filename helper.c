#include "helper.h"

bool string_is_alnum(char *str)
{
    if (*str == '\0')
    {
        return false;
    }

    while (*str) 
    {
        if (!isalnum((unsigned char)*str)) 
        {
            return false;
        }
        str++;
    }
    return true;
}

int get_filesize(FILE *source)
{
    char c;
    int filesize_counter = 0;
    while(fread(&c, sizeof(char), 1, source))
    {
        if (c == EOF)
        {
            break;
        }
        filesize_counter++;
    }

    rewind(source);
    return filesize_counter;
}
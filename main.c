#include "main.h"
#include "helper.h"
#include "lexer.h"


int main(int argc, char *argv[])
{
    /* check args */
    if (argc != 2)
    {
        fprintf(stderr, "PROPER USAGE: %s FILENAME.n\n", argv[0]);
        return 1;
    }
    ////////////////////////////////////////////////////


    /* check for correct filetype */
    char *filename = argv[1];
    char *dot_pos = strchr(filename, '.');
    if (dot_pos == NULL)
    {
        fprintf(stderr, "USE .n FILES ONLY\n");
        return 1;
    }

    int pos_filename_end = dot_pos - filename;

    if ((filename[pos_filename_end + 2] != 'n' && filename[pos_filename_end + 3] == '\0')
        || (strlen(filename) > (unsigned long)(pos_filename_end + 2)))
    {
        fprintf(stderr, "USE .n FILES ONLY\n");
        return 1;
    }
    ////////////////////////////////////////////////////


    /* open sourcecode */
    FILE *source = fopen(argv[1], "r");
    if(source == NULL)
    {
        fprintf(stderr, "FAILED TO OPEN %s\n", argv[1]);
        return 1;
    }
    ////////////////////////////////////////////////////


    /* create file to parse */
    if (strlen(filename) >= NAME_BUFFER_SIZE)
    {
        fprintf(stderr, "FILENAME TOO LARGE\n");
        return 1;
    }
    char parsed_name[NAME_BUFFER_SIZE];
    char append[] = ".target";


    size_t remaining_space = sizeof(parsed_name) - strlen(parsed_name) - 1;
    strncat(parsed_name, filename, remaining_space);

    parsed_name[strlen(parsed_name) - 2] = '\0';

    remaining_space = sizeof(parsed_name) - strlen(parsed_name) - 1;
    strncat(parsed_name, append, remaining_space);

    FILE *parsed = fopen(parsed_name, "w+");
    if(parsed == NULL)
    {
        fprintf(stderr, "FAILED TO OPEN %s\n", parsed_name);
        return 1;
    }

    size_t filesize = get_filesize(source);

    char *buffer = malloc(filesize);
    if (buffer == NULL) 
    {
        fprintf(stderr,"MALLOC FAILED\n");
        return 1;
    }

    size_t bytes_read = fread(buffer, 1, filesize, source);
    size_t bytes_written = fwrite(buffer, 1, bytes_read, parsed);
    if (bytes_written < bytes_read) 
    {
        fprintf(stderr,"FAILED TO WRITE TO PARSED FILE FROM SOURCE CODE\n");
    }

    free(buffer);
    ////////////////////////////////////////////////////

    /* lex source code */
    lexer(parsed);
    ////////////////////////////////////////////////////



    /* closing statements */
    if (remove(parsed_name) != 0) 
    {
        fprintf(stderr,"FAILED TO REMOVE %s\n", parsed_name);
        fclose(source);
        return 1;
    }
    fclose(source);
    return 0;
    ////////////////////////////////////////////////////
}


        /*if (file[index.one] == '\n')
        {
            column = 0;
            line++;
        } */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_BUFFER_SIZE 512
int get_filesize(FILE *source);

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
    char *dot_pos = strchr(argv[1], '.'); 
    int pos_filename_end = dot_pos - argv[1];
    if ((argv[1][pos_filename_end + 2] != 'n' && argv[1][pos_filename_end + 3] == '\0')
        || (sizeof(argv[1]) > (unsigned long)(pos_filename_end + 3)))
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
    
    char parsed_name[NAME_BUFFER_SIZE];
    char append[] = ".target";
    for (int i = 0; argv[1][i] != '.'; i++)
    {
        parsed_name[i] = argv[1][i];
    }

    size_t remaining_space = sizeof(parsed_name) - strlen(parsed_name) - 1;
    strncat(parsed_name, append, remaining_space);

    FILE *parsed = fopen(argv[1], "rw");
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

    /* parse source code */

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
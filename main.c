#include "main.h"
#include "helper.h"
#include "lexer.h"


char **to_free;
int tf_index;

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

    to_free = malloc(TOKEN_LITERAL_SIZE * sizeof(char *));

    /* lex source code */
    static token tokens[TEMPORARY_TOKEN_ARR_SIZE_WILL_BE_DYNAMIC_SOON];
    int last_token_index = lexer(tokens, source);
    if (last_token_index == -1)
    {
        fprintf(stderr,"FAILED TO ALLOCATE MEMORY WHILE LEXING\n");
        fclose(source);
        return 1;
    }
    else if (last_token_index == -2)
    {
        fprintf(stderr,"NOT ENOUGH MEMORY FOR FILE ARRAY\n");
        fclose(source);
        return 1;
    }
    else if (last_token_index == -3)
    {
        fprintf(stderr,"NOT ENOUGH MEMORY FOR TOKEN ARRAY\n");
        fclose(source);
        return 1;
    }
    ////////////////////////////////////////////////////

    for (int i = 0; i < last_token_index; i++)
    {
        printf("%s\n", tokens[i].line);
    }

    /* closing statements */
    free(to_free);
    
    fclose(source);
    return 0;
    ////////////////////////////////////////////////////
}
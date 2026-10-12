#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the input string into space-separated tokens */
char **tokenize(char *line)
{
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));

    int i, tokenIndex = 0, tokenNo = 0;

    for (i = 0; i < strlen(line); i++)
    {
        char ch = line[i];

        if (ch == ' ' || ch == '\t' || ch == '\n')
        {
            token[tokenIndex] = '\0';

            if (tokenIndex != 0)
            {
                tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE);
                strcpy(tokens[tokenNo], token);
                tokenNo++;
                tokenIndex = 0;
            }
        }
        else
        {
            token[tokenIndex++] = ch;
        }
    }

    free(token);
    tokens[tokenNo] = NULL;

    return tokens;
}

int main()
{
    char line[MAX_INPUT_SIZE];
    char **tokens;
    int i;

    while (1)
    {
        printf("shell> ");

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        tokens = tokenize(line);

        
        if (tokens[0] == NULL)
        {
            /* Ignore empty input */
        }

       
        if (strcmp(tokens[0], "exit") == 0)
        {
             /*exit command */
        }

        /*
         * 1. Create a child process using fork().
         * 2. In the child process, execute the command using execvp().
         * 3. If execvp() fails, print an error message and terminate the child.
         * 4. In the parent process, wait for the child to complete.
         */

        /* Free allocated memory */
        for (i = 0; tokens[i] != NULL; i++)
            free(tokens[i]);

        free(tokens);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

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

int execute_command(char *command)
{
    char **tokens;
    int i;
    int background = 0;

    tokens = tokenize(command);

    if (tokens[0] == NULL)
    {
        free(tokens);
        return 0;
    }

    /* Check for & */
    for (i = 0; tokens[i] != NULL; i++)
        ;

    if (i > 0 && strcmp(tokens[i - 1], "&") == 0)
    {
        background = 1;
        tokens[i - 1] = NULL;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    else if (pid == 0)
    {
        /* Handle redirection */
        for (i = 0; tokens[i] != NULL; i++)
        {
            if (strcmp(tokens[i], "<") == 0)
            {
                int fd = open(tokens[i + 1], O_RDONLY);

                if (fd < 0)
                {
                    perror("open");
                    exit(1);
                }

                dup2(fd, STDIN_FILENO);
                close(fd);

                tokens[i] = NULL;
            }
            else if (strcmp(tokens[i], ">") == 0)
            {
                int fd = open(tokens[i + 1],
                              O_WRONLY | O_CREAT | O_TRUNC,
                              0644);

                if (fd < 0)
                {
                    perror("open");
                    exit(1);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);

                tokens[i] = NULL;
            }
        }

        execvp(tokens[0], tokens);

        perror("execvp");
        exit(1);
    }

    else
    {
        int status;

        if (background)
        {
            printf("[background] started pid %d\n", pid);
        }
        else
        {
            waitpid(pid, &status, 0);

            if (WIFEXITED(status))
            {
                int exit_status = WEXITSTATUS(status);

                for (i = 0; tokens[i] != NULL; i++)
                    free(tokens[i]);

                free(tokens);

                return exit_status;
            }
        }
    }

    for (i = 0; tokens[i] != NULL; i++)
        free(tokens[i]);

    free(tokens);

    return 0;
}

void reap_background()
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("[background] pid %d done (exit status %d)\n",
                   pid, WEXITSTATUS(status));
        }
    }
}

int main()
{
    char line[MAX_INPUT_SIZE];

    while (1)
    {
        reap_background();

        printf("shell> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        char *command = line;

        while (command != NULL)
        {
            char *next = strstr(command, "&&");

            if (next != NULL)
            {
                *next = '\0';
                next += 2;
            }

            if (strcmp(command, "exit") == 0 || strcmp(command, "exit\n") == 0)
            {
                printf("exiting\n");
                return 0;
            }

            int exit_status = execute_command(command);

            printf("[exit status: %d]\n", exit_status);

            if (exit_status != 0)
                break;

            command = next;
        }
    }

    return 0;
}
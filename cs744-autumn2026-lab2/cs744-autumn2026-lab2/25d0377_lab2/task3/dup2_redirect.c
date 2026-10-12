#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int file_fd = open("dup2.txt",O_WRONLY | O_CREAT | O_TRUNC,0644); 

    int copy_out = dup2(file_fd, STDOUT_FILENO);

    fprintf(stdout,"Hi there, Welcome to cs744.\n");

    close(file_fd);

    return 0;
}

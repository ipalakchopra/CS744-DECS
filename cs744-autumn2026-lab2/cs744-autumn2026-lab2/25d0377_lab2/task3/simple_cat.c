#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    pid_t pid = fork();

    if(pid > 0){
        wait(NULL);
    }

    else if(pid == 0){

        char buffer[1024];
        ssize_t bytes_read;

        if(argc < 2){
            bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
        }        
        else{
            int fd = open(argv[1], O_RDONLY);
            bytes_read = read(fd, buffer, sizeof(buffer));
        }

        write(STDOUT_FILENO, buffer, bytes_read);
    }

    else{
        fprintf(stdout, "Fork error.");
    }
    return 0;
}

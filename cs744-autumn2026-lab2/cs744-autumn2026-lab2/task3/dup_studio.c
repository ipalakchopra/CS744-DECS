#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int copy_in = dup(STDIN_FILENO);
    int copy_out = dup(STDOUT_FILENO);

    char buffer[1024];
    ssize_t bytes_read;
    
    while((bytes_read = read(copy_in, buffer, sizeof(buffer))) > 0){
        if(write(copy_out, buffer, bytes_read) == -1){
            fprintf(stdout,"Error in writing.");
        }
    }

    close(copy_in);
    close(copy_out);

    return 0;
}

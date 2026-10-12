#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){
    
    fprintf(stdout,"cmd_exec PID: %d\n", getpid());

    char* argument_list[] = {"ls", "-l", NULL}; 

    int status_code = execvp("ls",argument_list);

    if (status_code == -1) {
        fprintf(stdout,"Execution failed\n");    
        exit(1);
    }

    return 0;
}
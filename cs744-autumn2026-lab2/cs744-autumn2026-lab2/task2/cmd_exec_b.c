#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    
    fprintf(stdout,"cmd_exec PID: %d\n", getpid());

    if (argc < 2) {
        fprintf(stderr, "No command provided\n");
        fprintf(stderr, "Execute: %s <command> <arguments>\n", argv[0]);
        return 1;
    }

    int status_code = execvp(argv[1],&argv[1]);

    if (status_code == -1) {
        fprintf(stdout,"Execution failed\n");    
        exit(1);
    }

    return 0;
}
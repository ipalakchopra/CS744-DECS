#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    
    pid_t pid = fork();

    if(pid > 0){
        fprintf(stdout,"Parent PID: %d\n", getpid());
        int child_exit_status;
        wait(&child_exit_status);

        if (WEXITSTATUS(child_exit_status) > 0){
            fprintf(stdout, "Child process terminated.\n");
        }
        else{
            fprintf(stdout, "Child completed successfully.\n");
        }
    }

    else if(pid == 0){
        fprintf(stdout,"Child PID: %d\n", getpid());
        
        if (argc < 2) {
            fprintf(stderr, "No command provided\n");
            fprintf(stderr, "Execute: %s <command> <arguments>\n", argv[0]);
        }

        int status_code = execvp(argv[1],&argv[1]);

        if (status_code == -1) {
            fprintf(stdout,"Execution failed\n");    
            exit(1);
        }
    }

    else{
        fprintf(stdout, "Fork failed.\n");
    }

    return 0;
}
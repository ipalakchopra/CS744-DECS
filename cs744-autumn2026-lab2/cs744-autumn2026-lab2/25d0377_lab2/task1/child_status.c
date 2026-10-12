#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv){
    
    pid_t child_pid = fork();

    if(child_pid == 0){
        fprintf(stdout,"Child process %d , parent PID: %d\n", getpid(), getppid());
        exit(10);
    }

    else if(child_pid > 0){
        fprintf(stdout,"Parent process %d , child PID: %d\n",getpid(),child_pid);

        int child_exit_status;
        wait(&child_exit_status);
        
        fprintf(stdout,"Child process has terminated.\n");
        fprintf(stdout,"Child process Exit Status = %d\n",WEXITSTATUS(child_exit_status));
    }

    else{
        fprintf(stdout,"Error!\n");
    }

    return 0;
}
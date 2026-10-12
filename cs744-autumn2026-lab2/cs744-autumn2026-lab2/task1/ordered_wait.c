#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv){
    
    pid_t child1_pid = fork();

    // Child 1
    if(child1_pid == 0){
        fprintf(stdout,"First child process %d , parent PID: %d\n", getpid(), getppid());
        sleep(5);
        exit(160);
    }

    else if(child1_pid > 0){
        
        pid_t child2_pid = fork();

        // Child 2
        if(child2_pid == 0){
            fprintf(stdout,"Second child process %d , parent PID: %d\n\n", getpid(), getppid());
            sleep(2);
            exit(64);
        }

        int child1_exit_status;
        waitpid(child1_pid, &child1_exit_status,0);
        fprintf(stdout,"First Child process has terminated.\n");
        fprintf(stdout,"First Child process Exit Status = %d\n\n",WEXITSTATUS(child1_exit_status));

        int child2_exit_status;
        waitpid(child2_pid, &child2_exit_status,0);
        fprintf(stdout,"Second Child process has terminated.\n");
        fprintf(stdout,"Second Child process Exit Status = %d\n",WEXITSTATUS(child2_exit_status));
    }

    else{
        fprintf(stdout,"Error!\n");
    }

    return 0;
}
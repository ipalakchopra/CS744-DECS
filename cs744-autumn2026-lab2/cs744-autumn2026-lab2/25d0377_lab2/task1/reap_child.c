#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    
    int child_pid = fork();

    if(child_pid == 0){
        fprintf(stdout,"Child process %d , parent PID: %d\n", getpid(), getppid());
        sleep(2);
    }
    else if(child_pid > 0){
        fprintf(stdout,"Parent process %d , child PID: %d\n",getpid(),child_pid);
        wait(NULL);
        fprintf(stdout,"Child process %d has finished\n",child_pid);
    }
    else{
        fprintf(stdout,"Error!\n");
    }

    return 0;
}
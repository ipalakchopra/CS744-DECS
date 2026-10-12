#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
    
    int child_pid = fork();

    if(child_pid == 0){
        fprintf(stdout,"Child process %d , parent PID: %d\n", getpid(), getppid());
    }
    else if(child_pid > 0){
        fprintf(stdout,"Parent process %d , child PID: %d\n",getpid(),child_pid);
    }
    else{
        fprintf(stdout,"Error!\n");
    }

    return 0;
}
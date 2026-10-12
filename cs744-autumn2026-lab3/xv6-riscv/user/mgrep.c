#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{

  if(argc < 3){
    printf("not enough args");
  }

  int n = argc - 2;
  int pid = 1;
  int status = 0;
  while(n > 0){
    if(pid > 0){
      pid = fork();
      n--;
    }
    if(pid == 0){
      char* arglist[] = {"grep", argv[1], argv[argc-n+1]};
      status = exec("grep",arglist);

    }
    
  }  

  if(pid > 0){
    wait(&status);
  
  }
  exit(0);
}

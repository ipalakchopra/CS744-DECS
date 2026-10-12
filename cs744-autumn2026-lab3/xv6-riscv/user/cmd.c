#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
  int pid = fork();

  if(pid == 0){
    exec(argv[1],&argv[1]);
  }
	
  exit(0);
}

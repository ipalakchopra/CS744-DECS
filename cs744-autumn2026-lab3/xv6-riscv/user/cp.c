#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{

  if(argc < 3){
    printf("not enough args");
  }
  
  int fd1 = open(argv[1],O_RDONLY);
  int fd2 = open(argv[2], O_CREATE | O_WRONLY);

  char c;

  while(read(fd1, &c, 1) > 0){
    write(fd2, &c, 1);
  
  }
  close(fd1);
  close(fd2);
  exit(0);
}

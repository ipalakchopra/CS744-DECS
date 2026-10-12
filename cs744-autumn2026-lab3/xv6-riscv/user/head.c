#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{

  if(argc < 3){
    printf("not enough args");
  }
  
  int fd = open(argv[1],O_RDONLY);
  
  int n = atoi(argv[2]);
  int i = 0;
  char c;

  while(i<n){
    read(fd,&c,1);
    write(1,&c,1);
    if(c == '\n'){
      i++;    
    }
  }

  exit(0);
}

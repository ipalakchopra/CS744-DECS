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
  int j = 0;
  char c;

  char (*lines)[1024] = malloc(n*sizeof(*lines));

  while(read(fd, &c, 1) > 0){
    lines[i][j++] = c;
    
    if(c == '\n'){
      i++;
      i=i%n;
      j = 0;    
    }
  }

  for(int k = 0; k<n; k++){
    write(1,lines[k],sizeof(lines[k]));
  
  }

  exit(0);
}

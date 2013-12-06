#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>

int main(int argv, char **argc)
{
  int fd;
  int ret;
  
  int option = (int)atoi(argc[1]);

  if (option == 1)
  {
  	ret =syscall(378, "/data/local/tmp/a","/data/local/tmp/b");
  	syscall(378, "/data/local/tmp/a","/data/local/tmp/c");
  	syscall(378, "/data/local/tmp/a","/data/local/tmp/d");
  	printf("COWCOPY return value :  %d\n", ret );
  }
  else
  {
  	  fd = open("/data/local/tmp/b", O_WRONLY | O_APPEND);
  	  write(fd,"world",6);
  	  close(fd);
  	  printf("file descriptor :  %d\n", fd );	
  }
  
  return 0;
}
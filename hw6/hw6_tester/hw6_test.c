#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>

int main()
{
  int fd;
  int ret;
  


  ret =syscall(378, "/data/local/tmp/source","/data/local/tmp/dest1");
  syscall(378, "/data/local/tmp/source","/data/local/tmp/dest2");
  //syscall(378, "/data/local/tmp/src","/data/local/tmp/dest2");
  printf("return value :  %d\n", ret );
  fd = open("/data/local/tmp/source", O_WRONLY);
  write(fd,"wassup world",13);
  close(fd);
  return 0;
}
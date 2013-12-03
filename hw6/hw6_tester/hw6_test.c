#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>

int main()
{
  int fd;
  int ret;
  
  ret = syscall(378, "/data/local/tmp/hw5_test","/data/local/tmp/dest");
  printf("return value :  %d\n", ret );
  fd = open("/data/local/tmp/hw5_test", O_RDWR);
  return 0;
}
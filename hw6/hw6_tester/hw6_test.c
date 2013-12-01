#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>

int main()
{
  int fd;
  int ret;
  fd = open("/data/local/tmp/hw5_test.c", O_RDWR);
  ret = syscall(378, "/data/local/tmp/hw5_test.c","/data/local/tmp");
  printf("return value :  %d\n", ret );
  return 0;
}
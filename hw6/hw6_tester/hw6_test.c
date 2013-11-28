#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

int main()
{
  int ret = syscall(378, "/data/local/tmp","/data/local/tmp");
  printf("return value :  %d\n", ret );
  return 0;
}
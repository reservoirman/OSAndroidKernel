#include <stdio.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
	int check;
	pid_t pid;
	struct sched_param param;
	check=sched_setscheduler(pid,6, &param);
	param.sched_priority = 2 ;
	pid = getpid();
	printf("sched_setscheduler returns :  %d\n", check );
	return 0;
	
}
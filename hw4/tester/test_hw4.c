#include <stdio.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
	int check;
	pid_t pid;
	struct sched_param param;
	param.sched_priority = 0 ;
	pid = getpid();
	check=sched_setscheduler(pid,6, &param);
	printf("sched_setscheduler returns :  %d\n", check );
	printf(" %d \n",pid );
	printf("working\n");
	return 0;
	
}
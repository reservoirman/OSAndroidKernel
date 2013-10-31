#include <sys/types.h>
#include <unistd.h>
#include <sched.h>


int main()
{
	pid_t pid;
	int check;
	struct sched_param param;
	param.sched_priority = 2 ;
	pid = getpid();

	check=sched_setscheduler(pid, SCHED_MYCFS, &param);
	printf("%d\n", check );
	return 0;
}
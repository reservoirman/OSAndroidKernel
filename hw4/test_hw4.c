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

	if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) 
	{
    	perror("sched_setscheduler");
	}
	check = sched_setscheduler(pid, SCHED_MYCFS,  );
	printf("return of sched_setscheduler %d\n", check);
	return 0;
}
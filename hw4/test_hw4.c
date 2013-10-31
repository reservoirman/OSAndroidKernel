#include <sys/types.h>
#include <unistd.h>
#include <sched.h>


int main()
{
	pid_t pid;
	int check;
	pid = getpid();
	check = sched_setscheduler(pid, SCHED_MYCFS,  );
	printf("return of sched_setscheduler %d\n", check);
	return 0;
}
#include <stdio.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
	int check;
	int i = 0, j = 0;
	pid_t pid, pid2;
	struct sched_param param;
	param.sched_priority = 0 ;
	pid = getpid();
	printf("We are in test_hw4\n");
	check=sched_setscheduler(pid,6, &param);
	printf("sched_setscheduler returns :  %d\n", check );
	printf("Process %d \n",pid );
	printf("working\n");

	//pid = fork();

	if (fork() == 0)
	{
		

		for (;;)
		{

			j++;
			if (j % 100000000 == 0)
			{
				j = 0;
				printf("Process %d is running!\n", getpid());
			}
		}

	}

	for (;;)
	{

		i++;
		if (i % 100000000 == 0)
		{
			i = 0;
			printf("Process %d is running!\n", pid);
		}
	}

	return 0;
	
}
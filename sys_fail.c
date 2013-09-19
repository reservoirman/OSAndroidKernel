#include <linux/sched.h>

struct task_struct *task;

static int counter;
static int n;
static int sys_fail_ran = 0;

asmlinkage long sys_fail(int N)
{

	counter = 0;
	n = N;
	sys_fail_ran = 1;
	return 0;

}

long should_fail(void)
{
/*
	if (sys_fail_ran == 1)
	{
		counter++;
	}	
	if (counter > 0)
	{
		return 1;
	} else { return 0; }
	if(( counter ) == n)
		return 1;
	else
*/
		return 0;
}
 

long fail_syscall(void)
{
	return 0;
}


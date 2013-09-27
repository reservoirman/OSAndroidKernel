#include <linux/sched.h>
#include <asm-generic/errno-base.h>
#include <linux/syscalls.h>
#include <asm/thread_info.h>


int n;
int sys_fail_ran = 0;
 
asmlinkage long sys_fail(int N)
{
	current_thread_info()->task->hw2_count = 0;
	n= N;
	sys_fail_ran = 1;
	return 0;

}

long should_fail(void)
{
	
	if (sys_fail_ran == 1)
	{
		current_thread_info()->task->hw2_count++;
	
		if(current_thread_info()->task->hw2_count < n)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}
 

long fail_syscall(void)
{
	sys_fail_ran = 0;
	n=0;
	current_thread_info()->task->hw2_count = 0;
	return(-EINVAL);
}


#include <linux/sched.h>
#include <asm-generic/errno-base.h>
#include <linux/syscalls.h>
#include <asm/thread_info.h>


int n;
int sys_fail_ran = 0;
//int flag = 0;
 
asmlinkage long sys_fail(int N)
{
	n= N;	
	if(n > 0)
	{	
		current_thread_info()->task->hw2_count = 0;
		sys_fail_ran = 1;
		return 0;
	}
	else if( n<=0)
	{
		return(-EINVAL);
	}
	else if( n == 0 && sys_fail_ran == 1)
	{
		sys_fail_ran = 0;
		return 0;
	}
	else
	{
		return 0;
	}

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
	//flag =1 ;
	return(-EINVAL);
}


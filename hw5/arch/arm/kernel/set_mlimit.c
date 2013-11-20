#include <linux/sched.h>
#include <asm-generic/errno-base.h>
#include <linux/syscalls.h>
#include <asm/thread_info.h>

uid_t user;
int start;

asmlinkage int set_mlimit(uid_t uid, long mem_max)
{
	struct task_struct *p;
	struct user_struct *usr;
	usr= find_user(uid);
	usr->cumulative_mem = 0;
	for_each_process(p)
	{
		if(p->real_cred->uid == uid)
		{
			usr->cumulative_mem += get_mm_rss(p->mm)*PAGE_SIZE;
		}
	}
	user=uid;
	usr->mem_max = mem_max;
	start = 1;
	return 0;
}
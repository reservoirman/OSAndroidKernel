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
	printk("PJ: 1.user = %d uid= %d\n",user,uid);
	usr= find_user(uid);
	for_each_process(p)
	{
		if(p->real_cred->uid == uid)
		{
			usr->cumulative_mem += get_mm_rss(p->mm)*PAGE_SIZE;
		}
	}
	printk("cumulative_mem = %lu\n",usr->cumulative_mem );
	user=uid;
	printk("PJ: 1.user = %d uid= %d\n",user,uid);
	usr->mem_max = mem_max;
	printk("mem_max = %lu\n",usr->mem_max );
	start = 1;
	return usr->cumulative_mem;
}
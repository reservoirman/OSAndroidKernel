#include <linux/sched.h>
#include <asm-generic/errno-base.h>
#include <linux/syscalls.h>
#include <asm/thread_info.h>

uid_t user;

asmlinkage int set_mlimit(uid_t uid, long mem_max)
{
	
	struct user_struct *usr;
	usr= find_user(uid);
	user=uid;
	//usr->mem_max = mem_max;
	return current_thread_info()->task->real_cred->uid;
}
#include <linux/sched.h>
#include <asm-generic/errno-base.h>
#include <linux/syscalls.h>
#include <asm/thread_info.h>

asmlinkage int set_mlimit(uid_t uid, long mem_max)
{
	
	struct user_struct * user = find_user(uid);
	user->mem_max = mem_max;
	return user->mem_max;
}
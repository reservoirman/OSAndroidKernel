
#include <linux/sched.h>
#include <asm-generic/errno-base.h>
#include <linux/syscalls.h>
#include "netlock.h"
/* Syscall 378. Acquire netlock. type indicates
        whether a regular or exclusive lock is needed. Returns 0 on success 
	and -1 on failure.  
      */
    asmlinkage int netlock_acquire(netlock_t type){return 0;}

     /* Syscall 379. Release netlock. Return 0 on success and -1 on failure.  
      */
     asmlinkage int netlock_release(void){return 0;}

  

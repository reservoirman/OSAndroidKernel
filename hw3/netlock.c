
#include <linux/sched.h>
#include <asm-generic/errno-base.h>
#include <linux/syscalls.h>
#include <linux/spinlock.h>
#include "netlock.h"
/* Syscall 378. Acquire netlock. type indicates
whether a regular or exclusive lock is needed. Returns 0 on success 
and -1 on failure.  */
spinlock_t mr_lock;
int read=1, write=1, mutex1=1, mutex2=1, mutex3 = 1, readers = 0, writers = 0;

int wait(int mutex)
{
	int ret;

    DEFINE_SPINLOCK(mr_lock);
	spin_lock(&mr_lock);	
    if (mutex == 1)
	{
	    printk("WAIT (mutex available)! \n");
       	    mutex = 0;
	    ret = 0;
	    spin_unlock(&mr_lock);
	}	
	else
	{
	  printk("WAIT (mutex unavailable)! \n");
	  ret = -1;
	  spin_unlock(&mr_lock);
    } 
	return ret;
}


int signal(int mutex)
{
    DEFINE_SPINLOCK(mr_lock);
	spin_lock(&mr_lock);
	printk("SIGNAL! ( mutex released)\n");
	
	if(mutex == 0)
		mutex = 1;
	spin_unlock(&mr_lock);
	return 0;
}       

asmlinkage int netlock_acquire(netlock_t type)
{
	// Requests regular (read) lock
	if(type == NET_LOCK_R) {
		/* if (lock not available) {
			wait.exclusive = 0;
			add_wait_queue(&q, &wait);
			schedule();
		}
		else {} */
		/* future: if (a process is currently holding or waiting for an exclusive lock)
		{
			add this process to the wait queue
		}
		else if no process is currently holding or waiting for an exclusive lock)
		{
			acquire lock
		}*/


		{
			wait(mutex3);
			wait(read);
			wait(mutex1);
			readers = readers + 1;
			if (readers == 1) {
				wait(write);
			}
			signal(mutex1);
			signal(read);
			signal(mutex3);
		}
    }

    // Requests exclusive (write) lock
    else if (type == NET_LOCK_E) {
		/*	if (lock not available) {
			wait.exclusive = 1;
			add_wait_queue(&q, &wait);
			schedule();
		}
		else {} */
		wait(mutex2);
		writers = writers + 1;
		if (writers == 1) {
			wait(read);
		}
		signal(mutex2);
		wait(write);
    }
    // Requests no lock
    else if (type == NET_LOCK_N) {

    }
	return 4321;
}

/* Syscall 379. Release netlock. Return 0 on success and -1 on failure.  */

asmlinkage int netlock_release(void){
return 0;
//	if (queue is empty) {
		//...
//	}
//...

/*
	// Releases regular (read) lock
	if (peekInQuueue()->type == NET_LOCK_R) {
		signal(write);
		wait(mutex2);
		writers = writers - 1;
		if (writers == 0) {
			signal(read);
		}
		signal(mutex2);
		removeFromQueue(type);
	}
    // Releases exclusive (write) lock
    else if (peekInQueue()->type == NET_LOCK_E) {
		wait(mutex1);
		readers = readers - 1;
		if (readers == 0) {
			signal(write);
		}
		signal(mutex1);
		removeFromQueue(type);
	}
    // Releases no lock
	else if (type == NET_LOCK_N) {
    }
    */

	/* 
	if (queue is empty)
	{
		read = 1;
		write = 1;

	}
	else if (not empty)
	{
		traverse through the queue
		search for the first task with type E
			wake up this task
			write = 0;
		if no task with type E
			wake up first task with type R
			remove from queue (Wait_queue_remove)
			read = 0;
	}
	return 0;
	*/
}

  

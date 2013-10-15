
#include <linux/sched.h>
#include <asm-generic/errno-base.h>
#include <linux/syscalls.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include "netlock.h"
/* Syscall 378. Acquire netlock. type indicates
whether a regular or exclusive lock is needed. Returns 0 on success 
and -1 on failure.  */


spinlock_t lock;
wait_queue_t wait_queue;
int read_lock_available=1, write_lock_available=1;
int reader_count = 0;
int queue_exists = 0;
int lock_exists = 0;
asmlinkage int netlock_acquire(netlock_t type)
{
	
	int ret = 0;
	if (queue_exists == 0)
	{
		DEFINE_WAIT(wait_queue);
		queue_exists = 1;

	spin_lock(&lock);
	}
	if (lock_exists == 0)
	{
		DEFINE_SPINLOCK(lock);
		lock_exists = 1;

	spin_lock(&lock);
	}
    // Requests regular (read) lock
    if(type == NET_LOCK_R) 
    {
	
		if (write_lock_available == 1)
		{
			reader_count++;
		}
		else
		{
			wait_queue_head_t *newThread;
			newThread = (wait_queue_head_t *)kmalloc(sizeof(wait_queue_head_t),GFP_KERNEL);
			newThread->netlock_flag = 0;	//0 indicates regular lock, 1 indicates exclusive
			add_wait_queue(newThread,&wait_queue);
			schedule();

		}
		ret = 0;
    }

    // Requests exclusive (write) lock
    else if (type == NET_LOCK_E) 
    {
    	if (write_lock_available == 1 && read_lock_available == 1)
    	{
    		write_lock_available = 0;
    	}
    	else
    	{
    		wait_queue_head_t *newThread;
			newThread = (wait_queue_head_t *)kmalloc(sizeof(wait_queue_head_t),GFP_KERNEL);
			newThread->netlock_flag = 1;	//0 indicates regular lock, 1 indicates exclusive
			add_wait_queue(newThread,&wait_queue);
			schedule();
    	}
		ret = 0;
    }
    // Requests no lock
    else if (type == NET_LOCK_N) 
    {
	
		ret = -1;
    }

    spin_unlock(&lock);
	return ret;
	
	return 0;
}

/* Syscall 379. Release netlock. Return 0 on success and -1 on failure.  */

asmlinkage int netlock_release(void)
{
	
	spin_lock(&lock);
	if (list_empty(&wait_queue.task_list))
	{
		if( reader_count <= 1)
		{
			reader_count = 0;
			read_lock_available = 1;
			write_lock_available = 1;
		}
		else
		{
			reader_count--;
		}
	}
	else
	{
		wait_queue_head_t *pos;
		int exclusiveFound = 0;
		wait_queue_head_t temp;

        // Save the head of the list
		wait_queue_head_t *head = (wait_queue_head_t *) kmalloc(sizeof(wait_queue_head_t), GFP_KERNEL);
		head->task_list = wait_queue.task_list; 
		pos = head;

		for (pos->task_list = *(&wait_queue.task_list)->next; \
			(pos->task_list.next != *(&wait_queue.task_list.next)) && (pos->task_list.prev != *(&wait_queue.task_list.prev)); \
			pos->task_list = *(pos->task_list.next))
		{
			if (pos->netlock_flag == 1)		//1 indicates exclusive
			{
				if(exclusiveFound == 0)
				{
					exclusiveFound = 1;
					temp = *pos;
				}
			}
			if (pos->netlock_flag == 0)		//1 indicates exclusive
			{
				reader_count++;
			}

		}
		if(exclusiveFound == 1)
		{
			write_lock_available = 0;
			remove_wait_queue(&temp, &wait_queue);
			kfree(pos);
			wake_up(&temp);
		}
		else
		{
			if(reader_count > 0)
			{
				read_lock_available = 0;
			 	wake_up_all(head);
			}
		}
		kfree(head);
						
	}

	spin_unlock(&lock);
	return 0;
    
	/*
	if (queue is empty)
	{	
                
		if( reader_count <= 1)
		{
			reader_count = 0;
			read_lock_available = 1;
			write_lock_available = 1;
		}
		else
		{
			reader_count--;
		}
		

	}
	else if (not empty)
	{
		traverse through the queue
		search for the first task with type E
			wake up this task
			remove from the queue
			write_lock_available=0;
		if no task with type E
			wake up all task with type R
			loop : remove from queue (Wait_queue_remove) ; reader_count++;
			read_lock_available = 0;
			
	}	
	return 0;
	*/

    return 0;
}

  

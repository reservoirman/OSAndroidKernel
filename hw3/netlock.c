
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

wait_queue_t wait_queue;
static int read_lock_available=1, write_lock_available=1;
static int reader_count = 0;
//static int queue_exists = 0;
//static int lock_exists = 0;
DEFINE_SPINLOCK(lock);

asmlinkage int netlock_acquire(netlock_t type)
{
	
	int ret = 0;

	DEFINE_WAIT(wait_queue);
    spin_lock(&lock);
   
    // Requests regular (read) lock
    if(type == NET_LOCK_R) 
    {
	
		if (write_lock_available == 1)
		{
			reader_count++;
			spin_unlock(&lock);
		}
		else
		{
			wait_queue_head_t *newThread;
			newThread = (wait_queue_head_t *)kmalloc(sizeof(wait_queue_head_t),GFP_KERNEL);
			newThread->netlock_flag = 0;	//0 indicates regular lock, 1 indicates exclusive
			add_wait_queue(newThread,&wait_queue);
			spin_unlock(&lock);
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
    		spin_unlock(&lock);
    	}
    	else
    	{
    		wait_queue_head_t *newThread;
			newThread = (wait_queue_head_t *)kmalloc(sizeof(wait_queue_head_t),GFP_KERNEL);
			newThread->netlock_flag = 1;	//0 indicates regular lock, 1 indicates exclusive
			add_wait_queue(newThread,&wait_queue);
			spin_unlock(&lock);
			schedule();
			
    	}
		ret = 0;
    }
    // Requests no lock
    else if (type == NET_LOCK_N) 
    {
		ret = -1;
		spin_unlock(&lock);
    }
	return ret;
}

/* Syscall 379. Release netlock. Return 0 on success and -1 on failure.  */

asmlinkage int netlock_release(void)
{
	DEFINE_WAIT(wait_queue);
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
		spin_unlock(&lock);
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
			spin_unlock(&lock);
			wake_up(&temp);
			//prepare_to_wait(&temp, &wait_queue, TASK_INTERRUPTIBLE);
			//finish_wait(&temp, &wait_queue);
		}
		else
		{
			if(reader_count > 0)
			{
				read_lock_available = 0;
				spin_unlock(&lock);
			 	wake_up_all(head);
			}
			else
			{
				spin_unlock(&lock);
			}
		}
		kfree(head);
		
	}

	return 0;
    
}

  

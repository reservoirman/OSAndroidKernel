/*
 * NAME, etc.
 *
 * sync.c
 *
 * Synchronization routines for SThread
 */

#define _REENTRANT

#include "sthread.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * semaphore routines
 */

unsigned long int flag=0;

//inserts a thread to the waiting queue if semaphore count =0
void insertNode(struct sthread_sem_struct *sem, sthread_t data) 
{
    if (sem->sthread_count == 0)
	{
	    sem->head = (struct Node*) malloc((size_t)sizeof(struct Node));
	   	sem->start = sem->head ;
        sem->head->data = data;
       	sem->head->next = NULL;
        sem->sthread_count++;          
    } 
    else 
	{
		struct Node *temp = (struct Node *) malloc((size_t)sizeof(struct Node));
        sem->head->next = temp;
		temp->data = data;
        temp->next = NULL;
        sem->head = temp;
	    sem->sthread_count++;
    }
}

//removes a thread from the waiting queue as soon as semaphore count > 0
struct Node * removeNode(struct sthread_sem_struct *sem) 
{
	struct Node *temp;
   	temp = sem->start;
    sem->start = sem->start->next;
    sem->sthread_count--;
    return temp;
}


void lock()													//Its a spin lock used to lock the semaphore implementation
{
	while (test_and_set(&flag));
}
void unlock()
{
	flag=0;
}



// initializes the sem->count to variable count passed by the calling process
int sthread_sem_init(sthread_sem_t *sem, int count)
{
	lock();
    sem->count = count;
    sem->head = NULL;
	sem->start = NULL ;
    sem->sthread_count = 0;
    int ret;
    if ((sem->count == count) && (sem->head == NULL))
	{	    
    	ret =  0;
	}        
	else 
	{	
	    ret = -1;
	}
    unlock();
	return ret;
}


//Destroys the semaphore by initializing the count to 0 and head to NULL
int sthread_sem_destroy(sthread_sem_t *sem)
{

	lock();
    sem->count = 0;
    sem->head = NULL;
	int ret;
	if ((sem->count == 0) && (sem->head == NULL))
	{   
		ret =  0;
	}        
	else 
	{	
	    ret = -1;
	}
    unlock();
    return ret;
}
/*
down() - It checks if sem-> > 0 , If true then decreases the count and gives the semaphore else adds the thread to the waiting queue and suspends it.
*/
int sthread_sem_down(sthread_sem_t *sem)
{
	lock();		
	int ret;
    if (sem->count > 0)
	{  
		sem->count--;
	    ret = 0;
	    unlock();
	}	
	else
	{
      	insertNode(sem, sthread_self());
	  	ret = -1;
	  	unlock();
	  	sthread_suspend();  
    } 
	return ret; 
}


/*
try_down() - It checks if sem-> > 0 , If true then decreases the count else just returns
*/
int sthread_sem_try_down(sthread_sem_t *sem)
{
	lock();
	int ret;
	if (sem->count > 0)
	{
		sem->count--;
		ret = 0;
	}
	else
	{
		ret = -1;
	}
	unlock();
    return ret;
}

/*
up()-> It is called by a thread when a semaphore is released. If the waiting queue is empty then just increment sem->count else remove and wake up the first thread on the waiting queue.
*/
int sthread_sem_up(sthread_sem_t *sem)
{	
	lock();
	if (sem->sthread_count <= 0)
	{
		sem->count++;
		unlock();
	}
	else	
	{	
		struct Node *longestWaitingThread = removeNode(sem);
		struct Node temp;
		temp.data = longestWaitingThread->data;
		free(longestWaitingThread);
		unlock();
		sthread_wake(temp.data);
	}
	return 0;
}       
